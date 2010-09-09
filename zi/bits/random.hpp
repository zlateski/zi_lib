/*
 * AUTHORS:
 *   Aleksandar Zlateski <zlateski@mit.edu>
 *
 * Do not share without authors permission.
 */

#ifndef ZI_BITS_RANDOM_HPP
#define ZI_BITS_RANDOM_HPP 1

#include <zi/config/config.hpp>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <random>
#  define ZI_RANDOM_NAMESPACE ::std
#else
#  if defined( ZI_USE_TR1 ) || defined( ZI_NO_BOOST )
#    include <tr1/random>
#    define ZI_RANDOM_NAMESPACE ::std::tr1
#  else
#    include <boost/tr1/random.hpp>
#    define ZI_RANDOM_NAMESPACE ::std::tr1
#  endif
#endif

namespace zi {

using ZI_RANDOM_NAMESPACE::variate_generator;
using ZI_RANDOM_NAMESPACE::mersenne_twister;
using ZI_RANDOM_NAMESPACE::discard_block;
using ZI_RANDOM_NAMESPACE::uniform_int;
using ZI_RANDOM_NAMESPACE::geometric_distribution;
using ZI_RANDOM_NAMESPACE::poisson_distribution;
using ZI_RANDOM_NAMESPACE::binomial_distribution;
using ZI_RANDOM_NAMESPACE::uniform_real;
using ZI_RANDOM_NAMESPACE::exponential_distribution;
using ZI_RANDOM_NAMESPACE::normal_distribution;
using ZI_RANDOM_NAMESPACE::gamma_distribution;

using ZI_RANDOM_NAMESPACE::minstd_rand0;
using ZI_RANDOM_NAMESPACE::minstd_rand;
using ZI_RANDOM_NAMESPACE::mt19937;
using ZI_RANDOM_NAMESPACE::ranlux_base_01;
using ZI_RANDOM_NAMESPACE::ranlux64_base_01;
using ZI_RANDOM_NAMESPACE::ranlux3;
using ZI_RANDOM_NAMESPACE::ranlux4;
using ZI_RANDOM_NAMESPACE::ranlux3_01;
using ZI_RANDOM_NAMESPACE::ranlux4_01;

} // namespace zi

#undef ZI_RANDOM_NAMESPACE
#endif
