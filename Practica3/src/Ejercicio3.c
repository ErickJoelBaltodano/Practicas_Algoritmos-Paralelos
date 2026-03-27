#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//Definimos el tamaño de los vectores
#define TAM 2  
 
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <num_hilos>\n", argv[0]);
        return 1;
    }
    // Definimos el número de los hilos
    int n = atoi(argv[1]);  
 
    int u[TAM] = {2, 34};
    int v[TAM] = {5, 23};
    long long producto = 0;
 
    // Producto punto paralelo con reducción
    #pragma omp parallel for num_threads(n) reduction(+:producto)
    for (int i = 0; i < TAM; i++) {
        int parcial = u[i] * v[i];
        printf("[Hilo %d] u[%d] * v[%d] = %d * %d = %d\n",
               omp_get_thread_num(), i, i, u[i], v[i], parcial);
        producto += parcial;
    }
 
    printf("\nProducto punto u · v = %lld\n", producto);
 
    return 0;
}
 

