#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SIZE 256
#define MAXLINE 4096
#define PORT 2222

void str_echo(FILE *fp,int sock);

int main (int argc,char *argv[])
{
	int sock;
	char com[SIZE];
	struct sockaddr_in adr;
	struct hostent *hp, *gethostbyname();

	if (argc != 2)
	{
		fprintf(stderr, "Uso: %s<host> \n",argv[0] );
		exit(1);
	}

	if ((sock = socket (PF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("imposible crecion del socket ");
		exit(2);
	}

	if ((hp = gethostbyname(argv[1]))==NULL)
	{
		perror("Error: nombre de la maquina desconocido.");
		exit(3);
	}

	adr.sin_family = PF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);
	adr.sin_port = htons (PORT);
	bcopy(hp->h_addr,&adr.sin_addr,hp->h_length);

	if (connect(sock,&adr,sizeof(adr))==-1)
	{
		perror("connect");
		exit(4);
	}

	str_echo (stdin, sock);

}

void str_echo(FILE *fp, int sock)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	int n;

	while (fgets(sendline,MAXLINE,fp) != NULL)
	{

		write(sock,sendline,strlen(sendline));
		
		n=read(sock,recvline,MAXLINE);

		if(n==0)
		{
			printf("servidor desconectado\n");
			exit(5);
		}
		else{
			printf("%i\n", n);
			FILE *f2;
			f2=fopen("dedesclient.txt","w");
		while(n>=MAXLINE)
		{
			fwrite(recvline,1,strlen(recvline),f2);
			memset(recvline,0,strlen(recvline));
			
			n=read(sock,recvline,MAXLINE);

		}
		fwrite(recvline,1,strlen(recvline),f2);

		memset(recvline,0,MAXLINE);
		fclose(f2);

	}


		printf("recvline asi ? \n" );
		
		memset(sendline,0,MAXLINE);
		break;
	}
}