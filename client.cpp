#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ev.h>
#include "ev++.h"

struct ev_loop* g_pEvLoop = NULL;

// IO事件回调
void cbEvents(struct ev_loop *loop, struct ev_io *w, int revents);

int main()
{
    // 创建套接字
    int sockc = socket(AF_INET, SOCK_STREAM, 0);
    if (sockc < 0)
    {
        printf("create socket failed! \n");
        return -1;
    }
    
    // 设置连接的服务器地址
    struct sockaddr_in sinto;
    sinto.sin_family = AF_INET;
    sinto.sin_addr.s_addr = inet_addr("127.0.0.1");
    sinto.sin_port = htons(8888);

    // 连接服务器
    int ret = connect(sockc, (struct sockaddr*)&sinto, sizeof(sinto));
    if (ret < 0)
    {
        printf("connect failed! \n");
        return -1;
    }
    printf("connect ok! \n");
    // 初使化ev_loop对象
    g_pEvLoop = ev_default_loop(0);
    // 注册读事件
    ev_io evio;
    ev_io_init(&evio, &cbEvents, sockc, EV_READ);
    ev_io_start(g_pEvLoop, &evio);

    // 启动事件循环
    printf("ev start! \n");
    ev_run(g_pEvLoop, 0);
    printf("ev stop! \n");
    close(sockc);
    ev_loop_destroy(g_pEvLoop);
    return 0;
}

// IO事件回调
void cbEvents(struct ev_loop *loop, struct ev_io *w, int revents)
{
    int sockc = w->fd;

    // 检测读事件
    if ((revents & EV_READ) == EV_READ)
    {
        char sbuf[1024] = { 0 };
        int bytes_in = recv(sockc, sbuf, 1023, 0);

        // 连接中断
        if (bytes_in <= 0)
        {
            printf("disconnect! \n");
            // 取消事件注册
            ev_io_stop(g_pEvLoop, w);
            // 释放连接资源
            close(sockc);
            // 退出事件循环
            ev_break(g_pEvLoop, EVBREAK_ALL);
            return;
        }
        // 文本内容，可简单打印
        printf("recv: %s \n", sbuf);
        // 简单回传
        send(sockc, sbuf, bytes_in, 0);
    }
}