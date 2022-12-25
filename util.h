#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <stdlib.h>
#include <stdio.h>

// TODO: replace these with the more comprehensive ones from ekam

#define die(fmt, ...) do { printf(fmt, __VA_ARGS__); exit(1); } while (0)
#define dienz(x, fmt, ...) do { if (!x) { printf(fmt, __VA_ARGS__); exit(1); } } while (0);
#define max(x, y) ((x) > (y) ? (x) : (y))

void *malloc_s(size_t sz)
{
    void *ptr = malloc(sz);
    if (!ptr)
        die("MALLOC FAIL%s", "\n");
    return ptr;
}

void *calloc_s(size_t nmemb, size_t sz)
{
    void *ptr = calloc(nmemb, sz);
    if (!ptr)
        die("CALLOC FAIL%s", "\n");
    return ptr;
}

void *realloc_s(void *ptr, size_t sz)
{
    void *rptr = realloc(ptr, sz);
    if (!rptr)
        die("REALLOC FAIL%s", "\n");
    return rptr;
}

#endif
