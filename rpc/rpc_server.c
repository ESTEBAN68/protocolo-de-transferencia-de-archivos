/*
* El servidor rpc, se encarga de realizar la labor de login o logout de los clientes 
* del protocolo
*/
#include "rpc.h"
#include "string.h"
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
/*Funcion split que sirve para recibir una cadena de caracteres separada por algun simbolo, y devolver 
	cada una de las palabras en una posicion de un vector de cadenas de caracteres*/
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
rta * login_1_svc(proy_in *in, struct svc_req *rqstp)
{
	char buffer[1000];
	FILE *fp;
	/*abrimos el archivo llamado users.txt, con permisos de lectura*/
	fp=fopen ("users.txt","r");
	/*Se lee y se guarda en buffer el stream*/
	fscanf(fp,"%s",buffer);
	char **tokens;
	char **user;
	int desicion=0;
	/*tokens recibe en cada una de sus posiciones la informacion que retorna el split */
	tokens= str_split(buffer,',');
	if(tokens)
	{
		int i;
		for(i=0;*(tokens+i);i++)
		{
			/*se realiza otro split para sacar el nombre de usuario y la contrasenia o pass*/
			user=str_split(*(tokens+i),'-');
			if(strcmp(in->usuario,user[0])==0)
			{
				if(strcmp(in->pass,user[1])==0)
				{
					/*bandera */
					desicion=1;
				}
			}
		}
	}
	static rta result;
	/*si se extrajo el usuario y contrasenia de manera correcta se crea un token*/
	if(desicion==1)
	{
		result.rta=rand()%1000+1;
	}
	else
	{
		result.rta=0;
	}
	fclose(fp);
	if(result.rta!=0)
	{
		char buffer[sizeof(result.rta)];
		snprintf(buffer,sizeof(buffer),"%d",result.rta);
		FILE *fp;
		fp=fopen(in->usuario,"w");
		/*Se escribe el archivo con el nombre de usuario y el token creado*/
		fputs(buffer,fp);
		fclose(fp);
	}
	return &result;
}
/*Aqui esta el codigo para la desconexion de los clientes, el servidor sabe que usuario se desconecta
  y borrara su archivo con la informacion del usuario correspondiente.*/
rta *
logout_1_svc(proy_in2 *in2, struct svc_req *rqstp)
{
	static rta result;
	char leer[10];
	FILE *fp;
	/*Se lee el archivo del usuario que se quiere desconectar*/
	fp=fopen(in2->usuario,"r");
	fscanf(fp,"%s",leer);
	char token[sizeof(in2->token)];
	result.rta=2;
	snprintf(token,sizeof(token),"%d",in2->token);
	printf("Usuario: %s desconectado. \n",in2->usuario);
	printf("Token: %d \n",in2->token );
	if(strcmp(token,leer)==0)
	{
		result.rta=0;
	}
	else{
		result.rta=1;
	}
	fclose(fp);
	/*Se elimina el archivo conrrespondiente a el usuario que se va*/
	remove(in2->usuario);
	return &result;
}