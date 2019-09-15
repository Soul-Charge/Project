#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <WinSock2.h>
#define MEGLEN 10

void error(char * str);

int main(void)
{
    int listener_d;
    int reuse;
    char message[MEGLEN] = {0};

    /* initialize */
    system("chcp 65001");
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    /* open socket */
    if ((listener_d = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        error("无法打开套接字");
    /* set reuse port */
    if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)))
        error("无法设置套接字的“重新使用端口”选项");
    /* bind port */
    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(2333);
    name.sin_addr.s_addr = inet_addr("192.168.1.101");
    if (bind(listener_d, (struct sockaddr *)&name, sizeof(name)) == SOCKET_ERROR)
        error("无法绑定端口");
    /* listen */
    if (listen(listener_d, 10) == SOCKET_ERROR)
        error("无法设置监听队列");
    puts("Waiting for connection");
    
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d = 0;
    char * msg = "hello!";
    while (1)
    {
        connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        if (connect_d == -1)
            error("无法打开副套接字");

        puts("已连接到客户端");
        printf("正在发送消息\"%s\"\n", msg);
        send(connect_d, msg, strlen(msg), 0);
        send(connect_d, "\n", strlen("\n"), 0);
        puts("正在接收消息");
        while (!message[0])
        {
            recv(connect_d, message, MEGLEN, 0);
            if (message[0])
            {
                printf("接收到来自客户端的消息：%s\n", message);
                printf("%s\n", message);
                message[0] = 0;
            }
        }
        puts("正在断开连接");
        closesocket(connect_d);
    }

    return 0;
}

void error(char * str)
{
    fprintf(stderr, "%s\n", str);
    system("pause");
    exit(1);
}