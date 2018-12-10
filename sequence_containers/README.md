# Sequence Containers
&emsp; &emsp; 所谓序列式容器，其中的元素都可序（*ordered*），但未必有序（*sorted*）。C++本身提供了一个序列式容器`array`，
STL 另外提供了`vector`、`list`、`deque`、`stack`、`queue`、`priority_queue`等，其中`stack`和`queue`是容器适配器。
## 进度
### 基本接口(功能)
- [x] `Vector`
- [x] `List`
- [ ] `Deque`
- [ ] `Stack`
- [ ] `Queue`
- [ ] `Heap`
- [ ] `Priority_Queue`
- [ ] `Forward_List`
### 基本单元测试
- [x] `Vector`
- [x] `List`
- [ ] `Deque`
- [ ] `Stack`
- [ ] `Queue`
- [ ] `Heap`
- [ ] `Priority_Queue`
- [ ] `Forward_List`
### 增加符合*C++ 11*规范的接口
- [ ] `Vector`
- [ ] `List`
- [ ] `Deque`
- [ ] `Stack`
- [ ] `Queue`
- [ ] `Heap`
- [ ] `Priority_Queue`
- [ ] `Forward_List`
### *C++ 11*接口的单元测试
- [ ] `Vector`
- [ ] `List`
- [ ] `Deque`
- [ ] `Stack`
- [ ] `Queue`
- [ ] `Heap`
- [ ] `Priority_Queue`
- [ ] `Forward_List`
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
### `deque`
&emsp; &emsp; `deque`是双向开口的连续线性空间，可以在头尾两端分别做元素的插入和删除操作。`deque`和`vector`的最大差异在于
`deque`允许在常数时间内对**头端**元素进行插入操作，并且`deque`没有所谓容量的概念，因为它是以**分段连续**空间组合而成。
对`deque`进行的排序操作，为了最高效率，可将`deque`完整复制到一个`vector`，将`vector`排序后，再复制回`deque`。

&emsp; &emsp; `deque`采用一块所谓的*map*（不是STL里面的`map`容器）作为主控制中心。这个*map*是一小块连续空间，其中每个元素（称为一个节点，*node*）都是指针，
指向另一端较大的线性连续空间，称为缓冲区。缓冲区才是`deque`的存储空间主体。
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

### `deque`
&emsp; &emsp; 仔细观察，可以发现`deque`的控制中心`map`实际上是一个`T**`（实际上`map`是一个指针数组），也就是说它是一个指针，所指之物也是一个指针，
指向一块型别为`T`的空间（实际上是数组）。

&emsp; &emsp; `deque`的迭代器的实现中有如下几个关键函数：`set_node(map_pointer new_node)`是一个辅助函数，在`deque`的迭代器前进或者后退时来辅助修改当前迭代器的`node`指针（`node`指针前进或者后退）。
`operator-(const self& x)`用来计算两个迭代器之间的距离，要求`x`所指元素在`*this`之前，实现时先计算两个迭代器`node`的差值，再加上`x.last-x.cur`和`*this.cur-*this.first`即可。
再就是在实现`operator++`、`operator--`、`operator+=`等运算符时要注意利用`set_node`设置迭代器指向新的控制中心节点。在实现`operator+=`时，用`offset`来表示从当前迭代器所在的缓冲区(`node`)开始到要跳转的位置的偏移量。
用`node_offset`来表示`node`的偏移量。但是当`offset`为负数时应该先减一，表示从`cur`跳到下一个节点的尾部，然后再计算`node_offset`。

&emsp; &emsp; `deque`里面定义了两个专属空间配置器，一个用来配置元素，一个用来配置控制中心(map)。`deque`里面的属性主要有三个：`start`、`finish`和`map`。`map`指向控制中心，`start`为一个迭代器，指向第一个节点，
`end`也是一个迭代器，指向最后一个节点之后的位置。注意，`deque`的`clear()`函数清除这个缓冲区，但是由于`deque`的最初状态保有一个缓冲区，因此，`clear()`完成之后回复初始状态，也一样要保留一个缓冲区。
在`deque`的`erase()`函数中，为了提高速度，当清除点之前的元素个数较少时我们将清楚点之前的所有元素都往后移一位，覆盖掉清除点处的元素；如果清除点之后的元素个数较少，
那我们就把清除点之后的所有元素往前移，覆盖掉清除点处的元素。清除区间元素时也是一样。`insert()`函数里也是如此，如果插入点之前的元素比较少，就把插入点之前的元素往前往前移一位；
否则就把插入点之后的元素往后移一位（为了效率无所不用其极...）。`deque`的接口如下：

名称|描述
:-:|:-:
`begin()`|指向第一个元素的迭代器
`end()`|指向最后一个元素之后位置的迭代器
`operator[]`|容器的下标操作符，调用迭代器的下标操作符
`front()`|返回容器第一个元素
`back()`|返回容器最后一个元素
`size()`|返回容器元素个数
`max_size()`|最大元素个数
`empty()`|是否为空
`push_back()`|在尾部插入一个元素
`push_front()`|在头部插入一个元素
`pop_back()`|弹出尾部元素
`pop_front()`|弹出头部元素
`clear()`|清空`deque`
`erase()`|删除指定位置的元素
`insert()`|在指定位置插入元素

## 遇到的问题
Q: 书上的代码在`Vector`的尾部插入元素时会有 **bug**？

A: 在尾部插入时单独处理。

Q: `list`的后缀`operator++()`为什么返回`const`？

A: 返回常量是为了避免写出`(x++)++`这样的代码，参考[这里](https://stackoverflow.com/questions/52871026/overloaded-operator-returns-a-non-const-and-clang-tidy-complains)。

Q: `deque`的`max_size()`函数实现为`size_type(-1)`是否表示无符号数的最大值？

A: \TODO