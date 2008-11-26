/*
 * linuxarms/armserver/src/acsprocess.c
 * 系统进程信息显示相关函数
 * Niu Tao<niutao0602@gmail.com>
 */
#include "asprocess.h"
#include "asthread.h"
#include "proc.h"
/*
 * 进程信息显示主调模块
 */
boolean do_show_process(struct asthread_struct *asthread)
{
	/*
	 * 调用其他模块完成读取进程信息，发送进程信息，
	 * 接收反馈信息等。
	 */
}

boolean process_init(struct asprocess_struct *asprocess)
{
	if (!asprocess)
		return FALSE;
	asprocess->send = process_send_info;
	asprocess->recv = process_recv_info;
	return FALSE;
}
/*
 * 发送进程信息函数
 * @asthread:  
 */
boolean process_send_info(struct asprocess_struct *asprocess)
{
}

/*
 * 接收hostclient的反馈信息（是否成功接收完毕）
 */
boolean process_recv_info(struct asprocess_struct *asprocess)
{
}
/*
 * 从proc文件系统中读取进程信息
 */
boolean process_read_info(struct asprocess_struct *asprocess)
{

}
/*
 * 杀死进程处理函数
 */
boolean kill_process(struct asprocess_struct *asprocess)
{
}
