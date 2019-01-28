//  Boost string_algo library slist_traits.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. 
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef HBOOST_STRING_STD_SLIST_TRAITS_HPP
#define HBOOST_STRING_STD_SLIST_TRAITS_HPP

#include <hboost/algorithm/string/config.hpp>
#include <hboost/algorithm/string/yes_no_type.hpp>
#include HBOOST_SLIST_HEADER 
#include <hboost/algorithm/string/sequence_traits.hpp>

namespace hboost {
    namespace algorithm {

//  SGI's std::slist<> traits  -----------------------------------------------//


    // stable iterators trait
        template<typename T, typename AllocT>
        class has_stable_iterators< HBOOST_STD_EXTENSION_NAMESPACE::slist<T,AllocT> >
        {
        public:
#if HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            HBOOST_STATIC_CONSTANT(bool, value=true);
#endif // HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<has_stable_iterators<T>::value> type;
        };

    // const time insert trait
        template<typename T, typename AllocT>
        class has_const_time_insert< HBOOST_STD_EXTENSION_NAMESPACE::slist<T,AllocT> >
        {
        public:
#if HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            HBOOST_STATIC_CONSTANT(bool, value=true);
#endif // HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<has_const_time_insert<T>::value> type;
        };

    // const time erase trait
        template<typename T, typename AllocT>
        class has_const_time_erase< HBOOST_STD_EXTENSION_NAMESPACE::slist<T,AllocT> >
        {
        public:
#if HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            HBOOST_STATIC_CONSTANT(bool, value=true);
#endif // HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<has_const_time_erase<T>::value> type;
        };


    } // namespace algorithm
} // namespace hboost


#endif  // HBOOST_STRING_STD_LIST_TRAITS_HPP
