#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

int main(  int args , char** argv) {


    // Verificamos el numero de hilos por terminal
    int numero_hilos = atoi(argv[1]);
    omp_set_num_threads(numero_hilos);

    long long suma = 0;
    long long N = 1000000000LL;

    struct timeval inicio, fin;
    long long tiempo;

    //  Vermos la hora actual
    gettimeofday(&inicio, NULL);

    
    #pragma omp parallel
    {
        int id_hilo = omp_get_thread_num();
        int totalHilos = omp_get_num_threads();

        // Hilo 0 anuncia el inicio y reparte el rango
        if (id_hilo == 0) {
            printf("Hilo manager ha iniciado el calculo con %d hilo/\n", totalHilos);
        }

        // Cada hilo calcula su rango
        long long rango = N / totalHilos;
        long long inicio_hilo = (long long)id_hilo * rango;
        long long fin_hilo = (id_hilo == totalHilos - 1) ? N : inicio_hilo + rango - 1;

        // Suma parcial local (sin race condition)
        long long suma_local = 0;
        for (long long i = inicio_hilo; i <= fin_hilo; i++) {
            suma_local += i;
        }

        // Cada hilo imprime el rango que le toca
        printf("Hilo %d: rango [%lld - %lld]\n", id_hilo, inicio_hilo, fin_hilo);
        

        // Seccion critica: cada hilo actualiza la suma total
        #pragma omp critical
        {
            suma += suma_local;
        }
    }

    gettimeofday(&fin, NULL);
    tiempo = (fin.tv_sec - inicio.tv_sec) * 1000000LL + (fin.tv_usec - inicio.tv_usec);

    printf("\n======= RESULTADO ========\n");
    printf("Suma con %d hilo/s:       %lld\n",numero_hilos, suma);
    printf("Tiempo de ejecucion:   %lld microsegundos\n", tiempo);
    printf("\n==========================\n");

   
}