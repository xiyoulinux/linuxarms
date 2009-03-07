/*
 * linuxarms/hostclient/src/hnet.c
 * 建立网络客户端
 * Niu Tao<niutao0602@gmail.com>
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "anet.h"
#include "linuxarms.h"
#include "error.h"
#include "debug.h"
#define ERROR "读取本机ip出错"
#define MAXINTERFACES 16
static char armserver_ip[IP_LEN];
/* initialized in main.c */
char hostclient_ip[IP_LEN];
/*
 * 初始化struct anet_struct 结构。如果参数anet或者ip为空，则返回错误
 * @anet:  要初始化的数据结构
 * @ip:    hostcleint端的ip地址
 * @port:  建立网络连接时使用的端口
 */
boolean anet_init(struct anet_struct *anet, const char *ip, int port)
{
	if (!anet) {
		debug_where();
		print_error(ESYSERR, "anet is null");
		return FALSE;
	}
	strcpy(anet->ip, (!ip || strlen(ip) > IP_LEN) ? "0.0.0.0" : ip);
	anet->port = port;
	return TRUE;
}
/*
 * 建立tcp服务器端
 */
boolean create_tcp_server(struct anet_struct *anet)
{
	socklen_t size;
	int socket_fd, bind_ret = 1;
	struct sockaddr_in serv_addr,client_addr;
	
	if ((socket_fd = (socket(AF_INET, SOCK_STREAM, 0))) == -1) {
		print_error(ESYSERR,"socket");
		goto out;
	}
	debug_print("create tcp server\n");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(anet->port);
	serv_addr.sin_addr.s_addr = inet_addr(anet->ip);
	bzero(&(serv_addr.sin_zero), 8);
	size = sizeof(struct sockaddr);
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&bind_ret, sizeof(bind_ret)) == -1) {
		print_error(ESYSERR, "setsockopt");
		goto label;
	}
	if ((bind(socket_fd, (struct sockaddr *)&serv_addr, size)) == -1) {
		print_error(ESYSERR, "bind");
		goto label;
	}
	if ((listen(socket_fd, 1)) == -1) {
		print_error(ESYSERR, "listen");
		goto label;
	}

	if ((anet->tcp = accept(socket_fd, (struct sockaddr *)&client_addr, &size)) == -1) {
		print_error(ESYSERR, "accept");
		goto label;
	}
	debug_print("create tcp server success\n");

	return TRUE;
label:
	close(socket_fd);
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
		shutdown(anet->tcp, SHUT_RDWR);
		//close(anet->tcp);
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
	if ((send(tcp, data, len, 0)) == -1) {
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
	if ((recv(tcp, data, len, 0)) <= 0) {
		goto out;
	}
	return TRUE;
out:
	return FALSE;
}

/*
 * 读取armserver端的ip地址
 * @return: 如果正确获取，则返回ip地址，否则返回NULL
 */
const char *get_localhost_ip()
{
	int fd, interface;
	struct ifreq buf[MAXINTERFACES];
	struct ifconf ifc;

	if (strlen(armserver_ip) > 0)
		return armserver_ip;
	if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
		print_error(ESYSERR, ERROR);
		return NULL;
	}
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;
  	if (ioctl (fd, SIOCGIFCONF, (char *)&ifc) == -1) {
		print_error(ESYSERR, ERROR);
		goto out;
	}
	/* 获取接口信息 */
	interface = ifc.ifc_len / sizeof(struct ifreq);
	//printf("interface num is interface=%d\n",interface);
	/* 根据借口信息循环获取设备IP和MAC地址 */
	while (interface-- > 0) {
		/* 获取设备名称 */
		if (strcmp(buf[interface].ifr_name, "eth0") ||
		    strcmp(buf[interface].ifr_name, "eth1")) {
			//debug_print("net device %s\n", buf[interface].ifr_name);
			/* 判断网卡类型 */
			if ((ioctl (fd, SIOCGIFFLAGS, (char *)&buf[interface])) == -1) {
				print_error(ESYSERR, ERROR);
				goto out;
			}
			if (buf[interface].ifr_flags & IFF_PROMISC) {
				//debug_print("the interface is PROMISC\n");
			} else {
				//debug_print("cpm: ioctl device %s\n", buf[interface].ifr_name);
	 		}
			/* 判断网卡状态 */
			if (buf[interface].ifr_flags & IFF_UP) {
				//debug_print("the interface status is UP\n");
			} else {
				//debug_print("the interface status is DOWN\n");
			}
			/* 获取当前网卡的IP地址 */
			if (!(ioctl (fd, SIOCGIFADDR, (char *)&buf[interface]))) {
				//debug_print ("print IP address is: %s",
				//inet_ntoa(((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr));
				strncpy(armserver_ip, 
				inet_ntoa(((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr),
				16);
				close(fd);
				return armserver_ip;
			}
		}
	}
out:
	close (fd);
	return NULL;
}

const char *get_hostclient_ip()
{
	if (strlen(hostclient_ip) > 0)
		return hostclient_ip;
	return "0.0.0.0";
}
