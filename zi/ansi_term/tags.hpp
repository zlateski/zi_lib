/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ANSI_TERM_TAGS_HPP
#define ZI_ANSI_TERM_TAGS_HPP 1

#include <zi/config/config.hpp>

namespace zi {
namespace tos {
namespace detail {

struct fg_color_tag          ;
struct bg_color_tag          ;
struct color_pair_tag        ;
struct weight_tag            ;
struct decoration_tag        ;

struct add_decoration_tag    ;
struct remove_decoration_tag ;

struct push_flags_tag {};
struct pop_flags_tag  {};
struct flush_tag      {};
struct reset_tag      {};

struct move_to_tag     ;
struct move_forward_tag;

} // namespace detail

namespace {
static detail::push_flags_tag push_flags;
static detail::pop_flags_tag  pop_flags ;
static detail::flush_tag      flush     ;
static detail::reset_tag      reset     ;
}

namespace detail {

inline void kill_warnings()
{
    (void)push_flags;
    (void)pop_flags;
    (void)flush;
    (void)reset;
}

} // namespace detail

} // namespace tos
} // namespace zi

#endif
