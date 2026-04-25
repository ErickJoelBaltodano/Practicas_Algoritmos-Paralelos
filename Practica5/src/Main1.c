#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
// f(x) = 100 - (x - 10)^4 + 50(x + 10)^2  en [3, 17]
double f(double x) {
    double a = x - 10.0;
    double b = x + 10.0;
    return 100.0 - (a * a * a * a) + 50.0 * (b * b);
}
 
int main(int argc, char *argv[]) {
    // Leemos los argumentos
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }
    // Inicializamos Variables
    long n   = atol(argv[1]);
    double a = 3.0;
    double b = 17.0;
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
 

