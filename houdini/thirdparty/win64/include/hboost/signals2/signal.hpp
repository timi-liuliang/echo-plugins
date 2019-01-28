//  A thread-safe version of Boost.Signals.

// Copyright Frank Mori Hess 2007-2009
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef HBOOST_SIGNALS2_SIGNAL_HPP
#define HBOOST_SIGNALS2_SIGNAL_HPP

#include <algorithm>
#include <hboost/assert.hpp>
#include <hboost/config.hpp>
#include <hboost/function.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/noncopyable.hpp>
#include <hboost/shared_ptr.hpp>
#include <hboost/signals2/connection.hpp>
#include <hboost/signals2/detail/unique_lock.hpp>
#include <hboost/signals2/detail/replace_slot_function.hpp>
#include <hboost/signals2/detail/result_type_wrapper.hpp>
#include <hboost/signals2/detail/signals_common.hpp>
#include <hboost/signals2/detail/signals_common_macros.hpp>
#include <hboost/signals2/detail/slot_groups.hpp>
#include <hboost/signals2/detail/slot_call_iterator.hpp>
#include <hboost/signals2/optional_last_value.hpp>
#include <hboost/signals2/mutex.hpp>
#include <hboost/signals2/slot.hpp>
#include <functional>

#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
#include <hboost/signals2/preprocessed_signal.hpp>
#else
#include <hboost/signals2/variadic_signal.hpp>
#endif

namespace hboost
{
  namespace signals2
  {
    // free swap function, findable by ADL
    template<typename Signature,
      typename Combiner,
      typename Group,
      typename GroupCompare,
      typename SlotFunction,
      typename ExtendedSlotFunction,
      typename Mutex>
      void swap(
        signal<Signature, Combiner, Group, GroupCompare, SlotFunction, ExtendedSlotFunction, Mutex> &sig1,
        signal<Signature, Combiner, Group, GroupCompare, SlotFunction, ExtendedSlotFunction, Mutex> &sig2)
    {
      sig1.swap(sig2);
    }
  }
}

#endif // HBOOST_SIGNALS2_SIGNAL_HPP
