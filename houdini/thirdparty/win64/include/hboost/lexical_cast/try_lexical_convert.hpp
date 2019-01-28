// Copyright Kevlin Henney, 2000-2005.
// Copyright Alexander Nasonov, 2006-2010.
// Copyright Antony Polukhin, 2011-2016.
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

#ifndef HBOOST_LEXICAL_CAST_TRY_LEXICAL_CONVERT_HPP
#define HBOOST_LEXICAL_CAST_TRY_LEXICAL_CONVERT_HPP

#include <hboost/config.hpp>
#ifdef HBOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#if defined(__clang__) || (defined(__GNUC__) && \
    !(defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)) && \
    (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif

#include <string>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/is_arithmetic.hpp>

#include <hboost/lexical_cast/detail/is_character.hpp>
#include <hboost/lexical_cast/detail/converter_numeric.hpp>
#include <hboost/lexical_cast/detail/converter_lexical.hpp>

#include <hboost/range/iterator_range_core.hpp>
#include <hboost/container/container_fwd.hpp>

namespace hboost {
    namespace detail
    {
        template<typename T>
        struct is_stdstring
            : hboost::false_type
        {};

        template<typename CharT, typename Traits, typename Alloc>
        struct is_stdstring< std::basic_string<CharT, Traits, Alloc> >
            : hboost::true_type
        {};

        // Sun Studio has problem with partial specialization of templates differing only in namespace.
        // We workaround that by making `is_booststring` trait, instead of specializing `is_stdstring` for `hboost::container::basic_string`.
        template<typename T>
        struct is_booststring
            : hboost::false_type
        {};

        template<typename CharT, typename Traits, typename Alloc>
        struct is_booststring< hboost::container::basic_string<CharT, Traits, Alloc> >
            : hboost::true_type
        {};

        template<typename Target, typename Source>
        struct is_arithmetic_and_not_xchars
        {
            typedef hboost::mpl::bool_<
                    !(hboost::detail::is_character<Target>::value) &&
                    !(hboost::detail::is_character<Source>::value) &&
                    hboost::is_arithmetic<Source>::value &&
                    hboost::is_arithmetic<Target>::value
                > type;
        
            HBOOST_STATIC_CONSTANT(bool, value = (
                type::value
            ));
        };

        /*
         * is_xchar_to_xchar<Target, Source>::value is true, 
         * Target and Souce are char types of the same size 1 (char, signed char, unsigned char).
         */
        template<typename Target, typename Source>
        struct is_xchar_to_xchar 
        {
            typedef hboost::mpl::bool_<
                     sizeof(Source) == sizeof(Target) &&
                     sizeof(Source) == sizeof(char) &&
                     hboost::detail::is_character<Target>::value &&
                     hboost::detail::is_character<Source>::value
                > type;
                
            HBOOST_STATIC_CONSTANT(bool, value = (
                type::value
            ));
        };

        template<typename Target, typename Source>
        struct is_char_array_to_stdstring
            : hboost::false_type
        {};

        template<typename CharT, typename Traits, typename Alloc>
        struct is_char_array_to_stdstring< std::basic_string<CharT, Traits, Alloc>, CharT* >
            : hboost::true_type
        {};

        template<typename CharT, typename Traits, typename Alloc>
        struct is_char_array_to_stdstring< std::basic_string<CharT, Traits, Alloc>, const CharT* >
            : hboost::true_type
        {};

        // Sun Studio has problem with partial specialization of templates differing only in namespace.
        // We workaround that by making `is_char_array_to_booststring` trait, instead of specializing `is_char_array_to_stdstring` for `hboost::container::basic_string`.
        template<typename Target, typename Source>
        struct is_char_array_to_booststring
            : hboost::false_type
        {};

        template<typename CharT, typename Traits, typename Alloc>
        struct is_char_array_to_booststring< hboost::container::basic_string<CharT, Traits, Alloc>, CharT* >
            : hboost::true_type
        {};

        template<typename CharT, typename Traits, typename Alloc>
        struct is_char_array_to_booststring< hboost::container::basic_string<CharT, Traits, Alloc>, const CharT* >
            : hboost::true_type
        {};

        template <typename Target, typename Source>
        struct copy_converter_impl
        {
// MSVC fail to forward an array (DevDiv#555157 "SILENT BAD CODEGEN triggered by perfect forwarding",
// fixed in 2013 RTM).
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && (!defined(HBOOST_MSVC) || HBOOST_MSVC >= 1800)
            template <class T>
            static inline bool try_convert(T&& arg, Target& result) {
                result = static_cast<T&&>(arg); // eqaul to `result = std::forward<T>(arg);`
                return true;
            }
#else
            static inline bool try_convert(const Source& arg, Target& result) {
                result = arg;
                return true;
            }
#endif
        };
    }

    namespace conversion { namespace detail {

        template <typename Target, typename Source>
        inline bool try_lexical_convert(const Source& arg, Target& result)
        {
            typedef HBOOST_DEDUCED_TYPENAME hboost::detail::array_to_pointer_decay<Source>::type src;

            typedef hboost::mpl::bool_<
                hboost::detail::is_xchar_to_xchar<Target, src >::value ||
                hboost::detail::is_char_array_to_stdstring<Target, src >::value ||
                hboost::detail::is_char_array_to_booststring<Target, src >::value ||
                (
                     hboost::is_same<Target, src >::value &&
                     (hboost::detail::is_stdstring<Target >::value || hboost::detail::is_booststring<Target >::value)
                ) ||
                (
                     hboost::is_same<Target, src >::value &&
                     hboost::detail::is_character<Target >::value
                )
            > shall_we_copy_t;

            typedef hboost::detail::is_arithmetic_and_not_xchars<Target, src >
                shall_we_copy_with_dynamic_check_t;

            // We do evaluate second `if_` lazily to avoid unnecessary instantiations
            // of `shall_we_copy_with_dynamic_check_t` and improve compilation times.
            typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::if_c<
                shall_we_copy_t::value,
                hboost::mpl::identity<hboost::detail::copy_converter_impl<Target, src > >,
                hboost::mpl::if_<
                     shall_we_copy_with_dynamic_check_t,
                     hboost::detail::dynamic_num_converter_impl<Target, src >,
                     hboost::detail::lexical_converter_impl<Target, src >
                >
            >::type caster_type_lazy;

            typedef HBOOST_DEDUCED_TYPENAME caster_type_lazy::type caster_type;

            return caster_type::try_convert(arg, result);
        }

        template <typename Target, typename CharacterT>
        inline bool try_lexical_convert(const CharacterT* chars, std::size_t count, Target& result)
        {
            HBOOST_STATIC_ASSERT_MSG(
                hboost::detail::is_character<CharacterT>::value,
                "This overload of try_lexical_convert is meant to be used only with arrays of characters."
            );
            return ::hboost::conversion::detail::try_lexical_convert(
                ::hboost::iterator_range<const CharacterT*>(chars, chars + count), result
            );
        }

    }} // namespace conversion::detail

    namespace conversion {
        // ADL barrier
        using ::hboost::conversion::detail::try_lexical_convert;
    }

} // namespace hboost

#if defined(__clang__) || (defined(__GNUC__) && \
    !(defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)) && \
    (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic pop
#endif

#endif // HBOOST_LEXICAL_CAST_TRY_LEXICAL_CONVERT_HPP

