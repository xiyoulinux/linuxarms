//#define __DEBUG__

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

#include "linuxarms.h"
#include "amthread.h"
#include "afthread.h"
#include "asthread.h"
#include "acthread.h"
#include "atthread.h"
#include "login.h"
#include "support.h"
#include "config.h"
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

char *login_user;

int main(int args, char *argv[])
{
	int user_process;
	int size;
	int socket_fd, bind_ret = 1;
	int tcp_fd;
	struct sockaddr_in serv_addr,client_addr;
	char *config_file;
	
#ifdef GTK_THREAD
	if (!g_thread_supported())
		g_thread_init(NULL);
#endif
	if (getuid() != 0) {
		linuxarms_print("Sorry, we must start armserver with root...\n");
		return 1;
	}
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
	config_file = find_file("port.conf");
	config_init(config_file);
	
	if ((socket_fd = (socket(AF_INET, SOCK_STREAM, 0))) == -1) {
		print_error(ESYSERR,"socket");
		goto err;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(get_mthread_port());
	//serv_addr.sin_addr.s_addr = inet_addr(get_localhost_ip());
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);
	size = sizeof(struct sockaddr);
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&bind_ret, sizeof(bind_ret)) == -1) {
		print_error(ESYSERR, "setsockopt");
		goto err;
	}
	if (bind(socket_fd, (struct sockaddr *)&serv_addr, size) == -1) {
		print_error(ESYSERR, "bind");
		goto err;
	}
	if (listen(socket_fd, 2) == -1) {
		print_error(ESYSERR, "listen");
		goto err;
	}
	while (TRUE) {
		linuxarms_print("create main tcp server to accept tcp request...\n");
		if ((tcp_fd = accept(socket_fd, (struct sockaddr *)&client_addr, 
						(socklen_t *)&size)) == -1) {
			print_error(ESYSERR, "accept");
			goto err;
		}
		debug_where();
		user_process = fork();
		if (user_process == 0) {
			linuxarms_print("one client request,we create "
					"process %d to handle it...\n", getpid());
			strcpy(hostclient_ip, inet_ntoa(client_addr.sin_addr));
		        struct login_struct login;
			struct amthread_struct amthread;
			struct linuxarms_struct linuxarms;

			/* 初始化afthread结构体 */
			struct afview_struct afview;
			struct atthread_struct atthread;
			struct afthread_struct afthread;
			/* afview->path = afthread->trans.path */
			afview_init(&afview, afthread.trans.path, &afthread.socket);			
			atthread_init(&atthread, &afthread.socket);
			afthread_init(&afthread, &afview, &atthread);
			
			/* 初始化asthread结构体 */			
			struct assinfo_struct assinfo;
			struct asprocess_struct asprocess;
			struct asthread_struct asthread;			
			/* asprocess.kill = &asthread->trans.kill */
			asprocess_init(&asprocess, &asthread.trans.kill, &asthread.socket);
			assinfo_init(&assinfo, &asthread.socket);
			asthread_init(&asthread, &assinfo, &asprocess);

			/* 初始化acthread结构体 */
			struct acthread_struct acthread;
			acthread_init(&acthread);
			
			/* 初始化amthread结构体 */
			login_init(&login, &amthread.socket);
			user_struct_init(&login.user);
			amthread_init(&amthread, &login.user);
			anet_init(&amthread.socket, get_localhost_ip(), get_mthread_port());
			amthread.socket.tcp = tcp_fd;

			linuxarms.login = &login;
			linuxarms.amthread = &amthread;
			linuxarms.afthread = &afthread;
			linuxarms.asthread = &asthread;
			linuxarms.acthread = &acthread;
			debug_where();
			amthread_thread(&linuxarms);
		} else {
			debug_print("main: close tcp\n");
			close(tcp_fd);
		}
	}
err:
	munmap(login_user, USER_NAME_LEN);
	linuxarms_print("close armserver...\n");
	delete_file_directory();
	return 0;
}
