/*
 * linuxarms/hostclient/src/hcsthread.c
 * Niu Tao <niutao0602@gmail.com>
 */
#define __DEBUG__

#include <stdlib.h>
#include <unistd.h>
#include "linuxarms.h"
#include "hsthread.h"
#include "hnet.h"
#include "ssinfo.h"
#include "sprocess.h"
#include "error.h"
#include "debug.h"
#include "config.h"
#include "login.h"
#include "statusbar.h"

static void hsthread_down_lock(struct hsthread_struct *hsthread);
static void hsthread_up_lock(struct hsthread_struct *hsthread);
static boolean hsthread_send(struct hsthread_struct *hsthread);
static boolean hsthread_recv(struct hsthread_struct *hsthread);
static boolean hsthread_set_protocol(struct hsthread_struct *hsthread,
					protocol_sthread protocol);
/*
 * 初始化hsthread_struct结构
 * @return:  如果有一个参数为空，则返回FALSE.
 */
boolean hsthread_init(struct hsthread_struct *hsthread,
		      struct hssinfo_struct *hssinfo,
		      struct hsprocess_struct *hsprocess)
{
	if (!hsthread || !hssinfo || !hsprocess) {
		debug_where();
		print_error(ESYSERR, "初始化数据为空");
		return FALSE;
	}
	hsthread->thread = NULL;
	hsthread->hssinfo   = hssinfo;
	hsthread->hsprocess = hsprocess;
	
	hsthread_trans_init(&hsthread->trans);
	//hnet_init(&hsthread->socket, get_armserver_ip(), get_sthread_port());
	hsthread_timer_init(&hsthread->timer);
	hsthread->set_protocol = hsthread_set_protocol;
	hsthread->down_lock = hsthread_down_lock;
	hsthread->up_lock = hsthread_up_lock;
	hsthread->send     = hsthread_send;
	hsthread->recv     = hsthread_recv;
	
	hsthread->up_lock(hsthread);
	return TRUE;
}
/*
 * 线程主执行体，负责传输arm系统信息和进程信息，并且在用户请求杀死
 * 进程的时候，传输杀死进程所需信息
 * @return:  如果该函数执行返回，则线程执行结束
 */
boolean hsthread_thread(void *p)
{
	struct hsthread_struct *hsthread = (struct hsthread_struct *)p;
	
	linuxarms_print("create hsthread thread...\n");
	hsthread->thread = linuxarms_thread_self();
	hnet_init(&hsthread->socket, get_armserver_ip(), get_sthread_port());
	if (!create_tcp_client(&hsthread->socket)) {
		print_error(ESYSERR,"create tcp error");
		return FALSE;
	}
	debug_print("hsthread socket ip : %s tcp: %d port: %d\n", hsthread->socket.ip,
				hsthread->socket.tcp, hsthread->socket.port);	

	hsthread->set_protocol(hsthread, SSYSINFO);
	//hsthread->send(hsthread);
	hsthread_create_timer(hsthread); 
	while (hsthread->thread) {
		if (!hsthread->recv(hsthread)) {
			linuxarms_print("hsthread recive data error\n");
			break;
		}
		hsthread->down_lock(hsthread);
		switch (hsthread->trans.protocol) {
		case SSYSINFO:
			/*显示系统信息*/
			debug_print("protocol->hsthread: 显示系统信息\n");
			hsthread->set_protocol(hsthread, SSYSINFO);
			break;
		case SPROCESS:
			/*显示进程信息*/
			debug_print("protocol->hsthread: 显示进程信息\n");
			do_show_process(hsthread->hsprocess);
			hsthread->set_protocol(hsthread, SPROCESS);
			break;
		case SKILLSUC:
			/* 接收到的成功杀死进程反馈信息 */
			debug_print("protocol->hsthread: 杀死进程成功\n");
			if (hsthread->timer.timer == -1)
				hsthread_create_timer(hsthread);
			hsthread_kill_success(hsthread);
			hsthread->set_protocol(hsthread, SPROCESS);
			break;
		case SKILLERR:
			/* 接收到的杀死进程失败反馈信息 */
			debug_print("protocol->hsthread: 杀死进程失败\n");
			if (hsthread->timer.timer == -1)
				hsthread_create_timer(hsthread);
			statusbar_set_text("杀死进程失败");
			hsthread->set_protocol(hsthread, SPROCESS);
			break;
		case SMAX:
			debug_print("protocol->hsthread: 无效协议\n");
		default:
			break;
		}
		hsthread->up_lock(hsthread);
	}
	return TRUE;
}
/*
 * 设置要传输的数据
 * @protocol:  要发送的协议
 * @kill:      要杀死的进程的进程号，如果进程号无效，则置为-1
 * @return:    成功设置好则则返回TRUE.
 */
