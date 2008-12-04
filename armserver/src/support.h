#ifndef _SUPPORT_H
#define _SUPPORT_H

/* 添加一个文件路径 */
void add_file_directory(const char *directory);
/* 查找一个文件所在路径 */
char *find_file(const char *filename);

void delete_file_directory(void);
#endif
