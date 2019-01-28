#ifndef HBOOST_SMART_PTR_HPP_INCLUDED
#define HBOOST_SMART_PTR_HPP_INCLUDED

//
//  smart_ptr.hpp
//
//  For convenience, this header includes the rest of the smart
//  pointer library headers.
//
//  Copyright (c) 2003 Peter Dimov  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  http://www.boost.org/libs/smart_ptr/smart_ptr.htm
//

#include <hboost/config.hpp>

#include <hboost/scoped_ptr.hpp>
#include <hboost/scoped_array.hpp>
#include <hboost/shared_ptr.hpp>
#include <hboost/shared_array.hpp>

#if !defined(HBOOST_NO_MEMBER_TEMPLATES) || defined(HBOOST_MSVC6_MEMBER_TEMPLATES)
# include <hboost/weak_ptr.hpp>
# include <hboost/intrusive_ptr.hpp>
# include <hboost/enable_shared_from_this.hpp>
# include <hboost/make_shared.hpp>
#endif

#endif // #ifndef HBOOST_SMART_PTR_HPP_INCLUDED
