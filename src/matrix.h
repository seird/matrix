#ifndef __MATRIX_H__
#define __MATRIX_H__


#include <stdlib.h>


enum MulMethod {
    MM_DEFAULT,
    MM_SWAPPED,
    MM_TRANSPOSE,
    MM_AVX2,
    MM_AVX2FMA,
    MM_AVX512,
    MM_AVX512FMA,
    MM_COUNT,
};


enum TMulMethod {
    TMM_DEFAULT,
    TMM_SWAPPED,
    TMM_AVX2,
    TMM_AVX512,
    TMM_COUNT,
};


struct Matrix {
    float ** m;
    size_t rows;
    size_t cols;
};


struct Matrix matrix_init(size_t M, size_t N);
void matrix_free(struct Matrix M);
void matrix_print(struct Matrix M);
void matrix_populate(struct Matrix M);
void matrix_zero(struct Matrix M);

void matrix_mul(struct Matrix R, struct Matrix M1, struct Matrix M2, enum MulMethod method);
void matrix_tmul(struct Matrix R, struct Matrix M1, struct Matrix M2, enum TMulMethod method, int n);

typedef void (* func_mul)(struct Matrix R, struct Matrix M1, struct Matrix M2);
func_mul * get_func_mul(enum MulMethod method);

typedef void (* func_tmul)(struct Matrix R, struct Matrix M1, struct Matrix M2, int n);
func_tmul * get_func_tmul(enum TMulMethod method);

#endif // __MATRIX_H__
