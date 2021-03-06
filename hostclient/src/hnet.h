#ifndef _HNET_H
#define _HNET_H
#include "linuxarms.h"
#define IP_LEN 16

#define TCP_CONNECT_NUMS 4
#define HMTHREAD_TCP_FD 0
#define HSTHREAD_TCP_FD 1
#define HFTHREAD_TCP_FD 2
#define HCTHREAD_TCP_FD 3
/*
 * 用于建立网络连接
 * @ip:	armserver的ip地址
 * @port: armserver的端口
 * @tcp: 指向网络连接的文件指针(socket)
 */
struct hnet_struct {
	char ip[IP_LEN];
	int port;
	int tcp;
};
/* 初始化hnet_struct结构 */
boolean hnet_init(struct hnet_struct *hnet, const char *ip, int port);
/* 创建一个TCP连接的客户端 */
boolean create_tcp_client(struct hnet_struct *hnet);
/* 发送数据 */
boolean hnet_send(int tcp, void *data, unsigned int len);
/* 接收数据 */
boolean hnet_recv(int tcp, void *data, unsigned int len);
/* 关闭TCP连接 */
boolean close_tcp_client(struct hnet_struct *hnet);
boolean create_tcp_connect(int tcps[TCP_CONNECT_NUMS]);
int wait_hfthread_connect();
extern char armserver_ip[IP_LEN]; 
const char *get_armserver_ip();
#endif
