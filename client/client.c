#include "common.h"

int main(int argc, char *argv[])
{
	if(3 != argc)
	{
		printf("Usage : %s <PORT> <SERVER IP>\n", argv[0]);
		exit(-1);
	}


	int my_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == my_fd)
	{
		perror("socket");
		exit(-1);
	}

	struct sockaddr_in ser_addr = {0};

	ser_addr.sin_family      = AF_INET;
	ser_addr.sin_port        = htons(atoi(argv[1]));
	ser_addr.sin_addr.s_addr = inet_addr(argv[2]);
	
	int len = sizeof ser_addr;

	if(-1 == connect(my_fd, (struct sockaddr*)&ser_addr, len))
	{
		perror("connect");
		exit(-1);
	}
	char buf[100]="0";
	while(1)
	{
		printf("请输入你的操作：");
		fflush(stdout);
		gets(buf);
		if(0==strncmp(buf,"put", 3))
			client_put(buf+4,my_fd);
			
		else if(0==strncmp(buf,"get", 3))
			client_get(buf+4,my_fd);
			
		else if(0==strcmp(buf,"list"))
			client_list(my_fd);
		
		else if(0==strcmp(buf,"h")||0==strcmp(buf,"?")||0==strcmp(buf,"help"))
			client_help();
			
		else if(0==strcmp(buf,"q")||0==strcmp(buf,"quit"))
		{
			client_quit(my_fd);
			break;
		}
			
		else
			client_error();
	}
	close(my_fd);
}
