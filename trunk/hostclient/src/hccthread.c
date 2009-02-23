#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "hcthread.h"
#include "linuxarms.h"
#include "hfthread.h"
#include "hnet.h"
#include "login.h"
#include "config.h"
#include "sctrl.h"

/*
 * 初始化实时控制主数据结构
 *
 */
boolean hcthread_init(struct hcthread_struct *hcthread)
{
	LINUXARMS_POINTER(hcthread);

	linuxarms_thread_init(&hcthread->thread);
	hcthread->send = hcthread_send;
	hcthread->recv = hcthread_recv;

	hcthread->showinfo = showinfo;
	hcthread->competence = FALSE;

	return TRUE;
}
/*
 * 发送一个请求到armserver上
 */
boolean hcthread_send(struct hcthread_struct *hcthread)
{
	return hnet_send(hcthread->socket.tcp, 
			&hcthread->trans, sizeof(struct hcthread_trans));
}
/*
 *接受一个数据从armserver
 */
boolean hcthread_recv(struct hcthread_struct *hcthread)
{
	return hnet_recv(hcthread->socket.tcp, 
			&hcthread->trans, sizeof(struct hcthread_trans));
}

boolean showinfo(struct hcthread_struct *hcthread)
{
	/* 显示信息 */
	GtkWidget *textview = hcthread->widget.textview_ctrl;
	GtkTextBuffer *buffer;

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
	if (hcthread->trans.buffer) {
		gtk_widget_set_sensitive(hcthread->widget.entry_input, FALSE);
		gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(buffer), 
				 hcthread->trans.buffer, 
				 strlen(hcthread->trans.buffer));
		memset(hcthread->trans.buffer, '\0', 
				 strlen(hcthread->trans.buffer));
	}
	return TRUE;
}
/*
 *实时控制主线程执行体
 *@p:  struct hcthread_struct
 */ 
gboolean hcthread_thread(void *p)
{
	struct hcthread_struct *hcthread = (struct hcthread_struct *)p;
	linuxarms_print("create hcthread thread...\n");
	hcthread->thread.id = linuxarms_thread_self();
	/* 建立网络连接 */
	hnet_init(&hcthread->socket, get_armserver_ip(), get_cthread_port());
	create_tcp_client(&hcthread->socket);
	debug_print("hcthread socket ip : %s tcp: %d port: %d\n", hcthread->socket.ip,
				hcthread->socket.tcp, hcthread->socket.port);	
	while (hcthread->thread.id) {
		do {
			hcthread_recv(hcthread);        /* 接收从arm端输出过来的数据 */  
			if(hcthread->trans.protocol == CSEND) {
				hcthread->trans.protocol = CRECV;
				hcthread_send(hcthread);/* 发送协议CRECV */
				showinfo(hcthread);     /* 显示所接受的数据 */
			}
		} while (hcthread->trans.protocol != CSENDALL);
		hcthread_send(hcthread);                 /* 发送协议CRECVALL */
	}
	return TRUE;
}
