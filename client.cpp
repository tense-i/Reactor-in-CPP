
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

    while (true)
    {
        memset(buf, 0, sizeof(buf));
        printf("please input: ");
        scanf("%s", buf);

        if (send(sockfd, buf, sizeof(buf), 0) <= 0)
        {

            printf("write failed\n");
            return -1;
                }
        memset(buf, 0, sizeof(buf));
        if (recv(sockfd, buf, sizeof(buf), 0) <= 0)
        {
            printf("read falied\n");
            close(sockfd);
            return -1;
        }
        printf("recv:%s\n", buf);
        /* code */
    }
}