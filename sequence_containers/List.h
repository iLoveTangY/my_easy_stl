//
// Created by ty on 2018/11/19.
//

#ifndef EASY_STL_EASY_STL_LIST_H
#define EASY_STL_EASY_STL_LIST_H

#include <easy_stl_iterator.h>
#include "easy_stl_alloc.h"
#include "easy_stl_construct.h"

namespace easy_stl
{
    /* 双向链表的节点类型*/
    template<typename T>
    struct __list_node
    {
        __list_node<T> *prev;
        __list_node<T> *next;
        T data;
    };

    template<typename T, typename Ref, typename Ptr>
    struct __list_iterator
    {
        typedef __list_iterator<T, T &, T *> iterator;
        typedef __list_iterator<T, Ref, Ptr> self;

        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef __list_node<T> *link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        link_type node; // 迭代器中的普通指针，指向list的节点

        /* constructor */
        __list_iterator(link_type x) : node(x)
        {}

        __list_iterator() = default;

        __list_iterator(const iterator &x) : node(x.node)
        {}

        bool operator==(const self &x) const
        {
            return node == x.node;
        }

        bool operator!=(const self &x) const
        {
            return node != x.node;
        }

        reference operator*() const
        {
            return (*node).data;
        }

        pointer operator->() const
        {
            return &(operator*());
        }

        /* 前缀++ */
        self &operator++()
        {
            node = node->next;
            return *this;
        }

        /* 后缀++ */
        const self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self &operator--()
        {
            node = node->prev;
            return *this;
        }

        const self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };

    template<typename T, typename Alloc=MoreEfficientAlloc>
    class List
    {
    public:
        typedef T value_type;
        typedef value_type *pointer;
        typedef value_type &reference;
        typedef std::size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef __list_node<T> list_node;
        typedef simple_alloc<list_node, Alloc> list_node_allocator;
        typedef list_node *link_type;
        typedef __list_iterator<T, T &, T *> iterator;
        typedef List list;

        List()
        {
            empty_initialize();
        }

        iterator begin() const
        {
            return *(node->next);   // 从 link_type 构造一个iterator
        }

        iterator end() const
        {
            return node;
        }

        bool empty() const
        {
            return node->next == node;
        }

        size_type size() const
        {
            size_type result = 0;
            result = static_cast<size_type>(distance(begin(), end()));
            return result;
        }

        reference front() const
        {
            return *begin();
        }

        reference back() const
        {
            return *(--end());
        }

        void push_front(const T &x)
        {
            insert(begin(), x);
        }

        void push_back(const T &x)
        {
            insert(end(), x);
        }

        iterator erase(iterator position)
        {
            link_type next_node = position.node->next;
            link_type prev_node = position.node->prev;
            prev_node->next = next_node;
            next_node->prev = prev_node;
            destroy_node(position.node);
            return next_node;
        }

        iterator insert(iterator position, const T &x)
        {
            link_type p = create_node(x);
            p->next = position.node;
            p->prev = position.node->prev;
            position.node->prev->next = p;
            position.node->prev = p;
        }

        void pop_front()
        {
            erase(begin());
        }

        void pop_back()
        {
            iterator tmp = end();
            erase(--tmp);
        }

        void clear();

        void remove(const T &value);

        void unique();

        void splice(iterator positon, list &x);

        void splice(iterator position, list &, iterator i);

        void splice(iterator positon, list &, iterator first, iterator last);

        void merge(list &x);

        void reverse();

        void sort();


    private:
        link_type node; // 只要一个指针，即可表示整个双向环状链表
        void transfer(iterator position, iterator first, iterator last)
        {
            if (position != last)
            {
                last.node->prev->next = position.node;
                first.node->prev->next = last.node;
                position.node->prev->next = first.node;
                link_type tmp = position.node->prev;
                position.node->prev = last.node->prev;
                last.node->prev = first.node->prev;
                first.node->prev = tmp;
            }
        }

        void empty_initialize()
        {
            node = get_node();
            node->next = node;
            node->prev = node;
        }

        // 配置一个节点并返回
        link_type get_node()
        {
            return list_node_allocator::allocate();
        }
        // 释放一个节点
        void put_node(link_type p)
        {
            list_node_allocator::deallocate(p);
        }
        // 产生（配置并构造）一个节点，带有元素值
        link_type create_node(const T &x)
        {
            link_type p = get_node();
            construct(&p->data, x);
            return p;
        }
        // 销毁（释放并析构）一个节点
        void destroy_node(link_type p)
        {
            destroy(&p->data);
            put_node(p);
        }
    };

    template<typename T, typename Alloc>
    void List<T, Alloc>::clear()
    {
        link_type cur = node->next;
        while (cur != node)
        {
            link_type tmp = cur;
            cur = cur->next;
            destroy_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }

}

#endif //EASY_STL_EASY_STL_LIST_H
