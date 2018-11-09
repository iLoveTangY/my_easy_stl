# Iterators
&emsp; &emsp; 迭代器(Iterators)是一种抽象的设计概念。在设计模式中Iterator设计模式定义如下：提供一种方法，使之能够依序访问某个聚合物(容器)所含的各个元素，而又无需暴露该聚合物的内部表述方式。
## 读书笔记
### 迭代器设计思维
&emsp; &emsp; STL中心思想在于：将数据容器和算法分开，彼此独立设计，最后再以一帖粘着剂将它们撮合在一起。而迭代器就是这个粘着剂。

&emsp; &emsp; 迭代器实际上是一种智能指针，而指针最重要的也就是内容提领（*dereference*）和成员访问（*member access*）。因此迭代器最重要的编程工作就是对`operator*`和`operator->`进行重载。

&emsp; &emsp; 由于在算法中经常会用迭代器所指的类型来定义变量，因此我们得想办法得到迭代器所指的类型。比如，我们想实现一个函数`func`，其中需要利用迭代器所指
类型来定义变量，那我们可以利用模板参数推导来获取迭代器所指的类型：
```c++
template <typename I, typename T>
void func_impl(I iter, T t)
{
    T tmp;  // 这里我们得到了迭代器所指的型别
    ...
}

template <typename I>
inline void func(I iter)
{
    func_impl(iter, *iter); // 通过做一次模板参数推导得到迭代器所指的型别
}
```
但是并不是所有的情况都可以利用上面的模板参数推导来取得想要的型别，因此需要更全面的解法。比如，我们可以在迭代器的定义中
声明内嵌型别：
```c++
template <typename T>
struct MyIter
{
    typedef T value_type;
};
```
在这种情况下实现上面提到的`func()`就很简单了：
```c++
template <typename I>
inline void func(I iter)
{
    typename I::value_type tmp; // 必须得有typename
    ...
}
```
&emsp; &emsp; 但是，这里面还有个隐晦的问题：并不是所有的迭代器都是 *class type*，原生指针就不是！如果不是 *class type*
就没法为它定义内嵌型别。但 STL 必须接受原生指针作为一种迭代器，所以这样还不够。接下来，我们就需要模板偏特化（*template partial specialization*）了。
### 模板偏特化的意义
&emsp; &emsp; 模板偏特化的意义：**提供另一份模板定义式，其本身仍为模板，或者针对（任何）模板参数更进一步的条件限制设计出来的一个特化版本。** 也就是说，如果我们有下面一个模板：
```c++
template <typename T>
class C {...};
```
那我们可以对它进行如下的偏特化：
```c++
template <typename T>
class C<T*> { ... };   // 这个特化版本仅适用于“T 为原生指针”的情况，“T 为原生指针”即为对T的进一步的条件限制。
```
这样，我们就能定义如下的一个`class`来提取迭代器所指的类型：
```c++
template <typename T>
struct iterator_traits
{
    typedef typename I::value_type  value_type;
};

template <typename T>
struct iterator_traits<T*>  // 偏特化版本，提取原生指针的value_type
{
    typedef T value_type;
};

template <typename T>
struct iterator_traits<const T*> // 偏特化版本，提取pointer-to-const的value_type
{
    typedef T value_type;
}
```
这样我们就可以按照如下的方式来实现上面提到的`func()`：
```c++
template <class I>
typename iterator_traits<I>::value_type
func(I ite)
{ return *ite; }
```
#### 迭代器的五种相应型别
* `value_type`：如上节所述；
* `difference type`：`difference type`用来表示两个迭代器之间的举例，因此也可以用来表示一个容器的最大容量，因为对于连续空间的容器而言
头尾之间的距离就是最大容量。对于指针类型，我们用内建的`ptrdiff_t`来表示`difference`；
* `reference type`
* `pointer type`
* `iterator_category`
## 实现细节
## 遇到的问题