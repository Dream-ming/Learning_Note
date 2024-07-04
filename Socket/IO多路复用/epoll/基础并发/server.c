#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/epoll.h>

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
            if (fd == lfd)
            {
                int cfd = accept(fd, NULL, NULL);
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else 
            {
                char buf[1024];
                int len = recv(fd, buf, sizeof(buf), 0);
                if (len == -1)
                {
                    perror("recv error");
                    return -1;
                }
                else if (len == 0)
                {
                    printf("client break...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                    continue;
                }
                else 
                {
                    printf("read buf = %s\n", buf);
                    send(fd, buf, strlen(buf), 0);
                }
            }
        }
    }
    close(lfd);
    return 0;
}
