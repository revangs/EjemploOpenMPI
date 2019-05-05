/* Control N2 
Nombre: Ruben Gazitua

Asignatura: Computacion Paralela y Distribuida
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale>

/* cabecera de las llamadas a MPI */
#include <mpi.h> 

/**
 * Manda un mensaje desde todos los procesos con
 * rango distinto de 0 al proceso 0. El proceso 0
 * los saca por pantalla.
 * Entrada: ninguna
 * Salida: contenido de los mensajes recibidos por 
 * el proceso 0, que los imprime
 */


int main(int argc, char** argv) {
    int mi_rango; /* rango del proceso    */
    int p; /* numero de procesos   */
    int fuente; /* rango del que envia  */
    int dest; /* rango del que recibe */
    int tag = 0; /* etiqueta del mensaje */
    int namelen; /* Largo del nombre de la maquina*/
    std::time_t t = std::time(nullptr); /* Fecha y hora */
    char mbstr[100]; /*Largo fecha y hora */
    char nombre[MPI_MAX_PROCESSOR_NAME]; /*nombre de la maquina */
    char mensaje[100]; /* mensaje  */
    MPI_Status estado; /* devuelve estado al recibir*/

    /* Comienza las llamadas a MPI */
    MPI_Init(&argc, &argv);

    /* Averiguamos el rango de nuestro proceso */
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

    /* Averiguamos el número de procesos que estan 
     * ejecutando nuestro porgrama 
     */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    MPI_Get_processor_name(nombre,&namelen); /*Comando para obtener nombre de la maquina usada*/


    if (mi_rango != 0) {
       if (std::strftime(mbstr, sizeof(mbstr), "%F %T", std::localtime(&t))) { /* %F =  "%Y-%m-%d" (the ISO 8601 date format) y T = "%H:%M:%S" (the ISO 8601 time format) (Agregando fecha y hora segun fortmato ISO)*/
        /* Crea mensaje */
        sprintf(mensaje,"%s %s",nombre, mbstr);
        dest = 0;
        /* Usa strlen+1 para que la marca /0 se transmita */
        MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR,
                dest, tag, MPI_COMM_WORLD);
       }
    } else { /* mi_rango == 0 */
        for (fuente = 1; fuente < p; fuente++) {
            MPI_Recv(mensaje, 100, MPI_CHAR, fuente,
                    tag, MPI_COMM_WORLD, &estado);
            printf("%s\n", mensaje);
        }
    }

    /* Termina con MPI. Recordemos que después de 
     * esta llamada no podemos llamar a funciones 
     * MPI, ni siquiera de nuevo a MPI_Init 
     */
    MPI_Finalize();
    return EXIT_SUCCESS;
}
