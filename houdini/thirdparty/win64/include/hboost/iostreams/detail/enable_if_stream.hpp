// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_ENABLE_IF_STREAM_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_ENABLE_IF_STREAM_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <hboost/config.hpp>                // HBOOST_NO_SFINAE.
#include <hboost/utility/enable_if.hpp>                  
#include <hboost/iostreams/traits_fwd.hpp>  // is_std_io.

#if !defined(HBOOST_NO_SFINAE) && \
    !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x592))
# define HBOOST_IOSTREAMS_ENABLE_IF_STREAM(T) \
    , typename hboost::enable_if< hboost::iostreams::is_std_io<T> >::type* = 0 \
    /**/
# define HBOOST_IOSTREAMS_DISABLE_IF_STREAM(T) \
    , typename hboost::disable_if< hboost::iostreams::is_std_io<T> >::type* = 0 \
    /**/
#else 
# define HBOOST_IOSTREAMS_ENABLE_IF_STREAM(T)
# define HBOOST_IOSTREAMS_DISABLE_IF_STREAM(T)
#endif

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_ENABLE_IF_STREAM_HPP_INCLUDED
