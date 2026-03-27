#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
// Definimos el tamaño de los vectores
#define TAM 100  
 
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <num_hilos>\n", argv[0]);
        return 1;
    }
    // Definimos el tamaño de los hilos
    int n = atoi(argv[1]);  
 
    int a[TAM], b[TAM], c[TAM];
 
    // Inicializar vectores con valores aleatorios entre [0, 99]
    srand(42);
    for (int i = 0; i < TAM; i++) {
        a[i] = rand() % 100;
        b[i] = rand() % 100;
    }
 
    // Suma paralela: c = a + b
    #pragma omp parallel for num_threads(n)
    for (int i = 0; i < TAM; i++) {
        c[i] = a[i] + b[i];
    }
 
    // Imprimimos los resultados
    printf("Suma paralela de vectores a + b con %d hilos\n", n);
    printf("%-5s  %-5s  %-5s  %-5s\n", "i", "a[i]", "b[i]", "c[i]");
    for (int i = 0; i < TAM; i++) {
        printf("%-5d  %-5d  %-5d  %-5d\n", i, a[i], b[i], c[i]);
    }
 
    return 0;
}
 