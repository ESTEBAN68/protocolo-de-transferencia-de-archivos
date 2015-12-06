/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "errno.h"
#include "rpc/rpc.h"
#include "string.h"
#include "rpc.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "assert.h"

#define SIZE 256
#define MAXLINE 4096
#define PORT 2222
#define SERV_PORT 14000

char 	usuario [256];
int 	clave;
char 	rserv[256];
int 	inicio();
void 	imprimir(char * comando);
void 	imprimirayuda();
void 	listar();
void 	infor(char* name);
void 	traer(char* ruta);
void 	str_echo(FILE *fp,int sock,char *traer);
void 	dg_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, long servlen,char *informacion);

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

 	int inicio()
 	{
 		char salida[256];
		printf(">>");
		scanf("%s",salida);
		int c=0;
		char **tokens;
		if(salida[0]!='<')
		{
			if(strstr(salida,"<")!=NULL)
			{
				c=1;
				tokens= str_split(salida,'<');
			}
		}
		if(strcmp(salida,"salir")==0)
		{
			memset(salida,0,256);
			return 1;
		} 
		else if(strcmp(salida,"listar")==0)
		{
			printf("aca");
			memset(salida,0,256);
			listar();
			return 0;
		} 
		else if (strcmp(salida,"--help")==0)
		{
			memset(salida,0,256);
			imprimirayuda();	
			return 0;
		}
		else if ((c==1))
		{
			if((strcmp(tokens[0],"info")==0))
			{
				char **tokens2;
				if(strstr(tokens[1],">")!=NULL)
				{
					if(tokens[1][0]!='>')
					{
						tokens2= str_split(tokens[1],'>');
						infor(tokens2[0]);

					}
					else
					{
						imprimir(salida);
					}
				}
				else
				{
					imprimir(salida);
				}
				memset(salida,0,256);
				return 0;
			}
			else if((strcmp(tokens[0],"traer")==0))
			{
				char **tokens2;
				if(strstr(tokens[1],">")!=NULL)
				{
					if(tokens[1][0]!='>')
					{
						tokens2= str_split(tokens[1],'>');
						traer(tokens2[0]);
					}
					else
					{
						imprimir(salida);
					}
				}
				else
				{
					imprimir(salida);
				}
				memset(salida,0,256);
				return 0;
			}
			else 
			{
				imprimir(salida);
				return 0;
			}
		}
		else
		{
			imprimir(salida);
			memset(salida,0,256);
			return 0;
		}	
 	}
 	void listar()
 	{
 		int sockfd;
 		struct sockaddr_in serveraddr;
 		char informacion[256];
		memset(informacion,0,256);
		strcat(informacion,usuario);
 		strcat(informacion,",");
 		char token[25];
 		sprintf(token,"%i",clave);
		strcat(informacion,token);
		strcat(informacion,",");
		strcat(informacion,"Listar");
 		bzero(&serveraddr,sizeof(serveraddr));
 		serveraddr.sin_family = PF_INET;
		serveraddr.sin_port = htons(SERV_PORT);
		serveraddr.sin_addr.s_addr = inet_addr(rserv);
 		sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if(socket<0)
	{error("Error creando el socket.");}
	/*Hacemos uso del socket para enviar la informacion al servidor*/
	dg_cli(stdin,sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr),informacion);

 	}
 	void infor(char* name)
 	{
 		int sockfd;
 		struct sockaddr_in serveraddr;
 		char informacion[256];
		memset(informacion,0,256);
		strcat(informacion,usuario);
 		strcat(informacion,",");
 		char token[25];
 		sprintf(token,"%i",clave);
		strcat(informacion,token);
		strcat(informacion,",");
		strcat(informacion,name);
 		bzero(&serveraddr,sizeof(serveraddr));
 		serveraddr.sin_family = PF_INET;
		serveraddr.sin_port = htons(SERV_PORT);
		serveraddr.sin_addr.s_addr = inet_addr(rserv);
 		sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if(socket<0)
	{error("Error creando el socket.");}
	/*Hacemos uso del socket para enviar la informacion al servidor*/
	dg_cli(stdin,sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr),informacion);

 	}

 	void traer(char* traer)
 	{
		int sock;
		char com[SIZE];
		struct sockaddr_in adr;
		struct hostent *hp, *gethostbyname();
		if ((sock = socket (PF_INET,SOCK_STREAM,0)) == -1)
		{
			perror("imposible crecion del socket ");
			exit(2);
		}
		if ((hp = gethostbyname(rserv))==NULL)
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
		str_echo (stdin, sock,traer);
 	}
 	void str_echo(FILE *fp, int sock,char *traer)
	{
		char sendline[MAXLINE], recvline[MAXLINE];
		int n;
		while (fgets(sendline,MAXLINE,fp) != NULL)
		{
			strcpy(sendline,traer);
			write(sock,usuario,strlen(usuario));
			n=read(sock,recvline,MAXLINE);
			if(strcmp(recvline,"leido")==0)
			{
				printf("ACCEDIENDO AL SERVIDOR .... \n");
			}
			else
			{
				printf("error de autentificacion ...\n");
				exit(-1);
			}
			char pass[256];
			sprintf(pass,"%i",clave);
			write(sock,pass,5);
			n=read(sock,recvline,MAXLINE);
			if(strcmp(recvline,"leido")==0)
			{
				printf("AUTENTIFICANDO \n");
			}
			else
			{
				printf("ERROR DE autentificacion \n");
				exit(-1);
			}
			write(sock,sendline,strlen(sendline));
			n=read(sock,recvline,MAXLINE);
			if(n==0)
			{
				printf("servidor desconectado\n");
				exit(5);
			}
			else if(strcmp(recvline,"no")==0)
			{
				printf("Este archivo no existe \n");
			}
			else
			{
				FILE *f2;
				f2=fopen(traer,"w");
				while(n>=MAXLINE)
				{
					fwrite(recvline,1,strlen(recvline),f2);
					memset(recvline,0,strlen(recvline));
					n=read(sock,recvline,MAXLINE);
				}
				fwrite(recvline,1,strlen(recvline),f2);
				memset(recvline,0,MAXLINE);
				fclose(f2);
				printf("completado ..\n");
			}
			memset(sendline,0,MAXLINE);
			close(sock);
			break;
		}
	}
 	void imprimir(char* comando)
 	{
 		printf("El comando < %s >  no se reconoce intente --help \n",comando);
 	}
 	void imprimirayuda()
 	{
 		printf("Comandos validos\n");
 		printf("listar                -observa en una lista los archivos alojados en el servidor\n");
	 	printf("info<nombre_archivo>       -Obtener detalles del archivo especificado   \n");
	 	printf("traer<nombre_archivo>      -Obtener una copia del archivo especificado  \n");
	 	printf("salir                      -salir\n");
	}
	int main (int argc, char **argv)
	{
		CLIENT *cl;
		CLIENT *cl2;
		proy_in in;
		proy_in2 in2;
		char user[256];
		char pass[256];
		rta *tok;
		rta *tok2; 	
	  	if(argc!=2)
	  	{
	  		printf("\n argumentos insuficientes");
	  		exit(-1);
	  	}
	  	strcpy(rserv,argv[1]);
	  	cl=clnt_create(rserv,PROY_PROG,PROY_VERS,"tcp");
	  	printf("si \n");
	  	if(cl==NULL)
	  	{
	  		printf("\n error %s",strerror(errno));
	  		exit(-1);
	  	}
	  	printf ("Escriba el usuario ");
	  	scanf("%s",(in.usuario)); 
	  	strcpy(usuario,in.usuario);
	  	printf ("Escriba la contraseña ");
	  	scanf("%s",(in.pass));     
	  	printf ("tu usuario es : %s y la contraseña es %s \n",(in.usuario),(in.pass));
		if((tok=login_1(&in,cl))==NULL)
		{
			printf("\nerror :%s",clnt_sperror(cl,argv[1]));
			exit(-1);
		}
		if(tok->rta==0)
		{
			printf("usuario o contraseña invalido\n");
			exit(1);
		}

		else
		{
			printf("bienvenido amigo \n");
		}
		clave=tok->rta;
		printf("el token es %d\n",tok->rta);
		int opc=0;
		while(opc==0)
		{
			opc=inicio();
		}
		strcpy(in2.usuario,in.usuario);
		in2.token=tok->rta;
		if((tok2=logout_1(&in2,cl))==NULL)
		{
			printf("\nerror :%s",clnt_sperror(cl,argv[1]));
			exit(-1);
		}
		printf("hasta pronto ... \n");
	}