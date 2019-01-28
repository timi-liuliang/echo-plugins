//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : simple facilities for accessing type information at runtime
// ***************************************************************************

#ifndef HBOOST_TEST_UTILS_RTTI_HPP
#define HBOOST_TEST_UTILS_RTTI_HPP

// C Runtime
#include <cstddef>

namespace hboost {
namespace rtti {

// ************************************************************************** //
// **************                   rtti::type_id              ************** //
// ************************************************************************** //

typedef std::ptrdiff_t id_t;

namespace rtti_detail {

template<typename T>
struct rttid_holder {
    static id_t id() { return reinterpret_cast<id_t>( &inst() ); }

private:
    struct rttid {};

    static rttid const& inst() { static rttid s_inst; return s_inst; }
};

} // namespace rtti_detail

//____________________________________________________________________________//

template<typename T>
inline id_t
type_id()
{
    return rtti_detail::rttid_holder<T>::id();
}

//____________________________________________________________________________//

#define HBOOST_RTTI_SWITCH( type_id_ ) if( ::hboost::rtti::id_t switch_by_id = type_id_ )
#define HBOOST_RTTI_CASE( type )       if( switch_by_id == ::hboost::rtti::type_id<type>() )

//____________________________________________________________________________//

} // namespace rtti
} // namespace hboost

#endif // HBOOST_TEST_UTILS_RTTI_HPP
