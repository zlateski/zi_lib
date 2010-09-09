/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_STATIC_OR_HPP
#define ZI_UTILITY_STATIC_OR_HPP 1

namespace zi {


template< bool V1, bool V2,
          bool V3 = false, bool V4 = false, bool V5 = false, bool V6 = false,
          bool V7 = false, bool V8 = false, bool V9 = false, bool VA = false >
struct static_or;

template< bool V1, bool V2, bool V3, bool V4, bool V5,
          bool V6, bool V7, bool V8, bool V9, bool VA >
struct static_or
{
    static const bool value = true;
};

template<>
struct static_or< false, false, false, false, false,
                  false, false, false, false, false >
{
    static const bool value = false;
};


template< bool V1, bool V2,
          bool V3 = false, bool V4 = false, bool V5 = false, bool V6 = false,
          bool V7 = false, bool V8 = false, bool V9 = false, bool VA = false >
struct or_;

template< bool V1, bool V2, bool V3, bool V4, bool V5,
          bool V6, bool V7, bool V8, bool V9, bool VA >
struct or_
{
    static const bool value = true;
};

template<>
struct or_< false, false, false, false, false,
            false, false, false, false, false >
{
    static const bool value = false;
};


} // namespace zi

#endif
