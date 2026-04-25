#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// f(x) = (x - 1)^2 + 25  en [5, 20]
double f(double x) {
    double t = x - 1.0;
    return t * t + 25.0;
}

int main(int argc, char *argv[]) {
    // Leemos los argumentos
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }
    
    // Inicializamos Variables
    long n   = atol(argv[1]);
    double a = 5.0;
    double b = 20.0;
    double dx = (b - a) / n;
    double sum = 0.0;

    // Medimos el tiempo
    double inicio = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (long i = 0; i < n; i++) {
        double x = a + i * dx;
        sum += f(x) * dx;
    }

    double final = omp_get_wtime();

    printf("Resultado: %.10f\n", sum);
    printf("Tiempo:    %.6f segundos\n", final - inicio);

    return 0;
}