/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZLOG_TOKEN_HPP
#define ZI_ZLOG_TOKEN_HPP 1

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/enable_if.hpp>
#include <zi/utility/is_printable.hpp>
#include <zi/utility/address_of.hpp>
#include <zi/debug/printable_type.hpp>
#include <zi/time/now.hpp>
#include <zi/bits/cstdint.hpp>

#include <sstream>


namespace zi {
namespace zlog {

// forward declaration
class sink;

class token: non_copyable
{
private:
    bool               done_;
    std::ostringstream out_ ;

    token(): done_( false ), out_()
    {
        out_ << zi::now::usec();
    }

    void mark_done()
    {
        done_ = true;
    }

    bool is_done() const
    {
        return done_;
    }

    friend class sink;

public:

    template< class T >
    typename enable_if< is_printable< T >::value, token& >::type
    operator<< ( const T& v )
    {
        out_ << "\t" << v;
        return *this;
    }

    template< class T >
    typename disable_if< is_printable< T >::value, token& >::type
    operator<< ( const T& v )
    {
        out_ << "\t< " << debug::printable_type< T >() << " @"
             << address_of( v ) << ">";
        return *this;
    }

};

} // namespace zlog
} // namespace zi

#endif

