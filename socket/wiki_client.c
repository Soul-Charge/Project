#include <stdio.h>
#include "Socket/Socket/myWinSocketAPI.h"
#define BUFSIZE 255

int main(int argc, char* argv[])
{
	int d_sock;
	int bytesRecvd;
	char buf[BUFSIZE];
	char rec[256];

	/* open socket */
	d_sock = open_domain_socket("en.wikipedia.org", "80");

	/* send information */
	sprintf_s(buf, sizeof(char) * BUFSIZE, "GET /wiki/%s http/1.1\r\n", argv[1]);
	say(d_sock, buf);
	say(d_sock, "Host: en.wikipedia.org\r\n\r\n");

	/* recive information */
	bytesRecvd = recv(d_sock, rec, 255, 0);
	while (bytesRecvd)
	{
		if (bytesRecvd == -1)
			errorex("无法从服务器读取信息");
		rec[bytesRecvd] = '\0';
		printf("%s", rec);
		bytesRecvd = recv(d_sock, rec, 255, 0);
	}
	closesocket(d_sock);
	return 0;
}