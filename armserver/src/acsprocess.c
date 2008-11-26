/*
 * linuxarms/armserver/src/acsprocess.c
 * 系统进程信息显示相关函数
 * Niu Tao<niutao0602@gmail.com>
 */
#include "asprocess.h"
#include "asthread.h"
#include "proc.h"
/*
 * 发送进程信息函数
 * @asthread:  
 * @process:
 */
boolean send_process_info(struct asthread_struct *asthread, 
			  struct asprocess_struct *process)
{
	/* 循环接收进程信息 */
	/* 每接收一条信息，就调用显示进程信息函数将信息
	 * 显示在图形界面上 
	 */
	/* 判断时候接收完毕，如果完毕，则退出循环 */
}
boolean read_process_info(struct proc_struct *proc)
{

}
/*
 * 杀死进程处理函数
 */
boolean kill_process(struct asthread_struct *asthread,
		     struct asprocess_struct *process)
{
	/* 如果返回值为KILLSUC(asthread->trans.ctrl = KILLSUC),
	 * 则表示成功杀死进程，则更新本地进程信息显示界面。
	 * 否则提示用户杀死进程失败
	 */
}
