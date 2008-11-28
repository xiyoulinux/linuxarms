#include "afview.h"
#include "afthread.h"
#include "linuxarms.h"
#include "debug.h"
/*
 * 初始化afview_struct结构
 */
boolean afview_init(struct afview_struct *afview,
		    struct anet_struct *socket)
{

	debug_where();
	if (!afview) {
		print_error(EWARNING,"初始化数据结构为空");
		return FALSE;
	}
}

/*
 * 接收反馈信息
 */
boolean afview_recv_info(struct afview_struct *afview)
{
	return anet_recv(afview->socket->tcp, 
			afview->frecv, sizeof(struct afview_recv));
}

/*
 * 发送文件信息
 */
boolean afview_send_info(struct afview_struct *afview)
{
}

/*
 * 重命名文件
 */
boolean afview_rename(struct hfthread_struct *hfthread)
{
}
/*
 * 删除文件
 */
boolean afview_delete(struct hfthread_struct *hfthread)
{
}
/*
 * afview主调函数
 */
boolean do_file_view(struct afview_struct *afview)
{
	/*
	 * 调用其他模块，完成文件浏览功能
	 */
}

