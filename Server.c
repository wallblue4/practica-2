//quiero crear un servidor que reciba datos del cliente
//y ejecute un programa en el servidor
//el cliente envia un comando y el servidor lo ejecuta
//el servidor envia la salida del comando al cliente
//el cliente recibe la salida del comando y la muestra en pantalla
//el cliente envia un comando de terminacion y el servidor termina
//el servidor registra en un archivo .log las acciones realizadas
//el servidor recibe un comando de terminacion y termina

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "libreria.h"
#include <errno.h>


#define PORT 8080 //puerto de conexion
#define BACKLOG 31 //cantidad de conexiones permitidas   
#define MAXDATASIZE 1024 //cantidad de datos a recibir
#define MAXLINE 4096 /*max text line length*/
#define LISTENQ 32 /*maximum number of client connections*/

void guardarRegistro(char *registro, char *ip,int idOrigen, int idDestino, int hora){
    FILE *file;
    file = fopen("registro.log", "a");
    if(file == NULL){
        perror("Error al abrir el archivo\n");
        exit(EXIT_FAILURE);
}
    time_t tiempo = time(NULL);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
    fprintf(file, "%s %s %d-%d-%d\n", output, ip, idOrigen, idDestino, hora);
    fclose(file);
}

int main()
{   
    HASH();
    printf("hash Completed.\n");
    

    int idOrigen, idDestino, hora;
    float tiempoMedio;
    int opcion;

    int fd, fd2; // ficheros descriptores
    struct sockaddr_in server; // informacion del servidor
    struct sockaddr_in client; // informacion del cliente
    socklen_t sin_size;
    char buf[MAXDATASIZE]; // buffer de datos recibidos
    char buf2[MAXDATASIZE]; // buffer de datos rec
    char ip[INET_ADDRSTRLEN]; //ip del cliente
    

    if((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
        printf("error en socket()\n");
        exit(-1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY; // usa mi IP   
    bzero(&(server.sin_zero),8); // funcion que rellena con ceros
    if(bind(fd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1) {
        printf("error en bind() \n");
        exit(-1);
    }
    if(listen(fd,BACKLOG) == -1) {  // llamada a listen()
        printf("error en listen()\n");
        exit(-1);
    }
    printf("Servidor iniciado\n");

    if((fd2 = accept(fd,(struct sockaddr *)&client,&sin_size))==-1) {
        printf("error en accept()\n");
        exit(-1);
    }
    printf("Se obtuvo una conexión desde %s\n", inet_ntoa(client.sin_addr) ); // muestra la IP del cliente

    while(1){
        int bytesRecibidos = recv(fd2,buf,MAXDATASIZE,0);
        printf("bytes recibidos %d\n",bytesRecibidos);
        printf("buf %s\n",buf);
        if(bytesRecibidos == -1){
            perror("Error al recibir datos\n");
            exit(EXIT_FAILURE);
        }
        else if(bytesRecibidos == 0){
            printf("El cliente se ha desconectado\n");
            break;
        }else if(bytesRecibidos > 0){
            
            char *token = strtok(buf, ";");
            idOrigen = atoi(token);
            token = strtok(NULL, ";");
            idDestino = atoi(token);
            token = strtok(NULL, ";");
            hora = atoi(token);


            printf("bufer %s",buf);
            tiempoMedio = Buscar(idOrigen, idDestino, hora);
            snprintf(buf2, sizeof(buf2), "%f", tiempoMedio);
            printf("tiempo medio %f\n",tiempoMedio);
            write(fd2, buf2, sizeof(buf2));
            guardarRegistro(buf, inet_ntoa(client.sin_addr),idOrigen, idDestino, hora);


        }}
    return 0;    
    }
