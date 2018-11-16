cmake_minimum_required(VERSION 3.12)
project(easy_stl)

set(CMAKE_CXX_STANDARD 11)

include_directories(test allocator iterators adapters algorithm asso_containers func_objects sequence_containers)

add_executable(easy_stl main.cpp test/test_allocator.cpp allocator/easy_stl_construct.h allocator/easy_stl_alloc.h allocator/easy_stl_alloc.cpp allocator/easy_stl_uninitialized.h iterators/easy_stl_iterator.h sequence_containers/easy_stl_vector.h test/VectorTest.h test/VectorTest.cpp)