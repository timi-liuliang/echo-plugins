// ----------------------------------------------------------------------------
// msvc_disambiguater.hpp : msvc workarounds. (for put_{head|last} overloads)
//               the trick was described in boost's list  by Aleksey Gurtovoy
// ----------------------------------------------------------------------------

//  Copyright Samuel Krempp 2003. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// see http://www.boost.org/libs/format for library home page

// ----------------------------------------------------------------------------

#ifndef HBOOST_MSVC_DISAMBIGUATER_HPP
#define HBOOST_MSVC_DISAMBIGUATER_HPP

#if HBOOST_WORKAROUND(__DECCXX_VER, HBOOST_TESTED_AT(60590042))

#include <hboost/format/group.hpp>
#include <ostream>

namespace hboost {
namespace io {
namespace detail {

template< class Ch, class Tr, class T >
struct disambiguater
{
   template< typename U >
   static void put_head(HBOOST_IO_STD basic_ostream<Ch, Tr>& os, group1<U> const& x, long)
   {
       os << group_head(x.a1_); 
   }
   static void put_head(HBOOST_IO_STD basic_ostream<Ch, Tr>& os, T const& x, int)
   {
   }
   template< typename U >
   static void put_last(HBOOST_IO_STD basic_ostream<Ch, Tr>& os, group1<U> const& x, long)
   {
       os << group_last(x.a1_); 
   }
   static void put_last(HBOOST_IO_STD basic_ostream<Ch, Tr>& os, T const& x, int)
   {
     os << x;
   }
};

} // namespace detail
} // namespace io
} // namespace hboost

#endif // -__DECCXX_VER

#endif // -HBOOST_MSVC_DISAMBIGUATER_HPP
