/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZUNIT_REGISTRY_HPP
#define ZI_ZUNIT_REGISTRY_HPP 1

#include <zi/zunit/config.hpp>
#include <zi/zunit/test_case.hpp>
#include <zi/zunit/test_suite.hpp>
#include <zi/zunit/tags.hpp>

#include <zi/time/timer.hpp>

#include <zi/utility/enable_singleton_of_this.hpp>
#include <zi/utility/non_copyable.hpp>
#include <zi/utility/for_each.hpp>

#include <zi/bits/ref.hpp>

#include <list>
#include <cstddef>

namespace zi {
namespace zunit {

class registry: public enable_singleton_of_this< registry >
{
private:

    std::list< reference_wrapper< test_suite > > suites_;

public:

    registry(): suites_() {} /// add_suite<suite_tag<void>::type >(); }

    void add_suite( test_suite& suite )
    {
        suites_.push_front( ref( suite ) );
    }

    void run_all()
    {
        std::size_t total_tests_passed  = 0;
        std::size_t total_suites_passed = 0;

        FOR_EACH( it, suites_ )
        {
            std::size_t tests_passed = it->get().run_all();

            if ( tests_passed >= 0 )
            {
                total_tests_passed += tests_passed;
                ++total_suites_passed;
            }
        }
    }

};

} // namespace zunit
} // namespace zi

#endif
