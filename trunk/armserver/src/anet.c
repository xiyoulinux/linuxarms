/*
 * linuxarms/hostclient/src/hnet.c
 * 建立网络客户端
 * Niu Tao<niutao0602@gmail.com>
 */
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<net/if.h>
#include<arpa/inet.h>
#include<netdb.h>
#include "anet.h"
#include "linuxarms.h"
#include "error.h"
#include "debug.h"

/*
 * 初始化struct anet_struct 结构。如果参数anet或者ip为空，则返回错误
 * @anet:  要初始化的数据结构
 * @ip:    hostcleint端的ip地址
 * @port:  建立网络连接时使用的端口
 */
boolean anet_init(struct anet_struct *anet, char *ip, int port)
{
	if (!anet || !ip) {
		debug_where();
		print_error(ESYSERR,"anet or ip is null");
		return FALSE;
	}

	strncpy(anet->ip, ip, 15);
	anet->port = port;
	return TRUE;
}
/*
 * 建立tcp服务器端
 */
boolean create_tcp_server(struct anet_struct *anet)
{
	int size;
	int socket;
	struct sockaddr_in serv_addr,client_addr;
	
	if ((socket = (socket(AF_INET, SOCK_STREAM, 0))) == -1) {
		print_error(ESYSERR,"socket");
		goto out;
	}
	debug_print("create tcp server\n");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(anet->port);
	serv_addr.sin_addr.s_addr = inet_addr(anet->ip);
	bzero(&(serv_addr.sin_zero), 8);
	sin_size = sizeof(struct sockaddr);

	if (bind(socket, (struct sockaddr *)&serv_addr, size) == -1) {
		print_error(ESYSERR, "bind");
		goto label;
	}
	if (listen(socket, 1) == -1) {
		print_error(ESYSERR, "listen");
		goto label;
	}

	if (anet->tcp = accept(socket, (struct sockaddr *)&client_addr, &size)) {
		print_error(ESYSERR, "accept");
		goto label;
	}
	debug_print("create tcp client success\n");

	return TRUE;
label:
	close(socket);
out:
	return FALSE;
}
/*
 * 关闭服务器端
 */
boolean close_tcp_server(struct anet_struct *anet)
{
	if (!anet) {
		debug_where();
		print_error(ENULL, "无效的数据");
		return FALSE;
	}
	if (anet->tcp != -1)
		close(anet->tcp);
	return TRUE;
}

/*
 * 发送数据
 * @tcp:    网络连接描述符
 * @data:   要发送的数据
 * @len:    要发送数据的长度
 * @return: 如果给出的参数 出错或者发送失败则返回FALSE.
 */
boolean anet_send(int tcp, void *data, unsigned int len)
{
	if (tcp < 0 || !data) {
		print_error(EWARNING, "发送失败，没有建立TCP连接或者"
				"发送数据为空");
		goto out;
	}
	if (!len) {
		print_error(EWARNING, "要发送的数据长度为0");
		goto out;
	}
	if (tcp == -1) {
		print_error(ENOSOCKET, "无效的网络连接描述符");
		goto out;

	}
	if (send(tcp, data, len, 0) == -1) {
		print_error(EWARNING, "发送数据失败");
		goto out;
	}

	return TRUE;
out:
	return FALSE;
}

/*
 * 接收数据
 * @tcp:    网络连接描述符
 * @data:   要接收的的数据存放缓冲区
 * @len:    要接收的数据的长度
 * @return: 如果给出的参数 出错或者发送失败则返回FALSE.
 */
boolean anet_recv(int tcp, void *data,unsigned int len)
{
	if (tcp < 0 || !data) {
		print_error(EWARNING,  "接收失败，没有建立TCP连接");
		goto out;
	}
	if (!len) {
		print_error(EWARNING, "要接收的数据长度为0");
		goto out;
	}
	if (tcp == -1) {
		print_error(ENOSOCKET,  "无效的网络连接描述符");
		goto out;

	}
	if (recv(tcp, data, len, 0) == -1) {
		print_error(EWARNING, "接收数据失败");
		goto out;
	}
	return TRUE;
out:
	return FALSE;
}
