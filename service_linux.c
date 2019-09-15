#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error(char * str);

int main(void)
{
    int listener_d;
    int reuse;
    char ipv4_str[16];
    /* open socket */
    if ((listener_d = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error("无法打开套接字");
    /* set reuse port */
    if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)))
        error("无法设置套接字的“重新使用端口”选项");
    /* bind port */
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = htons(2333);
    inet_pton(AF_INET, "127.0.0.1", &name.sin_addr.s_addr);
    if (bind(listener_d, (struct sockaddr *)&name, sizeof(name)) == -1)
        error("无法绑定端口");
    printf("绑定的ip为:%s\n", inet_ntop(AF_INET, &name.sin_addr.s_addr, ipv4_str, sizeof(ipv4_str)));
    printf("address: %s\n", ipv4_str);
    /* listen */
    if (listen(listener_d, 10))
        error("无法设置监听队列");
    puts("Waiting for connection");
    
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d;
    char * msg = "hello!\n";
    while (1)
    {
        connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        send(connect_d, msg, strlen(msg), 0);
        close(connect_d);
    }

    return 0;
}

void error(char * str)
{
    fprintf(stderr, "%s\n", str);
    exit(1);
}