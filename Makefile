COMMON_CFLAGS := -DMTB_DEBUG \
                 -I. \
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
                 -Wno-override-init

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

assemble_mtb:
	echo -e "#ifndef MTB_H\n#define MTB_H\n\n#ifndef _GNU_SOURCE\n#define _GNU_SOURCE\n#endif\n" > mtb.h
	cat mtb_macro.h \
        mtb_type.h \
        mtb_rng.h \
        mtb_perf.h \
        mtb_list.h \
        mtb_arena.h \
        mtb_dynarr.h \
        mtb_segarr.h \
        mtb_hmap.h \
        mtb_string.h \
        >> mtb.h
	echo -e "\n#endif //MTB_H" >> mtb.h

# GCC

clean_gcc:
	rm -f ./tests_gcc.o ./bench_gcc.o

build_test_gcc: assemble_mtb tests.c
	gcc tests.c -o tests_gcc.o $(COMMON_CFLAGS) -fsanitize=address,bounds,undefined

test_gcc: build_test_gcc
	./tests_gcc.o

build_bench_gcc: assemble_mtb bench.c
	gcc bench.c -o bench_gcc.o $(COMMON_CFLAGS) -fsanitize=address,bounds,undefined

bench_gcc: build_bench_gcc
	./bench_gcc.o


# CLANG

clean_clang:
	rm -f ./tests_clang.o ./bench_clang.o

build_test_clang: assemble_mtb tests.c
	clang tests.c -o tests_clang.o $(COMMON_CFLAGS)

test_clang: build_test_clang
	./tests_clang.o

build_bench_clang: assemble_mtb bench.c
	clang bench.c -o bench_clang.o $(COMMON_CFLAGS)

bench_clang: build_bench_clang
	./bench_clang.o
