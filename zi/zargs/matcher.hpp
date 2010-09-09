/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZARGS_MATCHER_HPP
#define ZI_ZARGS_MATCHER_HPP 1

#include <string>
#include <list>
#include <cstddef>

namespace zi {
namespace zargs_ {

template< class Type > struct matcher_base
{
    virtual bool match( const std::string &name, std::list< std::string > &q ) const
    {
        if ( q.empty() )
        {
            return false;
        }

        std::string s = q.front();

        if ( s == ("-" + name) || s == ("--" + name) )
        {
            q.pop_front();
            return true;
        }

        if ( detail::begins_with( "-"  + name + "=", s ) ||
             detail::begins_with( "--" + name + "=", s ) )
        {
            std::string all    = q.front();
            std::size_t eq_pos = all.find_first_of( '=', 0 );

            q.pop_front();
            q.push_front(detail::strip_quotes
                         (all.substr( eq_pos + 1, all.size() - eq_pos - 1) ) );
            return true;
        }

        return false;
    }
};

template< class Type > struct matcher: matcher_base< Type >
{
};

template<> struct matcher< bool >: matcher_base< bool >
{
    bool match( const std::string &name, std::list<std::string> &q ) const
    {
        std::string s = q.front();

        if ( s == ("-" + name) || s == ("--" + name) )
        {
            q.pop_front();
            q.push_front( "1" );
            return true;
        }

        if ( s == ("-no" + name) || s == ("--no" + name) )
        {
            q.pop_front();
            q.push_front("0");
            return true;
        }

        return matcher_base< bool >::match( name, q );
    }
};


} // namespace zargs_
} // namespace zi

#endif
