#include "myWinSocketAPI.h"

/* ��һ��ipv4�����׽��� */
int open_InetStream_socket(void)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
        errorp("�޷����׽���");
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
		errorp("�޷�������ַ");
	/* open socket */
	d_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (d_sock == -1)
		errorp("�޷����׽���");
	/* connect */
	c = connect(d_sock, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	if (c == -1)
		errorp("�޷����ӵ��׽���");
	return d_sock;
}

/* �󶨶˿ں�ipv4�ĵ�ַ������������ʹ�ö˿� */
void bind_to_port_ipv4(int socket, int port, char* ipv4_str)
{
    int reuse = 1;
    struct sockaddr_in name;

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
	inet_pton(AF_INET, ipv4_str, &name.sin_addr.s_addr);
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int)) == -1)
        errorp("�޷������׽��ֵġ�����ʹ�ö˿ڡ�ѡ��");
    if (bind(socket, (struct sockaddr *) &name, sizeof(name)) == -1)
        errorp("�޷��󶨶˿�");
}

/* �������ӣ��򿪲����ظ��׽��� */
int my_accept(int socket)
{
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect_d = accept(socket, (struct sockaddr*)&client_addr, &address_size);
    if (connect_d == -1)
        errorp("�޷��򿪸��׽���");
    return connect_d;
}

/* ���׽������ӵ�һ��Զ��ipv4��ַ */
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
        errorp("�޷����ӵ�Զ�̶˿�");
}

/* ͨ���������׽��ַ�����Ϣ */
int say(int socket, char* s)
{
    int result = send(socket, s, strlen(s) ,0);
	if (result == -1)
		error("ʹ���׽��ַ�����Ϣʱ����");
    return result;
}

/* ���׽����ж�ȡ���з�ǰ�����ݵ�����������,�����ض�ȡ���ַ��� */
/* len: ����ȡ�� */
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

/* ��Ŀ���ַ�����һ�����ַ�ǰ�������ַ�ת��Ϊ���ַ� */
void empty_str(char* str)
{
    for (unsigned int i = 0; i < strlen(str); i++)
        str[i] = '\0';
}

/* ���������Ϣ(��Ϣ����Դ�����) */
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

/* ���������Ϣ���˳�����(��Ϣ����Դ�����) */
void errorex(char* str)
{
	error(str);
	exit(1);
}

/* ���������Ϣ������ͣ������������˳� (��Ϣ����Դ�����) */
void errorp(char* str)
{
	error(str);
	system("pause");
	exit(1);
}
