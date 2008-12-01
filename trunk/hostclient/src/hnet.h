#ifndef _HNET_H
#define _HNET_H
#include "linuxarms.h"
/*
 * 用于建立网络连接
 * @ip:	armserver的ip地址
 * @port: armserver的端口
 * @tcp: 指向网络连接的文件指针(socket)
 */
struct hnet_struct {
	char ip[15];
	int port;
	int tcp;
};
/* 初始化hnet_struct结构 */
boolean hnet_init(struct hnet_struct *hnet, char *ip, int port);
/* 创建一个TCP连接的客户端 */
boolean create_tcp_client(struct hnet_struct *hnet);
/* 发送数据 */
boolean hnet_send(int tcp, void *data, unsigned int len);
/* 接收数据 */
boolean hnet_recv(int tcp, void *data, unsigned int len);
/* 关闭TCP连接 */
boolean close_tcp_client(struct hnet_struct *hnet);
#endif
