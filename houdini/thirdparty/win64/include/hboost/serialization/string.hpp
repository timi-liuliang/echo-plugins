#ifndef  HBOOST_SERIALIZATION_STRING_HPP
#define HBOOST_SERIALIZATION_STRING_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// serialization/string.hpp:
// serialization for stl string templates

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>

#include <hboost/config.hpp>
#include <hboost/serialization/level.hpp>

HBOOST_CLASS_IMPLEMENTATION(std::string, hboost::serialization::primitive_type)
#ifndef HBOOST_NO_STD_WSTRING
HBOOST_CLASS_IMPLEMENTATION(std::wstring, hboost::serialization::primitive_type)
#endif

#endif // HBOOST_SERIALIZATION_STRING_HPP