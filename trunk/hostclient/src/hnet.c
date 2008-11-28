/*
 * linuxarms/hostclient/src/hnet.c
 * 建立网络客户端
 * Niu Tao<niutao0602@gmail.com>
 */
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
//#include "linuxarms.h"
#include "error.h"
#include "debug.h"
/*
 * 初始化struct hnet_struct 结构。如果参数hnet或者ip为空，则返回错误
 */
boolean hnet_init(struct hnet_struct *hnet, char *ip, int port)
{
	if (!hnet || !ip) {
		debug_where();
		print_error(ESYSERR,"hnet or ip is null");
		return FALSE;
	}

	strncpy(hnet->ip, ip, 15);
	hnet->port = port;
	return TRUE;
}
/*
 * 建立tcp客户端
 */
boolean create_tcp_client(struct hnet_struct *hnet)
{
	gint sin_size;
	struct sockaddr_in serv_addr;
	
	if ((hnet->tcp = (socket(AF_INET, SOCK_STREAM, 0))) == -1) {
		print_error(ESYSERR,"socket");
		return FALSE;
	}
	debug_print("create tcp client\n");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(hnet->port);
	serv_addr.sin_addr.s_addr = inet_addr(hnet->ip);
	bzero(&(serv_addr.sin_zero), 8);
	sin_size = sizeof(struct sockaddr);

	if (connect(hnet->tcp, (struct sockaddr *)&serv_addr, sin_size) == -1) {
		close(hnet->tcp);
		print_error(ESYSERR, "connect");
		return FALSE;
	}
	debug_print("create tcp client success\n");

	return TRUE;
}
boolean close_tcp_client(struct hnet_struct *hnet)
{
	if (!hnet) {
		debug_where();
		print_error(ESYSERR, "无效的数据");
		return FALSE;
	}
	close(hnet->tcp);
	return TRUE;
}
boolean hnet_send(int tcp, void *data, unsigned int len)
{
	if (tcp < 0 || !data) {
		print_error(EWARNIN, "发送失败，没有建立TCP连接或者"
				"发送数据为空");
		return FALSE;
	}
	if (!len) {
		print_error(EWARNING,"要发送的数据长度为0");
		return FALSE;
	}
	if (send(tcp, data, strlen(data), 0) == -1)
		return FALSE;
	return TRUE;
}

boolean hnet_recv(int tcp, void *data,unsigned int len)
{
	if (tcp < 0 || !data) {
		print_error(EWARNIN, "接收失败，没有建立TCP连接");
		return FALSE;
	}
	if (!len) {
		print_error(EWARNING,"要接收的数据长度为0");
		return FALSE;
	}
	if (recv(tcp, data, len, 0) == -1)
		return FALSE;
	return TRUE;
}
