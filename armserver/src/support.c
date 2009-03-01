#include "support.h"
#include "error.h"
#include "linuxarms.h"
#include <malloc.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define FILE_NAME_LEN 256

struct list {
	char path[PATH_LEN];
	struct list *next;
};

static struct list list_head;
static char path[PATH_LEN + FILE_NAME_LEN];
boolean init = TRUE;
void add_file_directory(const char *directory)
{
	struct list *tmp, *next;

	if (init) {
		list_head.next = &list_head;
		init = FALSE;
	}
	tmp = (struct list *)malloc(sizeof(struct list));
	if (!tmp) {
		print_error(ESYSERR, "申请内存空间错误");
		return;
	}
	strncpy(tmp->path, directory,  PATH_LEN);
	next = list_head.next;
	list_head.next = tmp;
	tmp->next = next;
}
/*
 * 查找一个文件
 * @filename:   要查找的文件名
 * @return:     如果没有找到，则返回NULL,否则返回文件所在路径
 */
char *find_file(const char *filename)
{
	struct list *tmp;

	if (!filename)
		goto out;
	tmp = list_head.next;
	while (tmp != &list_head) {
		snprintf(path, PATH_LEN + FILE_NAME_LEN, "%s/%s", tmp->path, filename);
		if (access(path, F_OK) == -1) {
			tmp = tmp->next;
			continue;
		} else
			break;
	}
	if (tmp != &list_head)
		return path;
	else
out:
	print_error(EWARNING, "没有找到文件");
	return NULL;
}

/*
 * 销毁链表
 */
void delete_file_directory()
{
	struct list *tmp, *delete;
	tmp = list_head.next;
	while (tmp != &list_head) {
		delete = tmp;
		tmp = tmp->next;
		free(delete);
	}
}
