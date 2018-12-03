//
// Created by ty on 2018/11/30.
//

#ifndef EASY_STL_DEQUE_H
#define EASY_STL_DEQUE_H

#include <cstddef>
#include "easy_stl_iterator.h"

namespace easy_stl
{
    /*
     * Deque类的迭代器
     * */
    template<typename T, typename Ref, typename Ptr, size_t Bufsize>
    struct __Deque_iterator
    {
        typedef __Deque_iterator<T, T &, T *, Bufsize> iterator;
        typedef __Deque_iterator<T, const T &, const T *, Bufsize> const_iterator;

        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef T **map_pointer;    // 指向控制中心的元素的类型

        T *cur;     // 指向当前元素
        T *first;   // 指向缓冲区第一个元素
        T *last;    // 指向缓冲区最后一个元素之后的位置
        map_pointer node;   // 每个迭代器都有一个指向控制中心的指针

        // 计算一个缓冲区存储的元素个数，默认缓冲区大小为512字节
        inline static size_t __Deque_buf_size(size_t n, size_t sz)
        {
            return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
        }

        static size_t buffer_size()
        {
            return __Deque_buf_size(Bufsize, sizeof(T));
        }

        void set_node(map_pointer new_node)
        {
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }

        reference operator*() const
        {
            return *cur;
        }

        pointer operator->() const
        {
            return &(operator*());
        }

        // 计算两个迭代器之间的距离，x所指元素需在*this之前
        difference_type operator-(const iterator &x) const
        {
            return static_cast<difference_type>(difference_type((node - x.node) * buffer_size()) + (x.last - x.cur) + (cur - first));
        }

        // 前置递增运算符
        iterator &operator++()
        {
            if (++cur == last)  // 如果超出了缓冲区则要挪动到下一个缓冲区
            {
                set_node(node + 1);
                cur = *node;
            }
            return cur;
        }

        // 后置递增运算符
        const iterator operator++(int)
        {
            iterator tmp = cur;
            ++cur;
            return tmp;
        }

        // 前置递减运算符
        iterator &operator--()
        {
            if(cur == first)
            {
                set_node(node-1);
                cur = last;
            }
            --cur;
            return cur;
        }

        // 前置递减运算符
        const iterator operator--(int)
        {
            iterator tmp = cur;
            --cur;
            return tmp;
        }

        iterator& operator+=(difference_type n)
        {
            // 需要考虑 n 为负值的情况
            auto offset = static_cast<difference_type>(n + (cur - first));
            if (offset >= 0 && offset < difference_type(buffer_size()))
            {
                cur += n;
            }
            else
            {
                difference_type node_offset = offset > 0 ? offset / buffer_size() : (-(-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + offset - node_offset * buffer_size();
            }
            return this;
        }

        iterator operator+(difference_type n)
        {
            iterator tmp = *this;
            return tmp += n;
        }

        iterator &operator-=(difference_type n)
        {
            return *this += -n;
        }

        iterator operator-(difference_type n)
        {
            iterator tmp = *this;
            return tmp -= n;
        }

        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }

        bool operator==(const iterator &x) const
        {
            return x.cur == cur;
        }

        bool operator!=(const iterator &x) const
        {
            return !(x == *this);
        }

        bool operator<(const iterator &x) const
        {
            return (node == x.node) ? (cur < x.cur) : (node < x.node);
        }
    };
}

#endif //EASY_STL_DEQUE_H
