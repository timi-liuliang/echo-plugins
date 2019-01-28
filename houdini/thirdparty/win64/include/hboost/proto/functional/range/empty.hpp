///////////////////////////////////////////////////////////////////////////////
/// \file empty.hpp
/// Proto callables for hboost::empty()
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_FUNCTIONAL_RANGE_EMPTY_HPP_EAN_27_08_2012
#define HBOOST_PROTO_FUNCTIONAL_RANGE_EMPTY_HPP_EAN_27_08_2012

#include <hboost/range/empty.hpp>
#include <hboost/proto/proto_fwd.hpp>

namespace hboost { namespace proto { namespace functional
{

    // A PolymorphicFunctionObject that wraps hboost::empty()
    struct empty
    {
        HBOOST_PROTO_CALLABLE()

        typedef bool result_type;

        template<typename Rng>
        bool operator()(Rng const &rng) const
        {
            return hboost::empty(rng);
        }
    };

}}}

#endif
