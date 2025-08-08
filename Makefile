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

SHELL := /bin/bash

.PHONY: all clean clean_gcc clean_clang build build_gcc build_clang test test_gcc test_clang

all: build

clean: clean_gcc clean_clang

build: build_gcc build_clang

test: test_gcc test_clang


# GCC

clean_gcc:
	rm -f ./tests_gcc

build_gcc: tests.c
	gcc tests.c -o tests_gcc $(COMMON_CFLAGS) -fsanitize=address,bounds,undefined

test_gcc: build_gcc
	./tests_gcc


# CLANG

clean_clang:
	rm -f ./tests_clang

build_clang: tests.c
	clang tests.c -o tests_clang $(COMMON_CFLAGS)

test_clang: build_clang
	./tests_clang
