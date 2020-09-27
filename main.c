/*************************************************************************
    > File Name: main.c
    > Author: reeves
    > Mail: seereeves@163.com 
    > Created Time: 25/09/2020 - 00:50
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define IP "127.0.0.1"
#define PORT 9002
#define MAX_CONNECTION 64
#define BUFF_SIZE 1024

static int bind_udp_socket(const char* ip, const int port);
static int get_buff_content(char* buff, int len);
static int net_loop();

int main(int argc, char *argv[]) {
    printf("Hello World!\n");
    // 创建socket对象
    int listen_fd;
    listen_fd = bind_udp_socket(IP, PORT);
    net_loop(listen_fd);    
    shutdown(listen_fd, SHUT_RD);
    sleep(1);
    close(listen_fd);

    return 0;
}

static int bind_udp_socket(const char* ip, const int port) {
    int listen_fd;
    if (-1 == (listen_fd = socket(AF_INET, SOCK_DGRAM, 0))) {
        perror("socket bind error:");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (-1 == (inet_pton(AF_INET, ip, &serv_addr.sin_addr))) {
        perror("inet_pton error\n");
        exit(-1);
    }
    serv_addr.sin_port = htons(port);
    if (-1 == bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
        perror("bind error:");
        exit(1);
    }
    return listen_fd;
}

static int get_buff_content(char* buff, int len) {
    char dst[BUFF_SIZE + 1] = {0};
    memcpy(dst, buff, len);
    dst[len] = '\0';
    printf("content--%s\n", dst);
    char* question = "The answer to life, the universe, and everything";
    if (0 == strcmp(question, dst)) {
        return 1;
    }
    return 0;
}

static int net_loop(int fd) {
    char recvbuf[BUFF_SIZE + 1] = {0};
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

    while(1) {
        int n = recvfrom(fd, recvbuf, BUFF_SIZE, 0, (struct sockaddr*)&client_addr, &clientlen);
        if (n > 0) {
            printf("recv size: %d\n", n);
            int ret = get_buff_content(recvbuf, n);
            if (1 == ret) {
                char* ans = "is 42.";
                sendto(fd, ans, 6, 0, (struct sockaddr*)&client_addr, clientlen);
            }
        }
        else {
            printf("recv error.\n");
        }
    }
    return 0;
}