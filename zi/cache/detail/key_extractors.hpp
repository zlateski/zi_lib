/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_CACHE_DETAIL_KEY_EXTRACTORS_HPP
#define ZI_CACHE_DETAIL_KEY_EXTRACTORS_HPP 1

#include <zi/cache/config.hpp>
#include <zi/cache/detail/if.hpp>
#include <zi/utility/static_if.hpp>
#include <zi/bits/type_traits.hpp>

namespace zi {
namespace cache {
namespace detail {

/*
template< class Class >
struct extract_type: if_t< is_reference< Class >,
                           extract_type< typename remove_reference< Class >::type >
                           if_t< is_const< Class >,
                                 extract_type< typename remove_const< Class >::type >,
                                 if_t<


    is_fundamental< WrappedClass >, WrappedClass >
{
    typedef Class type;
}
*/
// no voids allowed!


template< class Class >
struct has_key_t_extractor
{
    typedef typename Class::key_t key_t;
};

namespace has_key_t_checker_ {

template< class Type > static char tester( typename Type::key_t* );
template< class Type > static int  tester( ... );

} // namespace has_key_t_extractor_checker

template< class Class >
struct has_key_t_checker
{
    static const bool value =
        ( sizeof( has_key_t_checker_::tester<
                  typename remove_pointer<
                  typename remove_all_extents<
                  typename remove_cv<
                  typename remove_reference<
                  Class
                  >::type
                  >::type
                  >::type
                  >::type
                  >( 0 ) ) == 1 );
};




} // namespace detail
} // namespace cache
} // namespace zi

#endif
