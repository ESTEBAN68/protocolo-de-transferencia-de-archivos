
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <arpa/inet.h>

#define MAXLINE 256
#define SERV_PORT 14000


void dg_echo(int sockfd, struct sockaddr *pcliaddr, long clilen)
{	
	int n;
	socklen_t len;
	char msg[MAXLINE];
	char *res = "recibido.";

	for(;;)
	{
		
		len = clilen;
		n = recvfrom(sockfd,msg,MAXLINE,0,pcliaddr,&len);

	    printf ("Cliente: %s",msg);
		sendto(sockfd,res,sizeof(res),0,pcliaddr,len);

	}
}

int main (int argc, char **argv)
{
	int sockd;
	struct sockaddr_in serveraddr, cliaddr;
	sockd = socket(PF_INET,SOCK_DGRAM,0);

	if(socket<0){perror("Error en socket: \n"); return -1;}
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = PF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(SERV_PORT);
	//fprintf(serveraddr.sin_addr.s_addr, "%sINADDR_ANY\n");
	bind(sockd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	dg_echo(sockd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

}