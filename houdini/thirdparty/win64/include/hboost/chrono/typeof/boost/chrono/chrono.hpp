//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 20010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the unique_threader/unique_joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/chrono for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CHRONO_TYPEOF_CHRONO_HPP
#define HBOOST_CHRONO_TYPEOF_CHRONO_HPP

#include <hboost/chrono/chrono.hpp>
#include <hboost/typeof/typeof.hpp>

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::chrono::duration, (typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::chrono::time_point, (typename)(typename))
#if 0
HBOOST_TYPEOF_REGISTER_TYPE(hboost::chrono::system_clock)
#ifdef HBOOST_CHRONO_HAS_CLOCK_STEADY
HBOOST_TYPEOF_REGISTER_TYPE(hboost::chrono::steady_clock)
#endif
HBOOST_TYPEOF_REGISTER_TYPE(hboost::chrono::high_resolution_clock)

#endif
#endif
