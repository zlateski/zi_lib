/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZLOG_LOGS_HPP
#define ZI_ZLOG_LOGS_HPP 1

#include <zi/zlog/sink.hpp>
#include <zi/utility/non_copyable.hpp>
#include <zi/utility/enable_singleton_of_this.hpp>
#include <zi/bits/shared_ptr.hpp>

#include <iostream>
#include <fstream>

namespace zi {
namespace zlog {

struct log_sinks: enable_singleton_of_this< log_sinks >
{
    sink cout_;
    sink clog_;
    sink cerr_;

    sink *info_;
    sink *warning_;
    sink *error_;
    sink *debug_;

    struct sinks_container
    {
        virtual ~sinks_container()
        {
        }
    };
    shared_ptr< sinks_container > container_;

    // loads the defaults defaults ( everything to cout )
    log_sinks():
        cout_( std::cout ),
        clog_( std::clog ),
        cerr_( std::cerr ),
        info_(    &clog_ ),
        warning_( &cerr_ ),
        error_(   &cerr_ ),
        debug_(   &clog_ )
    {
    }

    struct default_initializer
    {
    };

#define ZI_ZLOG_DEFINE_INITIALIZER( x )                 \
                                                        \
    struct all_c##x##_initializer                       \
    {                                                   \
        all_c##x##_initializer()                        \
        {                                               \
            log_sinks &logs_ = log_sinks::instance();   \
            logs_.info_    = &logs_.c##x##_;            \
            logs_.warning_ = &logs_.c##x##_;            \
            logs_.error_   = &logs_.c##x##_;            \
            logs_.debug_   = &logs_.c##x##_;            \
        }                                               \
    }

    ZI_ZLOG_DEFINE_INITIALIZER( out );
    ZI_ZLOG_DEFINE_INITIALIZER( err );
    ZI_ZLOG_DEFINE_INITIALIZER( log );

#undef ZI_ZLOG_DEFINE_INITIALIZER

    struct file_initializer
    {
        struct container: sinks_container
        {
            std::ofstream oinfo_;
            std::ofstream owarning_;
            std::ofstream oerror_;
            std::ofstream odebug_;
            sink info_;
            sink warning_;
            sink error_;
            sink debug_;

            explicit container():
                oinfo_( "./log.INFO" ),
                owarning_( "./log.WARNING" ),
                oerror_( "./log.ERROR" ),
                odebug_( "./log.DEBUG" ),
                info_( oinfo_ ),
                warning_( owarning_ ),
                error_( oerror_ ),
                debug_( odebug_ )
            {
            }
        };

        file_initializer()
        {
            shared_ptr< container > sp( new container );
            log_sinks &logs_ = log_sinks::instance();
            logs_.container_ = sp;

            logs_.info_    = &sp->info_;
            logs_.warning_ = &sp->warning_;
            logs_.error_   = &sp->error_;
            logs_.debug_   = &sp->debug_;
        }

    };

};

namespace {

static log_sinks &log_sinks_ = log_sinks::instance();

}

} // namespace zlog
} // namespace zi

#endif

