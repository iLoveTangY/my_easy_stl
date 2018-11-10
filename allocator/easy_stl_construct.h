//
// Created by ty on 2018/11/7.
//

#ifndef EASY_STL_EASY_STL_CONSTRUCT_H
#define EASY_STL_EASY_STL_CONSTRUCT_H

#include <new>
#include "easy_stl_iterator.h"

namespace easy_stl
{
    /* 在指定的内存空间上构造对象 */
    template<typename T1, typename T2>
    inline void construct(T1 *p, const T2 &value)
    {
        new(p) T1(value);  // 定位new，在p指定的位置上构造T2，即调用T2::T2(value)
    }

    /* 析构对象 */
    template<typename T>
    inline void destroy(T *pointer)
    {
        pointer->~T();
    }

    /* destroy 第二版本，接收两个迭代器 */
    template<typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
    {
        for (; first < last; ++first)
        {
            destroy(&*first);
        }
    }

    template<typename ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
    {
    }

    template<typename ForwardIterator, typename T>
    inline void __destroy(ForwardIterator first, ForwardIterator last, T *)
    {
        typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }

    template<typename ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last)
    {
        __destroy(first, last, value_type(first));
    }
}

#endif //EASY_STL_EASY_STL_CONSTRUCT_H
