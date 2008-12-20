#ifndef _HSTHREAD_H
#define _HSTHREAD_H
#include "protocol.h"
#include "hnet.h"
#include <gtk/gtk.h>
/*
 * 定时更新的时间间隔
 */
typedef enum _Timeout {
	TM_THREE = 3,
	TM_FIVE = 5,
}timer_time;
#define HSTHREAD_IS_TIMER_TIME(time) (time >= TM_THREE && time <= TM_FIVE)
/*
 * hsthread_timer 定时器
 * @time:  定时时间
 * @timer: 指向定时器
 */
struct hsthread_timer {
	int time;
	int timer;
};
/*
 * hsthread_trans 系统信息显示和实时监视(进程信息显示)线程
 * 		  传送的数据
 * @protocol:  控制(见protocol_sthread)
 * @kill:  要杀死的进程的进程号(当ctrl = KILL时，该域才有效)
 */
struct hsthread_trans {
	protocol_sthread protocol;
	int kill;
};

struct hssinfo_struct;
struct hsprocess_struct;
struct hnet_struct;
/*
 * hsthread_struct  系统信息显示和实时监视(进程信息显示)线程
 *                  主数据结构
 * @ssinfo:    系统信息显示主数据结构
 * @sprocess:  实时监视主数据结构
 * @trans:     线程中传输的数据
 * @socket:    数据传输数据结构
 * @lock:      互斥访问锁
 * @timer:     定时更新的时间间距
 */
struct hsthread_struct {
	struct hssinfo_struct *ssinfo;
	struct hsprocess_struct *sprocess;
	struct hsthread_trans trans;
	struct hnet_struct socket;
	struct hsthread_timer timer;

	boolean (*send)(struct hsthread_struct *hsthread);
	boolean (*recv)(struct hsthread_struct *hsthread);
	boolean lock;
};
/* 初始化hsthread_struct结构体 */
boolean hsthread_init(struct hsthread_struct *hsthread,
		      struct hssinfo_struct *ssinfo,
		      struct hsprocess_struct *sprocess,
		      struct hsthread_trans *trans,
		      struct hnet_struct *socket,
		      struct hsthread_timer *timer);
/* 系统信息显示和进程信息显示线程执行体 */
gboolean hsthread_thread(void *p);
/* 
 * 发送一个信息给armserver，其中kill为要杀死的进程的id，
 * 如果不是杀死进程，则忽略该参数(设置为-1).
 */
boolean hsthread_send(struct hsthread_struct *hsthread);
boolean hsthread_set_timer_time(struct hsthread_struct *hsthread, timer_time time);
boolean hsthread_create_timer(struct hsthread_struct *hsthread);
boolean hsthread_close_timer(struct hsthread_struct *hsthread);
/* 接收armserver发送过来的信息 */
boolean hsthread_recv(struct hsthread_struct *hsthread);
boolean hsthread_set_trans(struct hsthread_struct *hsthread,
			   protocol_sthread protocol, int kill);
#endif
