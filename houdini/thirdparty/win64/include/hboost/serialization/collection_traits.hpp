#ifndef HBOOST_SERIALIZATION_COLLECTION_TRAITS_HPP
#define HBOOST_SERIALIZATION_COLLECTION_TRAITS_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// collection_traits.hpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// This header assigns a level implemenation trait to a collection type
// for all primitives.  It is needed so that archives which are meant to be
// portable don't write class information in the archive.  Since, not all
// compiles recognize the same set of primitive types, the possibility
// exists for archives to be non-portable if class information for primitive
// types is included.  This is addressed by the following macros.
#include <hboost/config.hpp>
//#include <hboost/mpl/integral_c.hpp>
#include <hboost/mpl/integral_c_tag.hpp>

#include <hboost/cstdint.hpp>
#include <hboost/integer_traits.hpp>
#include <climits> // ULONG_MAX
#include <hboost/serialization/level.hpp>

#define HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(T, C)          \
template<>                                                          \
struct implementation_level< C < T > > {                            \
    typedef mpl::integral_c_tag tag;                                \
    typedef mpl::int_<object_serializable> type;                    \
    HBOOST_STATIC_CONSTANT(int, value = object_serializable);        \
};                                                                  \
/**/

#if defined(HBOOST_NO_CWCHAR) || defined(HBOOST_NO_INTRINSIC_WCHAR_T)
    #define HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER_WCHAR(C)
#else
    #define HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER_WCHAR(C)   \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(wchar_t, C)        \
    /**/
#endif

#if defined(HBOOST_HAS_LONG_LONG)
    #define HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER_INT64(C)    \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(hboost::long_long_type, C)  \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(hboost::ulong_long_type, C) \
    /**/
#else
    #define HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER_INT64(C)
#endif

#define HBOOST_SERIALIZATION_COLLECTION_TRAITS(C)                     \
    namespace hboost { namespace serialization {                      \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(bool, C)            \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(char, C)            \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(signed char, C)     \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(unsigned char, C)   \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(signed int, C)      \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(unsigned int, C)    \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(signed long, C)     \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(unsigned long, C)   \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(float, C)           \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(double, C)          \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(unsigned short, C)  \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER(signed short, C)    \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER_INT64(C)            \
    HBOOST_SERIALIZATION_COLLECTION_TRAITS_HELPER_WCHAR(C)            \
    } }                                                              \
    /**/

#endif // HBOOST_SERIALIZATION_COLLECTION_TRAITS
