//
// Created by ty on 2018/11/12.
//

#ifndef EASY_STL_EASY_STL_VECTOR_H
#define EASY_STL_EASY_STL_VECTOR_H

#include "easy_stl_construct.h"
#include "easy_stl_alloc.h"
#include "easy_stl_uninitialized.h"
#include <algorithm>    // TODO 替换成自己实现的copy()

namespace easy_stl
{
    template<typename T, typename Alloc=MoreEfficientAlloc>
    class Vector
    {
    public:
        typedef T value_type;
        typedef value_type *pointer;
        typedef value_type *iterator;
        typedef value_type &reference;
        typedef std::size_t size_type;
        typedef ptrdiff_t difference_type;

        /* vector的5个构造函数 */
        Vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr)
        {}

        Vector(size_type n, const T &value)
        {
            fill_initialize(n, value);
        }

        Vector(int n, const T &value)
        {
            fill_initialize(static_cast<size_type>(n), value);
        }

        Vector(long n, const T &value)
        {
            fill_initialize(static_cast<size_type>(n), value);
        }

        explicit Vector(size_type n) // explicit 表示不能通过这个构造函数将一个n转换为一个vector
        {
            fill_initialize(n, T());
        }

        /* 析构函数 */
        ~Vector()
        {
            destroy(start, finish);
            deallocate();
        }

        /* 主要的功能函数 */
        iterator begin() const
        {
            return start;
        }

        iterator end() const
        {
            return finish;
        }

        size_type size() const
        {
            return size_type(end() - begin());
        }

        size_type capacity() const
        {
            return size_type(end_of_storage - begin());
        }

        bool empty() const
        {
            return begin() == end();
        }

        reference operator[](size_type n)
        {
            return *(begin() + n);
        }

        void insert(iterator position, size_type n, const T &x);

        void insert(iterator position, const T &x)
        {
            insert_aux(position, x);
        }

        reference front() const
        {
            return *begin();
        }

        reference back() const
        {
            return *(end() - 1);
        }

        void push_back(const T &x)
        {
            if (finish != end_of_storage)
            {
                construct(finish, x);
                ++finish;
            }
            else
                insert_aux(end(), x);
        }

        void pop_back()
        {
            if (finish != start)
            {
                --finish;
                destroy(finish);
            }
        }

        iterator erase(iterator position)
        {
            // 如果清除的不是最后一个元素，要将后续元素往前移动
            if (position + 1 != end())
                std::copy(position + 1, finish, position);    // TODO 替换
            --finish;
            destroy(finish);
            return position;
        }

        iterator erase(iterator first, iterator last);

        void resize(size_type new_size, const T &x)
        {
            if (new_size < size())
                erase(begin() + new_size, end());
            else
                insert(end(), new_size - size(), x);
        }

        void resize(size_type new_size)
        {
            resize(new_size, T());
        }

        void clear()
        {
            erase(begin(), end());
        }

    private:
        typedef simple_alloc<value_type, Alloc> data_allocator;

        iterator start;
        iterator finish;
        iterator end_of_storage;

        void insert_aux(iterator position, const T &x);

        /* 释放掉vector的所有空间 */
        void deallocate()
        {
            if (start)
                data_allocator::deallocate(start, end_of_storage - start);  // 释放掉所有空间
        }

        /* 分配空间并初始化 */
        void fill_initialize(size_type n, const T &value)
        {
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }

        iterator allocate_and_fill(size_type n, const T &x);
    };

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::insert(easy_stl::Vector<T, Alloc>::iterator position,
                                  unsigned int n, const T &x)
    {
        if (n != 0)
        {
            iterator old_finish = finish;
            // 备用空间大于新增元素个数
            if (end_of_storage - finish >= n)
            {
                const size_type elems_after = finish - position;    // 插入点之后的元素个数
                if (elems_after > n)    // 如果插入点之后的元素个数大于新增元素个数，则需要从后往前拷贝
                {
                    uninitialized_copy(finish - n, finish, finish);   // 往最后需要新增的n块“未初始化”的空间上拷贝适当的值
                    std::copy_backward(position, finish - n,
                                       finish); // 从finish开始往前填充position后，finish-n之前（没有拷贝到未初始化的空间上）的那些值
                    std::fill(position, position + n, x); // 从插入点开始填新值
                    finish += n;
                }
                else    // 插入点之后的元素个数小于新增元素个数，可以直接从前往后
                {
                    uninitialized_fill_n(finish, n - elems_after, x); // 新增空间的n-elems_after个元素一定要填x
                    finish += n - elems_after;
                    uninitialized_copy(position, old_finish, finish);   // 将position后面的所有elems_after个元素拷贝到尾部
                    finish += elems_after;
                    std::fill(position, old_finish, x); // 填充position开始的剩余elems_after个元素
                }
            }
            else    // 备用空间不够，必须配置额外的内存
            {
                const size_type old_size = size();
                const size_type len = old_size + std::max(old_size, n); // 要么是原来长度的两倍，要么是新增长度加上原来的长度
                iterator new_start = data_allocator::allocate(len);
                // 先将旧vector插入点之前的元素拷贝到新空间
                iterator new_finish = new_start;
                try
                {
                    new_finish = uninitialized_copy(start, position, new_start);
                    // 再将新增元素填充到新空间
                    new_finish = uninitialized_fill_n(new_finish, n, x);
                    // 将插入点之后的元素拷贝到新空间
                    new_finish = uninitialized_copy(position, finish, new_finish);
                }
                catch (...) // "commit or rollback"
                {
                    destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, len);
                    throw;
                }

                // 释放掉旧空间
                destroy(start, finish);
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + n;
            }
        }
    }

    template<typename T, typename Alloc>
    typename Vector<T, Alloc>::iterator Vector<T, Alloc>::erase(Vector::iterator first, Vector::iterator last)
    {
        iterator i = std::copy(last, finish, first);
        destroy(i, finish);
        finish = finish - (last - first);
        return first;
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::insert_aux(Vector::iterator position, const T &x)
    {
        if (finish != end_of_storage)   // 还有备用空间，因位此函数会被insert(position, x)直接调用，所以需要作此判断
        {
            if (finish == position)
            {
                construct(finish, x);
                ++finish;
            }
            else
            {
                construct(finish, *(finish - 1));
                ++finish;
                std::copy_backward(position, finish - 2, finish - 1);
                *position = x;
            }
        }
        else
        {
            const size_type old_size = size();
            const size_type len = old_size != 0 ? 2 * old_size : 1; // 如果原大小为0，则配置1，否则配置原大小的两倍大小，前半段放置原来的元素，后半段放置新元素
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try
            {
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish++, x);
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch (...) // "commit or rollback"
            {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }

            destroy(begin(), end());
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }

    // 配置空间并填满内容
    template<typename T, typename Alloc>
    typename Vector<T, Alloc>::iterator Vector<T, Alloc>::allocate_and_fill(Vector::size_type n, const T &x)
    {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }
}

#endif //EASY_STL_EASY_STL_VECTOR_H
