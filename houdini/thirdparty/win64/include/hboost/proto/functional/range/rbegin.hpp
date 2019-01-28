///////////////////////////////////////////////////////////////////////////////
/// \file rbegin.hpp
/// Proto callables for hboost::rbegin()
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_FUNCTIONAL_RANGE_RBEGIN_HPP_EAN_27_08_2012
#define HBOOST_PROTO_FUNCTIONAL_RANGE_RBEGIN_HPP_EAN_27_08_2012

#include <hboost/range/rbegin.hpp>
#include <hboost/proto/proto_fwd.hpp>

namespace hboost { namespace proto { namespace functional
{

    // A PolymorphicFunctionObject that wraps hboost::rbegin()
    struct rbegin
    {
        HBOOST_PROTO_CALLABLE()

        template<typename Sig>
        struct result;

        template<typename This, typename Rng>
        struct result<This(Rng)>
          : hboost::range_reverse_iterator<Rng const>
        {};

        template<typename This, typename Rng>
        struct result<This(Rng &)>
          : hboost::range_reverse_iterator<Rng>
        {};

        template<typename Rng>
        typename hboost::range_reverse_iterator<Rng>::type operator()(Rng &rng) const
        {
            return hboost::rbegin(rng);
        }

        template<typename Rng>
        typename hboost::range_reverse_iterator<Rng const>::type operator()(Rng const &rng) const
        {
            return hboost::rbegin(rng);
        }
    };

}}}

#endif
