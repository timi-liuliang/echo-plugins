/*
 * Distributed under the Boost Software License, Version 1.0.(See accompanying 
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)
 * 
 * See http://www.boost.org/libs/iostreams for documentation.
 *
 * File:        hboost/iostreams/detail/file_handle.hpp
 * Date:        Sun Jun 22 14:23:12 MDT 2008
 * Copyright:   2008 CodeRage, LLC
 * Author:      Jonathan Turkanis
 * Contact:     turkanis at coderage dot com
 *
 * Defines the type hboost::iostreams::detail::file_handle, representing an
 * operating system file handle.
 */

#ifndef HBOOST_IOSTREAMS_DETAIL_FILE_HANDLE_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_FILE_HANDLE_HPP_INCLUDED

#include <hboost/iostreams/detail/config/windows_posix.hpp>

namespace hboost { namespace iostreams { namespace detail {

#ifdef HBOOST_IOSTREAMS_WINDOWS
    typedef void*  file_handle;  // A.k.a. HANDLE
#else
    typedef int    file_handle;
#endif

} } } // End namespaces detail, iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_FILE_HANDLE_HPP_INCLUDED
