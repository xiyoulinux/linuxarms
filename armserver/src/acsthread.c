/*
 * linuxarms/hostclient/src/hcsthread.c
 * Niu Tao <niutao0602@gmail.com>
 */

#define __DEBUG__
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include "asthread.h"
#include "anet.h"
#include "error.h"
#include "debug.h"
#include "config.h"
#include "assinfo.h"
#include "asprocess.h"
#include "proc.h"

static boolean asthread_set_protocol(struct asthread_struct *asthread, 
					protocol_sthread protocol);
static boolean asthread_send(struct asthread_struct *asthread);
static boolean asthread_recv(struct asthread_struct *asthread);
/*
 * 初始化asthread_struct结构
 */
boolean asthread_init(struct asthread_struct *asthread,
		      struct assinfo_struct *assinfo,
		      struct asprocess_struct *asprocess)
{
	LINUXARMS_POINTER(asthread);

	linuxarms_thread_init(&asthread->thread);
	asthread->assinfo   = assinfo;
	asthread->asprocess = asprocess;
	
	asthread->set_protocol = asthread_set_protocol;
	asthread->send = asthread_send;
	asthread->recv = asthread_recv;
	asthread->competence = FALSE;

	asthread_trans_init(&asthread->trans);
	anet_init(&asthread->socket, get_localhost_ip(), get_sthread_port());
	proc_init(&asthread->proc, STOP, SMAX);

	return TRUE;
}
/*
 * 线程主执行体
 */
boolean asthread_thread(void *p)
{
	struct asthread_struct *asthread = (struct asthread_struct *)p;
	struct asprocess_struct *asprocess = asthread->asprocess;
	
	linuxarms_print("create asthread_thread...\n");
	asthread->thread.id = linuxarms_thread_self();

	anet_init(&asthread->socket, get_localhost_ip(), get_sthread_port());
	if (!create_tcp_server(&asthread->socket)) {
		print_error(ESYSERR,"create tcp error");
		return FALSE;
	}
	debug_print("asthread socket ip : %s tcp: %d port: %d\n", asthread->socket.ip,
				asthread->socket.tcp, asthread->socket.port);
	while (asthread->thread.id) {
		if (!asthread->recv(asthread)) {
			linuxarms_print("asthread recv data error,exit....\n");
			exit(1);
		}
		switch (asthread->trans.protocol) {
		case SSYSINFO:
		case SPROCESS:
			debug_print("protocol->asthread: 显示系统信息或者进程信息\n");
			asthread->set_protocol(asthread, asthread->trans.protocol);
			asthread->send(asthread);
			/*显示进程信息*/
			asthread->proc.set_ctrl(&asthread->proc, asthread->trans.protocol);
			asthread->proc.set_state(&asthread->proc, CONTINUE);
			read_proc(asthread);
			break;
		case SKILL:
			debug_print("protocol->asthread: 杀死进程\n");
			if (asthread_kill_process(asthread->trans.kill)) {
				asthread->set_protocol(asthread, SKILLSUC);
				asthread->send(asthread);
				break;
			}
			asthread->set_protocol(asthread, SKILLERR);
			asthread->send(asthread);
			break;
		case SMAX:
			break;
		default:
			break;
		}
	}
	return TRUE;
}

boolean asthread_set_trans(struct asthread_struct *asthread, protocol_sthread protocol, int kill)
{
	LINUXARMS_POINTER(asthread);
	if (!PROTOCOL_IS_STHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议，设置发送失败");
		return FALSE;
	}
	if (kill < 0)
		kill = -1;
	asthread->trans.protocol = protocol;
	asthread->trans.kill = kill;
	return TRUE;
}

/* 
 * 发送一个信息给hostclient
 */
static boolean asthread_send(struct asthread_struct *asthread)
{
	LINUXARMS_POINTER(asthread);
	return anet_send(asthread->socket.tcp, (void *)&asthread->trans, 
		 sizeof(struct asthread_trans));
}

/* 接收hostclient发送过来的信息 */
static boolean asthread_recv(struct asthread_struct *asthread)
{
	LINUXARMS_POINTER(asthread);
	return anet_recv(asthread->socket.tcp, (void *)&asthread->trans, 
			sizeof(struct asthread_trans));
}

static boolean asthread_set_protocol(struct asthread_struct *asthread, 
					protocol_sthread protocol)
{
	LINUXARMS_POINTER(asthread);
	return asthread_trans_set_protocol(&asthread->trans, protocol);
}

boolean asthread_trans_init(struct asthread_trans *astrans)
{
	LINUXARMS_POINTER(astrans);
	astrans->protocol = SMAX;
	astrans->kill = -1;
	return TRUE;
}

boolean asthread_trans_set_protocol(struct asthread_trans *astrans, 
					protocol_sthread protocol)
{
	LINUXARMS_POINTER(astrans);
	if (!PROTOCOL_IS_STHREAD(protocol)) {
		astrans->protocol = FMAX;
		return FALSE;
	}
	astrans->protocol = protocol;
	return TRUE;
}

boolean asthread_kill_process(int pid)
{
	if (pid <= 0)
		return FALSE;
	if (kill(pid, SIGTERM) == -1)
		return FALSE;
	return TRUE;

}
