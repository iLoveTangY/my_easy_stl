//
// Created by ty on 2018/11/13.
//

#ifndef EASY_STL_VECTORTEST_H
#define EASY_STL_VECTORTEST_H

#include "easy_stl_vector.h"
#include "catch.hpp"

class VectorTest
{
public:
    static void test_initialize();
    static void test_begin();
    static void test_end();
    static void test_size();
    static void test_capacity();
    static void test_empty();
    static void test_operator_index();
    static void test_insert();
    static void test_front();
    static void test_back();
    static void test_push_back();
    static void test_pop_back();
    static void test_erase();
    static void test_resize();
    static void test_clear();
    static void test_all()
    {
        test_initialize();
        test_begin();
        test_end();
        test_size();
        test_capacity();
        test_empty();
        test_operator_index();
        test_insert();
        test_front();
        test_back();
        test_push_back();
        test_pop_back();
        test_erase();
        test_clear();
        test_resize();
    }
};

#endif //EASY_STL_VECTORTEST_H
