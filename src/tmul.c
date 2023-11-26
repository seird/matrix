#include "matrix.h"
#include <stdio.h>
#include <pthread.h>
#include <immintrin.h>


void tmul_default(struct Matrix M1, struct Matrix M2, struct Matrix R, int n);
void tmul_swapped(struct Matrix M1, struct Matrix M2, struct Matrix R, int n);
void tmul_avx2(struct Matrix M1, struct Matrix M2, struct Matrix R, int n);
void tmul_avx512(struct Matrix M1, struct Matrix M2, struct Matrix R, int n);


struct ThreadArg {
    struct Matrix R;
    struct Matrix M1;
    struct Matrix M2;
    int id;
    int n;
};


func_tmul * get_func_tmul(enum TMulMethod method)
{
    static func_tmul functions[] = {
        tmul_default,
        tmul_swapped,
        tmul_avx2,
        tmul_avx512,
    };

    return &functions[(method < TMM_COUNT) ? method : 0];
}


static void *
tmul_default_worker(void * arg)
{
    struct ThreadArg * targ = (struct ThreadArg *) arg;

    for (size_t i=targ->id; i<targ->M1.rows; i+=targ->n)
        for (size_t j=0; j<targ->M2.cols; ++j)
            for (size_t k=0; k<targ->M1.cols; ++k) // dot product over the shared dimension
                targ->R.m[i][j] += targ->M1.m[i][k] * targ->M2.m[k][j];
                
    return NULL;
}


void
tmul_default(struct Matrix R, struct Matrix M1, struct Matrix M2, int n)
{
#ifndef BENCHMARK
    puts(__func__);
#endif
    pthread_t threads[n];
    struct ThreadArg args[n];

    for (int i=0; i<n; ++i) {
        args[i] = (struct ThreadArg) {.R = R, .M1 = M1, .M2 = M2, .id = i, .n = n};
        pthread_create(&threads[i], NULL, &tmul_default_worker, &args[i]);
    }

    for (int i=0; i<n; ++i)
        pthread_join(threads[i], NULL);
}


static void *
tmul_swapped_worker(void * arg)
{
    struct ThreadArg * targ = (struct ThreadArg *) arg;

    for (size_t i=targ->id; i<targ->M1.rows; i+=targ->n)
        for (size_t k=0; k<targ->M1.cols; ++k) // dot product over the shared dimension
            for (size_t j=0; j<targ->M2.cols; ++j)
                targ->R.m[i][j] += targ->M1.m[i][k] * targ->M2.m[k][j];

    return NULL;
}


void
tmul_swapped(struct Matrix R, struct Matrix M1, struct Matrix M2, int n)
{
#ifndef BENCHMARK
    puts(__func__);
#endif
    pthread_t threads[n];
    struct ThreadArg args[n];

    for (int i=0; i<n; ++i) {
        args[i] = (struct ThreadArg) {.R = R, .M1 = M1, .M2 = M2, .id = i, .n = n};
        pthread_create(&threads[i], NULL, &tmul_swapped_worker, &args[i]);
    }

    for (int i=0; i<n; ++i)
        pthread_join(threads[i], NULL);
}


static void *
tmul_avx2_worker(void * arg)
{
    struct ThreadArg * targ = (struct ThreadArg *) arg;

    for (size_t i=targ->id; i<targ->M1.rows; i+=targ->n) {
        for (size_t j=0; j<targ->M1.cols; ++j) {
            __m256 m1 = _mm256_set1_ps(targ->M1.m[i][j]);
            for (size_t k=0; k<targ->M2.cols; k+=8) {
                __m256 m2 = _mm256_load_ps(&targ->M2.m[j][k]);
                __m256 r = _mm256_load_ps(&targ->R.m[i][k]);
                r = _mm256_add_ps(r, _mm256_mul_ps(m1, m2));
                _mm256_store_ps(&targ->R.m[i][k], r);
            }
        }
    }
    return NULL;
}


void
tmul_avx2(struct Matrix R, struct Matrix M1, struct Matrix M2, int n)
{
#ifndef BENCHMARK
    puts(__func__);
#endif
    pthread_t threads[n];
    struct ThreadArg args[n];

    for (int i=0; i<n; ++i) {
        args[i] = (struct ThreadArg) {.R = R, .M1 = M1, .M2 = M2, .id = i, .n = n};
        pthread_create(&threads[i], NULL, &tmul_avx2_worker, &args[i]);
    }

    for (int i=0; i<n; ++i)
        pthread_join(threads[i], NULL);
}


static void *
tmul_avx512_worker(void * arg)
{
#ifdef __AVX512DQ__
    struct ThreadArg * targ = (struct ThreadArg *) arg;

    for (size_t i=targ->id; i<targ->M1.rows; i+=targ->n) {
        for (size_t j=0; j<targ->M1.cols; ++j) {
            __m512 m1 = _mm512_set1_ps(targ->M1.m[i][j]);
            for (size_t k=0; k<targ->M2.cols; k+=16) {
                __m512 m2 = _mm512_load_ps(&targ->M2.m[j][k]);
                __m512 r = _mm512_load_ps(&targ->R.m[i][k]);
                r = _mm512_add_ps(r, _mm512_mul_ps(m1, m2));
                _mm512_store_ps(&targ->R.m[i][k], r);
            }
        }
    }
#endif // __AVX512DQ__
    return NULL;
}


void
tmul_avx512(struct Matrix R, struct Matrix M1, struct Matrix M2, int n)
{
#ifdef __AVX512DQ__
#ifndef BENCHMARK
    puts(__func__);
#endif
    pthread_t threads[n];
    struct ThreadArg args[n];
    for (int i=0; i<n; ++i) {
        args[i] = (struct ThreadArg) {.R = R, .M1 = M1, .M2 = M2, .id = i, .n = n};
        pthread_create(&threads[i], NULL, &tmul_avx512_worker, &args[i]);
    }

    for (int i=0; i<n; ++i)
        pthread_join(threads[i], NULL);
#endif // __AVX512DQ__
}
