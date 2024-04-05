
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "./warpFun/wrap.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("error\n");
        printf("example:./client 192.168.150.128 5085\n");
        exit(-1);
    }
    int sockfd;
    struct sockaddr_in servAddr;
    char buf[1024];

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[2]));
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);

    Connect(sockfd, (sockaddr *)&servAddr, sizeof(servAddr));

    printf("connect ok \n");
    for (int i = 0; i < 5; i++)
    {
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "这是第%d个超级女生", i);
        char tmpBuf[1024];
        memset(tmpBuf, 0, sizeof(tmpBuf));
        int len = strlen(buf);   // 计算报文的大小
        memcpy(tmpBuf, &len, 4); // 拼接报文头部、将报文的长度拼接到tmpbuf中
        // tmpbuf="24"+"只是第1个超级女生"
        memcpy(tmpBuf + 4, buf, len); // 拼接报文内容
        send(sockfd, tmpBuf, len + 4, 0);
    }

    for (int i = 0; i < 5; i++)
    { // 拆包
        int len;
        recv(sockfd, (void *)&len, 4, 0);
        printf("client recv: len == %d \n", len);
        memset(buf, 0, sizeof(buf));
        int nread = Read(sockfd, buf, len);
        if (nread == -1)
        {
            printf("read failed\n");
            close(sockfd);
            return -1;
        }
        if (nread == 0)
        {
            printf("read EOF\n");
            close(sockfd);
            return -1;
        }
        printf("recv:%s\n", buf); // 接到的数据是带着报文头的}

        /* while (true)
        {
            memset(buf, 0, sizeof(buf));
            printf("please input: ");
            scanf("%s", buf);

            if (send(sockfd, buf, sizeof(buf), 0) <= 0)
            {

                printf("write failed\n");
                return -1;
            }t
            memset(buf, 0, sizeof(buf));
            if (recv(sockfd, buf, sizeof(buf), 0) <= 0)
            {
                printf("read falied\n");
                close(sockfd);
                return -1;
            }
            printf("recv:%s\n", buf);
        } */
    }
    sleep(5);
    return 0;
}