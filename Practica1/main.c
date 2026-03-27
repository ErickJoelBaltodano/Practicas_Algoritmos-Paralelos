#include <stdio.h>
#include <omp.h>

int main() {

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();

        printf("Hola desde el hilo %d de %d\n", id, total);
    }

    return 0;
}