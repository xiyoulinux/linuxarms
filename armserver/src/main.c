#include "amthread.h"
#include "login.h"
#include "support.h"
#include "config.h"
#include "debug.h"
#include "error.h"
#include <sys/mman.h>
int *user_process;
int *have_user;

int main(int args, char *argv[])
{
	int size;
	int socket_fd;
	int tcp_fd;
	struct sockaddr_in serv_addr,client_addr;
	char *config_file;
	have_user = (int *)mmap(NULL,sizeof(int),
			PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
	add_file_directory("/etc/linuxarms-armserver/config");
	add_file_directory("/usr/share/linuxarms-armserver/drive/beep");
	add_file_directory("/usr/share/linuxarms-armserver/drive/led");
/* if we are debug the program,we can use this config and resource file 
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
	debug_print("create main tcp server to accept tcp request\n");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(get_linuxarms_port());
	serv_addr.sin_addr.s_addr = inet_addr(get_localhost_ip());
	bzero(&(serv_addr.sin_zero), 8);
	size = sizeof(struct sockaddr);

	if (bind(socket_fd, (struct sockaddr *)&serv_addr, size) == -1) {
		print_error(ESYSERR, "bind");
		goto err;
	}
	if (listen(socket_fd, 2) == -1) {
		print_error(ESYSERR, "listen");
		goto err;
	}
	while (TRUE) {
		if (tcp_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &size)) {
			print_error(ESYSERR, "accept");
			goto err;
		}
		user_process = fork();
		if (user_process == 0) {
			/*struct amthread_struct amthread
			linuxarms_thread_create();*/
		} else {
			close(tcp_fd);
		}
	}
	
err:
	delete_file_directory();
	return 0;

}
