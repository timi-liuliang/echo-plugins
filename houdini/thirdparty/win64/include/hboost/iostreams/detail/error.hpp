// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_ERROR_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_ERROR_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              
                 
#include <hboost/iostreams/detail/ios.hpp>  // failure.

namespace hboost { namespace iostreams { namespace detail {

inline HBOOST_IOSTREAMS_FAILURE cant_read() 
{ return HBOOST_IOSTREAMS_FAILURE("no read access"); }

inline HBOOST_IOSTREAMS_FAILURE cant_write() 
{ return HBOOST_IOSTREAMS_FAILURE("no write access"); }

inline HBOOST_IOSTREAMS_FAILURE cant_seek() 
{ return HBOOST_IOSTREAMS_FAILURE("no random access"); }

inline HBOOST_IOSTREAMS_FAILURE bad_read() 
{ return HBOOST_IOSTREAMS_FAILURE("bad read"); }

inline HBOOST_IOSTREAMS_FAILURE bad_putback() 
{ return HBOOST_IOSTREAMS_FAILURE("putback buffer full"); }

inline HBOOST_IOSTREAMS_FAILURE bad_write() 
{ return HBOOST_IOSTREAMS_FAILURE("bad write"); }

inline HBOOST_IOSTREAMS_FAILURE write_area_exhausted() 
{ return HBOOST_IOSTREAMS_FAILURE("write area exhausted"); }

inline HBOOST_IOSTREAMS_FAILURE bad_seek() 
{ return HBOOST_IOSTREAMS_FAILURE("bad seek"); }

} } } // End namespaces detail, iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_ERROR_HPP_INCLUDED
