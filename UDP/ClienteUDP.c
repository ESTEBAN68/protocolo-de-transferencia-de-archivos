/* lector de archivos, para las funciones "listar archivos" y "informacion del archivo"
	cliente debe ingrear nombre y token, en caso de "informacion del archivo" tambien 
	el nombre del archivo que necesita consultar.*/

//cabeceras para consultar directorios
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
//cabeceras para udp
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//Costantes para la conexion del socket
#define MAXLINE 256
#define SERV_PORT 14000

/*METODOS PARA CONSULTAR DIRECTORIOS

 Función para devolver un error en caso de que ocurra */
void error(const char *s);
/* Función que hace algo con un archivo */
void procesoArchivo(char *archivo);

/*METODOS PARA UDP

 Funcion para envio de informacion a servidor*/
void dg_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, long servlen,char *directorio);
/* Funcion para comprobar el usuario */
int ComprobarUser(char *nombre,int Token);

int main(int argc,char **argv)
{
  /*Descriptor del socket*/
  int sockfd;
  /*Estructura para parametros del socket*/
  struct sockaddr_in serveraddr;
  //numero de token del usuario
  int Token;
  //nombre del usuario
  char *nombre;
  /* Con un puntero a DIR abriremos el directorio */
  DIR *dir;
  /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
  struct dirent *ent;
  /* observamos que se hallan cargado todos los datos para el cliente*/
  	if(argc != 4)
	{
		fprintf(stdout, "Uso: cliente <direccion IP> <Nombre> <Token>\n");
	}
	//extraemos el token
	Token = atoi(argv[3]);
	//extraemos el nombre 
	nombre = argv[2];
	printf("nombre: %s token: %d \n",nombre,Token );//QUITAR PRUEBA
	/*configuramos los parametros del la estructura sockaddr_in*/
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = PF_INET;
	serveraddr.sin_port = htons(SERV_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	/*Se crea el socket y se serciora que no halla error en la creacion */
	sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if(socket<0){printf("Error en socket\n"); exit(0);}
	/* Empezaremos a leer en el directorio actual */
    dir = opendir (".");
    /* Miramos que no haya error */
    if (dir == NULL)
    error("No puedo abrir el directorio");
	/*por seguridad se comprueba el usuario */
	if(!ComprobarUser(nombre,Token))
	{
		error("Usuario no encontrado.");
	}
    /* Una vez nos aseguramos de que no hay error*/
    /* Leyendo uno a uno todos los archivos que hay */
    while ((ent = readdir (dir)) != NULL)
    {
      /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
    {
      /* Una vez tenemos el archivo, lo pasamos junto con los datos del socket. "ent->d_name"*/
      dg_cli(stdin,sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr),ent->d_name);
    }
    }
  closedir (dir);

  return EXIT_SUCCESS;
}
int ComprobarUser(char *nombre,int token)
{
	return 1;
	//codigo de lectura de archivos para comprobar usuario.
}

void error(const char *s)
{
  /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
  perror (s);
  exit(EXIT_FAILURE);
}

void dg_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, long servlen,char *archivo)
{
	  /*vamos a decir su tamaño en bytes */
  FILE *fich;
  long ftam;
  char recvline[MAXLINE+1];
  char *envio = archivo;

  fich=fopen(archivo, "r");
  if (fich)
    {
      fseek(fich, 0L, SEEK_END);
      ftam=ftell(fich);
      fclose(fich);
      /* Si todo va bien, decimos el tamaño */
      printf ("%30s (%ld bytes)\n", archivo, ftam);
      /*concateno en "envio" todo el string de datos y lo envio a servidor */
      strcat(envio,(char *)ftam);
      sendto(sockfd,(char *)envio,strlen(envio),0,pservaddr,servlen);
    }else
    {
      /* Si ha pasado algo, sólo decimos el nombre */
      printf ("%30s (No info.)\n", archivo);
      sendto(sockfd,(char *)archivo,strlen(archivo),0,pservaddr,servlen);
    }
    
    recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
    printf("Servidor : %s\n",recvline );
}
