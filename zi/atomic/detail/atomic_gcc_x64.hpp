/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ATOMIC_DETAIL_ATOMIC_GCC_X64_HPP
#define ZI_ATOMIC_DETAIL_ATOMIC_GCC_X64_HPP 1

#ifndef ZI_ATOMIC_ATOMIC_HPP_INCLUDING
#  error "don't include this file directly, use atomic.hpp"
#endif

typedef int64_t atomic_word;

inline atomic_word compare_swap( volatile atomic_word *ptr, atomic_word asgn, atomic_word cmp )
{
    atomic_word old = cmp;
    asm volatile ( "lock\n\t"
                   "cmpxchgq %3, %1":
                   "=a"(old), "=m"(*(ptr)):
                   "0" (old), "r" (asgn):
                   "memory", "cc"
        );

    return old;
}

inline atomic_word add_swap( volatile atomic_word *ptr, atomic_word val)
{
    atomic_word result;
    asm volatile( "lock\n\t"
                  "xaddq %1, %0":
                  "+m"(*ptr), "=r"(result):
                  "1"(val):
                  "memory", "cc"
        );

    return result;
}


inline void increment( volatile atomic_word *ptr )
{
    asm volatile( "lock\n\t"
                  "incq %0":
                  "=m"(*ptr):
                  "m" (*ptr):
                  "cc"
        );
}

inline void decrement( volatile atomic_word *ptr )
{
    asm volatile( "lock\n\t"
                  "decq %0":
                  "=m"(*ptr):
                  "m" (*ptr):
                  "cc"
        );
}

inline atomic_word test_increment_swap( volatile atomic_word *ptr )
{
    atomic_word res, tmp;
    asm volatile( "movq %0, %%rax\n\t"
                  "0:\n\t"
                  "test %%rax, %%rax\n\t"
                  "je 1f\n\t"
                  "movq %%rax, %2\n\t"
                  "incq %2\n\t"
                  "lock\n\t"
                  "cmpxchgq %2, %0\n\t"
                  "jne 0b\n\t"
                  "1:":
                  "=m"(*ptr), "=&a"(res), "=&r"(tmp):
                  "m" (*ptr):
                  "cc"
        );

    return res;
}

inline atomic_word increment_swap( volatile atomic_word *ptr )
{
    return add_swap( ptr, 1 );
}

inline atomic_word decrement_swap( volatile atomic_word *ptr )
{
    return add_swap( ptr, -1 );
}

inline void write(atomic_word *ptr, atomic_word val )
{
    *ptr = val;
}

inline atomic_word read(atomic_word *ptr )
{
    return *ptr;
}

#endif
