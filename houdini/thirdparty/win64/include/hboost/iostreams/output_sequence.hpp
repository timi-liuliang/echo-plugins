// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_OUTPUT_SEQUENCE_HPP_INCLUDED
#define HBOOST_IOSTREAMS_OUTPUT_SEQUENCE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <utility>           // pair.
#include <hboost/config.hpp>  // DEDUCED_TYPENAME, MSVC.
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/detail/wrap_unwrap.hpp>
#include <hboost/iostreams/operations_fwd.hpp>  // is_custom 
#include <hboost/iostreams/traits.hpp>
#include <hboost/mpl/if.hpp>

// Must come last.
#include <hboost/iostreams/detail/config/disable_warnings.hpp>

namespace hboost { namespace iostreams {

namespace detail {

template<typename T>
struct output_sequence_impl;

} // End namespace detail.

template<typename T>
inline std::pair<
    HBOOST_DEDUCED_TYPENAME char_type_of<T>::type*,
    HBOOST_DEDUCED_TYPENAME char_type_of<T>::type*
>
output_sequence(T& t)
{ return detail::output_sequence_impl<T>::output_sequence(t); }

namespace detail {

//------------------Definition of output_sequence_impl------------------------//

template<typename T>
struct output_sequence_impl
    : mpl::if_<
          detail::is_custom<T>,
          operations<T>,
          output_sequence_impl<direct_tag>
      >::type
    { };

template<>
struct output_sequence_impl<direct_tag> {
    template<typename U>
    static std::pair<
        HBOOST_DEDUCED_TYPENAME char_type_of<U>::type*,
        HBOOST_DEDUCED_TYPENAME char_type_of<U>::type*
    >
    output_sequence(U& u) { return u.output_sequence(); }
};

} // End namespace detail.

} } // End namespaces iostreams, boost.

#include <hboost/iostreams/detail/config/enable_warnings.hpp>

#endif // #ifndef HBOOST_IOSTREAMS_OUTPUT_SEQUENCE_HPP_INCLUDED
