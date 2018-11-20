# Sequence Containers
&emsp; &emsp; 所谓序列式容器，其中的元素都可序（*ordered*），但未必有序（*sorted*）。C++本身提供了一个序列式容器`array`，
STL 另外提供了`vector`、`list`、`deque`、`stack`、`queue`、`priority_queue`等，其中`stack`和`queue`是容器适配器。
## 读书笔记
### `vector`
&emsp; &emsp; `vector`的实现关键在于其对大小的控制以及重新配置时的数据移动效率。一旦`vector`旧有空间满载，如果客户端每新增一个元素，
`vector`内部知识扩充一个元素的空间，实为不智，因为扩充空间（不论多大），是“配置新空间 -> 数据移动 -> 释放旧空间”的大工程，时间成本很高。
另外，对`vector`的任何操作，一旦引起空间重新配置，指向原`vector`的所有迭代器就都失效了。
### `list`
&emsp; &emsp; `list`每次插入或者删除一个元素，就配置或者删除一个元素空间。`list`不能像`vector`一样再使用普通指针作为迭代器，因为其节点不保证在存储空间中连续存在。
由于STL `list`是一个双向链表，迭代器必须具备前移、后移的能力，所以`list`提供的 **Bidirectional Iterators**。

&emsp; &emsp; 为了符合 STL 的“前闭后开”区间的规范，SGI STL 在`list`的最后一个节点之后加上了一个空白节点。
在实现`list`时，只要在`list`中保存一个指向这个空白节点的指针即可表示整个双向链表。`list`内部有一个`transfer()`操作，实质上就是将一段链表插到另一个链表中指定的位置之前。
## 实现细节
### `Vector`
&emsp; &emsp; `Vector`中注意`copy()`和`copy_backward()`的使用。`Vector`支持的所有操作如下：

名称|描述
:-:|:-:
`begin()` |返回容器元素开始位置的迭代器
`end()`|返回容器元素结束位置的迭代器
`size()`|返回容器总元素的个数
`capacity()`|容器容量
`empty()`|容器是否为空
`operator[]`|下标运算符
`Vector()`|默认构造函数
`Vector(n, value)`|构造函数，在容器中加入`n`个值为`value`的元素
`Vector(n)`|构造函数，在容器中加入`n`个元素，利用该值的默认构造函数构造
`insert(position, n, x)`|从`position`处开始插入`n`个值为`x`的元素
`insert(position, x)`|在`position`处插入元素`x`
`front()`|返回容器中第一个元素
`back()`|返回容器中最后一个元素
`push_back(x)`|在容器的最后一个元素之后插入元素`x`
`pop_back()`|弹出容器中最后一个元素
`erase(position)`|删除指定位置上的元素
`erase(first, last)`|删除从`[first, last)`的所有元素
`resize(new_size, x)`|将容器的`size`设置为`new_size`，多出来的元素用`x`填充
`resize(new_size)`|将容器的`size`设置为`new_size`，多出来的元素用默认构造函数构造的值填充
`clear()`|清除所有元素
### `list`
&emsp; &emsp; `list`的`size`是通过计算两个迭代器之间的距离算出来的。`list`支持的操作如下：

名称|描述
:-:|:-:
`List()`|默认构造函数
`begin()`|返回指向链表中第一个元素的迭代器
`end()`|返回指向链表中最后一个元素之后的位置的迭代器
`empty()`|链表是否为空
`size()`|返回链表元素个数
`front()`|返回链表中第一个元素
`back()`|返回链表中最后一个元素
`push_front(x)`|在链表的头部插入元素`x`
`push_back(x)`|在链表的尾部插入元素`x`
`erase(position)`|删除`position`处的元素
`insert(position, x)`|在`position`处插入元素`x`
`pop_front()`|删除链表头部元素
`pop_back()`|删除链表最后一个元素
`clear()`|删掉链表所有元素并释放空间
`remove(value)`|移除链表中所有值为`value`的元素
`unique()`|删除链表中的连续重复元素，只保留一个
`splice(position, x)`|将链表`x`插入当前链表的`position`之前
`splice(position, x, i)`|将`i`所指元素插入到`position`所指位置之前
`splice(psotion, x, first, last)`|将链表中`[first, last)`所有元素插到`position`所指位置之前
`merge(x)`|将链表`x`归并到当前链表，要求两个链表都有序
`reverse()`|逆置链表
`sort()`|链表排序

## 遇到的问题
Q: 书上的代码在`Vector`的尾部插入元素时会有 **bug**？

A: 在尾部插入时单独处理。

Q: `list`的后缀`operator++()`为什么返回`const`？

A: 返回常量是为了避免写出`(x++)++`这样的代码，参考[这里](https://stackoverflow.com/questions/52871026/overloaded-operator-returns-a-non-const-and-clang-tidy-complains)。