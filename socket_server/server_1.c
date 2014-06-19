#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024

int main()
{
    int ret,on;
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
   
    // 在应用了 SO_REUSEADDR 选项之后，bind API 函数将允许地址的立即重用。
    /* Enable address reuse */
    on = 1;
    ret = setsockopt( server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    
    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//client add

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        // exit(1);
        return -1;
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        close(server_sockfd);
        // exit(1);
        return -2;

    }

    ///客户端套接字
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///成功返回非负描述字，出错返回-1
    int client_fd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(client_fd<0)
    {
        perror("client_fdect");
        close(server_sockfd);
        // exit(1);
        return -3;
    }

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int len = recv(client_fd, buffer, sizeof(buffer),0);
        if(strcmp(buffer,"exit\n")==0)
            break;
        fputs(buffer, stdout);
        send(client_fd, buffer, len, 0);
    }
    close(client_fd);
    close(server_sockfd);
    return 0;
}