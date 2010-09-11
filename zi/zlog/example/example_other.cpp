#include <zi/logging.hpp>

// The function will do some logging, assuming the logs are
// already setup in some other file!

void do_some_logging()
{
  ZiLOG( INFO, FirstLog ) << "I'm in the other file!";
}

namespace other_namespace {

void do_some_logging()
{
    ZiLOG( INFO, FirstLog ) << "I'm in the other other_namespace, and in the other file!";
}

} // other_namespace
