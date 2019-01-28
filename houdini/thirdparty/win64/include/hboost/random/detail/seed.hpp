/* boost random/detail/seed.hpp header file
 *
 * Copyright Steven Watanabe 2009
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 */

#ifndef HBOOST_RANDOM_DETAIL_SEED_HPP
#define HBOOST_RANDOM_DETAIL_SEED_HPP

#include <hboost/config.hpp>

// Sun seems to have trouble with the use of SFINAE for the
// templated constructor.  So does Borland.
#if !defined(HBOOST_NO_SFINAE) && !defined(__SUNPRO_CC) && !defined(__BORLANDC__)

#include <hboost/utility/enable_if.hpp>
#include <hboost/type_traits/is_arithmetic.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost {
namespace random {
namespace detail {

template<class T>
struct disable_seed : hboost::disable_if<hboost::is_arithmetic<T> > {};

template<class Engine, class T>
struct disable_constructor : disable_seed<T> {};

template<class Engine>
struct disable_constructor<Engine, Engine> {};

#define HBOOST_RANDOM_DETAIL_GENERATOR_CONSTRUCTOR(Self, Generator, gen) \
    template<class Generator>                                           \
    explicit Self(Generator& gen, typename ::hboost::random::detail::disable_constructor<Self, Generator>::type* = 0)

#define HBOOST_RANDOM_DETAIL_GENERATOR_SEED(Self, Generator, gen)    \
    template<class Generator>                                       \
    void seed(Generator& gen, typename ::hboost::random::detail::disable_seed<Generator>::type* = 0)

#define HBOOST_RANDOM_DETAIL_SEED_SEQ_CONSTRUCTOR(Self, SeedSeq, seq)    \
    template<class SeedSeq>                                             \
    explicit Self(SeedSeq& seq, typename ::hboost::random::detail::disable_constructor<Self, SeedSeq>::type* = 0)

#define HBOOST_RANDOM_DETAIL_SEED_SEQ_SEED(Self, SeedSeq, seq)   \
    template<class SeedSeq>                                     \
    void seed(SeedSeq& seq, typename ::hboost::random::detail::disable_seed<SeedSeq>::type* = 0)

#define HBOOST_RANDOM_DETAIL_ARITHMETIC_CONSTRUCTOR(Self, T, x)  \
    explicit Self(const T& x)

#define HBOOST_RANDOM_DETAIL_ARITHMETIC_SEED(Self, T, x) \
    void seed(const T& x)
}
}
}

#else

#include <hboost/type_traits/is_arithmetic.hpp>
#include <hboost/mpl/bool.hpp>

#define HBOOST_RANDOM_DETAIL_GENERATOR_CONSTRUCTOR(Self, Generator, gen) \
    Self(Self& other) { *this = other; }                                \
    Self(const Self& other) { *this = other; }                          \
    template<class Generator>                                           \
    explicit Self(Generator& gen) {                                     \
        hboost_random_constructor_impl(gen, ::hboost::is_arithmetic<Generator>());\
    }                                                                   \
    template<class Generator>                                           \
    void hboost_random_constructor_impl(Generator& gen, ::hboost::mpl::false_)

#define HBOOST_RANDOM_DETAIL_GENERATOR_SEED(Self, Generator, gen)    \
    template<class Generator>                                       \
    void seed(Generator& gen) {                                     \
        hboost_random_seed_impl(gen, ::hboost::is_arithmetic<Generator>());\
    }\
    template<class Generator>\
    void hboost_random_seed_impl(Generator& gen, ::hboost::mpl::false_)

#define HBOOST_RANDOM_DETAIL_SEED_SEQ_CONSTRUCTOR(Self, SeedSeq, seq)    \
    Self(Self& other) { *this = other; }                                \
    Self(const Self& other) { *this = other; }                          \
    template<class SeedSeq>                                             \
    explicit Self(SeedSeq& seq) {                                       \
        hboost_random_constructor_impl(seq, ::hboost::is_arithmetic<SeedSeq>());\
    }                                                                   \
    template<class SeedSeq>                                             \
    void hboost_random_constructor_impl(SeedSeq& seq, ::hboost::mpl::false_)

#define HBOOST_RANDOM_DETAIL_SEED_SEQ_SEED(Self, SeedSeq, seq)           \
    template<class SeedSeq>                                             \
    void seed(SeedSeq& seq) {                                           \
        hboost_random_seed_impl(seq, ::hboost::is_arithmetic<SeedSeq>()); \
    }                                                                   \
    template<class SeedSeq>                                             \
    void hboost_random_seed_impl(SeedSeq& seq, ::hboost::mpl::false_)

#define HBOOST_RANDOM_DETAIL_ARITHMETIC_CONSTRUCTOR(Self, T, x)  \
    explicit Self(const T& x) { hboost_random_constructor_impl(x, ::hboost::mpl::true_()); }\
    void hboost_random_constructor_impl(const T& x, ::hboost::mpl::true_)

#define HBOOST_RANDOM_DETAIL_ARITHMETIC_SEED(Self, T, x) \
    void seed(const T& x) { hboost_random_seed_impl(x, ::hboost::mpl::true_()); }\
    void hboost_random_seed_impl(const T& x, ::hboost::mpl::true_)

#endif

#endif
