//#define __DEBUG__
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "linuxarms.h"
#include "login.h"
#include "init.h"
#include "debug.h"
#include "error.h"

int main(int args, char *argv[])
{
	int child;
	int user;
	int tcps[TCP_CONNECT_NUMS];
	check_user_permission();
	signal(SIGINT, armserver_signal_handle);
	signal(SIGCHLD, armserver_signal_handle);
	armserver_init();
	
	linuxarms_print("create main tcp server to accept tcp request...\n");
	while (TRUE) {
		user = wait_user_connect();
		if (user == -1)
			kill(getpid(), SIGINT);
		if (have_login_user(user)) {
			debug_print("已经有一个用户登录\n");
			close(user);
			continue;
		}
		tcp_set_login_address();
		if (!create_tcp_connect(tcps)) {
			print_error(EWARNING, "建立连接错误");
			continue;
		}
		tcps[AMTHREAD_TCP_FD] = user;
		debug_where();
		/* create a process to handle this request. */
		child = fork();
		if (child == -1) {
			/* create process error, close armserver */
			kill(getpid(), SIGINT);
		} else if (child == 0) {
			/* in child process, create a session to response request */
			signal(SIGTERM, armserver_signal_handle);
			create_session(tcps);
		} else {
			/* in father process, close connection,  
			 * and continue to wait for user connections
			 */
			close(tcps[AMTHREAD_TCP_FD]);
			close(tcps[ASTHREAD_TCP_FD]);
			close(tcps[AFTHREAD_TCP_FD]);
			close(tcps[ACTHREAD_TCP_FD]);
		}
	}
	return 0;
}
