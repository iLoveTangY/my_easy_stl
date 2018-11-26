# My_Easy_STL
是在读过 [《STL源码剖析》](https://book.douban.com/subject/1110934/) 一书之后利用C++11自己实现的一个简单的STL。
# 七大部分

[空间配置器](./allocator)  
[迭代器](./iterators)  
[序列式容器](./sequence_containers)  
[关联式容器](./asso_containers)  
[算法(algorithm)](./algorithm)  
[函数对象](./func_objects)  
[配接器](./adapters)  
每一部分都包含以下三小部分：  

* 读书笔记：《STL源码剖析》这本书中*我认为*值得记录的一些内容
* 实现细节：在自己实现该部分的STL库时的流程以及一些细节
* 遇到的问题：实现过程中遇到的一些问题以及解决方案
# 单元测试
使用[Catch2](https://github.com/catchorg/Catch2)这个开源的单元测试框架。感谢作者大大！
# 进度
## 空间配置器(*Allocator*)
- [x] 基本实现一级和二级空间配置器
- [ ] 根据`type_traits`实现更有效率的`uninitialized`函数
## 迭代器(*Iteartors*)
- [x] 实现`iterator_traits`和`type_traits`
## 序列容器(*Sequence Containers*)
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
- [ ] `List`
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
# 参考资料
* [《STL源码剖析》](https://book.douban.com/subject/1110934/)
* [Catch2 tutorial](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md)
* 待续...
