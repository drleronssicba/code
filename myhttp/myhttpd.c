#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <errno.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/sendfile.h>
#include<pthread.h>

#define _PORT_ 9999
#define _BACKLG_ 5
#define LINE_MAX 1024

void startup(int sock) {
  printf("%d\n", sock);
  struct sockaddr_in server_socket;

  //填写sockaddr_in结构
  bzero(&server_socket, sizeof(server_socket));
  server_socket.sin_family = AF_INET;
  server_socket.sin_addr.s_addr = htonl(INADDR_ANY);
  server_socket.sin_port = htons(_PORT_);
  
  //复用 
  int flag = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

  if(bind(sock, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in)) < 0) {
    perror("bind");
    close(sock);
  }

  if(listen(sock, _BACKLG_) < 0) {
    perror("listen");
    close(sock);
  }
}

int getlin(int sock, char line[]) {
    int i = 0;
    int c = 0;
    int n = 0;

    while(c != '\n' && i<LINE_MAX-1) {
        n = recv(sock, &c, 1, 0);
        if( n > 0 ) {
            if( c == '\r'  ){//将所有的\r\n(windows) 和 \r(mac os) 统一转化成\n
                n = recv(sock, &c, 1, MSG_PEEK); //对下一个字符制作检测，在内核接受缓冲区中，并不清除该字符
                if( n > 0 && c == '\n'  ){
                    recv(sock, &c, 1, 0);//读取该字符
                }else{
                    c = '\n';
                }
            }
            line[i++] = c;
        }else{
            c = '\n';
        }
    }
    line[i] = '\0';
    return i;
}

int clean_head(int sock) {
    char line[LINE_MAX];
    do{
        if(getlin(sock, line) == -1) {
            printf("getlin 失败\n");
            return -1;
        }
       // printf("%s",line);
    } while(line[0] != '\n');
    return 0;
}

void res_begin(int sock) {
    char buf[LINE_MAX / 32] = "HTTP/1.1 200 OK\r\n";
    send(sock, buf, strlen(buf), 0);
}

void res_end(int sock) {
    send(sock, "\r\n", 2, 0);
}

int send_path_file(int sock, char* path) {
    int fd = -1;
    char tmp_path[LINE_MAX / 8];
    memset(tmp_path, 0, sizeof(tmp_path));
    if(strcmp("/", path) == 0) {
        //发送主页
        strcpy(tmp_path, "./root/index.html");
        fd = open(tmp_path, 'r');
    } else {
        strcpy(tmp_path, "./root");
        strcpy(tmp_path+6, path); 
        memset(tmp_path, 0, sizeof(tmp_path));
        fd = open(tmp_path, 'r');
    }

    res_begin(sock);
    struct stat st;
    if(stat(path, &st) == -1) {
        //文件不存在
        //发送错误页面  TODO
        
    } else {
        //文件存在
        //可判断是不是目录
        //可判断文件有没有执行权限
        //TODO
        sendfile(sock, fd, NULL, st.st_size);
    }
    res_end(sock);
    close(fd);
    return 0;
}

void* server(void* arg) {
    int sock =  *(int*) arg; //sock为客户端套接字
    
    char first_line[LINE_MAX];
    memset(first_line, 0, sizeof(first_line));
    getlin(sock, first_line);
    //printf("first_line: %s", first_line);
    
    char* tmp = first_line;
    char* cur = first_line;
    char method[LINE_MAX / 32];
    char url[LINE_MAX / 4];
    char* path = NULL;
    char* para = NULL;
    int cgi = 0;
    memset(method, 0, sizeof(method));
    memset(url, 0, sizeof(url));
    while(*tmp++ != ' ');
    *(tmp - 1) = '\0';
    strcpy(method, cur);
    cur = tmp;
    while(*tmp++ != ' ');
    *(tmp - 1) = '\0';
    strcpy(url, cur);
    path = url;
    clean_head(sock);
   // if(strcmp("GET", method) == 0) {
   //     //GET方法
   //     tmp = url;
   //     while(*tmp != '\0' && *tmp != '?') {
   //         tmp++;
   //     }
   //     if(*tmp == '\0') {
   //         para = NULL;
   //     } 
   //     if(*tmp == '?') {
   //         *para = '\0';
   //         para = tmp + 1;
   //     }
   //     if(para == NULL) {
   //         cgi = 0;
   //     } else {
   //         cgi = 1;
   //     }
   // } 
   // else if(strcmp("POST", method) == 0) {
   //     

   // } else {
   //     //未定义方法
   //     printf("未定义方法，不做处理\n");
   //     close(sock);
   //     return NULL;
   // }


  //  if(cgi == 0) {
  //      if( send_path_file(sock, path) == -1) {
  //          //发送文件失败
  //          printf("send_path_file 发送文件失败\n");
  //      }
  //  } else {

  //  }


    printf("method: %s\n", method);
    printf("path: %s\n", path);
    printf("paramete: %s\n", para);
    close(sock);
    return NULL;
}

int main()
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    perror("socket");
    return 1;
  }
  startup(sock);

  //printf("bind and listen success, wait accept...\n");

  while(1) {
    struct sockaddr_in client_socket;
    socklen_t len = 0;

    //accept()
    //本函数从s的等待连接队列中抽取第一个连接，创建一个与s同类的新的套接口并返回句柄。\
    //如果队列中无等待连接，且套接口为阻塞方式，则accept()阻塞调用进程直至新的连接出现。
    //如果套接口为非阻塞方式且队列中无等待连接，则accept()返回一错误代码。
    //已接受连接的套接口不能用于接受新的连接，原套接口仍保持开放
    printf("等待链接...\n");
    int client_sock = accept(sock, (struct sockaddr*)&client_socket, &len);
    if(client_sock < 0) {
      perror("accept");
      continue;
    }
    
    //以下证明accept已经获得了一个新的套接字,创建一个线程去处理
    pthread_t tid = 0;
    pthread_create(&tid, NULL, server, (void*)&client_sock);
    printf("链接处理完成\n");
    pthread_detach(tid);
  }
  return 0;
}
