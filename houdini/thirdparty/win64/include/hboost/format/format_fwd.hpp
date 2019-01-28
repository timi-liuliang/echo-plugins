// ----------------------------------------------------------------------------
//  format_fwd.hpp :  forward declarations
// ----------------------------------------------------------------------------

//  Copyright Samuel Krempp 2003. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/format for library home page

// ----------------------------------------------------------------------------

#ifndef HBOOST_FORMAT_FWD_HPP
#define HBOOST_FORMAT_FWD_HPP

#include <string>
#include <iosfwd>

#include <hboost/format/detail/compat_workarounds.hpp> 

namespace hboost {

    template <class Ch, 
        class Tr = HBOOST_IO_STD char_traits<Ch>, class Alloc = std::allocator<Ch> >
    class basic_format;

    typedef basic_format<char >     format;

#if !defined(HBOOST_NO_STD_WSTRING)  && !defined(HBOOST_NO_STD_WSTREAMBUF)
    typedef basic_format<wchar_t >  wformat;
#endif

    namespace io {
        enum format_error_bits { bad_format_string_bit = 1, 
                                 too_few_args_bit = 2, too_many_args_bit = 4,
                                 out_of_range_bit = 8,
                                 all_error_bits = 255, no_error_bits=0 };
                  
    } // namespace io

} // namespace hboost

#endif // HBOOST_FORMAT_FWD_HPP
