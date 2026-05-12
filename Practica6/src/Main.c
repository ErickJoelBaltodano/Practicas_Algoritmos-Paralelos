#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

/* ──────────────────────────────────────────
   Constantes
   ────────────────────────────────────────── */
#define N          1000          // Número de partículas
#define G          6.674e-11     // Constante gravitacional (m³ kg⁻¹ s⁻²)
#define MASA_MAX   1.0e30        // Masa máxima (kg)  ~masa solar
#define POS_MAX    1.0e12        // Posición máxima (m)
#define EPSILON    1.0e-10       // Suavizador para evitar división por cero

/* ──────────────────────────────────────────
   Estructura de partícula
   ────────────────────────────────────────── */
typedef struct {
    double x, y, z;   // Posición (m)
    double masa;       // Masa (kg)
} Particula;

/* ──────────────────────────────────────────
   Genera un double aleatorio en [0, max]
   ────────────────────────────────────────── */
static double rand_double(double max) {
    return ((double)rand() / RAND_MAX) * max;
}

/* ──────────────────────────────────────────
   Distancia euclidiana entre partícula i y j
   ────────────────────────────────────────── */
static inline double distancia(const Particula *p, int i, int j) {
    double dx = p[i].x - p[j].x;
    double dy = p[i].y - p[j].y;
    double dz = p[i].z - p[j].z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}


/* ──────────────────────────────────────────
   Cálculo PARALELO con OpenMP (Fork-Join)
   El pragma parallel for distribuye las filas
   del bucle externo entre los hilos del equipo.
   La cláusula reduction(+:U) combina las sumas
   parciales de cada hilo en el JOIN.
   ────────────────────────────────────────── */
double energia_paralela(const Particula *p, int n, int num_hilos) {
    double U = 0.0;

    #pragma omp parallel for                \
        num_threads(num_hilos)              \
        schedule(dynamic, 16)              \
        reduction(+:U)                     \
        default(none) shared(p, n)
    for (int i = 0; i < n; i++) {
        double U_local = 0.0;
        for (int j = i + 1; j < n; j++) {
            double r = distancia(p, i, j) + EPSILON;
            U_local += (p[i].masa * p[j].masa) / r;
        }
        U += U_local;
    }

    return -G * U;
}

/* ──────────────────────────────────────────
   MAIN
   Uso: ./gravitacional <num_hilos>
   ────────────────────────────────────────── */
int main(int argc, char *argv[]) {

    /* --- Leer número de hilos desde argumentos --- */
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <num_hilos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_hilos = atoi(argv[1]);
    if (num_hilos < 1) {
        fprintf(stderr, "Error: el número de hilos debe ser >= 1.\n");
        return EXIT_FAILURE;
    }

    srand((unsigned)time(NULL));

    /* --- 1. Crear arreglo de N partículas --- */
    Particula *particulas = malloc(N * sizeof(Particula));
    if (!particulas) {
        fprintf(stderr, "Error: no se pudo reservar memoria.\n");
        return EXIT_FAILURE;
    }

    printf("=================================================\n");
    printf("   Práctica #6 — Energía Potencial Gravitacional \n");
    printf("   Modelo Fork-Join con OpenMP                   \n");
    printf("=================================================\n\n");

    printf("Partículas : %d\n", N);
    printf("Hilos      : %d\n\n", num_hilos);

    /* --- 2. Inicializar partículas --- */
    for (int i = 0; i < N; i++) {
        particulas[i].x    = rand_double(POS_MAX);
        particulas[i].y    = rand_double(POS_MAX);
        particulas[i].z    = rand_double(POS_MAX);
        particulas[i].masa = rand_double(MASA_MAX) + 1.0;
    }

    /* Mostrar primeras 5 partículas como muestra */
    printf("Muestra de las primeras 5 partículas:\n");
    printf("%-6s %-15s %-15s %-15s %-15s\n",
           "ID", "x (m)", "y (m)", "z (m)", "masa (kg)");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < 5; i++) {
        printf("%-6d %-15.3e %-15.3e %-15.3e %-15.3e\n",
               i,
               particulas[i].x,
               particulas[i].y,
               particulas[i].z,
               particulas[i].masa);
    }
    printf("\n");

    /* --- 3. Cálculo PARALELO (Fork-Join) --- */
    printf("--- Cálculo PARALELO (Fork-Join con OpenMP) ---\n");
    double t0 = omp_get_wtime();
    double U  = energia_paralela(particulas, N, num_hilos);
    double t1 = omp_get_wtime();

    printf("  U_total = %.6e J\n", U);
    printf("  Tiempo  = %.4f s\n\n", t1 - t0);

    free(particulas);
    return EXIT_SUCCESS;
}