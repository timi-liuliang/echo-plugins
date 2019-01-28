///////////////////////////////////////////////////////////////////////////////
/// \file end.hpp
/// Proto callables for hboost::end()
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_FUNCTIONAL_RANGE_END_HPP_EAN_27_08_2012
#define HBOOST_PROTO_FUNCTIONAL_RANGE_END_HPP_EAN_27_08_2012

#include <hboost/range/end.hpp>
#include <hboost/proto/proto_fwd.hpp>

namespace hboost { namespace proto { namespace functional
{

    // A PolymorphicFunctionObject that wraps hboost::end()
    struct end
    {
        HBOOST_PROTO_CALLABLE()

        template<typename Sig>
        struct result;

        template<typename This, typename Rng>
        struct result<This(Rng)>
          : hboost::range_iterator<Rng const>
        {};

        template<typename This, typename Rng>
        struct result<This(Rng &)>
          : hboost::range_iterator<Rng>
        {};

        template<typename Rng>
        typename hboost::range_iterator<Rng>::type operator()(Rng &rng) const
        {
            return hboost::end(rng);
        }

        template<typename Rng>
        typename hboost::range_iterator<Rng const>::type operator()(Rng const &rng) const
        {
            return hboost::end(rng);
        }
    };

}}}

#endif
