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


MU_TEST(test_tmul_default)
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
    matrix_tmul(R, M1, M2, TMM_DEFAULT, 2);

    MU_ASSERT("error, MM_DEFAULT != TMM_DEFAULT", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}


MU_TEST(test_tmul_swapped)
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
    matrix_tmul(R, M1, M2, TMM_SWAPPED, 2);

    MU_ASSERT("error, MM_DEFAULT != TMM_SWAPPED", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}


MU_TEST(test_tmul_avx2)
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
    matrix_tmul(R, M1, M2, TMM_AVX2, 2);

    MU_ASSERT("error, MM_DEFAULT != TMM_AVX2", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}


MU_TEST(test_tmul_avx512)
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
    matrix_tmul(R, M1, M2, TMM_AVX512, 2);

    MU_ASSERT("error, MM_DEFAULT != TMM_AVX512", compare(Rref, R) == 0);

    matrix_free(M1);
    matrix_free(M2);
    matrix_free(Rref);
    matrix_free(R);
}
