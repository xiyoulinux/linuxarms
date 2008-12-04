#include "amthread.h"
#include "login.h"
#include "support.h"

int main(int args, char *argv[])
{
	char *config_file;

	add_file_directory("/etc/linuxarms-armserver/config");
	add_file_directory("../config");
	add_file_directory("./config");
	config_file = find_file("port.conf");
	config_init(config_file);



	delete_file_directory();
	return 0;

}
