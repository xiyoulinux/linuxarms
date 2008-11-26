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
		print_error(ESYSERR,"bind");
		goto label;
	}
	if (listen(socket, 1) == -1) {
		print_error(ESYSERR,"listen");
		goto label;
	}

	if (anet->tcp = accept(socket, (struct sockaddr *)&client_addr, &size)) {
		print_error(ESYSERR,"accept");
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
	if (!anet)
		return FALSE;
	close(anet->tcp);
	return TRUE;
}

boolean anet_send(int tcp, void *data)
{
	if (tcp < 0 || !data)
		return FALSE;
	if (send(tcp, data, strlen(data), 0) == -1)
		return FALSE;
	return TRUE;
}

boolean anet_recv(int tcp, void *data)
{
	if (tcp < 0 || !data)
		return FALSE;
	if (recv(tcp, data, strlen(data), 0) == -1)
		return FALSE;
	return TRUE;
}
