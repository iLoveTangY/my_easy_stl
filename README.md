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

# 参考资料
* [《STL源码剖析》](https://book.douban.com/subject/1110934/)
* [Catch2 tutorial](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md)
* 待续...
