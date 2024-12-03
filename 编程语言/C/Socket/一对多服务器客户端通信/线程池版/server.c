#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "threadpool.h"

struct fd_addr
{
    int fd;
    struct sockaddr_in addr;
};

typedef struct PoolInfo
{
    ThreadPool* p;
    int fd;
} PoolInfo;

void working(void* arg);

void acceptConn(void* arg);

int main()
{
    // 1.创建监听的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        return -1;
    }

    // 2.绑定本地的IP port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY = 0.0.0.0
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }

    // 3.设置监听
    ret = listen(fd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }

    // 创建线程池
    ThreadPool *pool = threadPoolCreate(3, 8, 100);
    PoolInfo* info = (PoolInfo*)malloc(sizeof(PoolInfo));
    info->p = pool;
    info->fd = fd;
    threadPoolAdd(pool, acceptConn, info);

    pthread_exit(NULL);

    return 0;
}

void acceptConn(void* arg) 
{
    PoolInfo* poolInfo = (PoolInfo*) arg;
    // 4.阻塞并等待客户端的连接
    int addrlen = sizeof(struct sockaddr_in);
    while(1)
    {
        struct fd_addr* pinfo;
        pinfo = (struct fd_addr*)malloc(sizeof(struct fd_addr));
        pinfo->fd  = accept(poolInfo->fd, (struct sockaddr*)&pinfo -> addr, &addrlen);
        if (pinfo->fd == -1)
        {
            perror("accept");
            continue;
        }
        printf("listener thread %ld find a connect\n", pthread_self());
        // 添加通信的任务
        threadPoolAdd(poolInfo->p, working, pinfo);
    }

    close(poolInfo->fd);

}

void working(void* arg)
{

    struct fd_addr* pinfo = (struct fd_addr*)arg;
    // 连接建立成功，打印客户端的IP和端口
    char ip[24] = {0};
    printf("客户端的IP： %s，端口：%d\n",
           inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(pinfo->addr.sin_port));
    
    // 5.通信
    while (1)
    {
        // 接受数据
        char buff[1024];
        int len = recv(pinfo->fd, buff, sizeof(buff), 0);
        if (len > 0)
        {
            printf("client say: %s\n", buff);
            send(pinfo->fd, buff, len, 0);
        }
        else if (len == 0)
        {
            printf("client break...\n");
            break;
        }
        else {
            perror("recv");
            break;
        }
    }

    // 关闭文件描述符
    close(pinfo->fd);

}

