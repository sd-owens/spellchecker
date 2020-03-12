cmake_minimum_required(VERSION 3.15)
project(hashmap_spellcheck C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(hashmap_spellcheck
        CuTest.h
        CuTest.c
        tests.c
        hashMap.c
        hashMap.h)
