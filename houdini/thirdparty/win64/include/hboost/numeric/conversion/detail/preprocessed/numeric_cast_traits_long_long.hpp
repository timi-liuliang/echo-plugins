//
//! Copyright (c) 2011-2012
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
namespace hboost { namespace numeric {
    
    
    template <>
    struct numeric_cast_traits
        <
            char
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            char
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            signed char
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            signed char
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            unsigned char
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            unsigned char
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            short
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            short
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            unsigned short
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            unsigned short
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            int
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            int
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            unsigned int
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            unsigned int
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            long
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            long
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            unsigned long
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            unsigned long
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            float
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            float
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            double
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            double
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            long double
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            long double
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            hboost::long_long_type
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            hboost::long_long_type
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            hboost::ulong_long_type
          , hboost::long_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::long_long_type> rounding_policy;
    }; 
    
    template <>
    struct numeric_cast_traits
        <
            hboost::ulong_long_type
          , hboost::ulong_long_type
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<hboost::ulong_long_type> rounding_policy;
    }; 
}}
