/*
 *          Copyright Andrey Semashev 2007 - 2013.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

/*!
 * \file   explicit_operator_bool.hpp
 * \author Andrey Semashev
 * \date   08.03.2009
 *
 * This header defines a compatibility macro that implements an unspecified
 * \c bool operator idiom, which is superseded with explicit conversion operators in
 * C++11.
 */

#ifndef HBOOST_CORE_EXPLICIT_OPERATOR_BOOL_HPP
#define HBOOST_CORE_EXPLICIT_OPERATOR_BOOL_HPP

#include <hboost/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if !defined(HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)

/*!
 * \brief The macro defines an explicit operator of conversion to \c bool
 *
 * The macro should be used inside the definition of a class that has to
 * support the conversion. The class should also implement <tt>operator!</tt>,
 * in terms of which the conversion operator will be implemented.
 */
#define HBOOST_EXPLICIT_OPERATOR_BOOL()\
    HBOOST_FORCEINLINE explicit operator bool () const\
    {\
        return !this->operator! ();\
    }

/*!
 * \brief The macro defines a noexcept explicit operator of conversion to \c bool
 *
 * The macro should be used inside the definition of a class that has to
 * support the conversion. The class should also implement <tt>operator!</tt>,
 * in terms of which the conversion operator will be implemented.
 */
#define HBOOST_EXPLICIT_OPERATOR_BOOL_NOEXCEPT()\
    HBOOST_FORCEINLINE explicit operator bool () const HBOOST_NOEXCEPT\
    {\
        return !this->operator! ();\
    }

/*!
 * \brief The macro defines a constexpr explicit operator of conversion to \c bool
 *
 * The macro should be used inside the definition of a class that has to
 * support the conversion. The class should also implement <tt>operator!</tt>,
 * in terms of which the conversion operator will be implemented.
 */
#define HBOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL()\
    HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit operator bool () const HBOOST_NOEXCEPT\
    {\
        return !this->operator! ();\
    }

#else // !defined(HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)

#if (defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x530)) && !defined(HBOOST_NO_COMPILER_CONFIG)
// Sun C++ 5.3 can't handle the safe_bool idiom, so don't use it
#define HBOOST_NO_UNSPECIFIED_BOOL
#endif // (defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x530)) && !defined(HBOOST_NO_COMPILER_CONFIG)

#if !defined(HBOOST_NO_UNSPECIFIED_BOOL)

namespace hboost {

namespace detail {

#if !defined(_MSC_VER) && !defined(__IBMCPP__)

    struct unspecified_bool
    {
        // NOTE TO THE USER: If you see this in error messages then you tried
        // to apply an unsupported operator on the object that supports
        // explicit conversion to bool.
        struct OPERATORS_NOT_ALLOWED;
        static void true_value(OPERATORS_NOT_ALLOWED*) {}
    };
    typedef void (*unspecified_bool_type)(unspecified_bool::OPERATORS_NOT_ALLOWED*);

#else

    // MSVC and VACPP are too eager to convert pointer to function to void* even though they shouldn't
    struct unspecified_bool
    {
        // NOTE TO THE USER: If you see this in error messages then you tried
        // to apply an unsupported operator on the object that supports
        // explicit conversion to bool.
        struct OPERATORS_NOT_ALLOWED;
        void true_value(OPERATORS_NOT_ALLOWED*) {}
    };
    typedef void (unspecified_bool::*unspecified_bool_type)(unspecified_bool::OPERATORS_NOT_ALLOWED*);

#endif

} // namespace detail

} // namespace hboost

#define HBOOST_EXPLICIT_OPERATOR_BOOL()\
    HBOOST_FORCEINLINE operator hboost::detail::unspecified_bool_type () const\
    {\
        return (!this->operator! () ? &hboost::detail::unspecified_bool::true_value : (hboost::detail::unspecified_bool_type)0);\
    }

#define HBOOST_EXPLICIT_OPERATOR_BOOL_NOEXCEPT()\
    HBOOST_FORCEINLINE operator hboost::detail::unspecified_bool_type () const HBOOST_NOEXCEPT\
    {\
        return (!this->operator! () ? &hboost::detail::unspecified_bool::true_value : (hboost::detail::unspecified_bool_type)0);\
    }

#define HBOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL()\
    HBOOST_FORCEINLINE HBOOST_CONSTEXPR operator hboost::detail::unspecified_bool_type () const HBOOST_NOEXCEPT\
    {\
        return (!this->operator! () ? &hboost::detail::unspecified_bool::true_value : (hboost::detail::unspecified_bool_type)0);\
    }

#else // !defined(HBOOST_NO_UNSPECIFIED_BOOL)

#define HBOOST_EXPLICIT_OPERATOR_BOOL()\
    HBOOST_FORCEINLINE operator bool () const\
    {\
        return !this->operator! ();\
    }

#define HBOOST_EXPLICIT_OPERATOR_BOOL_NOEXCEPT()\
    HBOOST_FORCEINLINE operator bool () const HBOOST_NOEXCEPT\
    {\
        return !this->operator! ();\
    }

#define HBOOST_CONSTEXPR_EXPLICIT_OPERATOR_BOOL()\
    HBOOST_FORCEINLINE HBOOST_CONSTEXPR operator bool () const HBOOST_NOEXCEPT\
    {\
        return !this->operator! ();\
    }

#endif // !defined(HBOOST_NO_UNSPECIFIED_BOOL)

#endif // !defined(HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)

#endif // HBOOST_CORE_EXPLICIT_OPERATOR_BOOL_HPP
