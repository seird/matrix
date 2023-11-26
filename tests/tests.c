#include "tests.h"


int tests_run = 0;
int tests_failed = 0;
int tests_result = 0;


static void
ALL_TESTS()
{
    MU_RUN_TEST(test_tmul_default);
    MU_RUN_TEST(test_mul_swapped);
    MU_RUN_TEST(test_tmul_swapped);
    MU_RUN_TEST(test_mul_transpose);

    MU_RUN_TEST(test_mul_avx2);
    MU_RUN_TEST(test_mul_avx2fma);
    MU_RUN_TEST(test_tmul_avx2);

    MU_RUN_TEST(test_mul_avx512);
    MU_RUN_TEST(test_mul_avx512fma);
    MU_RUN_TEST(test_tmul_avx512);
}


int
main(void)
{
    ALL_TESTS();

    MU_STATS();

    return tests_failed != 0;
}
