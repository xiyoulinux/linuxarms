/*
 * linuxarms/hostclient/src/hnet.c
 * 建立网络客户端
 * Niu Tao<niutao0602@gmail.com>
 */
#define __DEBUG__
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <gtk/gtk.h>

#include "hnet.h"
#include "linuxarms.h"
#include "error.h"
#include "debug.h"
#include "config.h"
/*initialized in clogin.c, function: cb_login_ok_clicked*/
char armserver_ip[IP_LEN];
static struct sockaddr_in serv_addr;
/*
 * 初始化struct hnet_struct 结构。如果参数hnet或者ip为空，则返回错误
 * @hnet:  要初始化的数据结构
 * @ip:    用户输入的armserver端的ip地址
 * @port:  建立网络连接时使用的端口
 */
boolean hnet_init(struct hnet_struct *hnet, const char *ip, int port)
{
	if (!hnet) {
		debug_where();
		print_error(ESYSERR, "hnet is null");
		return FALSE;
	}
	strcpy(hnet->ip, (!ip || strlen(ip) > IP_LEN) ? "0.0.0.0" : ip);
	hnet->port = port;
	hnet->tcp = -1;
	return TRUE;
}
/*
 * 建立tcp客户端
 */
boolean create_tcp_client(struct hnet_struct *hnet)
{
	int sin_size;
	
	if ((hnet->tcp = (socket(AF_INET, SOCK_STREAM, 0))) == -1) {
		print_error(ESYSERR,"socket");
		goto out;
	}
	debug_print("create tcp client\n");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(hnet->port);
	serv_addr.sin_addr.s_addr = inet_addr(hnet->ip);
	bzero(&(serv_addr.sin_zero), 8);
	sin_size = sizeof(struct sockaddr);

	if ((connect(hnet->tcp, (struct sockaddr *)&serv_addr, sin_size)) == -1) {
		close(hnet->tcp);
		print_error(ESYSERR, "connect");
		goto out;
	}
	debug_print("create tcp client success\n");

	return TRUE;
out:	/* 无效的网络连接描述符 */
	hnet->tcp = -1;
	return FALSE;
}
static int wait_create_connect(void)
{
	size_t sin_size;
	int tcp = -1;
	if ((tcp = (socket(AF_INET, SOCK_STREAM, 0))) == -1)
		return -1;
	sin_size = sizeof(struct sockaddr);
	if (connect(tcp, (struct sockaddr *)&serv_addr, sin_size) == -1)
		return -1;
	return tcp;
}
boolean create_tcp_connect(int tcps[TCP_CONNECT_NUMS])
{
	struct hnet_struct hnet;
	debug_where();
	if ((tcps[HSTHREAD_TCP_FD] = wait_create_connect()) == -1)
		return FALSE;
	debug_where();
	if ((tcps[HFTHREAD_TCP_FD] = wait_create_connect()) == -1)
		return FALSE;
	if ((tcps[HCTHREAD_TCP_FD] = wait_create_connect()) == -1)
		return FALSE;
	return TRUE;
}
/*
 * 关闭网络连接
 */
boolean close_tcp_client(struct hnet_struct *hnet)
{
	if (!hnet) {
		debug_where();
		print_error(ENULL, "无效的数据");
		return FALSE;
	}
	if (hnet->tcp != -1)
		shutdown(hnet->tcp, SHUT_RDWR);
		//close(hnet->tcp);
	hnet->tcp = -1;
	return TRUE;
}
/*
 * 发送数据
 * @tcp:    网络连接描述符
 * @data:   要发送的数据
 * @len:    要发送数据的长度
 * @return: 如果给出的参数 出错或者发送失败则返回FALSE.
 */
boolean hnet_send(int tcp, void *data, unsigned int len)
{
	if (tcp < 0 || data == NULL) {
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
	if ((send(tcp, data, len, 0)) == -1) {
		print_error(EWARNING, "发送数据失败");
		perror("send");
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
boolean hnet_recv(int tcp, void *data, unsigned int len)
{
	if (tcp < 0 || data == NULL) {
		print_error(EWARNING, "接收失败，没有建立TCP连接");
		goto out;
	}
	if (!len) {
		print_error(EWARNING,"要接收的数据长度为0");
		goto out;
	}
	if (tcp == -1) {
		print_error(ENOSOCKET, "无效的网络连接描述符");
		goto out;

	}
	if ((recv(tcp, data, len, 0)) <= 0) {
		goto out;
	}
	return TRUE;
out:
	return FALSE;
}

const char *get_armserver_ip()
{
	if (strlen(armserver_ip) > 0)
		return armserver_ip;
	return "0.0.0.0";
}
