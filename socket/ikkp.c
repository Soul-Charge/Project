/* knock-knock 服务器程序 */
/* 运行系统windows */
#include <stdio.h>
#include "Socket/Socket/myWinSocketAPI.h"
#define LINESIZE 100

int main(void)
{
	int listener_d; //服务器主套接字
	int port = 2333;
	char ipv4_str[] = "192.168.1.101";
	char recv_meg[MESSAGELEN] = { 0 };
	
	/* initialize */
	//system("chcp 65001"); //更换控制台编码为utf-8
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	/* open socket */
	listener_d = open_InetStream_socket();
	/* bind */
	bind_to_port_ipv4(listener_d, port, ipv4_str);
	/* listen */
	if (listen(listener_d, 10) == -1)
		errorp("设置监听队列长度时出错");

	puts("等待连接...");
	while (1)
	{
		/* accept */
		int connect_d = my_accept(listener_d);
		/* begin */
		say(connect_d, "Knock! Knock!\n");
		read_in(connect_d, recv_meg, MESSAGELEN);
		printf("来自客户端的输入:%s\n", recv_meg);

		/* check first enquiry */
		if (strcmp(recv_meg, "Who's there?") != 0) //if reply is valid, explain and close
		{
			say(connect_d, "Do you know this joke?\n");
			say(connect_d, "You should say: Who's there?\n");
			closesocket(connect_d);
			empty_str(recv_meg);
		}
		else // the reply is "Who's there?"
		{
			say(connect_d, "Oscar\n");
			read_in(connect_d, recv_meg, MESSAGELEN);
			printf("来自客户端的输入:%s\n", recv_meg);
			/* check second enquiry */
			if (strcmp(recv_meg, "Oscar who?") != 0)
			{
				say(connect_d, "You should say: Oscar who?\n");
				closesocket(connect_d);
				empty_str(recv_meg);
			}
			else
				say(connect_d, "Oscar silly question, you get a silly answer.\n");
		}

		/* close */
		closesocket(connect_d);
		putchar('\n');
		puts("等待连接...");
	}
	return 0;
}