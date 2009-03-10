#ifndef _INIT_H
#define _INIT_H

#include "linuxarms.h"
#include "anet.h"

void check_user_permission(void);
void armserver_init(void);
int wait_user_connect(void);
boolean have_login_user(int tcp);
boolean create_tcp_connect(int fds[TCP_CONNECT_NUMS]);
void armserver_signal_handle(int sig);
void create_session(int tcps[TCP_CONNECT_NUMS]);
void tcp_set_login_address();
extern char *login_user;
#endif
