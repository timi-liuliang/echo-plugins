// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_IOS_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_IOS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              
                 
#include <hboost/config.hpp> // HBOOST_MSVC.
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/detail/config/wide_streams.hpp>
#ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES
# if !HBOOST_WORKAROUND(__MWERKS__, <= 0x3003)
#  include <ios>
# else
#  include <istream>
#  include <ostream>
# endif
#else 
# include <exception>
# include <iosfwd>
#endif 

namespace hboost { namespace iostreams { namespace detail {

#ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES //--------------------------------//
# define HBOOST_IOSTREAMS_BASIC_IOS(ch, tr)  std::basic_ios< ch, tr >
# if !HBOOST_WORKAROUND(__MWERKS__, <= 0x3003) && \
     !HBOOST_WORKAROUND(__BORLANDC__, < 0x600) && \
     !HBOOST_WORKAROUND(HBOOST_MSVC, < 1300) \
     /**/

#define HBOOST_IOS                std::ios
#define HBOOST_IOSTREAMS_FAILURE  std::ios::failure

# else

#define HBOOST_IOS                std::ios_base
#define HBOOST_IOSTREAMS_FAILURE  std::ios_base::failure

# endif
#else // #ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES //-----------------------//

#define HBOOST_IOS                          std::ios
#define HBOOST_IOSTREAMS_BASIC_IOS(ch, tr)  std::ios
#define HBOOST_IOSTREAMS_FAILURE            hboost::iostreams::detail::failure

class failure : std::exception {    
public:
    explicit failure(const std::string& what_arg) : what_(what_arg) { }
    const char* what() const { return what_.c_str(); }
private:
    std::string what_;
};

#endif // #ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES //----------------------//

} } } // End namespace failure, iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_IOS_HPP_INCLUDED
