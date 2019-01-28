/*
  A thread-safe version of Boost.Signals.

  Author: Frank Mori Hess <fmhess@users.sourceforge.net>
  Begin: 2007-01-23
*/
// Copyright Frank Mori Hess 2007-2008
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef HBOOST_SIGNALS2_PREPROCESSED_SIGNAL_HPP
#define HBOOST_SIGNALS2_PREPROCESSED_SIGNAL_HPP

#include <hboost/preprocessor/arithmetic.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/control/expr_if.hpp>
#include <hboost/preprocessor/iteration.hpp>
#include <hboost/preprocessor/repetition.hpp>
#include <hboost/signals2/detail/preprocessed_arg_type.hpp>
#include <hboost/type_traits/add_reference.hpp>
#include <hboost/type_traits/is_void.hpp> 
#include <hboost/utility/enable_if.hpp>

#define HBOOST_PP_ITERATION_LIMITS (0, HBOOST_SIGNALS2_MAX_ARGS)
#define HBOOST_PP_FILENAME_1 <hboost/signals2/detail/signal_template.hpp>
#include HBOOST_PP_ITERATE()

namespace hboost
{
  namespace signals2
  {
    template<typename Signature,
      typename Combiner = optional_last_value<typename hboost::function_traits<Signature>::result_type>,
      typename Group = int,
      typename GroupCompare = std::less<Group>,
      typename SlotFunction = function<Signature>,
      typename ExtendedSlotFunction = typename detail::extended_signature<function_traits<Signature>::arity, Signature>::function_type,
      typename Mutex = mutex >
    class signal: public detail::signalN<function_traits<Signature>::arity,
      Signature, Combiner, Group, GroupCompare, SlotFunction, ExtendedSlotFunction, Mutex>::type
    {
    private:
      typedef typename detail::signalN<hboost::function_traits<Signature>::arity,
        Signature, Combiner, Group, GroupCompare, SlotFunction, ExtendedSlotFunction, Mutex>::type base_type;
    public:
      signal(const Combiner &combiner_arg = Combiner(), const GroupCompare &group_compare = GroupCompare()):
        base_type(combiner_arg, group_compare)
      {}
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && HBOOST_WORKAROUND(HBOOST_MSVC, < 1800)
      signal(signal && other) : base_type(std::move(other)) {}
      signal & operator=(signal && other) { base_type::operator=(std::move(other)); return *this; }
#endif
    };
  }
}

#endif // HBOOST_SIGNALS2_PREPROCESSED_SIGNAL_HPP
