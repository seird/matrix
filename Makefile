CC = gcc
CFLAGS_DEBUG = -std=c2x -g -mavx2 -mfma -mavx512dq -pthread -Wall -Wextra -Wshadow
CFLAGS_RELEASE = -std=c2x -O0 -mavx2 -mfma -mavx512dq -pthread -Wall -Wextra -Wshadow -march=native
CFLAGS_TEST = -std=c2x -g -O0 -mavx2 -mfma -mavx512dq -pthread -Wall -Wextra -Wshadow -march=native -fprofile-arcs -ftest-coverage
CFLAGS_LIBRARY = -std=c2x -O3 -mavx2 -mfma -mavx512dq -pthread -Wall -Wextra -Wshadow -march=native

FILES = \
	$(wildcard src/*.c) \

FILES_TEST = \
	$(FILES) \
	$(wildcard tests/*.c) \

FILES_BENCH = \
	$(FILES) \
	$(wildcard benchmark/*.c) \

PNAME   ?= mmul


ifeq ($(OS),Windows_NT)
	PLATFORM_OS = WINDOWS
else
	PLATFORM_OS = LINUX
endif


build: 
	$(CC) -o $(PNAME) $(FILES) $(CFLAGS_RELEASE)

build-test:
	$(CC) -o test -DTEST $(FILES_TEST) $(CFLAGS_TEST)

run: build
	./$(PNAME) 16 32 16 0

debug:
	$(CC) -o $(PNAME)_debug $(FILES) $(CFLAGS_DEBUG)

profile:
	$(CC) -o bench -DBENCHMARK $(FILES_BENCH) $(CFLAGS_RELEASE) -g -fprofile-arcs -ftest-coverage
	valgrind --tool=callgrind ./bench

gprof:
	$(CC) -o test_gprof -DTEST $(FILES_TEST) $(CFLAGS_DEBUG) -pg
ifeq ($(PLATFORM_OS),WINDOWS)
	del gmon.out /s
	./test_gprof
	gprof test_gprof.exe gmon.out > test_gprof.out
else
	rm -fv gmon.out
	./test_gprof
	gprof test_gprof gmon.out > test_gprof.out
endif

memcheck: build-test
	valgrind --leak-check=full ./test

cache:
	$(CC) -o bench -DBENCHMARK $(FILES_BENCH) $(CFLAGS_RELEASE) -g -fprofile-arcs -ftest-coverage
	valgrind --tool=cachegrind ./bench
	#cg_annotate cachegrind.out.{PID}

test: build-test
	./test
	gcovr -e "tests/*" --xml-pretty --exclude-unreachable-branches --print-summary -o coverage.xml
	gcovr -e "tests/*" --html --html-details --print-summary -o coverage.html

coverage_html: build-test
	./test
	gcovr -e "tests/*" --html --html-details --print-summary -o coverage.html

bench:
	$(CC) -o bench -DBENCHMARK $(FILES_BENCH) $(CFLAGS_RELEASE)
	./bench

clean:
ifeq ($(PLATFORM_OS),WINDOWS)
	del *.o /s
	del *.exe /s
	del *.dll /s
	del *.out.* /s
	del *.so /s
	del *.a /s
	del *.gcda /s
	del *.gcno /s
else
	rm -fv *.o *.exe *.dll *.so *.out.* *.a *.gcda *.gcno
endif

.PHONY: all test bench clean
