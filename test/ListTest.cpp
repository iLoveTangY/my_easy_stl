//
// Created by ty on 2018/11/22.
//

#include "ListTest.h"

void ListTest::test_initialize()
{
    SECTION("默认构造函数测试")
    {
        easy_stl::List<int> lst;
        REQUIRE(lst.empty());
    }

    SECTION("初始化列表构造函数测试")
    {
        easy_stl::List<int> lst = {1, 2, 3, 4, 5};
        REQUIRE(lst.size() == 5);
        REQUIRE(lst.front() == 1);
        REQUIRE(lst.back() == 5);
        REQUIRE(*lst.begin() == 1);
        REQUIRE(*(--lst.end()) == 5);

        easy_stl::List<int> lst2 = {};
        REQUIRE(lst2.empty());
    }
}

void ListTest::test_begin()
{
    SECTION("单元素List的begin()测试")
    {
        easy_stl::List<int> lst = {248};
        REQUIRE(lst.size() == 1);
        REQUIRE(*lst.begin() == 248);
    }

    SECTION("多元素List的begin()测试")
    {
        easy_stl::List<int> lst = {135, 248, 1000, 234432, 323323};
        REQUIRE(lst.size() == 5);
        REQUIRE(*lst.begin() == 135);
    }
}

void ListTest::test_end()
{
    SECTION("单元素List的end()测试")
    {
        easy_stl::List<int> lst = {567};
        REQUIRE(--lst.end() == lst.begin());
        REQUIRE(*(--lst.end()) == 567);
    }

    SECTION("多元素List的end()测试")
    {
        easy_stl::List<int> lst = {154, 123, 3213, 323, 12, 321};
        REQUIRE(lst.size() == 6);
        REQUIRE(*(--lst.end()) == 321);
        easy_stl::List<int>::iterator it(lst.begin());
        auto p = lst.begin();
        for (int i = 0; i < 6; ++i)
            ++p;
        REQUIRE(p == lst.end());
    }
}

void ListTest::test_empty()
{
    SECTION("非空List的empty()测试")
    {
        easy_stl::List<int> lst = {1, 2, 3, 4, 5};
        REQUIRE(!lst.empty());
    }

    SECTION("空List的empty()测试")
    {
        easy_stl::List<int> lst;
        REQUIRE(lst.empty());
    }
}

void ListTest::test_size()
{
    SECTION("空List的size()测试")
    {
        easy_stl::List<int> lst;
        REQUIRE(lst.empty());
        REQUIRE(lst.size() == 0);
    }

    SECTION("非空List的size()测试")
    {
        easy_stl::List<int> lst = {3, 4, 2, 53, 23, 23};
        REQUIRE(!lst.empty());
        REQUIRE(lst.size() == 6);
    }
}

void ListTest::test_front()
{
    SECTION("空List的front()测试")
    {
        easy_stl::List<int> lst;
        // lst.front(); // 未定义行为
    }
    SECTION("非空List的front()测试")
    {
        easy_stl::List<int> lst = {1, 3, 12, 12, 21, 21, 343, 432};
        REQUIRE(lst.front() == 1);
        easy_stl::List<int> lst2 = {3, 12};
        REQUIRE(lst2.front() == 3);
        easy_stl::List<int> lst3 = {32};
        REQUIRE(lst3.front() == 32);
    }
}

void ListTest::test_back()
{
    SECTION("空List的back()测试")
    {
        easy_stl::List<int> lst;
        // lst.back(); // 未定义行为
    }

    SECTION("非空List的back()测试")
    {
        easy_stl::List<int> lst = {33};
        REQUIRE(lst.back() == 33);
        easy_stl::List<int> lst2 = {33, 44};
        REQUIRE(lst2.back() == 44);
    }
}

