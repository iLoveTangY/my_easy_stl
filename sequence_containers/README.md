# Sequence Containers
&emsp; &emsp; 所谓序列式容器，其中的元素都可序（*ordered*），但未必有序（*sorted*）。C++本身提供了一个序列式容器`array`，
STL 另外提供了`vector`、`list`、`deque`、`stack`、`queue`、`priority_queue`等，其中`stack`和`queue`是容器适配器。
## 读书笔记
### `vector`
&emsp; &emsp; `vector`的实现关键在于其对大小的控制以及重新配置时的数据移动效率。一旦`vector`旧有空间满载，如果客户端每新增一个元素，
`vector`内部知识扩充一个元素的空间，实为不智，因为扩充空间（不论多大），是“配置新空间 -> 数据移动 -> 释放旧空间”的大工程，时间成本很高。
另外，对`vector`的任何操作，一旦引起空间重新配置，指向原`vector`的所有迭代器就都失效了。
## 实现细节
### `vector`
&emsp; &emsp; `vector`中注意`copy()`和`copy_backward()`的使用。`vector`支持的所有操作如下：
名称|描述
:-:|:-:
`begin()` |返回容器元素开始位置的迭代器
`end()`|返回容器元素结束位置的迭代器
`size()`|返回容器总元素的个数
`capacity()`|容器容量
`empty()`|容器是否为空
`operator[]`|下标运算符
`vector()`|构造函数
`vector(n, value)`|在容器中加入`n`个值为`value`的元素
`vector(n)`|在容器中加入`n`个元素，利用该值的默认构造函数构造
`front()`|返回容器中第一个元素
`back()`|返回容器中最后一个元素
`push_back(x)`|在容器的最后一个元素之后插入元素`x`
`pop_back()`|弹出容器中最后一个元素
`erase(position)`|删除指定位置上的元素
`resize(new_size, x)`|将容器的`size`设置为`new_size`，多出来的元素用`x`填充
`resize(new_size)`|将容器的`size`设置为`new_size`，多出来的元素用默认构造函数构造的值填充
`clear()`|清除所有元素

## 遇到的问题