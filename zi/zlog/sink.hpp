/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZLOG_SINK_HPP
#define ZI_ZLOG_SINK_HPP 1

#include <zi/zlog/token.hpp>
#include <zi/concurrency/mutex.hpp>
#include <zi/concurrency/guard.hpp>

#include <ostream>
#include <deque>

namespace zi {
namespace zlog {

class sink
{
private:
    std::ostream        &out_   ;
    std::deque< token* > tokens_;
    zi::mutex            mutex_ ;

public:
    sink( std::ostream &out ): out_( out ), tokens_(), mutex_()
    {
    }

    token* get_token()
    {
        guard g( mutex_ );
        tokens_.push_back( new token );
        return tokens_.back();
    }

    void token_done( token* t )
    {
        guard g( mutex_ );

        t->mark_done();

        while ( tokens_.size() && tokens_.front()->is_done() )
        {
            out_ << tokens_.front()->out_.str() << "\n";
            delete tokens_.front();
            tokens_.pop_front();

        }
    }

};

struct token_wrapper: non_copyable
{
private:
    sink  &sink_ ;
    token &token_;

public:

    token_wrapper( sink &s ): sink_( s ), token_( *s.get_token() )
    {
    }

    ~token_wrapper()
    {
        sink_.token_done( &token_ );
    }

    token& get()
    {
        return token_;
    }

};

} // namespace zlog
} // namespace zi

#endif

