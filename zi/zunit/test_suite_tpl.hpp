/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZUNIT_TEST_SUITE_TPL_HPP
#define ZI_ZUNIT_TEST_SUITE_TPL_HPP 1

#include <zi/zunit/config.hpp>
#include <zi/zunit/test_suite.hpp>
#include <zi/zunit/registry.hpp>

#include <zi/utility/enable_singleton_of_this.hpp>

namespace zi {
namespace zunit {

template< class Tag > struct test_suite_tpl:
    test_suite,
    enable_singleton_of_this< test_suite_tpl< Tag > >
{
    test_suite_tpl():
        test_suite( suite_name< Tag >::name() )
    {
        registry::instance().add_suite( *this );
    }
};

} // namespace zunit
} // namespace zi

#endif
