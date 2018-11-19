//
// Created by ty on 2018/11/19.
//

#ifndef EASY_STL_EASY_STL_LIST_H
#define EASY_STL_EASY_STL_LIST_H

#include <easy_stl_iterator.h>
#include "easy_stl_alloc.h"

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

        List();

        iterator begin() const;

        iterator end() const;

        bool empty() const;

        size_type size() const;

        reference front() const;

        reference back() const;

        void push_front(const T &x);

        void push_back(const T &x);

        iterator erase(iterator position);

        iterator insert(iterator position, const T &x);

        void pop_front();

        void pop_back();

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
        void transfer(iterator postion, iterator first, iterator last);

        void empty_initialize();

        link_type get_node();
        void put_node(link_type p);
        link_type create_node(const T &x);
        void destroy_node(link_type p);
    };

}

#endif //EASY_STL_EASY_STL_LIST_H
