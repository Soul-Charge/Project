#include "myWinSocketAPI.h"

/* 打开一个ipv4的流套接字 */
int open_InetStream_socket(void)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
        errorp("无法打开套接字");
    return s;
}

int open_domain_socket(char* host, char* port)
{
	struct addrinfo* res;
	struct addrinfo hints;
	int d_sock;
	int c;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(host, port, &hints, &res) == -1)
		errorp("无法解析地址");
	/* open socket */
	d_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (d_sock == -1)
		errorp("无法打开套接字");
	/* connect */
	c = connect(d_sock, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	if (c == -1)
		errorp("无法连接到套接字");
	return d_sock;
}

/* 绑定端口和ipv4的地址，并设置重新使用端口 */
void bind_to_port_ipv4(int socket, int port, char* ipv4_str)
{
    int reuse = 1;
    struct sockaddr_in name;

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
	inet_pton(AF_INET, ipv4_str, &name.sin_addr.s_addr);
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int)) == -1)
        errorp("无法设置套接字的“重新使用端口”选项");
    if (bind(socket, (struct sockaddr *) &name, sizeof(name)) == -1)
        errorp("无法绑定端口");
}

/* 接受连接，打开并返回副套接字 */
int my_accept(int socket)
{
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d = accept(socket, (struct sockaddr*)&client_addr, &address_size);
    if (connect_d == -1)
        errorp("无法打开副套接字");
    return connect_d;
}

/* 将套接字连接到一个远程ipv4地址 */
void connect_ipv4(int socket, char * ipstr, int port)
{
    int cs;  //connection status
    struct sockaddr_in si;
    memset(&si, 0, sizeof(si));
    si.sin_family = AF_INET;
	inet_pton(AF_INET, ipstr, &si.sin_addr.s_addr);
    si.sin_port = htons(port);

    cs = connect(socket, (struct sockaddr*) &si, sizeof(si));
    if (cs == -1)
        errorp("无法连接到远程端口");
}

/* 通过给定的套接字发出消息 */
int say(int socket, char* s)
{
    int result = send(socket, s, strlen(s) ,0);
	if (result == -1)
		error("使用套接字发出消息时出错");
    return result;
}

/* 从套接字中读取换行符前的内容到给定缓冲区,并返回读取的字符数 */
/* len: 最大读取数 */
int read_in(int socket, char* buf, unsigned int len)
{
    char* s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    if (c == 0)
        buf[0] = '\0';

    while ((c > 0) && (s[c-1] != '\n'))
    {
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
    }
    if (c < 0)
        return c;
    else
        s[c-2] = '\0'; // instead '\r'
    return len - slen;
}

/* 将目标字符串第一个空字符前的所有字符转换为空字符 */
void empty_str(char* str)
{
    for (unsigned int i = 0; i < strlen(str); i++)
        str[i] = '\0';
}

/* 输出错误信息(消息输出自带换行) */
void error(char* str)
{
	char errorStr[ERRORSIZE];
	strerror_s(errorStr, sizeof(errorStr), errno);

	puts("---*error report*---");
	fprintf(stderr, "%s\n", str);
	fprintf(stderr, "errno: %s\n", errorStr);
	fprintf(stderr, "WSAGetLastError: %d\n", WSAGetLastError());
	puts("---*error report*---");
}

/* 输出错误信息并退出程序(消息输出自带换行) */
void errorex(char* str)
{
	error(str);
	exit(1);
}

/* 输出错误信息，并暂停，按任意键后退出 (消息输出自带换行) */
void errorp(char* str)
{
	error(str);
	system("pause");
	exit(1);
}
