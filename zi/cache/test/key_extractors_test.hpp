/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#include <zi/cache/detail/key_extractors.hpp>
#include <zi/zunit/zunit.hpp>

ZiSUITE( ZiLib_Cache_Tests );

namespace cache_tests {

struct has_key_t
{
    typedef int key_t;
};

struct no_key_t
{
};


} // namespace cache_tests

ZiTEST( Test_KeyExtractors )
{

    using zi::cache::detail::has_key_t_checker;

    EXPECT_TRUE ( has_key_t_checker< cache_tests::has_key_t >::value );
    EXPECT_FALSE( has_key_t_checker< cache_tests::no_key_t  >::value );


}
