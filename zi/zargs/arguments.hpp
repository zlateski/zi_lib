/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZARGS_ARGUMENTS_HPP
#define ZI_ZARGS_ARGUMENTS_HPP 1

#include <zi/zargs/parser.hpp>
#include <zi/zargs/matcher.hpp>

#include <zi/utility/for_each.hpp>
#include <zi/utility/exception.hpp>
#include <zi/utility/enable_singleton_of_this.hpp>

#include <cstring>
#include <iostream>

namespace zi {
namespace zargs_ {

struct arguments: enable_singleton_of_this< arguments >
{
public:
    struct handler
    {
        virtual ~handler() { }
        virtual std::string get_name()        const = 0;
        virtual std::string get_type()        const = 0;
        virtual std::string get_default()     const = 0;
        virtual std::string get_description() const = 0;
        virtual bool parse( std::list< std::string >& ) = 0;
    };

private:
    std::vector< handler* > handlers_ ;
    std::string             file_name_;

public:
    void register_handler( handler* h )
    {
        handlers_.push_back( h );
    }

    bool parse_arguments( int &argc, char **argv, bool remove_args = true )
    {
        file_name_ = std::string( argv[0] );

        std::list< std::string > q;
        for ( int i = 1; i < argc; ++i )
        {
            q.push_back( std::string( argv[i] ) );
        }

        FOR_EACH ( it, q )
        {

            if ( *it == "-h" || *it == "--help" || *it == "-help" )
            {
                std::cout << "Usage: " << file_name_ << " [OPTIONS] ...\n\n";
                FOR_EACH ( it, handlers_ )
                {
                    std::cout.width(29);
                    std::cout << std::left
                              << ("  --" + (*it)->get_name() + "=" + (*it)->get_type());

                    std::string desc = (*it)->get_description();
                    std::vector< std::string > lines;
                    detail::explode( lines, desc, '\n' );

                    FOR_EACH ( lit, lines )
                        std::cout << (*lit) << "\n" << std::string( 29, ' ' );
                    std::cout << "default=" << (*it)->get_default() << "\n";
                    std::cout << "\n";
                }

                std::cout << "Notes:\n"
                          << "  - For non-BOOLEAN types:\n"
                          << "      -key VALUE (is equivalent to --key=VALUE)\n"
                          << "      -key=VALUE (is equivalent to --key=VALUE)\n"
                          << "  - For BOOLEAN types:\n"
                          << "      -key   (is equivalent to --key=true)\n"
                          << "      -nokey (is equivalent to --key=false)\n\n\n";

                return false;
            }
        }

        if (remove_args) {
            argc = 1;
        }

        while ( !q.empty() )
        {

            bool found = false;
            FOR_EACH ( it, handlers_ )
            {
                try
                {
                    if ( (*it)->parse( q ) )
                    {
                        found = true;
                        break;
                    }
                }
                catch ( zi::exception &e )
                {
                    std::cout << "Error: " << e.what() << "\n";
                    exit( 1 );
                }
            }

            if ( !found && remove_args)
            {
                char *arg = new char[ q.front().size() + 1 ];
                std::strcpy( arg, q.front().c_str() );
                argv[ argc++ ] = arg;
            }
            q.pop_front();
        }

        return true;
    }

};


} // namespace zargs_

inline bool parse_arguments( int &argc, char **argv, bool remove_args = true )
{
    return zi::zargs_::arguments::instance().parse_arguments( argc, argv, remove_args );
}

} // namespace zi

#endif
