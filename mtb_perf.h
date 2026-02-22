#ifndef MTB_PERF_H
#define MTB_PERF_H

#ifdef MTB_IMPLEMENTATION
#define MTB_PERF_IMPLEMENTATION
#endif



#ifndef MTB_PERF_BLOCKS_MAX
#define MTB_PERF_BLOCKS_MAX 4096
#endif


typedef struct mtb_perf_barker_marker MtbPerfBlockMarker;
struct mtb_perf_barker_marker
{
    const char *blockName;
    u32 blockIndex;
    u32 parentBlockIndex;
    u64 cpuTimeEnter;
};

typedef struct mtb_perf_block MtbPerfBlock;
struct mtb_perf_block
{
    const char *name;
    u64 cpuTimeElapsed;
    u64 cpuTimeElapsedWithChildren;
    u64 hitCount;
};

typedef struct mtb_perf_profile MtbPerfProfile;
struct mtb_perf_profile
{
    MtbPerfBlock blocks[MTB_PERF_BLOCKS_MAX];
    u64 cpuTimeStart;
};


func void mtb_perf_start();
func void mtb_perf_print();

func u64 mtb_perf_cpu_time();
func u64 mtb_perf_cpu_freq();

func u64 mtb_perf_sys_time();
func u64 mtb_perf_sys_freq();

func MtbPerfBlockMarker mtb_perf_block_enter(const char *name, u32 index);
func void mtb_perf_block_exit(MtbPerfBlockMarker *marker);

#ifdef MTB_PERF_ENABLED
#define _mtb_perf_time_block(name, _m) \
    MtbPerfBlockMarker _m __attribute__((__cleanup__(mtb_perf_block_exit))) = mtb_perf_block_enter(name, (__COUNTER__ + 1))
#define mtb_perf_time_block(name) _mtb_perf_time_block(name, mtb_id(_m))
#define mtb_perf_time_func() mtb_perf_time_block(__FUNCTION__)
#else
#define mtb_perf_time_block(name)
#define mtb_perf_time_func()
#endif

#endif //MTB_PERF_H


#ifdef MTB_PERF_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>


#ifdef MTB_PERF_ENABLED
global MtbPerfProfile _mtb_perf_global_profile = {0};
global thread_local u32 _mtb_perf_current_block_index = 0;
#endif


func void
mtb_perf_start()
{
#ifdef MTB_PERF_ENABLED
    _mtb_perf_global_profile.cpuTimeStart = mtb_perf_cpu_time();
    memset(_mtb_perf_global_profile.blocks, 0, MTB_PERF_BLOCKS_MAX * sizeof(MtbPerfBlock));
#endif
}

func void
mtb_perf_print()
{
#ifdef MTB_PERF_ENABLED
    u64 cpuTimeEnd = mtb_perf_cpu_time();

    u64 cpuFreq = mtb_perf_cpu_freq();
    printf("cpu freq: %.2fGHz\n", (f32)cpuFreq / billion(1));

    u64 totalCpuTimeElapsed = cpuTimeEnd - _mtb_perf_global_profile.cpuTimeStart;
    printf("total cpu time: %lu\n", totalCpuTimeElapsed);

    for (u32 i = 1; i < MTB_PERF_BLOCKS_MAX; i++) {
        MtbPerfBlock *block = &_mtb_perf_global_profile.blocks[i];
        if (block->name) {
            printf("[%s]\n", block->name);
            printf("\thits: %lu\n", block->hitCount);
            printf("\tcpu time: %lu (%.2f%%)\n",
                   block->cpuTimeElapsed,
                   (f32)block->cpuTimeElapsed / (f32)totalCpuTimeElapsed * 100.0f);
            if (block->cpuTimeElapsedWithChildren != block->cpuTimeElapsed) {
                printf("\tcpu time w/ children: %lu (%.2f%%)\n",
                       block->cpuTimeElapsedWithChildren,
                       (f32)block->cpuTimeElapsedWithChildren / (f32)totalCpuTimeElapsed * 100.0f);
            }
        }
    }
#endif
}

func u64
mtb_perf_cpu_time()
{
    _mm_lfence();
    return __rdtsc();
}

func u64
mtb_perf_cpu_freq()
{
    u64 cpuTimeStart = mtb_perf_cpu_time();
    u64 sysTimeStart = mtb_perf_sys_time();
    u64 sysFreq = mtb_perf_sys_freq();

    usleep(2000); // 2ms

    u64 sysTimeEnd = mtb_perf_sys_time();
    u64 cpuTimeEnd = mtb_perf_cpu_time();
    u64 sysTimeElapsed = sysTimeEnd - sysTimeStart;
    u64 cpuTimeElapsed = cpuTimeEnd - cpuTimeStart;
    u64 cpuFreq = cpuTimeElapsed / sysTimeElapsed * sysFreq;

    return cpuFreq;
}

func u64
mtb_perf_sys_time()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        return 0;
    }
    return billion(ts.tv_sec) + ts.tv_nsec;
}

func u64
mtb_perf_sys_freq()
{
    return billion(1); // nanoseconds
}

func MtbPerfBlockMarker
mtb_perf_block_enter(const char *name, u32 index)
{
    MtbPerfBlockMarker marker = {0};
#ifdef MTB_PERF_ENABLED
    marker.blockName = name;
    marker.blockIndex = index;
    marker.parentBlockIndex = _mtb_perf_current_block_index;
    _mtb_perf_current_block_index = index;

    marker.cpuTimeEnter = mtb_perf_cpu_time();
#endif
    return marker;
}

func void
mtb_perf_block_exit(MtbPerfBlockMarker *marker)
{
#ifdef MTB_PERF_ENABLED
    u64 cpuTimeExit = mtb_perf_cpu_time();

    u64 cpuTimeElapsed = cpuTimeExit - marker->cpuTimeEnter;

    MtbPerfBlock *block = &_mtb_perf_global_profile.blocks[marker->blockIndex];
    block->name = marker->blockName;
    block->cpuTimeElapsed += cpuTimeElapsed;
    block->cpuTimeElapsedWithChildren += cpuTimeElapsed;
    block->hitCount++;

    if (marker->parentBlockIndex) {
        MtbPerfBlock *parentBlock = &_mtb_perf_global_profile.blocks[marker->parentBlockIndex];
        parentBlock->cpuTimeElapsed -= cpuTimeElapsed;
    }

    _mtb_perf_current_block_index = marker->parentBlockIndex;
#endif
}

#endif //MTB_PERF_IMPLEMENTATION