boolean hsthread_set_trans(struct hsthread_struct *hsthread, 
			   protocol_sthread protocol, int kill)
{
	LINUXARMS_POINTER(hsthread);
	if (!PROTOCOL_IS_STHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议，设置发送失败");
		return FALSE;
	}
	if (kill < -1) {
		kill = -1;
	}
	hsthread->trans.protocol = protocol;
	hsthread->trans.kill = kill;
	return TRUE;
}

static void hsthread_down_lock(struct hsthread_struct *hsthread)
{
	if (!hsthread)
		return ;
	while (hsthread->lock)
		sleep(1);
	hsthread->lock = TRUE;
}
static void hsthread_up_lock(struct hsthread_struct *hsthread)
{
	if (!hsthread)
		return;
	hsthread->lock = FALSE;
}

/* 
 * 发送一个信息给armserver，其中kill为要杀死的进程的id，
 * 如果不是杀死进程，则忽略该参数(设置为-1).
 */
static boolean hsthread_send(struct hsthread_struct *hsthread)
{
	LINUXARMS_POINTER(hsthread);
	if (hsthread->lock)
		return FALSE;
	return hnet_send(hsthread->socket.tcp, (void *)&hsthread->trans, 
			 sizeof(struct hsthread_trans));
}

/* 接收armserver发送过来的信息 */
static boolean hsthread_recv(struct hsthread_struct *hsthread)
{
	LINUXARMS_POINTER(hsthread);
	return hnet_recv(hsthread->socket.tcp, (void *)&hsthread->trans, 
			sizeof(struct hsthread_trans));
}

/*
 * 定时器函数，功能是定时向armserver发送更新信息
 * (系统信息和进程信息)请求.
 * @data:    struct hsthread_struct.
 * @return:  如果返回FALSE，则结束定时器。
 */
gboolean hsthread_timer(gpointer data)
{
	struct hsthread_struct *hsthread = (struct hsthread_struct *)data;
	if (!hsthread->lock) {
		hsthread->send(hsthread);
		debug_print("hsthread send data to armserver...\n");
	}
	debug_print("定时器在运行中 %d\n", hsthread->timer.timer);
	return TRUE;
}

boolean hsthread_timer_init(struct hsthread_timer *hstimer)
{
	LINUXARMS_POINTER(hstimer);
	hstimer->time = TM_FIVE *1000;
	hstimer->timer = -1;
	return TRUE;
}
/*
 * 设置定时器定时时间，如果给出的定时时间错误，则设置为默认值(3秒)
 */
boolean hsthread_set_timer_time(struct hsthread_struct *hsthread, timer_time time)
{
	LINUXARMS_POINTER(hsthread);
	if (!(HSTHREAD_IS_TIMER_TIME(time))) {
		print_error(EWARNING, "定时器时间错误");
		hsthread->timer.time = TM_THREE * 1000;
	} else {
		hsthread->timer.time = time * 1000;
	}
	return TRUE;

}
/*
 * 创建一个定时器
 */
boolean hsthread_create_timer(struct hsthread_struct *hsthread)
{
	LINUXARMS_POINTER(hsthread);
	hsthread->timer.timer = gtk_timeout_add(hsthread->timer.time, hsthread_timer, hsthread);
	debug_print("创建定时器\n");
	return TRUE;
	
}
boolean hsthread_close_timer(struct hsthread_struct *hsthread)
{
	LINUXARMS_POINTER(hsthread);
	gtk_timeout_remove(hsthread->timer.timer);
	hsthread->timer.timer = -1;
	debug_print("删除定时器\n");
	return TRUE;
}

static boolean hsthread_set_protocol(struct hsthread_struct *hsthread,
					protocol_sthread protocol)
{
	LINUXARMS_POINTER(hsthread);
	return hsthread_trans_set_protocol(&hsthread->trans, protocol);
}
boolean hsthread_trans_init(struct hsthread_trans *hstrans)
{
	LINUXARMS_POINTER(hstrans);
	hstrans->protocol = SMAX;
	hstrans->kill = -1;
	return TRUE;
}
boolean hsthread_trans_set_protocol(struct hsthread_trans *hstrans, 
					protocol_sthread protocol)
{
	LINUXARMS_POINTER(hstrans);
	if (!PROTOCOL_IS_STHREAD(protocol)) {
		hstrans->protocol = FMAX;
		return FALSE;
	}
	hstrans->protocol = protocol;
	return TRUE;
}
boolean hsthread_kill_success(struct hsthread_struct *hsthread)
{
	LINUXARMS_POINTER(hsthread);
	boolean select;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	
	debug_where();
	selection = gtk_tree_view_get_selection(
		GTK_TREE_VIEW(hsthread->hsprocess->widget.treeview));
	select = gtk_tree_selection_get_selected(selection, &list_store, &iter);
	if (select) {
		gtk_list_store_remove(GTK_LIST_STORE(list_store),&iter);
		return TRUE;
	}
	return FALSE;
}
