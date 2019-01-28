//  Boost string_algo library string_traits.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef HBOOST_STRING_STD_STRING_TRAITS_HPP
#define HBOOST_STRING_STD_STRING_TRAITS_HPP

#include <hboost/algorithm/string/yes_no_type.hpp>
#include <string>
#include <hboost/algorithm/string/sequence_traits.hpp>

namespace hboost {
    namespace algorithm {

//  std::basic_string<> traits  -----------------------------------------------//


    // native replace trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_native_replace< std::basic_string<T, TraitsT, AllocT> >
        {
        public:
#if HBOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true } ;
#else
            HBOOST_STATIC_CONSTANT(bool, value=true);
#endif // HBOOST_WORKAROUND( __IBMCPP__, <= 600 )

        typedef mpl::bool_<has_native_replace<T>::value> type;
        };



    } // namespace algorithm
} // namespace hboost


#endif  // HBOOST_STRING_LIST_TRAITS_HPP
