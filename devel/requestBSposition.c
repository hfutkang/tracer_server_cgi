#include   <stdio.h>
#include   <stdlib.h>
#include   <string.h>
#include   <sys/types.h>
#include   <sys/socket.h>
#include   <errno.h>
#include   <unistd.h>
#include   <netinet/in.h>
#include   <limits.h>
#include   <netdb.h>
#include   <arpa/inet.h>
#include   <ctype.h>
#include   <syslog.h>
#include   "http_parser.h" 
/********************************************
功能：搜索字符串右边起的第一个匹配字符
********************************************/
char* Rstrchr(char* s, char x)   
{
    int i = strlen(s);
    if(!(*s))
    {
        return 0;
    }
    while(s[i-1]) 
    {
        if(strchr(s+(i-1), x))    
        {
            return (s+(i-1));   
        }
        else  
        {
            i--;
        }
    }
    return 0;
}
 
/********************************************
功能：把字符串转换为全小写
********************************************/
void   ToLowerCase(char* s)   
{
    while(*s && *s!='\0' )   
    {
        *s=tolower(*s++);
    }
    *s = '\0';
}
 
/**************************************************************
功能：从字符串src中分析出网站地址和端口，并得到用户要下载的文件
***************************************************************/
void GetHost(char* src, char* web, char* file, int* port)   
{
    char* pA;
    char* pB;
    memset(web, 0, sizeof(web));
    memset(file, 0, sizeof(file));
    *port = 0;
    if(!(*src)) 
    {
        return;
    }
    pA = src;
    if(!strncmp(pA, "http://", strlen("http://")))  
    {
        pA = src+strlen("http://");
    }
    else if(!strncmp(pA, "https://", strlen( "https://")))    
    {
        pA = src+strlen( "https://");
    }
    pB = strchr(pA, '/');
    if(pB)    
    {
        memcpy(web, pA, strlen(pA)-strlen(pB));
        if(pB+1)  
        {
            memcpy(file, pB+1, strlen(pB)-1);
            file[strlen(pB)-1] = 0;
        }
    }
    else    
    {
        memcpy(web, pA, strlen(pA));
    }
    if(pB)
    {
        web[strlen(pA) - strlen(pB)] = 0;
    }
    else    
    {
        web[strlen(pA)] = 0;
    }
    pA = strchr(web, ':');
    if(pA)   
    {
        *port = atoi(pA + 1);
    }
    else  
    {
        *port = 80;
    }
}

/*********************************************************************
*filename:   httpclient.c
*purpose:   HTTP协议客户端程序，可以用来下载网页
*********************************************************************/
int requestBSposition(char *url, http_parser_settings *settings)
{
	int sockfd = 0;
	char buffer[1024] = "";
	struct sockaddr_in *server_addr = malloc(sizeof(struct sockaddr_in));
	struct hostent   *host;
	int portnumber = 0;
	int nbytes = 0;

	char host_addr[256] = "";
	char host_file[1024] = "";
	char request[1024] = "";

	int send = 0;
	int totalsend = 0;
	int nparsed = 0;
	char *pt;
	
	GetHost(url, host_addr, host_file, &portnumber);/*分析网址、端口、文件名等*/
	if((host=gethostbyname(host_addr)) == NULL)/*取得主机IP地址*/
	{
		return -1;
	}
	
	/*   客户程序开始建立   sockfd描述符   */
	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1)/*建立SOCKET连接*/
	{
		return -1;
	}
	
	/*   客户程序填充服务端的资料   */
	bzero(server_addr,sizeof(struct sockaddr_in));
	server_addr->sin_family=AF_INET;
	server_addr->sin_port=htons(portnumber);
	server_addr->sin_addr=*((struct in_addr*)host->h_addr);

	/*   客户程序发起连接请求   */
	if(connect(sockfd, (struct sockaddr*)(server_addr), sizeof(struct sockaddr)) == -1)/*连接网站*/
	{
		return -1;
	}
	
	sprintf(request,   "GET /%s HTTP/1.1\r\nAccept:*/*\r\nAccept-Language:zh-cn\r\nUser-Agent: Mozilla/4.0(compatible;MSIE5.01;WindowsNT5.0)\r\nHost:%s:%d\r\nConnection:Close\r\n\r\n ", host_file, host_addr, portnumber);
	/*取得真实的文件名*/
	if(host_file && *host_file)    
	{
		pt = Rstrchr(host_file, '/');
	}
	else  
	{
		pt = 0;
	}

	/*发送http请求request*/
	send = 0;
	totalsend = 0;
	nbytes=strlen(request);

	while(totalsend < nbytes) 
	{
		char p[10];
		send = write(sockfd, request+totalsend, nbytes-totalsend);
		if(send < 0)    
		{
		    return -1;
		}
		totalsend += send;
	}
	
//	http_parser_settings settings;
//	memset(&settings, 0, sizeof(settings));
//	settings.on_body = &on_body_callback;

	http_parser parser;
	http_parser_init(&parser, HTTP_RESPONSE);
    /*   连接成功了，接收http响应，response   */
    while((nbytes=read(sockfd,buffer,1024)))
    {	
	http_parser_execute(&parser, settings, buffer, nbytes);
    }
    /*   结束通讯   */
    close(sockfd);
    return 0;
}
//////////////////////////////httpclient.c   结束///////////////////////////////////////////
