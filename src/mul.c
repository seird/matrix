#include "matrix.h"
#include <stdio.h>
#include <immintrin.h>


void mul_default(struct Matrix R, struct Matrix M1, struct Matrix M2);
void mul_swapped(struct Matrix R, struct Matrix M1, struct Matrix M2);
void mul_transpose(struct Matrix R, struct Matrix M1, struct Matrix M2);
void mul_avx2(struct Matrix R, struct Matrix M1, struct Matrix M2);
void mul_avx2fma(struct Matrix R, struct Matrix M1, struct Matrix M2);
void mul_avx512(struct Matrix R, struct Matrix M1, struct Matrix M2);
void mul_avx512fma(struct Matrix R, struct Matrix M1, struct Matrix M2);


func_mul * get_func_mul(enum MulMethod method)
{
    static func_mul functions[] = {
        mul_default,
        mul_swapped,
        mul_transpose,
        mul_avx2,
        mul_avx2fma,
        mul_avx512,
        mul_avx512fma
    };

    return &functions[(method < MM_COUNT) ? method : 0];
}


void
mul_default(struct Matrix R, struct Matrix M1, struct Matrix M2)
{
#ifndef BENCHMARK
    puts(__func__);
#endif
    for (size_t i=0; i<M1.rows; ++i)
        for (size_t j=0; j<M2.cols; ++j)
            for (size_t k=0; k<M1.cols; ++k) // dot product over the shared dimension
                R.m[i][j] += M1.m[i][k] * M2.m[k][j];
}


void
mul_swapped(struct Matrix R, struct Matrix M1, struct Matrix M2)
{
#ifndef BENCHMARK
    puts(__func__);
#endif
    for (size_t i=0; i<M1.rows; ++i)
        for (size_t k=0; k<M1.cols; ++k) // dot product over the shared dimension
            for (size_t j=0; j<M2.cols; ++j)
                R.m[i][j] += M1.m[i][k] * M2.m[k][j];
}


void
mul_transpose(struct Matrix R, struct Matrix M1, struct Matrix M2)
{
#ifndef BENCHMARK
    puts(__func__);
#endif
    struct Matrix M2T = matrix_init(M2.cols, M2.rows);
    for (size_t i=0; i<M2.rows; ++i)
        for (size_t j=0; j<M2.cols; ++j)
            M2T.m[j][i] = M2.m[i][j];

    for (size_t i=0; i<M1.rows; ++i)
        for (size_t j=0; j<M2.cols; ++j)
            for (size_t k=0; k<M2T.cols; ++k) // dot product over the shared dimension
                R.m[i][j] += M1.m[i][k] * M2T.m[j][k];

    matrix_free(M2T);
}


void
mul_avx2(struct Matrix R, struct Matrix M1, struct Matrix M2)
/**
    X = 
    [
        [x11, x12, x13, x14],
        [x21, x22, x33, x34]
    ]

    Y =
    [
        [y11, y12],
        [y21, y22],
        [y31, y32],
        [y41, y42]
    ]

    X * Y = 
    [
        [x11*y11 + x12*y21 + x13*y31 + x14*y41, x11*y12 + x12*y22 + x13*y32 + x14*y42],
        [x21*y11 + x22*y21 + x23*y31 + x24*y41, x21*y12 + x22*y22 + x23*y32 + x24*y42]
    ]

    SIMD-2

    x11*y11, x11*y12
*/
{
#ifndef BENCHMARK
    puts(__func__);
#endif
    for (size_t i=0; i<M1.rows; ++i) {
        for (size_t j=0; j<M1.cols; ++j) {
            __m256 m1 = _mm256_set1_ps(M1.m[i][j]);
            for (size_t k=0; k<M2.cols; k+=8) {
                __m256 m2 = _mm256_load_ps(&M2.m[j][k]);
                __m256 r = _mm256_load_ps(&R.m[i][k]);
                r = _mm256_add_ps(r, _mm256_mul_ps(m1, m2));
                _mm256_store_ps(&R.m[i][k], r);
                // printf("i = %lld, j = %lld, k = %lld\n", i, j, k);
                // matrix_print(R);
            }
        }
    }
}


void
mul_avx2fma(struct Matrix R, struct Matrix M1, struct Matrix M2)
{
#ifndef BENCHMARK
    puts(__func__);
#endif
    for (size_t i=0; i<M1.rows; ++i) {
        for (size_t j=0; j<M1.cols; ++j) {
            __m256 m1 = _mm256_set1_ps(M1.m[i][j]);
            for (size_t k=0; k<M2.cols; k+=8) {
                __m256 m2 = _mm256_load_ps(&M2.m[j][k]);
                __m256 r = _mm256_load_ps(&R.m[i][k]);
                r = _mm256_fmadd_ps(m1, m2, r);
                _mm256_store_ps(&R.m[i][k], r);
            }
        }
    }
}


void
mul_avx512(struct Matrix R, struct Matrix M1, struct Matrix M2)
{
#ifdef __AVX512DQ__
#ifndef BENCHMARK
    puts(__func__);
#endif
    for (size_t i=0; i<M1.rows; ++i) {
        for (size_t j=0; j<M1.cols; ++j) {
            __m512 m1 = _mm512_set1_ps(M1.m[i][j]);
            for (size_t k=0; k<M2.cols; k+=16) {
                __m512 m2 = _mm512_load_ps(&M2.m[j][k]);
                __m512 r = _mm512_load_ps(&R.m[i][k]);
                r = _mm512_add_ps(r, _mm512_mul_ps(m1, m2));
                _mm512_store_ps(&R.m[i][k], r);
            }
        }
    }
#endif // __AVX512DQ__
}


void
mul_avx512fma(struct Matrix R, struct Matrix M1, struct Matrix M2)
{
#ifdef __AVX512DQ__
#ifndef BENCHMARK
    puts(__func__);
#endif
    for (size_t i=0; i<M1.rows; ++i) {
        for (size_t j=0; j<M1.cols; ++j) {
            __m512 m1 = _mm512_set1_ps(M1.m[i][j]);
            for (size_t k=0; k<M2.cols; k+=16) {
                __m512 m2 = _mm512_load_ps(&M2.m[j][k]);
                __m512 r = _mm512_load_ps(&R.m[i][k]);
                r = _mm512_fmadd_ps(m1, m2, r);
                _mm512_store_ps(&R.m[i][k], r);
            }
        }
    }
#endif // __AVX512DQ__
}
