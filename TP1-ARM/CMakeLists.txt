cmake_minimum_required(VERSION 3.27)
project(TP1_ARM C)

set(CMAKE_C_STANDARD 11)

include_directories(aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src)
include_directories(aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include)
include_directories(aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include-fixed)
include_directories(aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include-fixed/linux)
include_directories(src)

add_executable(TP1_ARM
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/gcov-io.c
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/gcov-io.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/gcov-iov.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/libgcov-driver-kernel.c
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/libgcov-driver.c
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/libgcov-kernel.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/libgcov-merge.c
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/libgcov-profiler.c
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/gcov-src/libgcov.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/arm_neon.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/float.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/iso646.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/omp.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdalign.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdarg.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdatomic.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdbool.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stddef.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdfix.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdint-gcc.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdint.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/stdnoreturn.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/unwind.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include/varargs.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include-fixed/linux/a.out.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include-fixed/limits.h
        aarch64-linux-android-4.9/lib/gcc/aarch64-linux-android/4.9/include-fixed/syslimits.h
        src/shell.c
        src/shell.h
        src/sim.c)
