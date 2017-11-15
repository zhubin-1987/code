#include "common.h"

void server_put(int my_fd, const char *filename)           //上传
{
	MYFILE f_file={0};
	int m=0;
	int fd=open(filename,O_WRONLY|O_CREAT|O_EXCL,0666);
	if(-1==fd)
	{
		f_file.state=ERROR;
		strcpy(f_file.f_buf,"创建文件出错，上传失败！");
		send(my_fd,&f_file,FILE_SIZE,0);
		return;
	}
	f_file.state=OK;
	send(my_fd,&f_file,FILE_SIZE,0);
	while(1)
	{
		recv(my_fd,&f_file,FILE_SIZE,MSG_WAITALL);
		if(BEG==f_file.state)
		{
			write(fd,f_file.f_buf,f_file.f_len);
		}
		else if(END==f_file.state)
		{
			close(fd);
			break;
		}	
	}
	f_file.state=OK;
	strcpy(f_file.f_buf,"文件上传成功！");
	send(my_fd,&f_file,FILE_SIZE,0);
	printf("Put OK!\n");
}

void server_get(int my_fd, const char *filename)           //下载
{
	MYFILE f_file={0};
	DATA s_data={0};
	int fd=open(filename,O_RDONLY);
	if(-1==fd)
	{
		f_file.state=ERROR;
		strcpy(f_file.f_buf,"打开文件出错，下载失败！");
		send(my_fd,&f_file,FILE_SIZE,0);
		return;
	}
	struct stat s;
	int i=stat(filename, &s);
	if(-1==i)
	{
		f_file.state=ERROR;
		strcpy(f_file.f_buf,"获取文件长度出错，下载失败！");
		send(my_fd,&f_file,FILE_SIZE,0);
		return;
	}
	f_file.state=OK;
	f_file.f_len=s.st_size;
	send(my_fd,&f_file,FILE_SIZE,0);
	
	while(i=read(fd,&f_file.f_buf,F_LEN))
	{
		f_file.f_len=i;
		f_file.state=BEG;
		send(my_fd,&f_file,FILE_SIZE,0);
	}
	close(fd);
	f_file.state=END;
	send(my_fd,&f_file,FILE_SIZE,0);
	printf("Get OK!\n");
}

void server_list(int my_fd)              //获取文件列表
{
	MYFILE f_file={0};
	DIR *fd=opendir(".");
	if(NULL==fd)
	{
		f_file.state=ERROR;
		strcpy(f_file.f_buf,"获取文件列表失败！");
		send(my_fd,&f_file,FILE_SIZE,0);
		return;
	}
	f_file.state=OK;
	send(my_fd,&f_file,FILE_SIZE,0);
	
	struct dirent *d;
	while(d=readdir(fd))
	{
		f_file.state=BEG;
		if('.'!=d->d_name[0])
		{
			strcpy(f_file.f_buf,d->d_name);
			send(my_fd,&f_file,FILE_SIZE,0);
		}
	}
	closedir(fd);
	f_file.state=END;
	send(my_fd,&f_file,FILE_SIZE,0);
	printf("List OK!\n");
}

void my_fun(int a)
{
	int i=0;
	while(i<=pid_no)
	{
		kill(SIGQUIT, c_pid[i]);
		i++;
	}
	exit(0);		
}

void child_fun(int b)
{
	printf("Quit\n");
	exit(0);
}

