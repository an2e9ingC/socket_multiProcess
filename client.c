/*************************************************************************
> File Name: client.c
> Author: Michael Jay
> mail: xuchuanwork@foxmail.com
> Created Time: 2016年07月06日 星期三 14时03分34秒
> Function: 
 ************************************************************************/

#include "myhead.h"

#define BUFF_SIZE 4096 
#define PORT 9888
#define IP "127.0.0.1"

int main(int argc, char* argv[])
{
	int sockfd;
	char buf[BUFF_SIZE];
	SAIN servaddr;
	int real_read;

	/*build socket */
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	/*init servaddr */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	//servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(IP);
	//servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	Connect(sockfd, (SA*) &servaddr, sizeof(servaddr));

	/*  send message to server */
	while(1)
	{
		printf("\ninput>");
		fgets(buf, BUFF_SIZE, stdin);
		if(strcmp("quit\n", buf)==0)
		{
			printf("QUITING...\n");
			//break; //这里不要使用break，因为break虽然结束本循环
			//但是本进程并未结束，server发出FIN请求后，不会收到本进程的回复
			//发送的FIN包中的是RST=set，而不是Fin=set
			close(sockfd);
			exit(EXIT_SUCCESS);
		}
		//发送给server
		Send(sockfd, buf, strlen(buf), 0);

		//清空buf，以备接受来自server的消息
		bzero(buf, BUFF_SIZE);

		//接收server的消息
		real_read =	Recv(sockfd, buf, BUFF_SIZE-1, 0);
		buf[real_read] = '\0';
		printf("Server: %s", buf);
		//清空buf，以备接受来自键盘的输入
		bzero(buf, BUFF_SIZE);
	}
	printf("Clinet Exit.\n");
	close(sockfd);

    exit(0);
}
