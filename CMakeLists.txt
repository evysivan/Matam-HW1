cmake_minimum_required(VERSION 3.15)
project(hw1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(libmtm)
include_directories(tests)

add_executable(hw1
        libmtm/list.h
        libmtm/set.h
#        tests/amount_set_main.c
#        tests/amount_set_tests.c
#        tests/amount_set_tests.h
#        tests/matamazom_main.c
#        tests/matamazom_tests.c
#        tests/matamazom_tests.h
#        tests/test_utilities.h
        amount_set.h
        list.h
        matamazom.h
        matamazom_print.c
        matamazom_print.h
        set.h amount_set.c linked_list.c linked_list.h amount_set_updated.c)
