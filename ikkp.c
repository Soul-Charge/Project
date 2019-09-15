/* knock-knock 服务器程序 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <WinSock2.h>
#define MESSAGELEN 100

int open_listener_socket(void);
void bind_to_port_ipv4(int socket, int port, char* ipv4_str);
int my_accept(int socket);
int say(int socket, char* s);
int read_in(int socket, char* buf, unsigned int len);
void error(char* str);
void empty_str(char* str);

int main(void)
{
    int listener_d; //服务器主套接字
    int port = 2333;
    char ipv4_str[] = "192.168.1.101";
    char recv_meg[MESSAGELEN] = {0};

    /* initialize */
    system("chcp 65001"); //更换控制台编码为utf-8
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    listener_d = open_listener_socket();
    /* bind */
    bind_to_port_ipv4(listener_d, port, ipv4_str);
    /* listen */
    if (listen(listener_d, 10) == -1)
        error("设置监听队列长度时出错");

    puts("Waiting for connection");
    while (1)
    {
        /* accept */
        int connect_d = my_accept(listener_d);
        /* begin */
        say(connect_d, "Knock! Knock!\n");
        read_in(connect_d, recv_meg, MESSAGELEN);
        if (strcmp(recv_meg, "Who's there?") != 0)
        {
            say(connect_d, "Do you know this joke?\n");
            say(connect_d, "You should say: Who's there?\n");
            closesocket(connect_d);
        }
        empty_str(recv_meg);

        say(connect_d, "Oscar\n");
        read_in(connect_d, recv_meg, MESSAGELEN);
        if (strcmp(recv_meg, "Oscar who?") != 0)
        {
            say(connect_d, "You should say: Oscar who?\n");
            closesocket(connect_d);
        }
        empty_str(recv_meg);

        say(connect_d, "Oscar silly question, you get a silly answer.");
        /* close */
        closesocket(connect_d);
    }
    return 0;
}

/* 打开一个ipv4的流套接字 */
int open_listener_socket(void)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
        error("无法打开套接字");
    return s;
}

/* 绑定端口和ipv4的地址 */
void bind_to_port_ipv4(int socket, int port, char* ipv4_str)
{
    int reuse = 1;
    struct sockaddr_in name;

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = inet_addr(ipv4_str);
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int)) == -1)
        error("无法设置套接字的“重新使用端口”选项");
    if (bind(socket, (struct sockaddr *) &name, sizeof(name)) == -1)
        error("无法绑定端口");
}

int my_accept(int socket)
{
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d = accept(socket, (struct sockaddr*)&client_addr, &address_size);
    if (connect_d == -1)
        error("无法打开副套接字");
}

/* 通过给定的套接字发出消息 */
int say(int socket, char* s)
{
    int result = send(socket, s, strlen(s) ,0);
    if (result == -1)
        fprintf(stderr, "向客户端发送信息时出错\n", strerror(errno));
    return result;
}

/* 从套接字中读取换行符前的内容到给定缓冲区,并返回读取的字符数 */
/* len: 最大读取数 */
int read_in(int socket, char* buf, unsigned int len)
{
    char* s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    while ((c > 0) && (s[c-1] != '\n'))
    {
        s += c;
        slen -= c;
    }
    if (c < 0)
        return c;
    else if (c == 0)
        buf[0] = '\0';
    else
        s[c-1] = '\0';
    return len - slen;
}

/* 将参数输出并打印错误原因，然后退出程序 */
void error(char* str)
{
    fprintf(stderr, "%s : %s\n", str, strerror(errno));
    exit(1);
}

/* 将目标字符串第一个空字符前的所有字符转换为空字符 */
void empty_str(char* str)
{
    for (int i = 0; i < strlen(str); i++)
        str[i] = '\0';
}