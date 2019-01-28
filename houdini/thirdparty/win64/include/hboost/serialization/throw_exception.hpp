#ifndef HBOOST_SERIALIZATION_THROW_EXCEPTION_HPP_INCLUDED
#define HBOOST_SERIALIZATION_THROW_EXCEPTION_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER)
# pragma once
#endif

//  hboost/throw_exception.hpp
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/config.hpp>

#ifndef HBOOST_NO_EXCEPTIONS
#include <exception>
#endif

namespace hboost {
namespace serialization {

#ifdef HBOOST_NO_EXCEPTIONS

inline void throw_exception(std::exception const & e) {
    ::hboost::throw_exception(e);
}

#else

template<class E> inline void throw_exception(E const & e){
    throw e;
}

#endif

} // namespace serialization
} // namespace hboost

#endif // #ifndef HBOOST_SERIALIZATION_THROW_EXCEPTION_HPP_INCLUDED
