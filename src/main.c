#include "main.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "matrix.h"


#if (!defined(TEST) && !defined(SHARED) && !defined(STATIC) && !defined(BENCHMARK))
int
main(int argc, char * argv[])
{
    (void) argc;
    (void) argv;

    srand(time(NULL));

    size_t M, N, P;

    if (argc != 5) {
        printf("Usage: %s M N P Method\n"
               "Multiply 2 matrices, M1 and M2, of size MxN and NxP, respectively.\n"
               "e.g.: %s 1024 1024 1024 0\n"
               , argv[0], argv[0]);
        return 0;
    }

    M = atoi(argv[1]);
    N = atoi(argv[2]);
    P = atoi(argv[3]);
    enum MulMethod method = atoi(argv[4]);

    struct Matrix M1 = matrix_init(M, N);
    struct Matrix M2 = matrix_init(N, P);
    struct Matrix R = matrix_init(M, P);

    matrix_populate(M1);
    matrix_populate(M2);

    printf("M1: [%lld x %lld], M2: [%lld x %lld], R: [%lld x %lld]\n", M1.rows, M1.cols, M2.rows, M2.cols, R.rows, R.cols);

    matrix_print(M1);
    matrix_print(M2);

    (void) method;
    for (int i=0; i<MM_COUNT; ++i) {
        matrix_zero(R);
        matrix_mul(R, M1, M2, i);
        matrix_print(R);
    }


    for (int i=0; i<TMM_COUNT; ++i) {
        matrix_zero(R);
        matrix_tmul(R, M1, M2, i, 8);
        matrix_print(R);
    }

    matrix_free(R);
    matrix_free(M1);
    matrix_free(M2);

    printf("Finished\n");
    
    return 0;
}
#endif
