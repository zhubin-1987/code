#include "common.h"

void child_proc(int c_fd, const char *c_ip)
{
	DATA s_data={0};
	signal(SIGQUIT, child_fun);
	while(1)
	{
		bzero(&s_data,DATA_SIZE);
		read(c_fd,&s_data,DATA_SIZE);
		switch(s_data.oper)
		{
			case PUT:
				server_put(c_fd, s_data.user_name);
				break;

			case GET:
				server_get(c_fd, s_data.user_name);
				break;

			case LIST:
				server_list(c_fd);
				break;
			case QUIT:
				printf("IP: %s quit !\n", c_ip);
				exit(0);
		}
	}
}



