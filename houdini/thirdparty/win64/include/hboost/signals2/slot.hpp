// Boost.Signals2 library

// Copyright Frank Mori Hess 2009.
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef HBOOST_SIGNALS2_SLOT_HPP
#define HBOOST_SIGNALS2_SLOT_HPP

#include <hboost/bind.hpp>
#include <hboost/config.hpp>
#include <hboost/function.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/ref.hpp>
#include <hboost/signals2/detail/signals_common.hpp>
#include <hboost/signals2/detail/signals_common_macros.hpp>
#include <hboost/signals2/detail/tracked_objects_visitor.hpp>
#include <hboost/signals2/slot_base.hpp>
#include <hboost/visit_each.hpp>
#include <hboost/weak_ptr.hpp>

#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
#include <hboost/signals2/preprocessed_slot.hpp>
#else
#include <hboost/signals2/variadic_slot.hpp>
#endif

#endif // HBOOST_SIGNALS2_SLOT_HPP
