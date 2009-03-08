//#define __DEBUG__
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "linuxarms.h"
#include "login.h"
#include "init.h"
#include "debug.h"

int main(int args, char *argv[])
{
	int child;
	int user;
	check_user_permission();
	signal(SIGINT, armserver_siganl_handle);
	signal(SIGCHLD, armserver_siganl_handle);
	armserver_init();
	
	linuxarms_print("create main tcp server to accept tcp request...\n");
	while (TRUE) {
		user = wait_user_connect();
		if (user == -1)
			kill(getpid(), SIGINT);
		debug_where();
		/* create a process to handle this request. */
		child = fork();
		if (child == -1) {
			/* create process error, close armserver */
			kill(getpid(), SIGINT);
		} else if (child == 0) {
			/* in child process, create a session to response request */
			create_session(user);
		} else {
			/* in father process, close connection,  
			 * and continue to wait for user connections
			 */
			close(user);
		}
	}
	return 0;
}
