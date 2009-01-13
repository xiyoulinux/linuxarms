#ifndef _ANET_H
#define _ANET_H
#include "linuxarms.h"
/*
 * 用于建立网络连接
 * @client: 代表与armserver交互的hostclient
 * @ip:	armserver的ip地址
 * @port: armserver的端口
 * @tcp: 指向网络连接的文件指针(socket)
 */
struct anet_struct {
	char ip[15];
	int port;
	int tcp;
};
/* 初始化hnet_struct结构 */
boolean anet_init(struct anet_struct *anet, char *ip, int port);
/* 创建一个TCP连接的服务器端 */
boolean create_tcp_server(struct anet_struct *anet);
/* 发送数据 */
boolean anet_send(int tcp, void *data, unsigned int len);
/* 接收数据 */
boolean anet_recv(int tcp, void *data, unsigned int len);
/* 关闭TCP连接 */
boolean close_tcp_server(struct anet_struct *anet);
char *get_localhost_ip();
#endif
