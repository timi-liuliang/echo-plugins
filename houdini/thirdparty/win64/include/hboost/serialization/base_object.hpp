#ifndef HBOOST_SERIALIZATION_BASE_OBJECT_HPP
#define HBOOST_SERIALIZATION_BASE_OBJECT_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// base_object.hpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// if no archive headers have been included this is a no op
// this is to permit HBOOST_EXPORT etc to be included in a 
// file declaration header

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>

#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/identity.hpp>

#include <hboost/type_traits/is_base_and_derived.hpp>
#include <hboost/type_traits/is_pointer.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/type_traits/is_polymorphic.hpp>

#include <hboost/static_assert.hpp>
#include <hboost/serialization/access.hpp>
#include <hboost/serialization/force_include.hpp>
#include <hboost/serialization/void_cast_fwd.hpp>

namespace hboost {
namespace serialization {

namespace detail
{
    // get the base type for a given derived type
    // preserving the const-ness
    template<class B, class D>
    struct base_cast
    {
        typedef typename
        mpl::if_<
            is_const<D>,
            const B,
            B
        >::type type;
        HBOOST_STATIC_ASSERT(is_const<type>::value == is_const<D>::value);
    };

    // only register void casts if the types are polymorphic
    template<class Base, class Derived>
    struct base_register
    {
        struct polymorphic {
            static void const * invoke(){
                Base const * const b = 0;
                Derived const * const d = 0;
                return & void_cast_register(d, b);
            }
        };
        struct non_polymorphic {
            static void const * invoke(){
                return 0;
            }
        };
        static void const * invoke(){
            typedef typename mpl::eval_if<
                is_polymorphic<Base>,
                mpl::identity<polymorphic>,
                mpl::identity<non_polymorphic>
            >::type type;
            return type::invoke();
        }
    };

} // namespace detail
template<class Base, class Derived>
typename detail::base_cast<Base, Derived>::type & 
base_object(Derived &d)
{
    HBOOST_STATIC_ASSERT(( is_base_and_derived<Base,Derived>::value));
    HBOOST_STATIC_ASSERT(! is_pointer<Derived>::value);
    typedef typename detail::base_cast<Base, Derived>::type type;
    detail::base_register<type, Derived>::invoke();
    return access::cast_reference<type, Derived>(d);
}

} // namespace serialization
} // namespace hboost

#endif // HBOOST_SERIALIZATION_BASE_OBJECT_HPP
