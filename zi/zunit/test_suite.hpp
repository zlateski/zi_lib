/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZUNIT_TEST_SUITE_HPP
#define ZI_ZUNIT_TEST_SUITE_HPP 1

#include <zi/zunit/config.hpp>
#include <zi/zunit/tags.hpp>
#include <zi/zunit/test_case.hpp>
#include <zi/zunit/exception.hpp>
#include <zi/ansi_term/term_ostream.hpp>

#include <zi/bits/ref.hpp>
#include <zi/bits/unordered_set.hpp>

#include <zi/time/timer.hpp>

#include <zi/utility/non_copyable.hpp>
#include <zi/utility/for_each.hpp>

#include <set>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstddef>

namespace zi {
namespace zunit {

class test_suite: non_copyable
{
private:

    std::string                                   name_ ;
    std::vector< reference_wrapper< test_case > > tests_;
    unordered_set< std::string >                  files_;

public:

    test_suite( const char* name ):
        name_(name),
        tests_(),
        files_()
    {
    }

    std::size_t add_test( test_case &test )
    {

        files_.insert( test.file() );
        tests_.push_back( ref( test ) );

        return tests_.size();
    }

    int run_all()
    {

        bool passing = true;
        std::size_t passed_tests   = 0;
        std::size_t passed_asserts = 0;

        tout << name_ << " ( "
             << files_.size() << " files, "
             << tests_.size() << " tests )\n" << tos::reset ;
        tout << std::setw( 46 ) << std::setfill( '-' ) << ""
             << "+--------+------------------------------------\n" << std::setfill( ' ' );

        timer               timer_;
        timer::tv< double > tv_   ;

        timer_.reset();

        tout << std::fixed << std::setprecision( 3 );

        FOR_EACH ( it, tests_ )
        {
            tout << "Test: " << std::setw(40) << std::left
                 << it->get().name() << tos::flush;

            try
            {
                it->get().run();
            }
            catch ( exception &e )
            {
                tout << "|" << tos::red << tos::bold << " failed " << tos::reset
                     << "|\n\n" << e.what() << "\n" << tos::flush;
                passing = false;
                break;
            }

            timer_.lap( tv_ );

            ++passed_tests;
            passed_asserts += it->get().passed();

            tout << "|" << tos::green << tos::bold << " passed " << tos::reset
                 << "| w: " << tv_.wall << " p: " << tv_.process << " "
                 << it->get().passed() << " asserts" << '\n' << tos::flush;
        }

        tout << std::setw( 46 ) << std::setfill( '-' ) << ""
             << "+--------+------------------------------------\n" << std::setfill( ' ' )
             << std::setw( 45 ) << std::left
             << "Suite total" << " |";


        if ( passing )
        {
            timer_.elpased( tv_ );
            tout << tos::green << tos::bold << " PASSED " << tos::reset
                 << "| w: " << tv_.wall << " p: " << tv_.process << " "
                 << passed_asserts << " asserts" << '\n';
        }
        else
        {
            tout << tos::red << tos::bold << " FAILED " << tos::reset << "|\n";
        }

        tout << std::setw( 46 ) << std::setfill( '-' ) << "" << std::setfill( ' ' )
             << "+--------+------------------------------------\n\n" << tos::flush;

        return passing ? static_cast< int >( passed_tests ) : -1;

    }

    std::string name() const
    {
        return name_;
    }

};

template< class SuiteTag > struct suite_name;

template<> struct suite_name< default_suite_tag >
{
  static const char* name() { return "No Suite"; }
};

} // namespace zunit
} // namespace zi

#endif
