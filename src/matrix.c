#include "matrix.h"
#include <stdio.h>
#include <string.h>


struct Matrix
matrix_init(size_t M, size_t N)
{
    size_t alignment = 64; // AVX512
    // size_t alignment = 32; // AVX2

    float ** m = (float **) malloc(sizeof(float *) * M);

    for (size_t i=0; i<M; ++i) {
    #if defined(_WIN64) || defined(_WIN32)
        m[i] = (float *) _aligned_malloc(sizeof(float) * N, alignment);
    #else
        m[i] = (float *) aligned_alloc(alignment, sizeof(float) * N);
    #endif
    }
    return (struct Matrix){.m = m, .rows = M, .cols = N};
}


void
matrix_free(struct Matrix M)
{
    for (size_t i=0; i<M.rows; ++i) {
    #if defined(_WIN64) || defined(_WIN32)
        _aligned_free(M.m[i]);
    #else
        free(M.m[i]);
    #endif
    }
    free(M.m);
}


void
matrix_print(struct Matrix M)
{
    printf("Size [%lld, %lld]:\n", M.rows, M.cols);
    for (size_t r=0; r<M.rows; ++r) {
        for (size_t c=0; c<M.cols; ++c)
            printf("%.02f ", M.m[r][c]);
        putchar('\n');
    }
}


void
matrix_populate(struct Matrix M)
{
    for (size_t r=0; r<M.rows; ++r)
        for (size_t c=0; c<M.cols; ++c)
            M.m[r][c] = (float) rand() / RAND_MAX;
}


void
matrix_zero(struct Matrix M)
{
    for (size_t r=0; r<M.rows; ++r)
        memset(M.m[r], 0, M.cols*sizeof(float));
}


void
matrix_mul(struct Matrix R, struct Matrix M1, struct Matrix M2, enum MulMethod method)
{
    func_mul * f = get_func_mul(method);
    (*f)(R, M1, M2);
}


void
matrix_tmul(struct Matrix R, struct Matrix M1, struct Matrix M2, enum TMulMethod method, int n)
{
    func_tmul * f = get_func_tmul(method);
    (*f)(R, M1, M2, n);
}
