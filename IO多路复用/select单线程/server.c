#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

    // 设置监听
    listen(lfd, 128);

    // 将监听的fd的状态检测委托给内核
    int maxfd = lfd;
    // 定义检测的读集合
    // rdset是要检测读集合的fd_set，rdtemp是被内核修改过的，这里要保留原始集合
    fd_set rdset;
    fd_set rdtemp;
    // 初始化全部置空，给监听的文件描述符设为1
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);

    // select循环检测，如果检测到数据就解除阻塞，向下执行处理数据
    while (1)
    {
        rdtemp = rdset;
        // 这里不处理写操作和异常值，时间设为NULL，无限等待，直到有文件描述符的读缓冲区有数据 
        int num = select(maxfd + 1, &rdtemp, NULL, NULL, NULL);

        // 监听位置有数据，说明有新连接，accept接受连接将不会阻塞
        if (FD_ISSET(lfd, &rdtemp))
        {
            struct sockaddr_in caddr;
            int caddrlen = sizeof(caddr);
            int cfd = accept(lfd, (struct sockaddr*)&caddr, &caddrlen);

            // 新的文件描述符写进要检测的集合
            FD_SET(cfd, &rdset);
            maxfd = cfd > maxfd ? cfd : maxfd;
        }

        // 处理通信位置
        for (int i = 0; i < maxfd + 1; ++i)
        {
            // 有数据，并且不是监听的描述符，则是通信的描述符
            if (i != lfd && FD_ISSET(i, &rdtemp))
            {
                // 接受数据，如果没有接受完，则读缓冲区还有数据，下一轮select继续接受
                char buf[10] = {0};
                int len = read(i, buf, sizeof(buf));
                if (len == 0)
                {
                    printf("client bread...\n");
                    // 客户端断开连接，清除文件描述符
                    FD_CLR(i, &rdset);
                    close(i);
                }
                else if (len > 0)
                {
                    // 收到了数据，发回
                    printf("recv: %s\n", buf);
                    write(i, buf, strlen(buf));
                }
                else {
                    perror("read");
                }
            }
        }
    }
    return 0;
}
