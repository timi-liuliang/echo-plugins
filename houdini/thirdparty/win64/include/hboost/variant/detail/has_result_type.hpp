//-----------------------------------------------------------------------------
// boost variant/detail/has_result_type.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2014-2015 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_HAS_RESULT_TYPE_HPP
#define HBOOST_VARIANT_DETAIL_HAS_RESULT_TYPE_HPP

#include "hboost/config.hpp"
#include "hboost/type_traits/remove_reference.hpp"


namespace hboost { namespace detail { namespace variant {

template <typename T >
struct has_result_type {
private:
    typedef char                      yes;
    typedef struct { char array[2]; } no;

    template<typename C> static yes test(typename hboost::remove_reference<typename C::result_type>::type*);
    template<typename C> static no  test(...);

public:
    HBOOST_STATIC_CONSTANT(bool, value = sizeof(test<T>(0)) == sizeof(yes));
};

}}} // namespace hboost::detail::variant

#endif // HBOOST_VARIANT_DETAIL_HAS_RESULT_TYPE_HPP