void ListTest::test_push_front()
{
    SECTION("List的push_front()测试")
    {
        easy_stl::List<int> lst;
        REQUIRE(lst.empty());
        lst.push_front(33);
        REQUIRE(lst.front() == 33);

        int size = 1000000;
        for (int i = 0; i < size; ++i)
        {
            lst.push_front(i);
            REQUIRE(lst.front() == i);
        }
        REQUIRE(lst.size() == size + 1);
    }
}

void ListTest::test_push_back()
{
    SECTION("List的push_back()测试")
    {
        easy_stl::List<int> lst;
        REQUIRE(lst.empty());
        lst.push_back(44);
        REQUIRE(lst.back() == 44);

        int size = 1000000;
        for (int i = 0; i < size; ++i)
        {
            lst.push_back(i);
            REQUIRE(lst.back() == i);
        }
        REQUIRE(lst.size() == size + 1);
    }
}

void ListTest::test_erase()
{
    SECTION("List的erase()测试")
    {
        easy_stl::List<int> lst;
        for (int i = 0; i < 100; ++i)
        {
            lst.push_back(i);
        }
        REQUIRE(lst.size() == 100);
        for (int i = 0; i < 100; ++i)
        {
            lst.erase(--lst.end());
            REQUIRE(lst.size() == 100 - i - 1);
        }
    }
}

void ListTest::test_insert()
{
    SECTION("空List的insert()测试")
    {
        easy_stl::List<int> lst;
        lst.insert(lst.end(), 10);
        REQUIRE(lst.size() == 1);
        REQUIRE(lst.back() == 10);
    }

    SECTION("非空List的头部插入测试")
    {
        easy_stl::List<int> lst = {1, 3, 5, 7, 9};
        REQUIRE(lst.size() == 5);
        auto ret = lst.insert(lst.begin(), 100);
        REQUIRE(*ret == 100);
        REQUIRE(lst.front() == 100);
        REQUIRE(lst.size() == 6);
        ret = lst.insert(lst.begin(), 10000);
        REQUIRE(*ret == 10000);
        REQUIRE(lst.size() == 7);
        REQUIRE(lst.front() == 10000);
    }

    SECTION("非空List中间插入测试")
    {
        easy_stl::List<int> lst = {1, 3, 5, 7, 9};
        auto b = lst.begin();
        auto ret = lst.insert(++b, 233);
        REQUIRE(lst.size() == 6);
        REQUIRE(*ret == 233);
        REQUIRE(b == ++ret);
        ret = lst.insert(++b, 2333);
        REQUIRE(lst.size() == 7);
        REQUIRE(*ret == 2333);
        REQUIRE(b == ++ret);
    }

    SECTION("非空List尾部插入测试")
    {
        easy_stl::List<int> lst = {1, 3, 5, 2, 9};
        for (int i = 10; i < 1000; ++i)
        {
            auto ret = lst.insert(lst.end(), i);
            REQUIRE(*ret == i);
            REQUIRE(ret == --lst.end());
        }
    }
}

void ListTest::test_pop_front()
{
    SECTION("List的pop_front()测试")
    {
        easy_stl::List<int> lst;
        for (int i = 0; i < 1000; ++i)
        {
            lst.push_back(i);
        }
        REQUIRE(lst.size() == 1000);
        for (int j = 0; j < 1000; ++j)
        {
            lst.pop_front();
            REQUIRE(lst.size() == 1000 - j - 1);
            if (!lst.empty())
                REQUIRE(lst.front() == j + 1);
        }
    }
}

void ListTest::test_pop_back()
{
    SECTION("List的pop_back()测试")
    {
        easy_stl::List<int> lst;
        for (int i = 0; i < 1000; ++i)
        {
            lst.push_back(i);
        }
        REQUIRE(lst.size() == 1000);
        for (int j = 0; j < 1000; ++j)
        {
            lst.pop_back();
            REQUIRE(lst.size() == 1000 - j - 1);
            if (!lst.empty())
                REQUIRE(lst.back() == 999 - j - 1);
        }
    }
}

