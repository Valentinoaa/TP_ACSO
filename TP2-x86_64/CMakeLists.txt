cmake_minimum_required(VERSION 3.27)
project(TP2_x86_64 C)

set(CMAKE_C_STANDARD 11)

include_directories(ej1)

add_executable(TP2_x86_64
        ej1/ej1.c
        ej1/ej1.h
        ej1/main.c
        ej1/tester.c)
