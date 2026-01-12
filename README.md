# MyToolBox (MTB)

This is a collection of reusable C utils.


### Requirements

1. GCC or Clang
2. C99+ codebase


### Contents

- [mtb.h](mtb.h) - contains all of MTB.
- [mtb_macro.h](./mtb_macro.h) - common macros.
- [mtb_type.h](./mtb_type.h) - common types and operations on them.
- [mtb_list.h](./mtb_list.h) - doubly linked list.
- [mtb_arena.h](./mtb_arena.h) - arena allocator.
- [mtb_dynarr.h](./mtb_dynarr.h) - dynamically growing array (aka vector).
- [mtb_segarr.h](./mtb_segarr.h) - segment array (aka growable stack w/o re-alloc).
- [mtb_hmap.h](./mtb_hmap.h) - hash map w/ linear probing.
- [mtb_string.h](./mtb_string.h) - strings with partial UTF-8 support.
- [mtb_rng.h](./mtb_rng.h) - simple & fast non-cryptographic pseudo-RNGs.
- [tests.h](./tests.c) - runs all unit tests.
- [bench.h](./bench.c) - runs all benchmarks.


### Heavily inspired by
- [hacktical-c](https://github.com/codr7/hacktical-c)
- [rad-debugger base module](https://github.com/EpicGamesExt/raddebugger/tree/master/src/base)
