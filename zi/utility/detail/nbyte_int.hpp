/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_DETAIL_NBYTE_INT_HPP
#define ZI_UTILITY_DETAIL_NBYTE_INT_HPP 1

#include <zi/bits/cstdint.hpp>

namespace zi {
namespace detail {

template<int I> struct nbyte_int;
template<> struct nbyte_int<1>  { typedef int8_t   type; };
template<> struct nbyte_int<2>  { typedef int16_t  type; };
template<> struct nbyte_int<4>  { typedef int32_t  type; };
template<> struct nbyte_int<8>  { typedef int64_t  type; };

template<int I> struct nbyte_uint;
template<> struct nbyte_uint<1> { typedef uint8_t  type; };
template<> struct nbyte_uint<2> { typedef uint16_t type; };
template<> struct nbyte_uint<4> { typedef uint32_t type; };
template<> struct nbyte_uint<8> { typedef uint64_t type; };

template<typename T> struct as_int
{
    typedef typename nbyte_int<sizeof(T)>::type type;
};

template<typename T> struct as_uint
{
    typedef typename nbyte_uint<sizeof(T)>::type type;
};


} // namespace detail
} // namespace zi

#endif
