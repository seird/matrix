#include "tests.h"
#include "../src/matrix.h"


static int
compare(struct Matrix M1, struct Matrix M2)
/* Compare two matrices
* Returns 1 if they are not equal
*/
{
    for (size_t r=0; r<M1.rows; r++){
        for (size_t c=0; c<M2.cols; c++){
            if (fabsf(M1.m[r][c] - M2.m[r][c]) > 0.0001){
                printf("[%lld,%lld] -- %f, %f", r, c, M1.m[r][c], M2.m[r][c]);
                return 1;
            }
        }
    }
    return 0;
}


MU_TEST(test_mul_swapped)
{
    // [MxN] x [NxP] = [MxP]
    size_t M = 64;
    size_t N = 64;
    size_t P = 64;
    
    struct Matrix M1 = matrix_init(M, N);
    struct Matrix M2 = matrix_init(N, P);
    struct Matrix Rref = matrix_init(M, P);
    struct Matrix R = matrix_init(M, P);

    matrix_populate(M1);
    matrix_populate(M2);
    matrix_zero(R);
    matrix_zero(Rref);

    matrix_mul(Rref, M1, M2, MM_DEFAULT);
    matrix_mul(R, M1, M2, MM_SWAPPED);

    MU_ASSERT("error, MM_DEFAULT != MM_SWAPPED", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}


MU_TEST(test_mul_transpose)
{
    // [MxN] x [NxP] = [MxP]
    size_t M = 64;
    size_t N = 64;
    size_t P = 64;
    
    struct Matrix M1 = matrix_init(M, N);
    struct Matrix M2 = matrix_init(N, P);
    struct Matrix Rref = matrix_init(M, P);
    struct Matrix R = matrix_init(M, P);

    matrix_populate(M1);
    matrix_populate(M2);
    matrix_zero(R);
    matrix_zero(Rref);

    matrix_mul(Rref, M1, M2, MM_DEFAULT);
    matrix_mul(R, M1, M2, MM_TRANSPOSE);

    MU_ASSERT("error, MM_DEFAULT != MM_TRANSPOSE", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}


MU_TEST(test_mul_avx2)
{
    // [MxN] x [NxP] = [MxP]
    size_t M = 64;
    size_t N = 64;
    size_t P = 64;
    
    struct Matrix M1 = matrix_init(M, N);
    struct Matrix M2 = matrix_init(N, P);
    struct Matrix Rref = matrix_init(M, P);
    struct Matrix R = matrix_init(M, P);

    matrix_populate(M1);
    matrix_populate(M2);
    matrix_zero(R);
    matrix_zero(Rref);

    matrix_mul(Rref, M1, M2, MM_DEFAULT);
    matrix_mul(R, M1, M2, MM_AVX2);

    MU_ASSERT("error, MM_DEFAULT != MM_AVX2", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}


MU_TEST(test_mul_avx2fma)
{
    // [MxN] x [NxP] = [MxP]
    size_t M = 64;
    size_t N = 64;
    size_t P = 64;
    
    struct Matrix M1 = matrix_init(M, N);
    struct Matrix M2 = matrix_init(N, P);
    struct Matrix Rref = matrix_init(M, P);
    struct Matrix R = matrix_init(M, P);

    matrix_populate(M1);
    matrix_populate(M2);
    matrix_zero(R);
    matrix_zero(Rref);

    matrix_mul(Rref, M1, M2, MM_DEFAULT);
    matrix_mul(R, M1, M2, MM_AVX2FMA);

    MU_ASSERT("error, MM_DEFAULT != MM_AVX2FMA", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}


MU_TEST(test_mul_avx512)
{
    // [MxN] x [NxP] = [MxP]
    size_t M = 64;
    size_t N = 64;
    size_t P = 64;
    
    struct Matrix M1 = matrix_init(M, N);
    struct Matrix M2 = matrix_init(N, P);
    struct Matrix Rref = matrix_init(M, P);
    struct Matrix R = matrix_init(M, P);

    matrix_populate(M1);
    matrix_populate(M2);
    matrix_zero(R);
    matrix_zero(Rref);

    matrix_mul(Rref, M1, M2, MM_DEFAULT);
    matrix_mul(R, M1, M2, MM_AVX512);

    MU_ASSERT("error, MM_DEFAULT != MM_AVX512", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}


MU_TEST(test_mul_avx512fma)
{
    // [MxN] x [NxP] = [MxP]
    size_t M = 64;
    size_t N = 64;
    size_t P = 64;
    
    struct Matrix M1 = matrix_init(M, N);
    struct Matrix M2 = matrix_init(N, P);
    struct Matrix Rref = matrix_init(M, P);
    struct Matrix R = matrix_init(M, P);

    matrix_populate(M1);
    matrix_populate(M2);
    matrix_zero(R);
    matrix_zero(Rref);

    matrix_mul(Rref, M1, M2, MM_DEFAULT);
    matrix_mul(R, M1, M2, MM_AVX512FMA);

    MU_ASSERT("error, MM_DEFAULT != MM_AVX512FMA", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}
