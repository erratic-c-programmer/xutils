#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <stdlib.h>
#include <stdio.h>

// TODO: replace these with the more comprehensive ones from ekam

#define die(fmt, ...) do { printf(fmt, __VA_ARGS__); exit(1); } while (0)
#define dienz(x, fmt, ...) do { if (!x) { printf(fmt, __VA_ARGS__); exit(1); } } while (0);
#define max(x, y) ((x) > (y) ? (x) : (y))

/* CHECKING MEMORY MANAGEMENT */

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

/* MISC */

int log2_floor(unsigned long i)
{
    return i ? __builtin_clzl(1) - __builtin_clzl(i) : -1;
}

/* SEGMENT TREE */

// TODO: generalise
// Currently this is a point-add range-max segtree, no laziness
#define ST_V_T size_t
struct Segtree {
    long long start, mid, end;
    ST_V_T val;
    struct Segtree *l_child, *r_child;
};

void segtree_init(struct Segtree **stp, long long s, long long e)
{
    *stp = calloc_s(1, sizeof(**stp));
    struct Segtree *st = *stp;
    st->start = s;
    st->end = e;
    st->mid = (s + e) >> 1;
    st->val = 0;
    if (s != e) {
        segtree_init(&st->l_child, s, st->mid);
        segtree_init(&st->r_child, st->mid + 1, e);
    }
}

void segtree_update(struct Segtree *st, long long pt, ST_V_T x)
{
    if (st->start == st->end)
        st->val += x;
    else if (pt <= st->mid)
        segtree_update(st->l_child, pt, x);
    else
        segtree_update(st->r_child, pt, x);

    if (st->start != st->end)
        st->val = max(st->l_child->val, st->r_child->val);
}

// inclusive range
ST_V_T segtree_query(struct Segtree *st, long long s, long long e)
{
    if (st->start == st->end || (st->start == s && st->end == e))
        return st->val;
    else if (e <= st->mid)
        return segtree_query(st->l_child, s, e);
    else if (s > st->mid)
        return segtree_query(st->r_child, s, e);
    else
        return max(segtree_query(st->l_child, s, st->mid), segtree_query(st->r_child, st->mid + 1, e));
}

void segtree_free(struct Segtree *st)
{
    if (st->start != st->end) {
        segtree_free(st->l_child);
        segtree_free(st->r_child);
    }
    free(st);
}

#endif
