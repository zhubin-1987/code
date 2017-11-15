#include "common.h"

void client_put(const char *filename, int my_fd)              //上传文件
{
	MYFILE my_file={0};
	DATA my_data={0};
	int i=0;
	int fd=open(filename,O_RDONLY);
	if(-1==fd)
	{
		perror("文件上传");
		return;
	}
	my_data.oper=PUT;
	strcpy(my_data.user_name, filename);
	send(my_fd, &my_data, DATA_SIZE,0);                 //服务器接收
	bzero(&my_data, DATA_SIZE);
	recv(my_fd,&my_file,FILE_SIZE,MSG_WAITALL);
	if(ERROR==my_file.state)
	{
		//printf("%s\n",my_file.f_buf);
		return;
	}
	if(OK==my_file.state)
	{
		
		bzero(&my_file,FILE_SIZE);
		my_file.state=BEG;
		while(i=read(fd,my_file.f_buf,F_LEN))
		{
			my_file.f_len=i;
			//printf("%d	",my_file.f_len);
			send(my_fd,&my_file,FILE_SIZE,0);
		}
		close(fd);
		my_file.state=END;
		send(my_fd,&my_file,FILE_SIZE,0);
		bzero(&my_file,FILE_SIZE);
		recv(my_fd,&my_file,FILE_SIZE,MSG_WAITALL);
		printf("%s\n",my_file.f_buf);
		return;
	}
	else
		printf("文件上传失败！\n");
}

void client_get(const char *filename, int my_fd)             //下载文件
{
	MYFILE my_file={0};
	DATA my_data={0};
	int len=0,i=0,n=0,m=0;
	int fd=open(filename,O_WRONLY|O_CREAT|O_EXCL,0666);
	if(-1==fd)
	{
		perror("文件下载");
		return;
	}
	my_data.oper=GET;
	strcpy(my_data.user_name,filename);
	send(my_fd,&my_data,DATA_SIZE,0);
	bzero(&my_data,DATA_SIZE);
	recv(my_fd,&my_file,FILE_SIZE,MSG_WAITALL);
	if(ERROR==my_file.state)
	{
		printf("%s\n",my_file.f_buf);
		return;
	}
	if(OK==my_file.state)
	{
		len=my_file.f_len;
		bzero(&my_file, FILE_SIZE);
		recv(my_fd,&my_file,FILE_SIZE,MSG_WAITALL);
		while(BEG==my_file.state)
		{
			write(fd,my_file.f_buf,my_file.f_len);
			i=i+my_file.f_len;
			bzero(&my_file, FILE_SIZE);
			recv(my_fd,&my_file,FILE_SIZE,MSG_WAITALL);
			
			/*m=0;
			while(FILE_SIZE > m)
			{
				m=m+read(my_fd,(char*)&my_file+m,FILE_SIZE-m);
			}
			printf("%d\n",m);
			printf("%d\n",my_file.state);
			*/
			
			n++;
			if(0==n%10)
				printf("%.2f%% \r	",(i*1.0/len)*100);
			if(0==n%50)
				printf("\n");	
		}
		if(END==my_file.state)
		{
			close(fd);
			printf("%.2f%% \r	",((i*1.0)/len)*100);
			printf("文件下载完成！\n");
		}
		else
			printf("文件下载出错！\n");
	}
}

void client_list(int my_fd)
{
	MYFILE my_file={0};
	my_file.state=LIST;
	int i=0;
	send(my_fd, &my_file, FILE_SIZE,0);
	bzero(&my_file, FILE_SIZE);
	recv(my_fd,&my_file,FILE_SIZE,MSG_WAITALL);
	if(ERROR==my_file.state)
	{
		printf("%s\n",my_file.f_buf);
		return;
	}
	if(OK==my_file.state)
	{
		bzero(&my_file,FILE_SIZE);
		while(1)
		{
			recv(my_fd,&my_file,FILE_SIZE,MSG_WAITALL);
			printf("%s	",my_file.f_buf);
			i++;
			if(0==i%5)
				printf("\n");
			if(END==my_file.state)
				break;
		}
		printf("\n");
	}
	else
		printf("获取文件列表失败！\n");
}

void client_quit(int my_fd)
{
	MYFILE my_file={0};
	my_file.state=QUIT;
	send(my_fd, &my_file, FILE_SIZE,0);
}

void client_help()
{
	printf("\n");
	printf("***********************************************\n");
	printf(" h    :-------------------------查看帮助信息\n");
	printf(" ?    :-------------------------查看帮助信息\n");
	printf(" help :-------------------------查看帮助信息\n");
	printf(" quit :---------------------------------退出\n");
	printf(" q    :---------------------------------退出\n");
	printf(" get  :----------------------从服务器下载文件\n");
	printf(" put  :----------------------上传文件到服务器\n");
	printf(" list :--------------------获取服务器文件列表\n");
	printf("***********************************************\n");
	printf("\n");
	
}

void client_error()
{
	printf("输入错误！\n");
	printf("你可以输入 'h', '?' 或 'help' 查看帮助信息！\n");
}

