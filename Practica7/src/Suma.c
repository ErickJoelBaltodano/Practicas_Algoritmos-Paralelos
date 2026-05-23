#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define MAX_BITS 64

// ─── Conversión Decimal → Binario ───────────────────────────────────────────

void decimal_a_binario(long long numero, int *bits, int *longitud) {
    *longitud = 0;
    if (numero == 0) {
        bits[0] = 0;
        *longitud = 1;
        return;
    }
    long long n = numero;
    while (n > 0) {
        bits[(*longitud)++] = n % 2;
        n /= 2;
    }
    // Invertir para que el bit más significativo quede primero
    for (int i = 0; i < *longitud / 2; i++) {
        int tmp = bits[i];
        bits[i] = bits[*longitud - 1 - i];
        bits[*longitud - 1 - i] = tmp;
    }
}

// Imprime un arreglo de bits como cadena binaria
void imprimir_binario(int *bits, int longitud) {
    for (int i = 0; i < longitud; i++)
        printf("%d", bits[i]);
}

// ─── Suma Binaria con Hilos (OpenMP) ─────────────────────────────────────────
//
// Estrategia:
//   1. Normalizar ambos números a MAX_BITS bits (LSB en índice 0).
//   2. Dividir los bits entre N hilos.
//   3. Cada hilo calcula la suma parcial de su segmento SIN carry externo.
//   4. Fase 2: propagar carries entre segmentos de forma secuencial
//      (el carry de salida de cada bloque alimenta al siguiente).
//   5. Convertir el resultado de vuelta a decimal.

long long suma_binaria_paralela(long long a, long long b, int n_hilos,
                                 int *bits_a, int len_a,
                                 int *bits_b, int len_b,
                                 int *bits_res, int *len_res) {

    // Representación LSB-first (índice 0 = bit menos significativo)
    int A[MAX_BITS] = {0};
    int B[MAX_BITS] = {0};

    // Copiar en orden LSB-first
    for (int i = 0; i < len_a; i++) A[i] = bits_a[len_a - 1 - i];
    for (int i = 0; i < len_b; i++) B[i] = bits_b[len_b - 1 - i];

    int total_bits = MAX_BITS;

    // Arrays de suma parcial y carry de salida por bloque
    int suma[MAX_BITS] = {0};    // suma[i] = A[i] XOR B[i] (sin carry aún)
    int carry_gen[MAX_BITS] = {0}; // carry generado: A[i] AND B[i]
    int carry_prop[MAX_BITS] = {0}; // carry propagado: A[i] XOR B[i]

    // ── Fase 1: calcular suma y carry local (paralelo) ──
    #pragma omp parallel for num_threads(n_hilos) schedule(static)
    for (int i = 0; i < total_bits; i++) {
        suma[i]       = A[i] ^ B[i];   // XOR
        carry_gen[i]  = A[i] & B[i];   // AND (genera carry)
        carry_prop[i] = A[i] ^ B[i];   // XOR (propaga carry)
    }

    // ── Fase 2: propagar carries (secuencial, rápido para 64 bits) ──
    int carry = 0;
    int resultado[MAX_BITS + 1] = {0};

    for (int i = 0; i < total_bits; i++) {
        int s = A[i] ^ B[i] ^ carry;
        int c = (A[i] & B[i]) | (carry & (A[i] ^ B[i]));
        resultado[i] = s;
        carry = c;
    }
    resultado[total_bits] = carry; // posible bit extra

    // ── Fase 3: convertir resultado LSB-first → decimal (paralelo) ──
    long long decimal = 0;
    // Reducción paralela de la suma ponderada
    #pragma omp parallel for num_threads(n_hilos) reduction(+:decimal) schedule(static)
    for (int i = 0; i <= total_bits; i++) {
        if (resultado[i])
            decimal += (long long)resultado[i] * (long long)(1LL << i);
    }

    // Preparar bits de resultado para impresión (MSB-first, sin ceros líderes)
    int top = total_bits;
    while (top > 0 && resultado[top] == 0) top--;
    *len_res = top + 1;
    for (int i = 0; i < *len_res; i++)
        bits_res[i] = resultado[top - i];

    return decimal;
}

//  Main 

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <N_hilos> <Numero1> <Numero2>\n", argv[0]);
        return 1;
    }

    int    n_hilos = atoi(argv[1]);
    long long n1   = atoll(argv[2]);
    long long n2   = atoll(argv[3]);

    if (n_hilos < 1) { fprintf(stderr, "El número de hilos debe ser >= 1\n"); return 1; }
    if (n1 < 0 || n2 < 0) { fprintf(stderr, "Solo se admiten enteros no negativos\n"); return 1; }

    //  Conversión a binario 
    int bits_a[MAX_BITS], len_a;
    int bits_b[MAX_BITS], len_b;
    int bits_r[MAX_BITS + 1], len_r;

    double t_conv_inicio = omp_get_wtime();

    // Las conversiones son independientes → paralelas
    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        decimal_a_binario(n1, bits_a, &len_a);

        #pragma omp section
        decimal_a_binario(n2, bits_b, &len_b);
    }

    double t_conv_fin = omp_get_wtime();

  
  
    printf("║         SUMA BINARIA PARALELA CON HILOS (OpenMP)    ║\n");
  
    printf("  --------------------------------------------------\n");
    printf("  Número de hilos  : %d\n\n", n_hilos);
    printf("  --------------------------------------------------\n");
    printf("  Número 1 (dec)   : %lld\n", n1);
    printf("  Número 1 (bin)   : ");
    imprimir_binario(bits_a, len_a);
    printf("\n\n");

    printf("  --------------------------------------------------\n");
    printf("  Número 2 (dec)   : %lld\n", n2);
    printf("  Número 2 (bin)   : ");
    imprimir_binario(bits_b, len_b);
    printf("\n\n");

    //  Suma binaria paralela 
    double t_suma_inicio = omp_get_wtime();
    long long resultado = suma_binaria_paralela(n1, n2, n_hilos,
                                                bits_a, len_a,
                                                bits_b, len_b,
                                                bits_r, &len_r);
    double t_suma_fin = omp_get_wtime();

    // Mostrar resultado 
    printf("  --------------------------------------------------\n");
    printf("  Resultado (bin)  : ");
    imprimir_binario(bits_r, len_r);
    printf("\n");
    printf("  Resultado (dec)  : %lld\n\n", resultado);

    //  Tiempos 
    double t_conv  = (t_conv_fin  - t_conv_inicio) * 1e6;
    double t_suma  = (t_suma_fin  - t_suma_inicio) * 1e6;
    double t_total = t_conv + t_suma;

    printf("   -------------------- TIEMPOS -------------------- \n");
    printf("  Conversión dec→bin  : %10.4f µs\n", t_conv);
    printf("  Suma binaria        : %10.4f µs\n", t_suma);
    printf("   ------------------------------------------------- \n");
    printf("  TOTAL               : %10.4f µs\n\n", t_total);


    return 0;
}