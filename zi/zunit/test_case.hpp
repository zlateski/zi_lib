/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZUNIT_TEST_CASE_HPP
#define ZI_ZUNIT_TEST_CASE_HPP 1

#include <zi/zunit/config.hpp>
#include <zi/utility/non_copyable.hpp>

namespace zi {
namespace zunit {

class test_case: non_copyable
{
protected:
    int passed_;

public:

    test_case(): passed_( 0 )
    {
    }

    virtual ~test_case()
    {
    }

    virtual const int passed() const
    {
        return passed_;
    }

    virtual const char* name()   const = 0;
    virtual const char* file()   const = 0;
    virtual const int   line()   const = 0;

    virtual void run() = 0;

};

} // namespace zunit
} // namespace zi

#endif
