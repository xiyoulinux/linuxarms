/*
 * linuxarms/armserver/src/clogin.c
 * Copyright (C) 2008 lizeliang<lizeliang.linux@gmail.com>
 */
#include "login.h"
#include "debug.h"
#include "error.h"
/*
 * some handling functions for armserver user_struct using
 */
static boolean set_ip(struct user_struct *user, const char *ip)
{
	
}
static boolean set_name(struct user_struct *user, const char *name)
{
	
}
static boolean set_passwd(struct user_struct *user, const char *passwd)
{
	
}
static boolean set_is_record(struct user_struct *user, int is_record)
{
	
}
static boolean get_is_record(struct user_struct *user)
{
	
}

/*
 * some handling functions for armserver configure using
 */
/*public interface*/
boolean create_file(const char *filename)
{
	
}
boolean read_from_config_file(const char *filename, char buffer[])
{
	
}
boolean write_to_config_file(const char *filename, const char *buffer)
{
	
}
/*inward interface*/
static boolean is_config_exist(void)
{
	
}
static boolean is_path_exist(const char *pathname)
{
	
}
static boolean is_file_exist(const char *path)
{
	
}
static boolean config_create() /*create $HOME/.linuxarms*/
{
	
}
static boolean create_path(const char *path)
{
	
}
static boolean create_file(const char *path, const char *filename)
{
	
}
static const char *search_file(const char *filename) /*return file path*/
{
	
}
static const char *read_file(const char *filename)
{
	
}
static const  char *write_file(const char *filename)
{
	
}

/*
 * some handling functions for armserver login
 */
boolean set_error_info(struct login_struct *login, const char *error_info)
{
	
}
const char *get_error(struct login_struct *login)
{
	
}
