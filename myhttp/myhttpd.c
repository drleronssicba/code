#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <errno.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
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
  
  if(bind(sock, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in)) < 0) {
    perror("bind");
    close(sock);
  }

  if(listen(sock, _BACKLG_) < 0) {
    perror("listen");
    close(sock);
  }


  //复用 TODO
}

void getlin(int sock, char line[]) {
  int i = 0;
  int c = 0;
  while(i<LINE_MAX-1 && c != '\n') {
    recv(sock, &c, 1, 0);
    if(c == '\r') {
      recv(sock, &c, 1, MSG_PEEK);
      if(c != '\n') {
        c='\n';
      } 
    }

    line[i++] = c;
  }
}

void* server(void* arg) {
  int sock =  *(int*) arg; //sock为客户端套接字
  char line[LINE_MAX] = {0};
  
  do{
    getlin(sock, line);
    printf("%s\n",line);
    line[0] = '\0';
  } while(line[0] != '\n');

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

  printf("bind and listen success, wait accept...\n");

  while(1) {
    struct sockaddr_in client_socket;
    socklen_t len = 0;

    //accept()
    //本函数从s的等待连接队列中抽取第一个连接，创建一个与s同类的新的套接口并返回句柄。\
    //如果队列中无等待连接，且套接口为阻塞方式，则accept()阻塞调用进程直至新的连接出现。
    //如果套接口为非阻塞方式且队列中无等待连接，则accept()返回一错误代码。
    //已接受连接的套接口不能用于接受新的连接，原套接口仍保持开放
    int client_sock = accept(sock, (struct sockaddr*)&client_socket, &len);
    if(client_sock < 0) {
      perror("accept");
      continue;
    }
    
    //以下证明accept已经获得了一个新的套接字,创建一个线程去处理
    pthread_t tid = 0;

    pthread_create(&tid, NULL, server, (void*)&client_sock);
    pthread_detach(tid);
  }
  return 0;
}
