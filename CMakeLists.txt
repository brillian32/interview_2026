cmake_minimum_required(VERSION 4.0)
project(Interview)

set(CMAKE_CXX_STANDARD 17)

find_package(doctest REQUIRED)


if (MSVC)
    # 对于 Visual Studio 编译器 执行源文件编译，和执行时为utf-8
    #/utf-8 选项 等价于 /source-charset:utf-8 + execution-charset:utf-8。
    message("Using MSVC compiler")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
endif()

add_executable(Interview
        src/ThreadPool.cpp
        src/list.cpp
        src/main.cpp
)


target_link_libraries(
        Interview
        PRIVATE
        doctest::doctest
)
