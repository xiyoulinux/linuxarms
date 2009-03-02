#ifndef _ASSINFO_INFO_H
#define _ASSINFO_INFO_H

#include "protocol.h"
#include "linuxarms.h"
#include "anet.h"

#define ASSINFO_MAX 80 /* 最大数 */
#define ASSINFO_SENDLINE 11 /* 总的发送行数 */

struct assinfo_trans {
	protocol_sthread protocol;
	char buffer[ASSINFO_SENDLINE][ASSINFO_MAX];
};
boolean assinfo_trans_init(struct assinfo_trans *astrans);
boolean assinfo_trans_set_protocol(struct assinfo_trans *astrans, protocol_sthread protocol);
/* 
 * 结构体声明
 * @keyword:   存放要搜索的关键字
 * @filename:  用来存放要搜索的文件的文件名
 * @gline:     用来存放要搜索的文件的初始化搜索行数
 */

struct assinfo_file{
	char keyword[20];
	char filename[40];
	int line;
};

struct assinfo_struct {
	struct assinfo_file *pfile;
	struct assinfo_trans trans;
	struct anet_struct *socket;
	
	boolean (*set_protocol)(struct assinfo_struct *assinfo, protocol_sthread protocol);
	boolean (*send)(struct assinfo_struct *assinfo);
	boolean (*recv)(struct assinfo_struct *assinfo);	
};
struct asthread_struct;
boolean assinfo_init(struct assinfo_struct *assinfo, struct anet_struct *socket);
boolean assinfo_read_info(struct asthread_struct *asthread);

#endif 

