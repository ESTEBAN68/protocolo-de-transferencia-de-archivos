/*Servidor quien basicamente hara los prodecimientos segun la peticion del cliente para devolverle una respuesta.*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
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
/*funcion para abrir el directorio y contar los archivos*/
char *Listar();
/*funcion que me retorna los detalles de un archivo*/
char *Info(char *peticion);
/*funcion que saca el tamanio del archivo*/
char *Tamanio(char *archivo);




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

char *Listar()
{	
	DIR *dir;
	/* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
	struct dirent *ent;
	/*Debido a que retornamos una variable local debemos asignar un espacio en memoria para que la misma se mantenga fuera de este metodo*/
	char *informacion = (char *)malloc(sizeof(char *)*256);
	memset(informacion,0,256);
	int pos=0;
	char posc[256];
	memset(posc,0,256);
	/* Empezaremos a leer en el directorio actual */
    dir = opendir (".");
    /* Miramos que no haya error */
    if (dir == NULL)
    {
    	perror("No puedo abrir el directorio.\n");
    	exit(0);	
    }
    /* Una vez nos aseguramos de que no hay error*/
    /* Leyendo uno a uno todos los archivos que hay */
    while ((ent = readdir (dir)) != NULL)
    {
      /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
    	{
    		pos++;
    		sprintf(posc,"%i) ",pos);
      		/* Leemos cada archivo y lo concatenamos en la informacion a retornar */
      		strcat(informacion,"@ ");
      		strcat(informacion,posc);
      		strcat(informacion,ent->d_name);
      		strcat(informacion,"\n");
    	}
    }

  	closedir (dir);
  	return informacion;
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
char *Tamanio(char *archivo)
{
	FILE *fich;
	int ftam;
	char tam[256];
	memset(tam,0,256);
 	char *envio = (char *)malloc(sizeof(char *)*256);
 	memset(envio,0,256);
 	strcpy(envio,"        Informacion del archivo   \n");
 	strcat(envio,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n@\n");
 	strcat(envio,"@ Archivo: ");
  	strcat(envio,archivo);

  fich=fopen(archivo, "r");
  if (fich)
    {
      fseek(fich, 0, SEEK_END);
      ftam=ftell(fich);
      fclose(fich);
      /* Si todo va bien, ingresamos el tamanio */
      sprintf(tam,"%i",ftam);
      strcat(envio,"\n@");
      strcat(envio," Tamanio: ");
      strcat(envio,tam);
      strcat(envio," bytes ");
      strcat(envio,"\n@");
    }else
    {
      strcat(envio," **** bytes ");
    }
    return envio;
}
char *Info(char *peticion)
{
	DIR *dir;
	struct dirent *ent;
	char *informacion = (char *)malloc(sizeof(char *)*500);
	memset(informacion,0,256);
    dir = opendir (".");
    if (dir == NULL)
    {
    	perror("No puedo abrir el directorio.\n");
    	exit(0);	
    }
    int h=0;
    while ((ent = readdir (dir)) != NULL)
    {
	    if(strcmp(peticion,ent->d_name)==0)
	    {
	    	if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
	    	{
	      		/* Leemos cada archivo y lo concatenamos en la informacion a retornar */
	      		strcpy(informacion,Tamanio(ent->d_name));
	    	}
	    	h=1;
	    }      
    }
  	closedir (dir);
  	if (h==1){
  	char path[256];
  	memset(path,0,256);
  	strcpy(path,peticion);
  	//strcat(path,peticion);
  	printf("path: %s\n",path);
  	struct stat st;
    	if( stat(path, &st) == -1 )
    	{	
    		strcat(informacion," Error leyendo el archivo especifico.");
    	}else
    	{
			strcat(informacion," Hora ultimo cambio: ");
	        strcat(informacion,ctime(&st.st_mtime));
	        strcat(informacion,"@");
	        strcat(informacion," Hora ultimo acceso: ");
	        strcat(informacion,ctime(&st.st_atime));
	        strcat(informacion,"@\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    	}
	}
	else
	{
		strcpy(informacion,"el archivo no existe");
	}
	memset(peticion,0,strlen(peticion));
  	return informacion;
}

void dg_echo(int sockfd, struct sockaddr *pcliaddr, long clilen)
{	
	int n;
	socklen_t len;

	char msg[MAXLINE];
	memset(msg,0,MAXLINE);

	char res[500];
	memset(res,0,500);
	/*Datos que vamos a almacenar del cliente */
	char **vectorDatos;
	char nombre[256];
	memset(nombre,0,256);
	char token[256];
	memset(token,0,256);
	char peticion[256];
	memset(peticion,0,256);

	printf("A la espera de un cliente......................\n");
	for(;;)
	{
		len = clilen;
		/*Recibimos la informacion del cliente*/
		memset(msg,0,256);
		n = recvfrom(sockfd,msg,MAXLINE,0,pcliaddr,&len);
		/*Aplicamos un split para obtener los datos apartados*/
		/*limpiando vector de datos*/

		vectorDatos = str_split(msg,',');
		/*guardamos los datos por aparte*/
		strcpy(nombre,vectorDatos[0]);
		strcpy(token,vectorDatos[1]);

		FILE *f3;
		f3=fopen(nombre,"r");
		if(f3!=NULL)
		{
		char lectura[1000];
		memset(lectura,0,1000);
		fread(lectura,1,1000,f3);
		printf("leyo: %s \n",lectura );
			if(strcmp(lectura,token)==0)
			{
				memset(peticion,0,256);
				strcpy(peticion,vectorDatos[2]);
				printf("peticion: %s\n",peticion );
				printf("vectorDatos: %s\n",vectorDatos[2]);
				if(strcmp(peticion,"Listar") == 0)
		    	{
		    		memset(res,0,500);
		    		strcpy(res,"      Lista    \n");
		    		strcat(res,"@@@@@@@@@@@@@@@@@@@@@\n");
		    		strcat(res,"@\n");
		    		strcat(res,Listar());
		    		strcat(res,"@\n");
		    		strcat(res,"@@@@@@@@@@@@@@@@@@@@@");
		    		sendto(sockfd,res,strlen(res),0,pcliaddr,len);
		    		memset(peticion,0,256);
		    		memset(res,0,500);
	    		}
	    		else
	    		{
	    			printf("%s  - \n",peticion);
		    		strcpy(res,Info(peticion));
		    		memset(peticion,0,256);
		    		sendto(sockfd,res,strlen(res),0,pcliaddr,len);
		    		memset(res,0,500);
	    		}	
	    		memset(peticion,0,256);

			}
			else
			{
				strcpy(res,"   Error en la autentificacion token ");
				sendto(sockfd,res,strlen(res),0,pcliaddr,len);

			}

		}
		else
		{
			strcpy(res,"   Error en la autentificacion usuario");
			sendto(sockfd,res,strlen(res),0,pcliaddr,len);
		}

		fclose(f3);
		

	}
}