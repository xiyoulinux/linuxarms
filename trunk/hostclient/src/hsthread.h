#ifndef _HSTHREAD_H
#define _HSTHREAD_H
#include "protocol.h"
#include "hnet.h"

/*
 * 定时更新的时间间隔
 */
enum _Timeout {
	THREE,
	FIVE,
};
/*
 * hsthread_timer 定时器
 */
struct hsthread_timer {
	int time;
	int timer;
};
/*
 * hsthread_trans 系统信息显示和实时监视(进程信息显示)线程
 * 		  传送的数据
 * @ctrl:  控制(见protocol_hsthread)
 * @kill:  要杀死的进程的进程号(当ctrl = KILL时，该域才有效)
 */
struct hsthread_trans {
	protocol_sthread ctrl;
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
	boolean lock;


	boolean (*send)(struct hsthread_struct *hsthread);
};

boolean hsthread_init(struct hsthread_struct *hsthread,
				   struct hssinfo_struct *ssinfo,
				   struct hsprocess_struct *hsprocess,
				   struct hsthread_trans *trans,
				   struct hnet_struct *socket,
				   struct hsthread_timer *timer);

#endif
