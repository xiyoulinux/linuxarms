#ifndef _HNET_H
#define _HNET_H
#include "socket.h"
#include "linuxarms.h"
/*
 * 用于建立网络连接
 * @client: 代表与armserver交互的hostclient
 * @ip:	armserver的ip地址
 * @port: armserver的端口
 * @tcp: 指向网络连接的文件指针(socket)
 */
struct hnet_struct {
//	struct sockaddr_in client;
	char ip[15];
	int port;
	int tcp;
};

boolean anet_init(struct anet_struct *anet, char *ip, int port);
boolean create_tcp_server(struct anet_struct *anet);
boolean close_tcp_server(struct anet_struct *anet);
#endif
