// Copyright Kevlin Henney, 2000-2005.
// Copyright Alexander Nasonov, 2006-2010.
// Copyright Antony Polukhin, 2011-2014.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// what:  lexical_cast custom keyword cast
// who:   contributed by Kevlin Henney,
//        enhanced with contributions from Terje Slettebo,
//        with additional fixes and suggestions from Gennaro Prota,
//        Beman Dawes, Dave Abrahams, Daryle Walker, Peter Dimov,
//        Alexander Nasonov, Antony Polukhin, Justin Viiret, Michael Hofmann,
//        Cheng Yang, Matthew Bradbury, David W. Birdsall, Pavel Korzh and other Boosters
// when:  November 2000, March 2003, June 2005, June 2006, March 2011 - 2014

#ifndef HBOOST_LEXICAL_CAST_DETAIL_CONVERTER_NUMERIC_HPP
#define HBOOST_LEXICAL_CAST_DETAIL_CONVERTER_NUMERIC_HPP

#include <hboost/config.hpp>
#ifdef HBOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <hboost/limits.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/make_unsigned.hpp>
#include <hboost/type_traits/is_signed.hpp>
#include <hboost/type_traits/is_integral.hpp>
#include <hboost/type_traits/is_arithmetic.hpp>
#include <hboost/type_traits/is_base_of.hpp>
#include <hboost/type_traits/is_float.hpp>

#include <hboost/numeric/conversion/cast.hpp>

namespace hboost { namespace detail {

template <class Source >
struct detect_precision_loss
{
    typedef Source source_type;
    typedef hboost::numeric::Trunc<Source> Rounder;
    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<
        hboost::is_arithmetic<Source>, Source, Source const&
    >::type argument_type ;

    static inline source_type nearbyint(argument_type s, bool& is_ok) HBOOST_NOEXCEPT {
        const source_type near_int = Rounder::nearbyint(s);
        if (near_int && is_ok) {
            const source_type orig_div_round = s / near_int;
            const source_type eps = std::numeric_limits<source_type>::epsilon();

            is_ok = !((orig_div_round > 1 ? orig_div_round - 1 : 1 - orig_div_round) > eps);
        }

        return s;
    }

    typedef typename Rounder::round_style round_style;
};

template <typename Base, class Source>
struct fake_precision_loss: public Base
{
    typedef Source source_type ;
    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<
        hboost::is_arithmetic<Source>, Source, Source const&
    >::type argument_type ;

    static inline source_type nearbyint(argument_type s, bool& /*is_ok*/) HBOOST_NOEXCEPT {
        return s;
    }
};

struct nothrow_overflow_handler
{
    inline bool operator() ( hboost::numeric::range_check_result r ) const HBOOST_NOEXCEPT {
        return (r == hboost::numeric::cInRange);
    }
};

template <typename Target, typename Source>
inline bool noexcept_numeric_convert(const Source& arg, Target& result) HBOOST_NOEXCEPT {
    typedef hboost::numeric::converter<
            Target,
            Source,
            hboost::numeric::conversion_traits<Target, Source >,
            nothrow_overflow_handler,
            detect_precision_loss<Source >
    > converter_orig_t;

    typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::if_c<
        hboost::is_base_of< detect_precision_loss<Source >, converter_orig_t >::value,
        converter_orig_t,
        fake_precision_loss<converter_orig_t, Source>
    >::type converter_t;

    bool res = nothrow_overflow_handler()(converter_t::out_of_range(arg));
    result = converter_t::low_level_convert(converter_t::nearbyint(arg, res));
    return res;
}

template <typename Target, typename Source>
struct lexical_cast_dynamic_num_not_ignoring_minus
{
    static inline bool try_convert(const Source &arg, Target& result) HBOOST_NOEXCEPT {
        return noexcept_numeric_convert<Target, Source >(arg, result);
    }
};

template <typename Target, typename Source>
struct lexical_cast_dynamic_num_ignoring_minus
{
    static inline bool try_convert(const Source &arg, Target& result) HBOOST_NOEXCEPT {
        typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::eval_if_c<
                hboost::is_float<Source>::value,
                hboost::mpl::identity<Source>,
                hboost::make_unsigned<Source>
        >::type usource_t;

        if (arg < 0) {
            const bool res = noexcept_numeric_convert<Target, usource_t>(0u - arg, result);
            result = static_cast<Target>(0u - result);
            return res;
        } else {
            return noexcept_numeric_convert<Target, usource_t>(arg, result);
        }
    }
};

/*
 * lexical_cast_dynamic_num follows the rules:
 * 1) If Source can be converted to Target without precision loss and
 * without overflows, then assign Source to Target and return
 *
 * 2) If Source is less than 0 and Target is an unsigned integer,
 * then negate Source, check the requirements of rule 1) and if
 * successful, assign static_casted Source to Target and return
 *
 * 3) Otherwise throw a bad_lexical_cast exception
 *
 *
 * Rule 2) required because hboost::lexical_cast has the behavior of
 * stringstream, which uses the rules of scanf for conversions. And
 * in the C99 standard for unsigned input value minus sign is
 * optional, so if a negative number is read, no errors will arise
 * and the result will be the two's complement.
 */
template <typename Target, typename Source>
struct dynamic_num_converter_impl
{
    static inline bool try_convert(const Source &arg, Target& result) HBOOST_NOEXCEPT {
        typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::if_c<
        	hboost::is_unsigned<Target>::value &&
        	(hboost::is_signed<Source>::value || hboost::is_float<Source>::value) &&
        	!(hboost::is_same<Source, bool>::value) &&
        	!(hboost::is_same<Target, bool>::value),
            lexical_cast_dynamic_num_ignoring_minus<Target, Source>,
            lexical_cast_dynamic_num_not_ignoring_minus<Target, Source>
        >::type caster_type;
        
#if 0

        typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::if_<
            HBOOST_DEDUCED_TYPENAME hboost::mpl::and_<
                hboost::is_unsigned<Target>,
                hboost::mpl::or_<
                    hboost::is_signed<Source>,
                    hboost::is_float<Source>
                >,
                hboost::mpl::not_<
                    hboost::is_same<Source, bool>
                >,
                hboost::mpl::not_<
                    hboost::is_same<Target, bool>
                >
            >::type,
            lexical_cast_dynamic_num_ignoring_minus<Target, Source>,
            lexical_cast_dynamic_num_not_ignoring_minus<Target, Source>
        >::type caster_type;
        
#endif

        return caster_type::try_convert(arg, result);
    }
};

}} // namespace hboost::detail

#endif // HBOOST_LEXICAL_CAST_DETAIL_CONVERTER_NUMERIC_HPP

