#ifndef PANIC_H
#define PANIC_H

#pragma once

#include <cerrno>
#include <cstdio>
#include <cstdlib>

inline void ERR(const char *file, const char *func, int line, const char *msg) {
    (void)std::fprintf(stderr, "err: %s:%d %s: %s\n", file, line, func, msg);
    std::abort();
}

inline void MSG(const char *file, const char *func, int line, const char *msg) {
    (void)std::fprintf(stderr, "err: %s:%d %s: %s\n", file, line, func, msg);
}

#define err(msg) ERR(__FILE__, __func__, __LINE__, msg)
#define msg(msg) MSG(__FILE__, __func__, __LINE__, msg)
#endif
