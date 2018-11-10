//
// Created by ty on 2018/11/9.
//

#ifndef EASY_STL_EASY_STL_UNINITIALIZED_H
#define EASY_STL_EASY_STL_UNINITIALIZED_H

#include "easy_stl_construct.h"

namespace easy_stl
{
    // TODO 利用type_traits实现更有效率的函数

    /* 以下函数返回的都是最后构造的位置 */
    template<typename ForwardIterator, typename Size, typename T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
    {
        ForwardIterator cur = first;
        for (; n > 0; ++cur)
            construct(&*cur, x);
        return cur;
    }

    template<typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {
        ForwardIterator cur = result;
        for (; first != last; ++first, ++cur)
            construct(&*cur, *first);
        return cur;
    }

    template<typename ForwardIterator, typename T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x)
    {
        ForwardIterator cur = first;
        for (; cur != last; ++cur)
            construct(&*cur, x);
        return cur;
    }
}

#endif //EASY_STL_EASY_STL_UNINITIALIZED_H
