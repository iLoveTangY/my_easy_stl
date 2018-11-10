//
// Created by ty on 2018/11/7.
//

#ifndef EASY_STL_EASY_STL_ALLOC_H
#define EASY_STL_EASY_STL_ALLOC_H

#include <new>      /* std::bad_alloc */
#include <cstdlib>  /* malloc() free() */
#include <cstddef>

namespace easy_stl
{
    /* 简单的利用malloc和free进行内存配置操作的第一级配置器 */
    class EasyAlloc
    {
    public:
        inline static void *allocate(std::size_t n)
        {
            auto result = malloc(n);   // 第一级配置器直接使用malloc分配内存
            if (result == nullptr)
                result = oom_malloc(n);
            return result;
        }

        /* 其实并不需要第二个参数，之所以加上是为了和之后的第二级配置器接口一致 */
        inline static void deallocate(void *p, std::size_t)
        {
            free(p);
        }

        inline static void *reallocate(void *p, std::size_t, std::size_t new_sz)
        {
            auto result = realloc(p, new_sz);
            if (result == nullptr)
                result = oom_realloc(p, new_sz);
            return result;
        }

        /* 类似于C++的set_new_handler
         * 这个函数声明可能有点让人困惑，set_malloc_handler是个函数，它的参数
         * 又是另外一个函数函数指针，它的返回值也是一个函数指针。
         * */
        inline static void (*set_malloc_handler(void (*f)()))()
        {
            auto old = alloc_oom_handler;
            alloc_oom_handler = f;

            return old;
        }

    private:
        static void *oom_malloc(std::size_t);            // 当allocate调用取不到足够的内存时调用
        static void *oom_realloc(void *, std::size_t);    // 当reallocate调用取不到足够的内存时调用
        // TODO 考虑利用函数对象替换这里的函数指针
        static void (*alloc_oom_handler)();        // 取不到足够内存时该调用的处理函数，类似于new_handler
    };

    /*******************************************************************************/

    /* 利用内存池机制的更有效率的空间配置器 */
    class MoreEfficientAlloc
    {
    public:
        static void *allocate(std::size_t n);

        static void deallocate(void *p, std::size_t n);

        static void *reallocate(void *p, std::size_t old_sz, std::size_t new_sz);

    private:
        // TODO 是否可以将此处的枚举用constexpr代替？
        enum
        {
            __ALIGN = 8
        };         // 小型区块的上调边界，最小配置的内存
        enum
        {
            __MAX_BYTES = 128
        };   //  小型区块的上界，大于此大小则调用第一级配置器
        enum
        {
            __NFREELISTS = __MAX_BYTES / __ALIGN
        };    // free-list个数，此处是16个

        union obj
        {
            union obj *next;
            char client_data[1];
        };

        static obj *free_list[__NFREELISTS];

        /* 使n向__ALIGN取整 */
        inline static std::size_t ROUND_UP(std::size_t bytes)
        {
            return (((bytes) + __ALIGN) & ~(__ALIGN - 1));
        }

        /* 给定要分配的大小，计算在free-list中的索引 */
        inline static std::size_t FREELIST_INDEX(std::size_t bytes)
        {
            return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
        }

        /* 在空闲链表中空间不足时重新填充空闲链表，并分配n个字节的空间 */
        static void *refill(std::size_t n);

        /* 配置一大块空间,可容纳nobjs个大小为"size"的区块
         * 但是如果空间不够，可能少于nobjs个 */
        static char *chunk_alloc(std::size_t size, int &nobjs);

        static char *start_free;    // 内存池起始位置，只在chunk_alloc中改变
        static char *end_free;      // 内存池结束位置，只在chunk_alloc中改变
        static std::size_t heap_size;   // 重新给内存池分配内存时的一个附加量，会越来越大
    };

    /* 为了满足STL的接口要求，在我们实现的配置器外面再做一层封装 */
    template<typename T, class Alloc>
    class simple_alloc
    {
    public:
        typedef T value_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef std::size_t size_type;
        typedef ptrdiff_t difference_type;

        static T *allocate(std::size_t n)
        {
            return 0 == n ? nullptr : (T *) Alloc::allocate(n * sizeof(T));
        }

        static T *allocate()
        {
            return (T *) Alloc::allocate(sizeof(T));
        }

        static void deallocate(T *p, std::size_t n)
        {
            if (0 != n)
                Alloc::deallocate(p, n * sizeof(T));
        }

        static void deallocate(T *p)
        {
            Alloc::deallocate(p, sizeof(T));
        }
    };
}

#endif //EASY_STL_EASY_STL_ALLOC_H
