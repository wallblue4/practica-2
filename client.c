#include "libreria.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define SERVER_IP "127.0.0.1"
#define Port 8080
#define MAX_SIZE 1024

    void IngresarOrigen(int* idOrigen) {
    int origen;
    printf("Ingrese el ID de origen: ");
    scanf("%d", &origen);
    if(origen >= 0 && origen <= 1160) {
        printf("El id ingresado ha sido validado.\n");
        *idOrigen = origen;
    } else {
        printf("El id ingresado no es valido.\n");
    }
}
void IngresarDestino(int* idDestino) {
    int destino;
    printf("Ingrese el ID de destino: ");
    scanf("%d",&destino);
    if(destino >= 0 && destino <= 1160) {
        printf("El id ingresado ha sido validado.\n");
        *idDestino = destino;
    } else {
        printf("El id ingresado no es valido.\n");
    }
}
void IngresarHora(int* idHora) {
    int hora;
    printf("Ingrese la hora: ");
    scanf("%d",&hora);
    if(hora >= 0 && hora <= 23) {
        printf("La hora ingresada ha sido validada.\n");
        *idHora = hora;
    } else {
        printf("La hora ingresada no es valida.\n");
    }
}

int main(void)
{   int opcion;
    int sockfd;
    int len;
    struct sockaddr_in server_addr;
    int result;
    char buffer[MAX_SIZE];
    char buffer2[MAX_SIZE];

    // Creo el socket para el cliente
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Nombre del socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(Port);

    // strcpy(address.sun_path, "server_socket");
    len = sizeof(server_addr);
    if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Conecto el socket al servidor
    result = connect(sockfd, (struct sockaddr *)&server_addr, len);

    if (result == -1)
    {
        perror("oops: client32");
        exit(1);
    }else{
    // Leo y escribo en el socket

    
    while (opcion != 5)
    {   printf("==== Bienvenido ====\n");
        printf("1. Ingresar origen\n");
        printf("2. Ingresar destino\n");
        printf("3. Ingresar hora\n");
        printf("4. Buscar tiempo de viaje medio\n");
        printf("5. Salir\n");
        
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);
        switch(opcion) {
            int IdOrigen,IdDestino,hora;
            float tiempoMedio;
            case 1:
                IngresarOrigen(&IdOrigen);
                break;
            case 2:
                IngresarDestino(&IdDestino);
                break; 
            case 3:
                IngresarHora(&hora);
                break; 
            case 4:
    
                snprintf(buffer, MAX_SIZE, "%d;%d;%d ", IdOrigen, IdDestino, hora);
                write(sockfd, buffer, strlen(buffer));
                sleep(1);
                recv(sockfd, buffer2, MAX_SIZE, 0);
                sscanf(buffer2, "%f", &tiempoMedio);
                printf("El tiempo de viaje medio es: %f\n", tiempoMedio);
                break;
            case 5:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }

                
    }}  

    close(sockfd);
    exit(0);
}
