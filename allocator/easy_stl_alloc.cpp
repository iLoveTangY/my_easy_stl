//
// Created by ty on 2018/11/7.
//
#include "easy_stl_alloc.h"

namespace easy_stl
{
    using std::size_t;
    using std::bad_alloc;

    void (*EasyAlloc::alloc_oom_handler)() = nullptr;

    void *EasyAlloc::oom_malloc(size_t n)
    {
        void *result;
        for (;;)    /* 无限循环，不断尝试调用处理函数，配置内存，调用处理函数，配置内存，直到内存配置成功 */
        {
            auto my_alloc_handler = alloc_oom_handler;
            if (nullptr == my_alloc_handler)
                throw bad_alloc();  /* 如果没有定义处理函数，直接抛出bad_alloc异常 */
            (*my_alloc_handler)();
            result = malloc(n);
            if (result != nullptr)
                return result;
        }
    }

    void *EasyAlloc::oom_realloc(void *p, std::size_t n)
    {
        void *result;

        for (;;)
        {
            auto my_alloc_handler = alloc_oom_handler;
            if (nullptr == my_alloc_handler)
                throw bad_alloc();
            (*my_alloc_handler)();
            result = realloc(p, n);
            if (result != nullptr)
                return result;
        }
    }

    /****************************************************************************************************/

    char *MoreEfficientAlloc::start_free = nullptr;
    char *MoreEfficientAlloc::end_free = nullptr;
    size_t MoreEfficientAlloc::heap_size = 0;
    MoreEfficientAlloc::obj *MoreEfficientAlloc::free_list[__NFREELISTS] =
            {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
             nullptr, nullptr, nullptr, nullptr};   /* 开始时所有链表都为空 */

    void *MoreEfficientAlloc::allocate(std::size_t n)
    {
        obj *result;

        if (n > static_cast<size_t>(__MAX_BYTES))
            return EasyAlloc::allocate(n);
        // 找到那条适合的链表的头指针
        auto my_free_list = free_list + FREELIST_INDEX(n);
        // 看是否还有空闲块
        result = *my_free_list;
        if (result == nullptr)
        {
            // 没找到可用的空闲块，则要重新填充free-list
            void *r = refill(ROUND_UP(n));
            return r;
        }
        // 重新调整free-list
        *my_free_list = result->next;
        return result;
    }

    void MoreEfficientAlloc::deallocate(void *p, std::size_t n)
    {
        obj *q = static_cast<obj *>(p);
        obj **my_free_list;

        if (n > static_cast<size_t>(__MAX_BYTES))
        {
            EasyAlloc::deallocate(p, n);
            return;
        }

        // 调整free-list，收回区间
        my_free_list = free_list + FREELIST_INDEX(n);
        q->next = *my_free_list;
        *my_free_list = q;
    }

    void *MoreEfficientAlloc::refill(std::size_t n)
    {
        int nobjs = 20; // 需要填充时缺省取得20个区块

        char *chunk = chunk_alloc(n, nobjs);    // 通过chunk_alloc()取得新内存区块
        obj **my_free_list;

        if (1 == nobjs) // 如果只取得一个区块，直接返回给调用者，不用放进链表中了
            return chunk;

        my_free_list = free_list + FREELIST_INDEX(n);
        obj *result = (obj *) chunk; // 第一块返回给客户端
        obj *next_obj, *current_obj;
        *my_free_list = next_obj = (obj *) (chunk + n); // 从下一块开始要加入到free-list中
        for (int i = 1;; ++i)
        {
            current_obj = next_obj;
            next_obj = (obj *) ((char *) next_obj + n);
            if (nobjs - 1 == i) // 到了最后一个块
            {
                current_obj->next = nullptr;
                break;
            }
            else
                current_obj->next = next_obj;
        }
        return result;
    }

    char *MoreEfficientAlloc::chunk_alloc(std::size_t size, int &nobjs)
    {
        char *result;
        size_t total_bytes = size * nobjs;          // 需要多少字节
        size_t bytes_left = end_free - start_free;  // 内存池剩余空间

        if (bytes_left >= total_bytes)  // 剩余空间满足需求
        {
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else if (bytes_left >= size)    // 剩余空间不能满足需求，但是还够供应一个以上的区块，这种情况下就会发生nobjs不为20
        {
            nobjs = bytes_left / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;  // 有多少返回多少
        }
        else    // 内存池中连一个区块都无法满足，则需要重新分配内存给内存池
        {
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
            // 如果内存池里面还有剩余空间，将它编入合适的free-list
            if (bytes_left > 0)
            {
                obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *) start_free)->next = *my_free_list;
                *my_free_list = (obj *) start_free;
            }
            start_free = (char *) malloc(bytes_to_get);
            if (nullptr == start_free)
            {// 如果没有分配到空间，就在我们现有的free-list中找看有没有足够的空间分配给客户端
                obj **my_free_list, *p;
                for (size_t i = size; i <= __MAX_BYTES; i += __ALIGN)
                {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (p != nullptr)   // 如果当前链表还有空间，将当前链表的一个空闲区块放入内存池中，然后重新调用chunk_alloc
                    {
                        *my_free_list = p->next;
                        start_free = (char *) p;
                        end_free = start_free + i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                // 调用第一级配置器看能不能分配到内存，因为其中可能设置了malloc_handler
                end_free = nullptr;
                start_free = (char *) EasyAlloc::allocate(bytes_to_get);
            }
            heap_size += bytes_to_get;  // 分配的内存会越来越大
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }

    // TODO 是否能够也采用内存池的方式更有效率的分配内存？
    void *MoreEfficientAlloc::reallocate(void *p, std::size_t old_sz, std::size_t new_sz)
    {
        return EasyAlloc::reallocate(p, old_sz, new_sz);
    }
}
