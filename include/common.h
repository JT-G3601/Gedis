#ifndef COMMON_H
#define COMMON_H

#pragma once

// C++ header file
#include <cassert>
#include <ctime>
#include <limits>
#include <memory>

namespace gedis {
// 容器长度相关   std::size_t
// 系统返回调用相关 ssize_t
// 协议传输       int32_t
// 其他           int

enum class CmdErr {
    ERR_UNKNOWN = 1,
    ERR_2BIG = 2,
    ERR_TYPE = 3,
    ERR_ARG = 4,
};

enum class SerType {
    SER_NIL = 0,
    SER_ERR = 1,
    SER_STR = 2,
    SER_INT = 3,
    SER_DBL = 4,
    SER_ARR = 5,
};

#define container_of(ptr, type, member)                    \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })
// 这是一个常用的宏，用于从包含成员的结构体中获取该成员的父结构体的指针。
//
// 具体来说，这个宏接受三个参数：
//
// ptr：要获取成员所在的指针。
// type：包含成员的结构体类型。
// member：成员的名称。
// 宏的作用是计算出包含指定成员的结构体的地址，从而得到该成员所在的父结构体的指针。
//
// 这个宏的工作原理是：
//
// 首先，使用 typeof 关键字获取成员的类型，这个类型将会是指定结构体的成员的类型。 然后，使用 offsetof 宏计算出成员在父结构体中的偏移量。
// 最后，将偏移量加到成员的地址上，就可以得到父结构体的地址。
// 这个宏使用了 GNU C 扩展，即使用了语句表达式 ({ ... })，这允许在宏中包含多个语句，并返回最后一个语句的值作为表达式的值。
//
// 总之，这个宏提供了一种方便的方式来从成员的地址获取包含该成员的父结构体的地址，从而方便地进行结构体成员的访问和处理。

inline uint64_t get_monotonic_usec() {
    timespec tv = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &tv);
    return uint64_t(tv.tv_sec) * 1000000 + tv.tv_nsec / 1000;
}

}   // namespace gedis

#include "panic.h"
#include "gtream.h"

#endif
