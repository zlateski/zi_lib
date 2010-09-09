/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ZUNIT_TAGS_HPP
#define ZI_ZUNIT_TAGS_HPP 1

namespace zi {
namespace zunit {

namespace {
struct this_file_tag;
}

struct default_suite_tag;

template< class FileTag = void > struct suite_tag
{
    typedef default_suite_tag tag ;
};

} // namespace zunit
} // namespace zi

#endif
