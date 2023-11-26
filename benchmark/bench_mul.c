#include "benchmarks.h"
#include "../src/matrix.h"


extern struct Matrix M1;
extern struct Matrix M2;
extern struct Matrix R;


BENCH_FUNC(bench_mul_default)
{
    matrix_zero(R);
    matrix_mul(R, M1, M2, MM_DEFAULT);
}


BENCH_FUNC(bench_mul_swapped)
{
    matrix_zero(R);
    matrix_mul(R, M1, M2, MM_SWAPPED);
}


BENCH_FUNC(bench_mul_transpose)
{
    matrix_zero(R);
    matrix_mul(R, M1, M2, MM_TRANSPOSE);
}


BENCH_FUNC(bench_mul_avx2)
{
    matrix_zero(R);
    matrix_mul(R, M1, M2, MM_AVX2);
}


BENCH_FUNC(bench_mul_avx2fma)
{
    matrix_zero(R);
    matrix_mul(R, M1, M2, MM_AVX2FMA);
}


BENCH_FUNC(bench_mul_avx512)
{
    matrix_zero(R);
    matrix_mul(R, M1, M2, MM_AVX512);
}


BENCH_FUNC(bench_mul_avx512fma)
{
    matrix_zero(R);
    matrix_mul(R, M1, M2, MM_AVX512FMA);
}
