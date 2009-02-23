#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <pwd.h>

#include "afview.h"
#include "afthread.h"
#include "linuxarms.h"
#include "debug.h"
#include "error.h"

static boolean afview_set_protocol(struct afview_struct *afview, protocol_fthread protocol);
static inline boolean afview_send_info(struct afview_struct *afview);
static inline boolean afview_recv_info(struct afview_struct *afview);
/*
 * 初始化afview_struct结构
 */
boolean afview_init(struct afview_struct *afview,char *path,
		    struct anet_struct *socket)
{

	debug_where();
	if (!afview || !path || !socket) {
		print_error(EWARNING,"初始化数据结构为空");
		return FALSE;
	}
	afview->path = path;
	afview->hide = FALSE;
	afview->socket = socket;
	
	afview->set_protocol = afview_set_protocol;
	afview->send = afview_send_info;
	afview->recv = afview_recv_info;
	afview_send_init(&afview->fsend);
	afview_recv_init(&afview->frecv);
	return TRUE;
}

/*
 * 接收反馈信息
 */
static boolean afview_recv_info(struct afview_struct *afview)
{
	LINUXARMS_POINTER(afview);
	return anet_recv(afview->socket->tcp,
			 (void *)&afview->frecv,
			 sizeof(struct afview_recv));			
}

/*
 * 发送文件信息
 */
static boolean afview_send_info(struct afview_struct *afview)
{
	LINUXARMS_POINTER(afview);
	return anet_send(afview->socket->tcp,
			 (void *)&afview->fsend,
			 sizeof(struct afview_send));
}

static boolean afview_set_protocol(struct afview_struct *afview, protocol_fthread protocol)
{
	LINUXARMS_POINTER(afview);
	return afview_send_set_protocol(&afview->fsend, protocol);
}
boolean afview_send_init(struct afview_send *fsend)
{
	LINUXARMS_POINTER(fsend);
	fsend->protocol = FMAX;
	memset(fsend->name, '\0', FILE_NAME_LEN);
	memset(fsend->user, '\0', FILE_USER_LEN);
	fsend->size = 0;
	fsend->type = 0;
	return TRUE;
}
boolean afview_send_set_protocol(struct afview_send *fsend, protocol_fthread protocol)
{
	LINUXARMS_POINTER(fsend);
	if (!PROTOCOL_IS_FTHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		fsend->protocol = FMAX;
		return FALSE;
	}
	fsend->protocol = protocol;
	return TRUE;
}
boolean afview_send_set_data(struct afview_send *fsend, unsigned int type, 
				const char *name, const  char *path)
{
	struct stat buf;
	char fpath[PATH_LEN + FILE_NAME_LEN];
	struct passwd *pwd;

	LINUXARMS_POINTER(fsend);
	LINUXARMS_CHAR(name);
	strcpy(fsend->name, name);
	fsend->type = type;
	LINUXARMS_CHAR(path);
	sprintf(fpath, "%s/%s", path, name);
	if (stat(fpath, &buf) == -1)
		return FALSE;
	fsend->mtime = buf.st_mtime;
	if (fsend->type == TYPE_DIR)
		fsend->size =0;
	else
		fsend->size = buf.st_size;
	pwd = getpwuid(buf.st_uid);
	if (pwd == NULL)
		strcpy(fsend->user, "none");
	else
		strcpy(fsend->user, pwd->pw_name);
	return TRUE;
}

boolean afview_recv_init(struct afview_recv *frecv)
{
	LINUXARMS_POINTER(frecv);
	frecv->protocol = FMAX;
	return TRUE;
}
boolean afview_recv_set_protocol(struct afview_recv *frecv, protocol_fthread protocol)
{
	LINUXARMS_POINTER(frecv);
	if (!PROTOCOL_IS_FTHREAD(protocol)) {
		debug_where();
		print_error(EWARNING, "无效的协议");
		frecv->protocol = FMAX;
		return FALSE;
	}
	frecv->protocol = protocol;
	return TRUE;
}

/*
 * afview主调函数
 */
boolean do_file_view(struct afview_struct *afview)
{
	/*
	 * 调用其他模块，完成文件浏览功能
	 */
	DIR * dir;
	struct dirent *dent;
	boolean ret;
	LINUXARMS_POINTER(afview);

	dir = opendir(afview->path);
	while ((dent = readdir(dir)) != NULL) {
		/*if (strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0)
			continue;*/
		if (afview->hide && *dent->d_name == '.')
			continue;
		ret = afview_send_set_data(&afview->fsend, (unsigned int)dent->d_type, 
					dent->d_name, afview->path);
		if (ret) {
			afview->set_protocol(afview, FVIEW);
			afview->send(afview);
		}
	}
	closedir(dir);
	afview->set_protocol(afview, FSENDALL);
	afview->send(afview);
	return TRUE;
}

