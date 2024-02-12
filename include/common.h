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

inline uint64_t get_monotonic_usec() {
    timespec tv = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &tv);
    return uint64_t(tv.tv_sec) * 1000000 + tv.tv_nsec / 1000;
}

}   // namespace gedis

#include "panic.h"
#include "gtream.h"

#endif
