/*
 * Distributed under the Boost Software License, Version 1.0.(See accompanying 
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)
 * 
 * See http://www.boost.org/libs/iostreams for documentation.
 *
 * File:        hboost/iostreams/detail/restrict.hpp
 * Date:        Sun Jan 06 12:57:30 MST 2008
 * Copyright:   2008 CodeRage, LLC
                2004-2007 Jonathan Turkanis
 * Author:      Jonathan Turkanis
 * Contact:     turkanis at coderage dot com
 *
 * Defines the class template hboost::iostreams::restriction and the 
 * overloaded function template hboost::iostreams::slice.
 *
 * This header is provided for platforms on which "restrict" is a keyword.
 */

#ifndef HBOOST_IOSTREAMS_RESTRICT_HPP_INCLUDED
#define HBOOST_IOSTREAMS_RESTRICT_HPP_INCLUDED

#include <hboost/iostreams/detail/restrict_impl.hpp>
#define HBOOST_IOSTREAMS_RESTRICT slice
#include <hboost/iostreams/detail/restrict_impl.hpp>
#undef HBOOST_IOSTREAMS_RESTRICT

#endif // #ifndef HBOOST_IOSTREAMS_RESTRICT_HPP_INCLUDED
