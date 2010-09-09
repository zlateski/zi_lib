/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_ASSERT_HPP
#define ZI_UTILITY_ASSERT_HPP 1
#
#include <zi/config/config.hpp>
#include <cassert>
#
#undef ZI_VERIFY
#
#if defined(NDEBUG) || defined (ZI_NO_DEBUG)
#
#  define ZI_VERIFY(what)   ((void)(what))
#  define ZI_VERIFY_T(what) ((void)(what))
#  define ZI_VERIFY_0(what) ((void)(what))
#
#else
#
#  define ZI_VERIFY(what)   assert(what)
#  define ZI_VERIFY_T(what) assert(what)
#  define ZI_VERIFY_0(what) assert(!(what))
#
#endif
#
#endif
