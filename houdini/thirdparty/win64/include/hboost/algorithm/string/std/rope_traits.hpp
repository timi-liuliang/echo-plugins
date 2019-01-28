//  Boost string_algo library string_traits.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef HBOOST_STRING_STD_ROPE_TRAITS_HPP
#define HBOOST_STRING_STD_ROPE_TRAITS_HPP

#include <hboost/algorithm/string/yes_no_type.hpp>
#include <rope>
#include <hboost/algorithm/string/sequence_traits.hpp>

namespace hboost {
    namespace algorithm {

//  SGI's std::rope<> traits  -----------------------------------------------//

    
    // native replace trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_native_replace< std::rope<T,TraitsT,AllocT> >
        {
        public:
#if HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            HBOOST_STATIC_CONSTANT(bool, value=true);
#endif // HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<value> type;     
        };

    // stable iterators trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_stable_iterators< std::rope<T,TraitsT,AllocT> >
        {
        public:
#if HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            HBOOST_STATIC_CONSTANT(bool, value=true);
#endif // HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<value> type;     
        };

    // const time insert trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_const_time_insert< std::rope<T,TraitsT,AllocT> >
        {
        public:
#if HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            HBOOST_STATIC_CONSTANT(bool, value=true);
#endif // HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<value> type;     
        };

    // const time erase trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_const_time_erase< std::rope<T,TraitsT,AllocT> >
        {
        public:
#if HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            HBOOST_STATIC_CONSTANT(bool, value=true);
#endif // HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<value> type;     
        };


    } // namespace algorithm
} // namespace hboost


#endif  // HBOOST_STRING_ROPE_TRAITS_HPP