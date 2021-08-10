#include <WinSock2.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <Ws2tcpip.h>
#define MESSAGELEN 600
#define ERRORSIZE 200
#pragma comment(lib,"Ws2_32.lib")

int open_InetStream_socket(void);
int open_domain_socket(char* host, char* port);
void bind_to_port_ipv4(int socket, int port, char* ipv4_str);
int my_accept(int socket);
void connect_ipv4(int socket, char * ipstr, int port);
int say(int socket, char* s);
int read_in(int socket, char* buf, unsigned int len);
void empty_str(char* str);
void error(char* str);
void errorex(char* str);
void errorp(char* str);
