//
// Created by ty on 2018/11/19.
//

#ifndef EASY_STL_EASY_STL_LIST_H
#define EASY_STL_EASY_STL_LIST_H

#include <initializer_list>
#include <iostream>

#include "easy_stl_iterator.h"
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
        explicit __list_iterator(link_type x) : node(x)
        {}

//        __list_iterator() = default;

        /* 拷贝构造函数 */
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

    template<typename T, typename Ref, typename Ptr>
    __list_iterator<T, Ref, Ptr> operator+(__list_iterator<T, Ref, Ptr> i, int n)
    {
        advance(i, n);
        return i;
    }

    template<typename T, typename Ref, typename Ptr>
    __list_iterator<T, Ref, Ptr> operator-(__list_iterator<T, Ref, Ptr> i, int n)
    {
        advance(i, -n);
        return i;
    }

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
        typedef typename __list_iterator<T, T &, T *>::iterator iterator;
        typedef List list;

        List()
        {
            empty_initialize();
        }

        List(std::initializer_list<T> lst)
        {
            empty_initialize();
            for (auto item : lst)
                push_back(item);
        }

        iterator begin() const
        {
            return iterator(node->next);   // 从 link_type 构造一个iterator
        }

        iterator end() const
        {
            return iterator(node);
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

        /* 删除position所指的节点*/
        iterator erase(iterator position)
        {
            link_type next_node = position.node->next;
            link_type prev_node = position.node->prev;
            prev_node->next = next_node;
            next_node->prev = prev_node;
            destroy_node(position.node);
            return iterator(next_node);
        }

        /* 在position之前插入一个值为x的节点，返回一个指向新插入节点的迭代器 */
        iterator insert(iterator position, const T &x)
        {
            link_type p = create_node(x);
            p->next = position.node;
            p->prev = position.node->prev;
            position.node->prev->next = p;
            position.node->prev = p;

            return iterator(p);
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

        /* 移除所有值为value的节点 */
        void remove(const T &value);

        /* 链表中连续重复的节点只保留一个 */
        void unique();

        /* 将链表x整个插入到position之前的位置,x必须不同于当前链表 */
        void splice(iterator positon, list &x);

        /* 将i所指的元素插入到position之前,position和i可指向同一个链表 */
        void splice(iterator position, iterator i);

        /* 将[first, last)范围内的元素插入到position之前
         * position和[first, last)可指向同一个链表
         * 但position不能位于[first, last)内
         * */
        void splice(iterator positon, iterator first, iterator last);

        /* 将链表x归并到当前链表(*this)中，两个链表都得是有序的 */
        void merge(list &x);

        /* 逆置当前链表(*this) */
        void reverse();

        /* 将当前链表(*this)排序 */
        void sort();


    private:
        link_type node; // 只要一个指针，即可表示整个双向环状链表
        /* 将链表中[first, last)范围内的节点插入到position中 */
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

    template<typename T, typename Alloc>
    void List<T, Alloc>::remove(const T &value)
    {
        iterator first = begin(), last = end();
        while (first != last)
        {
            iterator next = first;
            ++next;
            if (*first == value)
                erase(first);
            first = next;
        }
    }

    template<typename T, typename Alloc>
    void List<T, Alloc>::unique()
    {
        iterator first = begin(), last = end();
        if (first == last)
            return;
        iterator next = first;
        while (++next != last)
        {
            if (*first == *next)
                erase(next);
            else
                first = next;
            next = first;
        }
    }

    template<typename T, typename Alloc>
    void List<T, Alloc>::splice(List::iterator positon, List::list &x)
    {
        if (!x.empty())
            transfer(positon, x.begin(), x.end());
    }

    template<typename T, typename Alloc>
    void List<T, Alloc>::splice(List::iterator position, List::iterator i)
    {
        if (position == i || position == i + 1)
            return;
        transfer(position, i, i + 1);
    }

    template<typename T, typename Alloc>
    void List<T, Alloc>::splice(List::iterator positon, List::iterator first, List::iterator last)
    {
        if (first != last)
            transfer(positon, first, last);
    }

    template<typename T, typename Alloc>
    void List<T, Alloc>::merge(List::list &x)
    {
        iterator first1 = begin(), last1 = end();
        iterator first2 = x.begin(), last2 = x.end();

        while (first1 != last1 && first2 != last2)
        {
            if (*first2 < *first1)  // 如果 first2 所指元素小于 first1，则要插入到 first1前面
            {
                iterator next = first2 + 1;
                transfer(first1, first2, next);
                first2 = next;
            }
            else
                ++first1;
        }
        if (first2 != last2)    // *first2 > *last1，把剩余的都插到链表1的尾部
            transfer(last1, first2, last2);
    }

    template<typename T, typename Alloc>
    void List<T, Alloc>::reverse()
    {
        // 如果链表为空或者只有一个元素，就不进行任何操作，使用size()也可以，但是比较慢
        if (node->next == node || node->next->next == node)
            return;
        iterator first = begin(), last = end();
        while (first != last)
        {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }

    // TODO swap未实现，没看懂sort的实现
    template<typename T, typename Alloc>
    void List<T, Alloc>::sort()
    {
        if (node->next == node || node->next->next == node)
            return;
        List<T, Alloc> carry;
        List<T, Alloc> counter[64];
        int fill = 0;
        while (!empty())
        {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty())
            {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill)
                ++fill;
        }

        for (int i = 1; i < fill; ++i)
            counter[i].merge(counter[i - 1]);
        swap(counter[fill - 1]);
    }

}

#endif //EASY_STL_EASY_STL_LIST_H
