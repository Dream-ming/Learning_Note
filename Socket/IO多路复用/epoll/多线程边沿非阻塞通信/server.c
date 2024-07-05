#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

typedef struct socketinfo
{
    int fd;
    int epfd;
} SocketInfo;

void* acceptConn(void* arg)
{
    printf("pthread %ld working\n", pthread_self());
    SocketInfo* info = (SocketInfo*)arg;
    int cfd = accept(info->fd, NULL, NULL);
    int flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = cfd;
    epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);
    free(info);
    return NULL;
}

void* communication(void* arg)
{
    printf("pthread %ld working\n", pthread_self());
    SocketInfo* info = (SocketInfo*)arg;
    char buf[5];
    while (1)
    {
        int len = recv(info->fd, buf, sizeof(buf), 0);
        if (len == -1)
        {
            if (errno == EAGAIN)
            {
                printf("recv final...\n");
                break;
            }
            else 
            {
                perror("recv error");
                return NULL;    
            }
        }
        else if (len == 0)
        {
            printf("client break...\n");
            epoll_ctl(info->epfd, EPOLL_CTL_DEL, info->fd, NULL);
            close(info->fd);
            break;
        }
        else 
        {
            printf("read buf = %s\n", buf);
            send(info->fd, buf, strlen(buf), 0);
        }
    }
    free(info);
    return NULL;    
}

int main()
{
    // 创建套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        return -1;
    }

    // 绑定ip、port
    struct sockaddr_in addr;
    addr.sin_port = htons(9999);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd, (struct sockaddr*)& addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }

    // 监听
    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }

    // 创建epoll实例
    int epfd = epoll_create(1);
    if (epfd == -1)
    {
        perror("epoll_create");
        return -1;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);

    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(evs[0]);
    while (1)
    {
        int num = epoll_wait(epfd, evs, size, -1);
        printf("num = %d\n", num);
        for (int i = 0; i < num; ++i)
        {
            int fd = evs[i].data.fd;
            pthread_t tid;
            SocketInfo* info = (SocketInfo*)malloc(sizeof(SocketInfo));
            info->fd = fd;
            info->epfd = epfd;
            if (fd == lfd)
            {
                pthread_create(&tid, NULL, acceptConn, info);
                pthread_detach(tid);
            }
            else 
            {
                pthread_create(&tid, NULL, communication, info);
                pthread_detach(tid);
            }
        }
    }
    close(lfd);
    return 0;
}
