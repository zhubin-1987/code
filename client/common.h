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

typedef struct{                     //用户名和密码用
	TYPE oper;
	char user_name[LENTH];
	char user_pass[LENTH];
}DATA;

typedef struct{                     //文件传输
	TYPE state;
	int f_len;
	char f_buf[F_LEN];
}MYFILE;

void client_reg(int my_fd);
int client_log(int my_fd);
void client_put(const char *filename, int my_fd);
void client_get(const char *filename, int my_fd);
void client_list(int my_fd);
void client_quit(int my_fd);
void client_help(void);
void client_error(void);


#endif
