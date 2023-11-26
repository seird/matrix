#ifndef __BENCHMARKS_H__
#define __BENCHMARKS_H__


#include "benchmark.h"


BENCH_FUNC(bench_mul_default);
BENCH_FUNC(bench_tmul_default);
BENCH_FUNC(bench_mul_swapped);
BENCH_FUNC(bench_tmul_swapped);
BENCH_FUNC(bench_mul_transpose);

BENCH_FUNC(bench_mul_avx2);
BENCH_FUNC(bench_mul_avx2fma);
BENCH_FUNC(bench_tmul_avx2);

BENCH_FUNC(bench_mul_avx512);
BENCH_FUNC(bench_mul_avx512fma);
BENCH_FUNC(bench_tmul_avx512);

#endif
