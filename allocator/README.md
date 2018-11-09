# Allocator

&emsp; &emsp; 这是参考 SGI STL 实现的一个空间配置器，用在以后我们自己实现的各种容器中。

## 读书笔记
### 空间配置器的标准接口

​&emsp; &emsp; 根据 STL 的规范，`allocator` 必须要实现十几二十个必要接口。具体接口见书。SGI STL 使用了一个专属的、拥有次配置能力的、效率优越的特殊配置器。但是 SGI STL仍然提供了一个标准的配置器接口，只是把它做了一层封装。

### SGI 空间配置器

#### SGI 标准的空间配置器

​&emsp; &emsp; SGI STL 不使用这个标准的配置器的原因主要是效率不佳，只把 C++ 的`::operator new`和`::operator delete`做了一层薄薄的封装而已。

&emsp; &emsp; 一个问题：在`allocator`的特化版本中为什么不需要加上`template<>`？只有在书上的GCC版本中才能这样做，其他的编译器不允许这样做模板特化，都必须得显示写出`template<>`。

#### SGI 特殊的空间配置器：`std::alloc`

​&emsp; &emsp; C++内存配置和释放操作：

```c++
class Foo
{
    ...
};
Foo* pf = new Foo;	// 配置内存，然后构造对象
delete pf;	// 将对象析构，然后释放内存
```

`new`包含两阶段操作：（1）调用`::operator new`配置内存；（2）调用`Foo::Foo()`构造对象内容。`delete`也包含两阶段操作：（1）调用`Foo::~Foo()`将对象析构；（2）调用`::operator delete`释放内存。

​&emsp; &emsp; 为了精密分工，在 STL 中，内存配置操作由`alloc::allocate()`负责，内存释放操作由`alloc::deallocate()`负责；对象构造操作由`::construct()`负责，对象析构由`::destroy()`负责，**注意，这是两个定义在全局作用域中的函数**。

​&emsp; &emsp; 在`<stl_construct.h>`中的`destroy`里，SGI STL 判断元素是否有 *trivial destructor* 来对对象的析构做不同的操作：如果有 *trivial destructor* ，则说明元素的析构函数并不做什么事，那`destroy`函数也不需要做任何事；如果没有，那就需要真正的进行元素析构的操作。

&emsp; &emsp; 在`<stl_alloc.h>`中，SGI STL 以`malloc()`和`free()`完成内存的配置和释放。如果在内存分配时分配的”小型区块“过多，可能会造成内存碎片问题。为了解决这个问题，SGI 设计了双层配置器，第一级配置器直接使用`malloc()`和`free()`，第二级配置器则视情况采取不同的策略：当配置区块超过128 bytes时，视之为足够大，便调用第一级配置器，否则为了降低额外负担，便采用复杂的内存池（memory pool）方式，不再求助于第一级配置器。为了让`alloc`能够符合 STL 的规格，SGI 对 `alloc`又做了一层包装（`simple_alloc`），提供了 SGI 规定的一些接口，在后面的所有容器的实现中使用的都是封装好的`simple_alloc`。

​&emsp; &emsp; 第一级配置器可以设置类似于`set_new_handler()`的`set_malloc_handler()`。

​&emsp; &emsp; 第二级配置器每次会配置一大块内存，下次有需要时就直接从这一大块内存中取用。实际上 SGI 维护了16个 *free-lists* 各自管理大小分别为8，16，24，32，40，48，56，64，72，80，88，96，104，112，120，128 bytes的小额区块。注意，*free-lists* 的节点结构如下：

```c++
union obj
{
    union obj* free_list_link;	/* 链表下一个位置 */
    char client[1];	/* 真正的空闲内存块 */
};
```

为了节省空间，*free-lists* 使用`union`的形式。可以这么理解这个`union`：`obj`实际上是一大块内存（大小跟它所处的链表位置有关），里面只有四个字节（一个指针大小）用来存储链表下一个元素的位置，其余的部分都是空闲的；也就是说如果我们定义变量`obj* result`，假设`result`已经分配了足够的内存，那么执行`(void*)result`之后，`result`就指向我们实际需要的那个内存块了。

​&emsp; &emsp; 在`allocate()`中当 *free-list* 没有足够的空间时，会调用`refill()`来为 *free-list* 重新增加空间。为了增加效率，在`refill()`中又会调用`chunk_alloc()`来一次分配多个需要的空间（默认为20个）。在`chunk_alloc()`中，如果内存池的可用空间不足，则会调用`malloc()`来给内存池增加空间。如果`malloc()`也分配不了足够的空间，则尝试调用第一级配置器，因为第一级配置器中有内存不够时的处理机制，可能能够处理这种情况。但是一旦`malloc()`分配到了足够的空间，则`chunk_alloc()`会递归调用自己来重新尝试从内存池中分配空间给 *free-list*，同时修正`nobjs`。

## 实现细节

​&emsp; &emsp; STL 规定配置器定义于`<memory>`中，`construct`和`destroy`定义于`<stl_construct.h>`中。配置器`alloc`定义于`<stl_alloc.h>`中。而`<stl_uninitialized.h>`中定义了一些全局函数来填充（fill）或者复制（copy）大块内存数据。我们在实现自己的空间配置器时也参考这样的做法。

​&emsp; &emsp; SGI STL 的`<stl_alloc.h>`要考虑多线程状态，我们在实现时不考虑多线程。

​&emsp; &emsp; 在实现中我们经常利用`malloc`的返回值和`nullptr`比较，这是允许的，因为`nullptr`完全兼容`0`或者是`NULL`。

## 遇到的问题
Q: *free-list* 采用的union形式结构中定义的`char client[1]`的作用是什么？

A: \TODO

Q: 在内存池剩余空间不足时，计算应该分配的空间大小为什么是`size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);`？

A: 新的内存量的大小为需求量的两倍，再加上一个随着配置次数增加越来越大的附加量。

## 参考资料

* [trivial destructor](https://blog.csdn.net/wudishine/article/details/12307611)