void ListTest::test_clear()
{
    SECTION("空List的clear()")
    {
        easy_stl::List<int> lst;
        REQUIRE(lst.empty());
        lst.clear();
        REQUIRE(lst.empty());
    }
    SECTION("非空List的clear()")
    {
        easy_stl::List<int> lst = {1, 3, 5, 6, 9, 11};
        REQUIRE(lst.size() == 6);
        lst.clear();
        REQUIRE(lst.empty());
    }
}

void ListTest::test_remove()
{
    SECTION("List的remove()测试")
    {
        easy_stl::List<int> lst = {1, 1, 3, 5, 5, 6, 9};
        REQUIRE(lst.size() == 7);
        lst.remove(10);
        REQUIRE(lst.size() == 7);
        lst.remove(1);
        REQUIRE(lst.size() == 5);
        lst.remove(5);
        REQUIRE(lst.size() == 3);
        lst.remove(3);
        REQUIRE(lst.size() == 2);
        lst.remove(6);
        REQUIRE(lst.size() == 1);
        lst.remove(9);
        REQUIRE(lst.empty());
    }
}

void ListTest::test_unique()
{
    SECTION("空List的unique()测试")
    {
        easy_stl::List<int> lst;
        REQUIRE(lst.empty());
        lst.unique();
        REQUIRE(lst.empty());
    }

    SECTION("非空List的unique()测试")
    {
        easy_stl::List<int> lst = {1, 1, 3, 5, 7, 7, 9};
        REQUIRE(lst.size() == 7);
        lst.unique();
        REQUIRE(lst.size() == 5);
        lst.unique();
        REQUIRE(lst.size() == 5);
    }
}

void ListTest::test_splice()
{
    SECTION("整个List拼接(splice)测试")
    {
        easy_stl::List<int> lst;
        easy_stl::List<int> x = {1, 2, 3, 4};
        lst.splice(lst.begin(), x);
        REQUIRE(lst.size() == 4);
        auto p = lst.begin();
        for (int i = 0; i < lst.size(); ++i)
            REQUIRE(*p++ == i+1);
    }

    SECTION("单个元素拼接测试")
    {
        easy_stl::List<int> lst = {1, 2, 3, 4};
        easy_stl::List<int> x = {5, 6, 7, 8};
        REQUIRE(lst.size() == 4);
        lst.splice(lst.begin(), ++x.begin());
        REQUIRE(lst.size() == 5);
        REQUIRE(lst.front() == 6);
        lst.splice(lst.end(), --x.end());
        REQUIRE(lst.size() == 6);
        REQUIRE(lst.back() == 8);
    }

    SECTION("多元素拼接测试")
    {
        easy_stl::List<int> lst = {1, 2, 3};
        easy_stl::List<int> x = {4, 5, 6};
        auto first = x.begin();
        auto last = --x.end();
        REQUIRE(lst.size() == 3);
        lst.splice(lst.begin(), first, last);
        REQUIRE(lst.size() == 5);
        REQUIRE(lst.front() == 4);
        REQUIRE(*(++lst.begin()) == 5);
    }
}

void ListTest::test_merge()
{
    SECTION("List的merge测试")
    {
        easy_stl::List<int> lst1 = {1, 3, 5, 7};
        easy_stl::List<int> lst2 = {1, 4, 6, 8};
        lst1.merge(lst2);
        REQUIRE(lst1.size() == 8);
        REQUIRE(lst2.empty());  // 归并之后第二个链表变成空的
    }
}

void ListTest::test_reverse()
{
    SECTION("List的reverse测试")
    {
        easy_stl::List<int> lst;
        for (int i = 0; i < 100; ++i)
        {
            lst.push_back(i+1);
        }
        lst.reverse();
        for (int j = 0; j < 100; ++j)
        {
            REQUIRE(*(lst.begin()+j) == 100-j);
        }
    }
}
