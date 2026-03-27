#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//Definimos el tamaño de los vectores
#define TAM 100  
 
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <num_hilos>\n", argv[0]);
        return 1;
    }
    //Definimos el número de hilos
    int n = atoi(argv[1]);  
 
    int a[TAM], b[TAM];
    long long producto = 0;
 
    // Inicializar vectores con valores aleatorios entre [0, 99]
    srand(42);
    for (int i = 0; i < TAM; i++) {
        a[i] = rand() % 100;
        b[i] = rand() % 100;
    }
 
    // Mostrar vectores
    printf("Vector a: ");
    for (int i = 0; i < TAM; i++) printf("%d ", a[i]);
    printf("\n");
 
    printf("Vector b: ");
    for (int i = 0; i < TAM; i++) printf("%d ", b[i]);
    printf("\n\n");
 
    // Producto punto paralelo con reducción
    #pragma omp parallel for num_threads(n) reduction(+:producto)
    for (int i = 0; i < TAM; i++) {
        producto += (long long)a[i] * b[i];
    }
 
    printf("Producto punto a · b = %lld\n", producto);
 
    return 0;
}