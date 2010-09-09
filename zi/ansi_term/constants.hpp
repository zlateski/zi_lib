/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_ANIS_TERM_CONSTANTS_HPP
#define ZI_ANSI_TERM_CONSTANTS_HPP 1

#include <zi/config/config.hpp>

namespace zi {
namespace tos {

enum color_constants
{
    none         = 0x00,
    black        = 0x08,
    red          = 0x09,
    green        = 0x0a,
    brown        = 0x0b,
    blue         = 0x0c,
    purple       = 0x0d,
    cyan         = 0x0e,
    bright_gray  = 0x0f,

    dark_gray    = 0x18,
    bright_red   = 0x19,
    bright_green = 0x1a,
    yellow       = 0x1b,
    bright_blue  = 0x1c,
    pink         = 0x1d,
    bright_cyan  = 0x1e,
    white        = 0x1f
};

enum bg_color_constants
{
    bg_none         = 0x00,
    bg_black        = 0x08,
    bg_red          = 0x09,
    bg_green        = 0x0a,
    bg_brown        = 0x0b,
    bg_blue         = 0x0c,
    bg_purple       = 0x0d,
    bg_cyan         = 0x0e,
    bg_bright_gray  = 0x0f,

    bg_dark_gray    = 0x18,
    bg_bright_red   = 0x19,
    bg_bright_green = 0x1a,
    bg_yellow       = 0x1b,
    bg_bright_blue  = 0x1c,
    bg_pink         = 0x1d,
    bg_bright_cyan  = 0x1e,
    bg_white        = 0x1f
};

enum weight_constants
{
    regular      = 0x0000,
    bold         = 0x0800,
    light        = 0x0400
};

enum decoration_constants
{
    undecorated   = 0x0000,
    underline     = 0x1000,
    overline      = 0x2000,
    inverted      = 0x4000
};

} // namespace tos
} // namespace zi

#endif
