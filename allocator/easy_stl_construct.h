//
// Created by ty on 2018/11/7.
//

#ifndef EASY_STL_EASY_STL_CONSTRUCT_H
#define EASY_STL_EASY_STL_CONSTRUCT_H

#include <new>

namespace easy_stl
{
    /* 在指定的内存空间上构造对象 */
    template<typename T1, typename T2>
    inline void construct(T1* p, const T2& value)
    {
        new (p) T1(value);  // 定位new，在p指定的位置上构造T2，即调用T2::T2(value)
    }

    /* 析构对象 */
    template<typename T>
    inline void destroy(T* pointer)
    {
        pointer->~T();
    }

    // TODO 根据对象是否具有trival destructor来进行更有效率的析构对象操作
}

#endif //EASY_STL_EASY_STL_CONSTRUCT_H
