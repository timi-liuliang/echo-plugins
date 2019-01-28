/*=============================================================================
    Copyright (c) 2003 Jonathan de Halleux (dehalleux@pelikhan.com)
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef HBOOST_SPIRIT_ACTOR_DECREMENT_ACTOR_HPP
#define HBOOST_SPIRIT_ACTOR_DECREMENT_ACTOR_HPP

#include <hboost/spirit/home/classic/namespace.hpp>
#include <hboost/spirit/home/classic/actor/ref_actor.hpp>

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    ///////////////////////////////////////////////////////////////////////////
    //  Summary:
    //  A semantic action policy that calls the -- operator on a reference.
    //  (This doc uses convention available in actors.hpp)
    //
    //  Actions:
    //      --ref;
    //
    //  Policy name:
    //      decrement_action
    //
    //  Policy holder, corresponding helper method:
    //      ref_actor, decrement_a( ref );
    //
    //  () operators: both.
    //
    //  See also ref_actor for more details.
    ///////////////////////////////////////////////////////////////////////////
    struct decrement_action
    {
        template<
            typename T
        >
        void act(T& ref_) const
        {
            --ref_;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // helper method that creates a and_assign_actor.
    ///////////////////////////////////////////////////////////////////////////
    template<typename T>
    inline ref_actor<T,decrement_action> decrement_a(T& ref_)
    {
        return ref_actor<T,decrement_action>(ref_);
    }

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}}

#endif
