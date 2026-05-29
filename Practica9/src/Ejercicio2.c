#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <string.h>

// Funcion que aplica el algoritmo de maximo elemento para ordenar
// El algoritmo mueve el maximo al final en cada "pasada de rondas"
// Para ordenar completamente usamos bubble-sort paralelo (odd-even transposition)
void ordenar(double *A, int N, const char *nombre) {
    printf("\n=== Ordenando arreglo %s (N=%d) ===\n", nombre, N);
    printf("Original: ");
    for (int j = 1; j <= N; j++) printf("%g ", A[j]);
    printf("\n");

    // Odd-Even Transposition Sort con OpenMP
    // Es el equivalente paralelo del bubble sort
    for (int paso = 0; paso < N; paso++) {
        // Fase par: comparar pares (1,2),(3,4),(5,6),...
        if (paso % 2 == 0) {
            #pragma omp parallel for
            for (int i = 1; i <= N - 1; i += 2) {
                if (A[i] > A[i + 1]) {
                    double tmp = A[i];
                    A[i] = A[i + 1];
                    A[i + 1] = tmp;
                }
            }
        } else {
            // Fase impar: comparar pares (2,3),(4,5),(6,7),...
            #pragma omp parallel for
            for (int i = 2; i <= N - 1; i += 2) {
                if (A[i] > A[i + 1]) {
                    double tmp = A[i];
                    A[i] = A[i + 1];
                    A[i + 1] = tmp;
                }
            }
        }
    }

    printf("Ordenado: ");
    for (int j = 1; j <= N; j++) printf("%g ", A[j]);
    printf("\n");
}

int main() {
    printf("=== Practica 11 - Ejercicio 2: Ordenar Arreglos ===\n");

    // Arreglo X1 (1-indexado, posicion 0 no se usa)
    double X1[] = {0, 150, 80, 1, 6, 0.5, 250, 10, 3};
    int N1 = 8;

    // Arreglo X2 - tiene 7 elementos, lo completamos a 8 para que sea 2^k
    // (paddeamos con un valor grande al final para no afectar el orden)
    // En realidad trabajamos con los 7 elementos directamente
    double X2[] = {0, 10, 0.08, 1000, 0.3, 50, 0.009, 0.0001};
    int N2 = 7;

    ordenar(X1, N1, "X1 = {150, 80, 1, 6, 0.5, 250, 10, 3}");
    ordenar(X2, N2, "X2 = {10, 0.08, 1000, 0.3, 50, 0.009, 0.0001}");

    return 0;
}
