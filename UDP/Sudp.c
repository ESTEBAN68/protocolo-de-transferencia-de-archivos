/*Servidor quien basicamente hara los prodecimientos segun la peticion del cliente para devolverle una respuesta.*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"

#define MAXLINE 256
#define SERV_PORT 14000

/*funcion para el envio de informacion al cliente*/
void dg_echo(int sockfd, struct sockaddr *pcliaddr, long clilen);
/*funcion para separar los parametros del cliente*/
char ** str_split(char * a_str,const char a_delim);
/*se valida que el cliente este logeado*/
int CompClient(char *nombre,char *token);


int main (int argc, char **argv)
{
	/*descriptor del socket*/
	int sockd;
	/*Estructura para parametros del socket*/
	struct sockaddr_in serveraddr, cliaddr;
	/*creacion del socket*/
	sockd = socket(PF_INET,SOCK_DGRAM,0);
	/*verificacion del socket*/
	if(socket<0){perror("Error en socket: \n"); return -1;}
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = PF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(SERV_PORT);
	bind(sockd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	dg_echo(sockd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

}

int CompClient(char *nombre,char *token)
{
	printf("Comprobando cliente.\n");
	return 0;
}

char ** str_split(char * a_str,const char a_delim)
{
	char ** result =0;
	size_t count =0;
	char * last_comma =0;
	char *tmp =a_str;
	char delim[2];
	delim[0]=a_delim;
	delim[1]=0;
	while(*tmp)
	{
		if(a_delim==*tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}
	count += last_comma<(a_str + strlen(a_str)-1);
	count ++;
	result=malloc(sizeof(char*)* count);
	if(result)
	{
		size_t idx = 0;
		char* token = strtok(a_str,delim);
		while(token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0,delim);
		}
		assert(idx==count-1);
		*(result + idx)=0;
	}
	return result;
}

void dg_echo(int sockfd, struct sockaddr *pcliaddr, long clilen)
{	
	int n;
	socklen_t len;

	char msg[MAXLINE];
	memset(msg,0,MAXLINE);

	char res[256];
	memset(res,0,256);
	strcpy(res,"recibido");//prueba quitar
	/*Datos que vamos a almacenar del cliente */
	char **vectorDatos;
	char nombre[256];
	memset(nombre,0,256);
	char token[256];
	memset(token,0,256);
	char peticion[256];
	memset(peticion,0,256);

	printf("A la espera de un cliente.\n");
	for(;;)
	{
		len = clilen;
		/*Recibimos la informacion del cliente*/
		n = recvfrom(sockfd,msg,MAXLINE,0,pcliaddr,&len);
		printf ("Cliente: %s \n",msg);
		/*Aplicamos un split para obtener los datos apartados*/
		vectorDatos = str_split(msg,',');
		/*guardamos los datos por aparte*/
		strcpy(nombre,vectorDatos[0]);
		strcpy(token,vectorDatos[1]);
		strcpy(peticion,vectorDatos[2]);

	    
	    printf("Split nom: %s, tok: %s, pet: %s\n",nombre,token,peticion);
	    if(CompClient(nombre,token) == 0)
	    {
	    	sendto(sockfd,res,strlen(res),0,pcliaddr,len);	
	    }else{

	    	printf("usuario no se encuentra logeado.\n");
	    	exit(0);
	    }
		

	}
}