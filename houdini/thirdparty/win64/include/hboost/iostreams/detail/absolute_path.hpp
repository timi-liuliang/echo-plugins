/*
 * Distributed under the Boost Software License, Version 1.0.(See accompanying 
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)
 * 
 * See http://www.boost.org/libs/iostreams for documentation.

 * File:        hboost/iostreams/detail/execute.hpp
 * Date:        Thu Dec 06 13:21:54 MST 2007
 * Copyright:   2007-2008 CodeRage, LLC
 * Author:      Jonathan Turkanis
 * Contact:     turkanis at coderage dot com
 *
 * Defines the function hboost::iostreams::detail::absolute_path, used for 
 * debug output for mapped files.
 */

#ifndef HBOOST_IOSTREAMS_DETAIL_ABSOLUTE_PATH_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_ABSOLUTE_PATH_HPP_INCLUDED

#include <string>
#include <hboost/iostreams/detail/config/windows_posix.hpp>
#ifdef HBOOST_IOSTREAMS_WINDOWS
# include <cctype>
#endif
#include <hboost/iostreams/detail/current_directory.hpp>

namespace hboost { namespace iostreams { namespace detail {

// Resolves the given path relative to the current working directory
inline std::string absolute_path(const std::string& path)
{
#ifdef HBOOST_IOSTREAMS_WINDOWS
    return path.size() && (path[0] == '/' || path[0] == '\\') ||
           path.size() > 1 && std::isalpha(path[0]) && path[1] == ':' ?
               path :
               current_directory() + '\\' + path;
#else // #ifdef HBOOST_IOSTREAMS_WINDOWS
    return path.size() && (path[0] == '/') ?
        path :
        current_directory() + '/' + path;
#endif // #ifdef HBOOST_IOSTREAMS_WINDOWS
}

} } } // End namespaces detail, iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_ABSOLUTE_PATH_HPP_INCLUDED
