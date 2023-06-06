#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>



#define TAM_TABLA_HASH 1160

typedef struct Nodo {
    int idDestino;
    int hora;                       // 0 - 23
    float tiempoMedio;
    struct Nodo* siguiente;
} Nodo;

typedef struct TablaHash {
    Nodo* tabla[TAM_TABLA_HASH];
}TablaHash;

 int funcionHash(int idOrigen,int idDestino, int hora) {
     return (((idOrigen*31)^idDestino)*17^hora) % TAM_TABLA_HASH;
 }
 Nodo* crearNodo(int idDestino, int hora, float tiempoMedio) {
     Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
     nuevoNodo->idDestino = idDestino;
     nuevoNodo->hora = hora;
     nuevoNodo->tiempoMedio = tiempoMedio;
     nuevoNodo->siguiente = NULL;
     return nuevoNodo;
 }
 void insertarElemento(TablaHash* tabla, int idOrigen, int idDestino, int hora, float tiempoMedio) {
     int indice = funcionHash(idOrigen,idDestino, hora);
     Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
     nuevoNodo->idDestino = idDestino;
     nuevoNodo->hora = hora;
     nuevoNodo->tiempoMedio = tiempoMedio;
     nuevoNodo->siguiente = NULL;

     if (tabla->tabla[indice] == NULL) {
         tabla->tabla[indice] = nuevoNodo;
     } else {
         Nodo* temp = tabla->tabla[indice];
         while (temp->siguiente != NULL) {
             temp = temp->siguiente;
         }
         temp->siguiente = nuevoNodo;
     }
 }
 Nodo* BuscarTiempoMedio(TablaHash* tabla, int idOrigen, int idDestino, int hora) {
     int indice = funcionHash(idOrigen,idDestino, hora);
     Nodo* temp = tabla->tabla[indice];
     while (temp != NULL) {
         if (temp->idDestino == idDestino && temp->hora == hora) {
             return temp;
         }
         temp = temp->siguiente;
     }
     return NULL;
 }
Nodo* BuscarTiempoBin(FILE* binario, int idOrigen,int idDestino,int hora){
    Nodo* temp = (Nodo*)malloc(sizeof(Nodo));
    while(fread(temp, sizeof(Nodo), 1, binario)!=0){
        if(temp->idDestino == idDestino && temp->hora == hora){
            return temp;
        }
    }   
    return NULL;
    free(temp);
}

int HASH(){

    FILE* archivoCSV=fopen("bogota-cadastral-2019-3-All-HourlyAggregate.csv", "r");
    if(archivoCSV == NULL){
        printf("Error al abrir el archivo CSV.\n");
        return 1;
    }
    FILE* archivoBinario=fopen("indexado.bin", "wb");
    if(archivoBinario == NULL){
        printf("Error al abrir el archivo binario.\n");
        return 1;
    }
    char linea[100];
    while(fgets(linea, sizeof(linea), archivoCSV)){
        int idOrigen, idDestino, hora;
        float tiempoMedio;
        sscanf(linea, "%d,%d,%d,%e", &idOrigen, &idDestino, &hora, &tiempoMedio);
        Nodo* nuevoNodo = crearNodo(idDestino, hora, tiempoMedio);
        fwrite(nuevoNodo, sizeof(Nodo), 1, archivoBinario);
        free(nuevoNodo);
    }
    fclose(archivoCSV);
    fclose(archivoBinario);
    return 0;
}
float Buscar(int idOrigen, int idDestino, int hora){
    FILE* archivoBinario=fopen("indexado.bin", "rb");
    if(archivoBinario == NULL){
        printf("Error al abrir el archivo binario.\n");
        return 1;
    }
    Nodo* temp = BuscarTiempoBin(archivoBinario, idOrigen, idDestino, hora);
    if(temp == NULL){
        printf("NA \n");
    }
    else{
        //printf("\nTiempo medio de viaje: %f \n", temp->tiempoMedio);
        return temp->tiempoMedio;
        
    }
    fclose(archivoBinario);
    return 0;
}
