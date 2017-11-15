#ifndef  _COMMON_H_
#define  _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>

#define   DATA_SIZE    (sizeof(DATA))
#define   LENTH        20
#define   F_LEN        1024
#define   LINK_SIZE    (sizeof(datanode))
#define   FILE_SIZE    (sizeof(MYFILE))
#define   TRUE         1
#define   FALSE        0

typedef enum mytype{
	PUT=2,
	GET,
	LIST,
	ERROR,
	OK,
	BEG,
	QUIT,
	END
}TYPE;

typedef struct{                     //用户名和密码保存
	TYPE oper;
	char user_name[LENTH];
	char user_pass[LENTH];
}DATA;

typedef struct{                     //文件传输用
	TYPE state;
	int f_len;
	char f_buf[F_LEN];
}MYFILE;

typedef struct node{
	DATA data_target;
	struct node *next;
}datanode,*datalist;

extern pid_t c_pid[50];
extern int pid_no;

void child_proc(int c_fd, const char *c_ip);
void server_put(int my_fd, const char *filename);
void server_get(int my_fd, const char *filename);
void server_list(int my_fd);
void my_fun(int a);
void child_fun(int b);

#endif
