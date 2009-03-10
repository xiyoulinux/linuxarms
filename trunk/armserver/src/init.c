#define __DEBUG__
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "linuxarms.h"
#include "amthread.h"
#include "asthread.h"
#include "afthread.h"
#include "acthread.h"
#include "atthread.h"
#include "login.h"
#include "support.h"
#include "fconfig.h"
#include "debug.h"
#include "error.h"
#include "anet.h"
#include "thread.h"
#include "assinfo.h"
#include "arm.h"
#define _DEBUG_
#ifdef GTK_THREAD
#include <glib.h>
#endif

#define LISTEN_NUMS 10
static struct sockaddr_in serv_addr, login_addr, client_addr;
static int sockfd = -1;
char *login_user;

void check_user_permission(void)
{
	if (getuid() != 0) {
		linuxarms_print("Sorry, we must start armserver with root...\n");
		exit (1);
	}
}

void armserver_init(void)
{
	int size;
	int bind_ret = 1;
	
#ifdef GTK_THREAD
	if (!g_thread_supported())
		g_thread_init(NULL);
#endif
	login_user = (char *)mmap(NULL,sizeof(char) * USER_NAME_LEN,
			PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
	memset(login_user, '\0', USER_NAME_LEN);

	add_file_directory("/etc/linuxarms-armserver/config");
	add_file_directory("/usr/share/linuxarms-armserver/drive/beep");
	add_file_directory("/usr/share/linuxarms-armserver/drive/led");
/* 
 * if we are debug the program,we can use this config and resource file 
 * withoutinstall the programs
 */
#ifdef _DEBUG_
	add_file_directory("../config");
	add_file_directory("./config");
	add_file_directory("../drive/beep");
	add_file_directory("./drive/beep");
	add_file_directory("../drive/led");
	add_file_directory("./drive/led");
#endif
	config_init(find_file("port.conf"));
	
	if ((sockfd = (socket(AF_INET, SOCK_STREAM, 0))) == -1) {
		print_error(ESYSERR,"socket");
		goto err;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(get_armserver_port());
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);
	size = sizeof(struct sockaddr);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&bind_ret, sizeof(bind_ret)) == -1) {
		print_error(ESYSERR, "setsockopt");
		goto err;
	}
	if (bind(sockfd, (struct sockaddr *)&serv_addr, size) == -1) {
		print_error(ESYSERR, "bind");
		goto err;
	}
	if (listen(sockfd, LISTEN_NUMS) == -1) {
		print_error(ESYSERR, "listen");
		goto err;
	}
	return;
err:
	munmap(login_user, USER_NAME_LEN);
	linuxarms_print("close armserver...\n");
	delete_file_directory();
}

int wait_user_connect(void)
{
	int user;
	size_t size;
	size = sizeof(struct sockaddr);
	if ((user = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&size)) == -1)
		print_error(ESYSERR, "accept");
	return user;
}
void tcp_set_login_address()
{
	login_addr = client_addr;
}
boolean have_login_user(int tcp)
{
	struct amthread_trans amtrans;

	if (strlen(login_user) > 0) {
		anet_recv(tcp, &amtrans, sizeof(struct amthread_trans));
		amthread_trans_set_protocol(&amtrans, HAVEUSER);
		anet_send(tcp, &amtrans, sizeof(struct amthread_trans));
		return TRUE;
	}
	return FALSE;
}
boolean create_tcp_connect(int fds[TCP_CONNECT_NUMS])
{
	if ((fds[1] = wait_user_connect()) == -1)
		return FALSE;
	if (strcmp(inet_ntoa(login_addr.sin_addr), inet_ntoa(client_addr.sin_addr)) != 0)
		return FALSE;
	if ((fds[2] = wait_user_connect()) == -1)
		return FALSE;
	if (strcmp(inet_ntoa(login_addr.sin_addr), inet_ntoa(client_addr.sin_addr)) != 0)
		return FALSE;
	if ((fds[3] = wait_user_connect()) == -1)
		return FALSE;
	if (strcmp(inet_ntoa(login_addr.sin_addr), inet_ntoa(client_addr.sin_addr)) != 0)
		return FALSE;
	return TRUE;
}
void create_session(int tcps[TCP_CONNECT_NUMS])
{
	linuxarms_print("one client request,we create "
			"process %d to handle it...\n", getpid());
	strcpy(hostclient_ip, inet_ntoa(client_addr.sin_addr));
	struct login_struct login;
	struct amthread_struct amthread;
	struct linuxarms_struct linuxarms;

	/* 初始化asthread结构体 */	
	struct assinfo_struct assinfo;
	struct asprocess_struct asprocess;
	struct asthread_struct asthread;	
	/* asprocess.kill = &asthread->trans.kill */
	asprocess_init(&asprocess, &asthread.trans.kill, &asthread.socket);
	assinfo_init(&assinfo, &asthread.socket);
	asthread_init(&asthread, &assinfo, &asprocess);
	asthread.socket.tcp = tcps[ASTHREAD_TCP_FD];

	/* 初始化afthread结构体 */
	struct afview_struct afview;
	struct atthread_struct atthread;
	struct afthread_struct afthread;
	/* afview->path = afthread->trans.path */
	afview_init(&afview, afthread.trans.path, &afthread.socket);	
	atthread_init(&atthread, &afthread.socket);
	afthread_init(&afthread, &afview, &atthread);
	afthread.socket.tcp = tcps[AFTHREAD_TCP_FD];
	

	/* 初始化acthread结构体 */
	struct acthread_struct acthread;
	acthread_init(&acthread);
	acthread.socket.tcp = tcps[ACTHREAD_TCP_FD];
	
	/* 初始化amthread结构体 */
	login_init(&login, &amthread.socket);
	user_struct_init(&login.user);
	amthread_init(&amthread, &login.user);
	anet_init(&amthread.socket, get_localhost_ip(), get_armserver_port());
	amthread.socket.tcp = tcps[AMTHREAD_TCP_FD];

	linuxarms.login = &login;
	linuxarms.amthread = &amthread;
	linuxarms.afthread = &afthread;
	linuxarms.asthread = &asthread;
	linuxarms.acthread = &acthread;
	debug_where();
	amthread_thread(&linuxarms);
}

void armserver_signal_handle(int sig)
{
	int childret;
	switch (sig) {
	case SIGINT:
		munmap(login_user, USER_NAME_LEN);
		linuxarms_print("close armserver...\n");
		delete_file_directory();
		close(sockfd);
		kill(0, SIGTERM);
		exit(1);
		break;
	case SIGCHLD:
		wait(&childret);
		linuxarms_print("login user %s exit...\n", login_user);
		memset(login_user, '\0', USER_NAME_LEN);
		break;
	case SIGTERM:
		linuxarms_print("close session\n");
		exit(0);
		break;
	}
}

