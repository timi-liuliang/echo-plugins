// Copyright (C) 2005 Arkadiy Vertleyb, Peder Holt.
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_STD_string_hpp_INCLUDED
#define HBOOST_TYPEOF_STD_string_hpp_INCLUDED

#include <string>
#include <hboost/typeof/typeof.hpp>
#include <hboost/typeof/std/memory.hpp>

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

HBOOST_TYPEOF_REGISTER_TEMPLATE(std::char_traits, 1)
HBOOST_TYPEOF_REGISTER_TEMPLATE(std::basic_string, 1)
HBOOST_TYPEOF_REGISTER_TEMPLATE(std::basic_string, 2)
HBOOST_TYPEOF_REGISTER_TEMPLATE(std::basic_string, 3)

#ifndef __BORLANDC__
//Borland chokes on this "double definition" of string
HBOOST_TYPEOF_REGISTER_TYPE(std::string)
#endif

#endif//HBOOST_TYPEOF_STD_string_hpp_INCLUDED
