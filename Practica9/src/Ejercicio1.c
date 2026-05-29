#include <stdio.h>
#include <math.h>
#include <omp.h>

int main() {
    // Arreglo del ejemplo del PDF (1-indexado, posicion 0 no se usa)
    double A[] = {0, 8, 5, 1, 7, 2, 4, 6, 3}; // A[1..8]
    int N = 8;
    int k = 3; // N = 2^k => 8 = 2^3

    printf("=== Practica 11: Maximo Elemento ===\n");
    printf("Secuencia (N=%d, k=%d):\n", N, k);
    printf("Pos: ");
    for (int j = 1; j <= N; j++) printf("[%d] ", j);
    printf("\nVal: ");
    for (int j = 1; j <= N; j++) printf(" %g  ", A[j]);
    printf("\n\n");

    // K rondas
    for (int R = 1; R <= k; R++) {
        int iteraciones = (int)(N / pow(2, R)); // i = N / 2^R

        printf("--- Ronda R=%d | iteraciones=%d ---\n", R, iteraciones);

        #pragma omp parallel for num_threads(iteraciones)
        for (int i = 1; i <= iteraciones; i++) {
            int pos1 = (int)(pow(2, R) * i - pow(2, R - 1)); // 2^R * i - 2^(R-1)
            int pos2 = (int)(pow(2, R) * i);                 // 2^R * i

            // Si A[pos1] > A[pos2], intercambiar para que el mayor quede en pos2
            if (A[pos1] > A[pos2]) {
                double tmp = A[pos1];
                A[pos1] = A[pos2];
                A[pos2] = tmp;
                printf("  i=%d: A[%d]=%g > A[%d]=%g => intercambio\n",
                       i, pos1, A[pos2], pos2, A[pos1]);
            } else {
                printf("  i=%d: A[%d]=%g <= A[%d]=%g => sin cambio\n",
                       i, pos1, A[pos1], pos2, A[pos2]);
            }
        }

        printf("Estado tras ronda %d:\n", R);
        printf("Pos: ");
        for (int j = 1; j <= N; j++) printf("[%d] ", j);
        printf("\nVal: ");
        for (int j = 1; j <= N; j++) printf(" %g  ", A[j]);
        printf("\n\n");
    }

    printf("==> Maximo elemento: %.0f (en posicion A[%d])\n", A[N], N);
    return 0;
}
