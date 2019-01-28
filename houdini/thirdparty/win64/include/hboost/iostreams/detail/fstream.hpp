// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_FSTREAM_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_FSTREAM_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              
                 
#include <hboost/iostreams/detail/config/wide_streams.hpp>
#ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES
# include <fstream>
#else
# include <fstream.h>
#endif 

#ifndef HBOOST_IOSTREAMS_NO_STREAM_TEMPLATES
# define HBOOST_IOSTREAMS_BASIC_IFSTREAM(Ch, Tr) std::basic_ifstream<Ch, Tr>
# define HBOOST_IOSTREAMS_BASIC_OFSTREAM(Ch, Tr) std::basic_ofstream<Ch, Tr>
# define HBOOST_IOSTREAMS_BASIC_FSTREAM(Ch, Tr) std::basic_fstream<Ch, Tr>
# define HBOOST_IOSTREAMS_BASIC_FILEBUF(Ch) std::basic_filebuf<Ch>
#else 
# define HBOOST_IOSTREAMS_BASIC_IFSTREAM(Ch, Tr) std::ifstream
# define HBOOST_IOSTREAMS_BASIC_OFSTREAM(Ch, Tr) std::ofstream
# define HBOOST_IOSTREAMS_BASIC_FILEBUF(Ch) std::filebuf
#endif

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_FSTREAM_HPP_INCLUDED
