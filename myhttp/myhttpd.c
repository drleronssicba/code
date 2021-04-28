#include<stdio.h>
#include<stdlib.h>
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
#include<sys/wait.h>

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
                recv(sock, &c, 1, MSG_PEEK); //对下一个字符制作检测，在内核接受缓冲区中，并不清除该字符
                if( c != '\n'  ){
                    c = '\n';
                }else{
                    recv(sock, &c, 1, 0);
                }
            }
            line[i++] = c;
            if(c == '\n') {
                break;
            }
        }else{
            break;
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

int get_post_body_lenth(int sock) {
    int len = 0;
    char line[LINE_MAX];
    memset(line, 0, sizeof(line));
    do
    {
        getlin(sock, line);
        if(strncmp(line, "Content-Length: ", 16) == 0) {
            len = atoi(line + 16);
        }
    }while(strcmp(line, "\n") != 0);
    return len;
}

void get_post_body(int sock, char buf[], int len) {
    int i = 0;
    while(i < len) {
        recv(sock, &buf[i++], 1, 0); 
    }
    buf[i] = '\0';
}

void res_begin(int sock) {
    char buf[LINE_MAX / 32] = "HTTP/1.1 200 OK\r\n";
    send(sock, buf, strlen(buf), 0);
    //sprintf(buf,"\r\n");
    //send(sock, buf, strlen(buf), 0);
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

    printf("文件名：%s\n",tmp_path);
    res_begin(sock);
    struct stat st;
    if(stat(path, &st) == -1) {
        //文件不存在
        //发送错误页面  TODO
        printf("文件不存在\n");
    } else {
        //文件存在
        //可判断是不是目录
        //可判断文件有没有执行权限
        //TODO
        printf("文件存在，准备发送文件\n");
        
        if(sendfile(sock, fd, NULL, st.st_size) == -1) {
            //发送失败
            printf("文件发送失败\n");
        }
        res_end(sock);
        printf("文件发送成功\n");
    }
    close(fd);
    return 0;
}

int exe_cgi(int sock, char* path, char* method, char* para) {
    char para_env[LINE_MAX / 2];

    //发送响应报头 TODO
    
    char tmp_path[LINE_MAX / 2];
    memset(tmp_path, 0, LINE_MAX / 2);
    strcpy(tmp_path, "./root/cgi");
    strcpy(tmp_path + 10, path); 

    int input[2];
    int output[2];

    pipe(input);
    pipe(output);
    
    pid_t id = fork();
    //fork()之后，父子进程依然可以看见上面的参数部分

    if(id < 0) {
        return 404;
    } else if (id == 0) {
        //child
        close(input[1]);
        close(output[0]);
        dup2(input[0], 0);
        dup2(output[1], 1);
        
        //读父进程写进管道的参数
        sprintf(para_env, "PARA=%s", para);
        putenv(para_env);
        //printf("child: %s\n",para_env);
        //printf("child: %s\n",tmp_path);
        //printf("child: %s\n", getenv("PARA"));

        //printf("child: %s\n", tmp_path);
        //TODO path??  //替换可能不成功
        if(execl(tmp_path, tmp_path, NULL) == -1) {
            printf("程序替换失败\n");
        }
        //不execl，可以printf到浏览器, 所以execl有问题
        //printf("HTTP1.1 200 OK\r\n");
        //printf("\r\n");

        exit(1);
    } else {
        //father
        close(input[0]);
        close(output[1]);
        
        char c;
        while(read(output[0],&c, 1) > 0 ) {
            send(sock, &c, 1, 0);
            printf("%c", c);
        }
        waitpid(id, NULL, 0);
        close(input[1]);
        close(output[0]);
    }


    return 0;
}

void* server(void* arg) {
    //int sock =  *(int*) arg; //sock为客户端套接字
    int sock =  (int)arg; //sock为客户端套接字

    char first_line[LINE_MAX];
    memset(first_line, 0, sizeof(first_line));
    getlin(sock, first_line);
    //printf("first_line: %s", first_line);

    char* tmp = first_line;
    char* cur = first_line;
    char* method = NULL;
    char* path = NULL;
    char* para = NULL;
    char post_para[LINE_MAX / 8];
    int cgi = 0;

    //printf("%s\n", first_line);

    while(*tmp++ != ' ');
    *(tmp - 1) = '\0';
    method = cur;
    cur = tmp;
    path = tmp;

    while(*tmp++ != ' ');
    *(tmp - 1) = '\0';

    //get Get geT ...
    if(strcasecmp("GET", method) == 0) {
        clean_head(sock);
        tmp = path;
        //GET方法
        while(*tmp != '\0' && *tmp != '?') {
            tmp++;
        }
        if(*tmp == '\0') {
            para = NULL;
        } else if(*tmp == '?') {
            *tmp = '\0';
            para = tmp + 1;
        }
        if(para == NULL) {
            cgi = 0;
        } else {
            cgi = 1;
        }
    } 
    else if(strcasecmp("POST", method) == 0) {
        cgi = 1;
        char buf[LINE_MAX];
        para = buf;
        memset(buf, 0, sizeof(buf));
        int len = get_post_body_lenth(sock);
        get_post_body(sock, buf, len);

    } else {
        clean_head(sock);
        //未定义方法
        printf("未定义方法，不做处理\n");
        close(sock);
        return NULL;
    }


    if(cgi) {
       printf("cgi = 1\n");
       int ret = exe_cgi(sock, path, method, para);
    } else {
        printf("cgi = 0\n");
        if( send_path_file(sock, path) == -1) {
            //发送文件失败
            printf("send_path_file 发送文件失败\n");
        }
    }


   // printf("method: %s\n", method);
   // printf("path: %s\n", path);
   // printf("paramete: %s\n", para);
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
        pthread_create(&tid, NULL, server, (void*)client_sock);
        printf("链接处理完成\n");
        pthread_detach(tid);
    }
    return 0;
}
