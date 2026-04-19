#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 3

int main(int argc, char *argv[]) {
    int numero_hilos = (argc > 1) ? atoi(argv[1]) : 2;

    int A[N][N] = {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
    int B[N][N] = {{2, 0, 1}, {1, 3, 2}, {0, 1, 1}};
    int C[N][N];

    omp_set_num_threads(numero_hilos);

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            C[i][j] = A[i][j] + B[i][j];

    printf("Resultado A + B:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%4d", C[i][j]);
        printf("\n");
    }

    printf("Hilos usados: %d\n", numero_hilos);
    return 0;
}