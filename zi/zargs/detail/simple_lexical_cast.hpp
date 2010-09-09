/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZARGS_DETAIL_SIMPLE_LEXICAL_CAST_HPP
#define ZI_ZARGS_DETAIL_SIMPLE_LEXICAL_CAST_HPP 1

#include <typeinfo>
#include <exception>
#include <sstream>
#include <string>

namespace zi {
namespace zargs_ {

class bad_lexical_cast: public std::bad_cast
{
private:
    const std::type_info *source_type_;
    const std::type_info *target_type_;

public:
    bad_lexical_cast():
        source_type_( &typeid( void )),
        target_type_( &typeid( void ))
    {
    }

    bad_lexical_cast( const std::type_info &source_type,
                      const std::type_info &target_type ):
        source_type_( &source_type),
        target_type_( &target_type)
    {
    }

    const std::type_info &source_type() const
    {
        return *source_type_;
    }

    const std::type_info &target_type() const
    {
        return *target_type_;
    }

    virtual const char *what() const throw()
    {
        return "bad lexical cast: "
            "source type value could not be interpreted as target";
    }

    virtual ~bad_lexical_cast() throw()
    {
    }

};

namespace detail {

template< class Target, class Source >
struct lexical_caster
{
    static inline Target cast_it( const Source& source )
    {
        Target ret;
        std::stringstream ss;

        if ( ss << source && ss >> ret )
        {
            if ( ss.eof() )
            {
                return ret;
            }
        }

        throw bad_lexical_cast( typeid( Source ), typeid( Target ) );
    }
};


template< class Source >
struct lexical_caster< std::string, Source >
{
    static inline std::string cast_it( const Source& source )
    {
        std::ostringstream oss;

        if ( oss << source )
        {
            return oss.str();
        }

        throw bad_lexical_cast( typeid( Source ), typeid( std::string ) );
    }
};


} // namespace detail

template< class Target, class Source >
Target lexical_cast( const Source& source )
{
    return detail::lexical_caster< Target, Source >::cast_it( source );
}

} // namespace zargs_
} // namespace zi

#endif
