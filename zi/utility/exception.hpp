/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_UTILITY_EXCEPTION_HPP
#define ZI_UTILITY_EXCEPTION_HPP 1

#include <exception>
#include <string>

namespace zi {
namespace exception_ {

class exception: public std::exception
{
protected:
    const std::string message_;

public:
    exception(): message_()
    {
    }

    exception( const std::string& message ):
        message_( message )
    {
    }

    virtual ~exception() throw()
    {
    }

    virtual const char* what() const throw()
    {
        if ( message_.empty() )
        {
            return "default exception";
        }
        else
        {
            return message_.c_str();
        }
    }

};

} // namespace exception_

using exception_::exception;

} // namespace zi

#define ZI_EXCEPTION_STRINIGIFY_H( what ) #what
#define ZI_EXCEPTION_STRINIGIFY( what ) ZI_EXCEPTION_STRINIGIFY_H( what )

#define ZI_THROW( message )                                             \
    throw ::zi::exception( std::string( message ) +                     \
                           " [" + __FILE__ + ": " +                     \
                           ZI_EXCEPTION_STRINIGIFY( __LINE__ ) + "]" )

#define ZI_THROW_ON( cond, message )            \
    if ( cond ) ZI_THROW( message )

#endif
