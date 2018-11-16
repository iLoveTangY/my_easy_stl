//
// Created by ty on 2018/11/14.
//
#include "VectorTest.h"
#include <iostream>

void VectorTest::test_initialize()
{
    SECTION("默认构造函数测试")
    {
        easy_stl::Vector<int> ivec;
        REQUIRE(ivec.empty());
        REQUIRE(ivec.capacity() == 0);
        REQUIRE(ivec.begin() == nullptr);
        REQUIRE(ivec.end() == nullptr);
    }

    SECTION("指定数量和初值构造函数测试")
    {
        int inum = 5;
        easy_stl::Vector<int> ivec(inum, 0);
        REQUIRE(ivec.size() == 5);
        REQUIRE(ivec.capacity() >= 5);
        REQUIRE(*ivec.begin() == 0);
        REQUIRE(*(ivec.end() - 1) == 0);
    }

    SECTION("只指定数量构造函数测试")
    {
        easy_stl::Vector<int>::size_type num = 8;
        easy_stl::Vector<int> ivec(num);
        REQUIRE(ivec.size() == 8);
        REQUIRE(ivec.capacity() >= 8);
    }
}

void VectorTest::test_begin()
{
    SECTION("空Vector的begin测试")
    {
        easy_stl::Vector<int> ivec;
        REQUIRE(ivec.begin() == nullptr);
    }

    SECTION("有元素的begin测试")
    {
        easy_stl::Vector<int> ivec(5, 1);
        REQUIRE(*ivec.begin() == 1);
    }

}

void VectorTest::test_end()
{
    SECTION("空Vector的end测试")
    {
        easy_stl::Vector<int> ivec;
        REQUIRE(ivec.end() == ivec.begin());
    }

    SECTION("有元素的end测试")
    {
        easy_stl::Vector<int> ivec(5, 4);
        REQUIRE(*(ivec.end() - 1) == 4);
    }
}

void VectorTest::test_size()
{
    SECTION("测试空Vector的size")
    {
        easy_stl::Vector<int> ivec;
        REQUIRE(ivec.empty());
    }

    SECTION("测试指定大小Vector的size")
    {
        easy_stl::Vector<int> ivec(5);
        REQUIRE(ivec.size() == 5);

        easy_stl::Vector<int> ivec2(10);
        REQUIRE(ivec2.size() == 10);
    }
}

void VectorTest::test_capacity()
{
    SECTION("测试Vector容量")
    {
        easy_stl::Vector<int> ivec(5);
        REQUIRE(ivec.capacity() == 5);
    }
}

void VectorTest::test_empty()
{
    SECTION("测试Vector是否为空")
    {
        easy_stl::Vector<int> ivec;
        REQUIRE(ivec.empty());

        easy_stl::Vector<int> ivec2(10);
        REQUIRE(!ivec2.empty());
    }
}

void VectorTest::test_operator_index()
{
    SECTION("测试Vector下标运算符")
    {
        easy_stl::Vector<int> ivec(10, 9);
        for (size_t i = 0; i < ivec.size(); ++i)
            REQUIRE(ivec[i] == 9);
    }
}

void VectorTest::test_insert()
{
    SECTION("测试空Vector头部插入")
    {
        easy_stl::Vector<int> ivec;
        ivec.insert(ivec.begin(), 4);
        REQUIRE(ivec.size() == 1);
        REQUIRE(*ivec.begin() == 4);
    }

    SECTION("测试空Vector尾部插入")
    {
        easy_stl::Vector<int> ivec;
        ivec.insert(ivec.end(), 5);
        REQUIRE(ivec.size() == 1);
        REQUIRE(*ivec.begin() == 5);
    }

    SECTION("测试非空Vector单个元素插入")
    {
        easy_stl::Vector<int> ivec(5, 0);
        REQUIRE(ivec.size() == 5);
        auto position = ivec.begin() + 1;
        ivec.insert(position, 4);
        position = ivec.begin() + 1;    // 必须得重新给position赋值，插入之后迭代器可能失效
        REQUIRE(ivec.size() == 6);
        REQUIRE(*ivec.begin() == 0);
        REQUIRE(*(ivec.begin()+1) == 4);
        REQUIRE(*position == 4);
        ivec.insert(ivec.end(), 8);
        REQUIRE(*(ivec.end() - 1) == 8);
        REQUIRE(ivec.size() == 7);
    }

    SECTION("测试非空Vector多个元素插入")
    {
        easy_stl::Vector<int> ivec(5, 0);
        REQUIRE(ivec.size() == 5);
        auto position = ivec.begin();
        ivec.insert(position, 5, 4);
        REQUIRE(ivec.size() == 10);
        position = ivec.begin();
        for (auto i = position; i < position + 4; ++i)
            REQUIRE(*i == 4);
    }
}

void VectorTest::test_front()
{
    SECTION("测试非空Vector的front")
    {
        easy_stl::Vector<int> ivec(6, 8);
        REQUIRE(ivec.front() == 8);
    }
}

void VectorTest::test_back()
{
    SECTION("测试非空Vector的back")
    {
        easy_stl::Vector<int> ivec(10, 9);
        REQUIRE(ivec.back() == 9);
    }
}

void VectorTest::test_push_back()
{
    SECTION("测试大量元素push_back")
    {
        easy_stl::Vector<int> ivec;
        for (int i = 0; i < 10000; ++i)
        {
            ivec.push_back(i);
            REQUIRE(ivec.size() == i + 1);
            REQUIRE(ivec.back() == i);
        }
    }
}

void VectorTest::test_pop_back()
{
    SECTION("测试大量元素pop_back")
    {
        easy_stl::Vector<int> ivec(1000, 1);
        for (int i = 1000; i > 0; --i)
        {
            ivec.pop_back();
            REQUIRE(ivec.size() == i - 1);
        }
    }
}

void VectorTest::test_resize()
{
    SECTION("测试Vector的resize")
    {
        easy_stl::Vector<int> ivec;
        REQUIRE(ivec.empty());
        ivec.resize(10);
        REQUIRE(ivec.size() == 10);
        ivec.resize(20, 5);
        REQUIRE(ivec.size() == 20);
        for (auto pos = ivec.begin()+10; pos != ivec.end(); ++pos)
            REQUIRE(*pos == 5);
    }
}

void VectorTest::test_erase()
{
    SECTION("测试Vector的erase")
    {
        easy_stl::Vector<int> ivec(10, 0);
        REQUIRE(ivec.size() == 10);
        ivec.erase(ivec.begin());
        REQUIRE(ivec.size() == 9);
        ivec.erase(ivec.begin(), ivec.end());
        REQUIRE(ivec.empty());
    }
}

void VectorTest::test_clear()
{
    easy_stl::Vector<int> ivec(10, 0);
    REQUIRE(ivec.size() == 10);
    ivec.clear();
    REQUIRE(ivec.empty());
}
