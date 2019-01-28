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

#ifndef HBOOST_CHRONO_TYPEOF_RATIO_HPP
#define HBOOST_CHRONO_TYPEOF_RATIO_HPP

#include <hboost/ratio/ratio.hpp>
#include <hboost/typeof/typeof.hpp>

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::ratio, (hboost::intmax_t)(hboost::intmax_t))

#endif
