/* 网络计算器服务器 */
/* 只是用来练习一下套接字的使用 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <WinSock2.h> //Windows使用套接字的头文件

void error(char * str);

int main(void)
{
    short check; //用来检测返回值的变量
    int listener_d; //接收用套接字

    /* 初始化 */
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa); //MAKEWORD用来生成使用的套接字的版本号

    /* 打开套接字 */
    listener_d = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_d == INVALID_SOCKET)
    {
        error("无法打开套接字");
        exit(1);
    }

    /* 绑定 */
    struct sockaddr_in name;
    name.sin_family = AF_INET; //使用ipv4
    name.sin_port = htons(2333);
    name.sin_addr.s_addr = inet_addr("127.0.0.1");
    check = bind(listener_d, (struct sockaddr *)&name, sizeof(name));
    if (check == -1)
    {
        error("无法绑定套接字");
        exit(0);
    }
    /* 设置重新使用端口 */
    int reuse = 1;
    check = setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));
    if (check == -1)
    {
        error("无法设置重新使用端口");
        exit(1);
    }

    /* 监听 */
    listen(listener_d, 10);

    while (1)
    {
        puts("Waiting for connection...");

        /* 接受连接 */
        struct sockaddr_storage client_addr;
        unsigned int address_size = sizeof(client_addr);
        int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        if (connect_d == -1)
        {
            error("无法打开副套接字");
            exit(1);
        }
        puts("Connected.");

        /* 开始 */
        char * msg = "Hello World !";
        send(connect_d, msg, sizeof(msg), 0);
        send(connect_d, msg, sizeof(msg), 0);
        /* 断开连接 */
        closesocket(connect_d);
        puts("Disconnected.");
    }

    return 0;
}

void error(char * str)
{
    printf("%s\n", str);
    perror("errno");
    printf("WSAGetLastError()的返回值%d", WSAGetLastError());
}