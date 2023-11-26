#include "benchmarks.h"
#include "../src/matrix.h"


struct Matrix M1;
struct Matrix M2;
struct Matrix R;
int threads;


int
main(void)
{
    int num_runs = 1;

    // [MxN] x [NxP] = [MxP]
    size_t M = 1024;
    size_t N = 1024;
    size_t P = 1024;

    threads = 24;
    
    M1 = matrix_init(M, N);
    M2 = matrix_init(N, P);
    R = matrix_init(M, P);

    matrix_populate(M1);
    matrix_populate(M2);

    printf("\n=================================================\nBenchmarking ...\n");
    printf("\tNumber of runs     = %20d\n", num_runs);
    putchar('\n');

    // MUL
    BENCH_RUN(bench_mul_default, num_runs);
    BENCH_RUN(bench_mul_swapped, num_runs);
    BENCH_RUN(bench_mul_transpose, num_runs);
    
    BENCH_RUN(bench_mul_avx2, num_runs);
    BENCH_RUN(bench_mul_avx2fma, num_runs);

    BENCH_RUN(bench_mul_avx512, num_runs);
    BENCH_RUN(bench_mul_avx512fma, num_runs);

    // TMUL
    BENCH_RUN(bench_tmul_default, num_runs);
    BENCH_RUN(bench_tmul_swapped, num_runs);

    BENCH_RUN(bench_tmul_avx2, num_runs);

    BENCH_RUN(bench_tmul_avx512, num_runs);

    // Cleanup
    matrix_free(M1);
    matrix_free(M2);
    matrix_free(R);

    return 0;
}
