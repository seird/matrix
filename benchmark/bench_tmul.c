#include "benchmarks.h"
#include "../src/matrix.h"


extern struct Matrix M1;
extern struct Matrix M2;
extern struct Matrix R;
extern int threads;


BENCH_FUNC(bench_tmul_default)
{
    matrix_zero(R);
    matrix_tmul(R, M1, M2, TMM_DEFAULT, threads);
}


BENCH_FUNC(bench_tmul_swapped)
{
    matrix_zero(R);
    matrix_tmul(R, M1, M2, TMM_SWAPPED, threads);
}


BENCH_FUNC(bench_tmul_avx2)
{
    matrix_zero(R);
    matrix_tmul(R, M1, M2, TMM_AVX2, threads);
}


BENCH_FUNC(bench_tmul_avx512)
{
    matrix_zero(R);
    matrix_tmul(R, M1, M2, TMM_AVX512, threads);
}
