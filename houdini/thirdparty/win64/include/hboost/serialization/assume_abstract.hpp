#ifndef HBOOST_SERIALIZATION_ASSUME_ABSTRACT_HPP
#define HBOOST_SERIALIZATION_ASSUME_ABSTRACT_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// assume_abstract_class.hpp:

// (C) Copyright 2008 Robert Ramey
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// this is useful for compilers which don't support the hboost::is_abstract

#include <hboost/type_traits/is_abstract.hpp>
#include <hboost/mpl/bool_fwd.hpp>

#ifndef HBOOST_NO_IS_ABSTRACT

// if there is an intrinsic is_abstract defined, we don't have to do anything
#define HBOOST_SERIALIZATION_ASSUME_ABSTRACT(T)

// but forward to the "official" is_abstract
namespace hboost {
namespace serialization {
    template<class T>
    struct is_abstract : hboost::is_abstract< T > {} ;
} // namespace serialization
} // namespace hboost

#else
// we have to "make" one

namespace hboost {
namespace serialization {
    template<class T>
    struct is_abstract : hboost::false_type {};
} // namespace serialization
} // namespace hboost

// define a macro to make explicit designation of this more transparent
#define HBOOST_SERIALIZATION_ASSUME_ABSTRACT(T)        \
namespace hboost {                                     \
namespace serialization {                             \
template<>                                            \
struct is_abstract< T > : hboost::true_type {};        \
template<>                                            \
struct is_abstract< const T > : hboost::true_type {};  \
}}                                                    \
/**/

#endif // HBOOST_NO_IS_ABSTRACT

#endif //HBOOST_SERIALIZATION_ASSUME_ABSTRACT_HPP
