//
// Created by ty on 2018/11/22.
//

#ifndef EASY_STL_LISTTEST_H
#define EASY_STL_LISTTEST_H

#include "catch.hpp"
#include "List.h"


struct ListTest
{
    static void test_initialize();
    static void test_begin();
    static void test_end();
    static void test_empty();
    static void test_size();
    static void test_front();
    static void test_back();
    static void test_push_front();
    static void test_push_back();
    static void test_erase();
    static void test_insert();
    static void test_pop_front();
    static void test_pop_back();
    static void test_clear();
    static void test_remove();
    static void test_unique();
    static void test_splice();
    static void test_merge();
    static void test_reverse();
    // TODO static void test_sort();
    static void test_all()
    {
        test_initialize();
        test_begin();
        test_end();
        test_empty();
        test_size();
        test_front();
        test_back();
        test_push_front();
        test_push_back();
        test_erase();
        test_insert();
        test_pop_front();
        test_pop_back();
        test_clear();
        test_remove();
        test_unique();
        test_splice();
        test_merge();
        test_reverse();
        // TODO test_sort();
    }
};


#endif //EASY_STL_LISTTEST_H
