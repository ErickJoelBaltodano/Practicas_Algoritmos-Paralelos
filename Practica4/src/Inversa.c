#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 3

int main(int argc, char *argv[]) {
    /*Recibimos el numero de hilos como 1er argumento*/
    int numero_hilos = (argc > 1) ? atoi(argv[1]) : 2;

    // Matriz B
    double B[N][N] = {{2, 0, 1}, {1, 3, 2}, {0, 1, 1}};

    // Matriz aumentada [B | I]
    double aug[N][2 * N];

    omp_set_num_threads(numero_hilos);

    // Inicializar matriz aumentada en paralelo
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < 2 * N; j++)
            aug[i][j] = (j < N) ? B[i][j] : (i == j - N ? 1.0 : 0.0);

    // Eliminación Gauss-Jordan (pivoteo, NO paralelizable por dependencia)
    for (int col = 0; col < N; col++) {
        double pivot = aug[col][col];
        if (pivot == 0.0) {
            printf("La matriz no tiene inversa.\n");
            return 1;
        }

        // Normalizar fila pivote
        for (int j = 0; j < 2 * N; j++)
            aug[col][j] /= pivot;

        // Eliminar columna en las demás filas (paralelizable)
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            if (i != col) {
                double factor = aug[i][col];
                for (int j = 0; j < 2 * N; j++)
                    aug[i][j] -= factor * aug[col][j];
            }
        }
    }

    printf("Inversa de B:\n");
    for (int i = 0; i < N; i++) {
        for (int j = N; j < 2 * N; j++)
            printf("%8.4f", aug[i][j]);
        printf("\n");
    }

    printf("Hilos usados: %d\n", numero_hilos);
    return 0;
}