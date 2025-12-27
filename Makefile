COMMON_CFLAGS := -DMTB_DEBUG \
                 -I. \
                 -lm \
                 -std=c99 \
                 -O0 \
                 -g3 \
                 -Werror \
                 -Wall \
                 -Wextra \
                 -Wconversion \
                 -Wno-unused-parameter \
                 -Wno-unused-function \
                 -Wno-sign-conversion \
                 -Wno-override-init \

SHELL := /bin/bash

.PHONY: all \
	    clean clean_gcc clean_clang \
	    build build_test_gcc build_bench_gcc build_test_clang build_bench_clang \
	    test test_gcc test_clang \
	    bench bench_gcc bench_clang

all: build

clean: clean_gcc clean_clang

build: build_test_gcc build_test_clang

test: test_gcc test_clang

bench: bench_gcc bench_clang


# GCC

clean_gcc:
	rm -f ./tests_gcc.o ./bench_gcc.o

build_test_gcc: tests.c
	gcc tests.c -o tests_gcc.o $(COMMON_CFLAGS) -fsanitize=address,bounds,undefined

test_gcc: build_test_gcc
	./tests_gcc.o

build_bench_gcc: bench.c
	gcc bench.c -o bench_gcc.o $(COMMON_CFLAGS) -fsanitize=address,bounds,undefined

bench_gcc: build_bench_gcc
	./bench_gcc.o


# CLANG

clean_clang:
	rm -f ./tests_clang.o ./bench_clang.o

build_test_clang: tests.c
	clang tests.c -o tests_clang.o $(COMMON_CFLAGS)

test_clang: build_test_clang
	./tests_clang.o

build_bench_clang: bench.c
	clang bench.c -o bench_clang.o $(COMMON_CFLAGS)

bench_clang: build_bench_clang
	./bench_clang.o
