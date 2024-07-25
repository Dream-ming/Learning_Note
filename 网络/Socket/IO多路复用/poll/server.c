#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>

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

    // 等待连接
    // 用poll检测文件描述符的读缓冲区
    struct pollfd fds[1024];
    for (int i = 0; i < 1024; ++i)
    {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    int maxfd = 0;

    while (1)
    {
        // 无限等待，直到有数据为止
        ret = poll(fds, maxfd + 1, -1);
        if (ret == -1)
        {
            perror("poll");
            return -1;
        }

        // 检测到读缓冲区有变换
        // 监听符有数据，说明有新的连接
        if (fds[0].revents & POLLIN)
        {
            struct sockaddr_in caddr;
            int caddrlen = sizeof(caddr);
            int connfd = accept(lfd, (struct sockaddr*)&caddr, &caddrlen);

            int i;
            for (i = 0; i < 1024; ++i)
            {
                if (fds[i].fd == -1)
                {
                    // 在数组的空位置放入新的客户端连接的文件描述符
                    fds[i].fd = connfd;
                    break;
                }
            }
            maxfd = maxfd > i? maxfd : i;
        }

        // 检查通信符是否有数据
        for (int i = 1; i <= maxfd; ++i)
        {
            if (fds[i].revents & POLLIN)
            {
                char buf[10];
                int len = read(fds[i].fd, buf, sizeof(buf));
                if (len == -1)
                {
                    perror("read");
                    return -1;
                }
                else if (len == 0)
                {
                    printf("client break ...\n");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
                else {
                    printf("recv: %s\n", buf);
                    write(fds[i].fd, buf, len);
                }
            }
        }
    }
    close(lfd);
    return 0;
}
