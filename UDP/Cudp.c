/* lector de archivos, para las funciones "listar archivos" y "informacion del archivo"
	cliente debe ingrear nombre y token, en caso de "informacion del archivo" tambien 
	el nombre del archivo que necesita consultar.*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//Costantes para la conexion del socket
#define MAXLINE 256
#define SERV_PORT 14000

/*Función para devolver un error en caso de que ocurra */
void error(const char *s);
/* Funcion para envio de informacion a servidor*/
void dg_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, long servlen,char *informacion);

int main(int argc,char **argv)
{
	/*Descriptor del socket*/
	int sockfd;
	/*Estructura para parametros del socket*/
	struct sockaddr_in serveraddr;
	/*Token del usuario*/
	char token[256];
	memset(token,0,256);
	/*Nombre del usuario*/
	char nombre[256];
	memset(nombre,0,256);
	/*Peticion del usuario*/
	char peticion[256];
	memset(peticion,0,256);
	/*Informacion a enviar */
	char informacion[256];
	memset(informacion,0,256);

	if(argc != 5)
	{
		fprintf(stdout, "Uso: cliente <direccion IP> <Nombre> <Token> <peticion>\n");
	}

	/*extraemos el nombre*/ 
	strcpy(nombre,argv[2]);
	/*extraemos el token*/
	strcpy(token,argv[3]);
	/*extraemos la peticion*/
	strcpy(peticion,argv[4]);
	/*Concatenamos la informacion para enviar a servidor*/
	strcat(informacion,nombre);
	strcat(informacion,",");
	strcat(informacion,token);
	strcat(informacion,",");
	strcat(informacion,peticion);
	/*configuramos los parametros del la estructura sockaddr_in*/
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = PF_INET;
	serveraddr.sin_port = htons(SERV_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	/*Se crea el socket y se serciora que no halla error en la creacion */
	sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if(socket<0)
	{error("Error creando el socket.");}
	/*Hacemos uso del socket para enviar la informacion al servidor*/
	dg_cli(stdin,sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr),informacion);

	return EXIT_SUCCESS;

}
void error(const char *s)
{
  /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
  perror (s);
  exit(EXIT_FAILURE);
}

void dg_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, long servlen,char *informacion)
{
  char recvline[MAXLINE+1],sendline[MAXLINE];
  memset(sendline,0,MAXLINE);
  memset(recvline,0,(MAXLINE+1));
  /*Aplicamos la informacion a sendline*/
  strcpy(sendline,informacion);
  /*Enviamos a servidor*/
  sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
  /*Recibimos una respuesta de servidor*/
  recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
  printf("\n%s\n",recvline);
}