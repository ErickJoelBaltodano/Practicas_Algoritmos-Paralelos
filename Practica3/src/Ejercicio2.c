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
    //Definimos el número de hilos
    int n = atoi(argv[1]);  
 
    int u[TAM] = {2, 34};
    int v[TAM] = {5, 23};
    int w[TAM];
 
    // Suma paralela: w = u + v
    #pragma omp parallel for num_threads(n)
    for (int i = 0; i < TAM; i++) {
        w[i] = u[i] + v[i];
        printf("[Hilo %d] w[%d] = u[%d] + v[%d] = %d + %d = %d\n",
               omp_get_thread_num(), i, i, i, u[i], v[i], w[i]);
    }
 
    printf("\nResultado: u + v = (%d, %d)\n", w[0], w[1]);
 
    return 0;
}
 

