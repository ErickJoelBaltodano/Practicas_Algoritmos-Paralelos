#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 3

int main(int argc, char *argv[]) {
    /*Recibimos el numero de hilos como 1er argumento*/
    int numero_hilos = (argc > 1) ? atoi(argv[1]) : 2;
    
    // Inicializamos la matriz A 
    int A[N][N] = {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
    int T[N][N];

    omp_set_num_threads(numero_hilos);

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            T[j][i] = A[i][j];

    printf("Transpuesta de A:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%4d", T[i][j]);
        printf("\n");
    }

    printf("Hilos usados: %d\n", numero_hilos);
    return 0;
}