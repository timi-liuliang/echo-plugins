// Boost Lambda Library  suppress_unused.hpp -----------------------------
//
// Copyright (C) 2009 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org

// ------------------------------------------------------------

#ifndef HBOOST_LAMBDA_SUPPRESS_UNUSED_HPP
#define HBOOST_LAMBDA_SUPPRESS_UNUSED_HPP

namespace hboost { 
namespace lambda {
namespace detail {

template<class T>
inline void suppress_unused_variable_warnings(const T&) {}

}
}
}

#endif
