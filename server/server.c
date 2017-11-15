#include "common.h"

pid_t c_pid[50];
int pid_no;

void my_fun1(int i)
{
	printf("Stop !\n");
	while(1);
}

int main()
{
	int n=0,c_fd;
	char name[LENTH],pass[LENTH],c_ip[64];
	pid_t pid;
	DATA s_data={0};
	
	signal(SIGQUIT ,my_fun);
	signal(SIGSTOP ,my_fun);
	int listen_fd=socket(PF_INET,SOCK_STREAM,0);              //创建套接字
	if(-1==listen_fd)
	{
		perror("server socket");
		exit(-1);	
	}
	
	struct sockaddr_in server_addr,client_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(142);
	server_addr.sin_addr.s_addr=inet_addr("0.0.0.0");
	int add_len=sizeof server_addr;
	
	int x = 1;
	int i=setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &x , sizeof x);
	if(-1==i)
	{
		perror("server setsockopt");
		exit(-1);
	}
	
	i=bind(listen_fd,(struct sockaddr *)&server_addr,add_len);          //绑定
	if(-1==i)
	{
		perror("server bind");
		exit(-1);
	}
	i=listen(listen_fd,50);
	if(-1==i)
	{
		perror("server listen");
	}
	printf("Listen..........\n");
	while(1)
	{
		c_fd=accept(listen_fd,(struct sockaddr *)&client_addr,&add_len);      //有连接就创建一个子进程处理
		if(-1==c_fd)
		{
			perror("accept");
			continue;
		}
		strcpy(c_ip, inet_ntoa(client_addr.sin_addr));
		printf("IP: %s connected !\n", c_ip);
		pid=fork();
		c_pid[pid_no]=pid;
		pid_no++;
		if(0==pid)
		{
			child_proc(c_fd, c_ip);
		}
		else if(pid>0)
			close(c_fd);
			
		else 
		{
			perror("fork");
		}		
	}
}
