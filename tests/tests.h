#ifndef __TESTS_H__
#define __TESTS_H__


#include "minunit.h"


MU_TEST(test_tmul_default);
MU_TEST(test_mul_swapped);
MU_TEST(test_tmul_swapped);
MU_TEST(test_mul_transpose);

MU_TEST(test_mul_avx2);
MU_TEST(test_mul_avx2fma);
MU_TEST(test_tmul_avx2);

MU_TEST(test_mul_avx512);
MU_TEST(test_mul_avx512fma);
MU_TEST(test_tmul_avx512);


#endif // __TESTS_H__
