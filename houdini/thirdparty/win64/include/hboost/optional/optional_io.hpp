// Copyright (C) 2005, Fernando Luis Cacciola Carballal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/optional for documentation.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef HBOOST_OPTIONAL_OPTIONAL_IO_FLC_19NOV2002_HPP
#define HBOOST_OPTIONAL_OPTIONAL_IO_FLC_19NOV2002_HPP

#include <istream>
#include <ostream>

#include "hboost/none.hpp"
#include "hboost/optional/optional.hpp"


namespace hboost
{

template<class CharType, class CharTrait>
inline
std::basic_ostream<CharType, CharTrait>&
operator<<(std::basic_ostream<CharType, CharTrait>& out, none_t)
{
  if (out.good())
  {
    out << "--";
  }
   
  return out;
}

template<class CharType, class CharTrait, class T>
inline
std::basic_ostream<CharType, CharTrait>&
operator<<(std::basic_ostream<CharType, CharTrait>& out, optional<T> const& v)
{
  if (out.good())
  {
    if (!v)
         out << "--" ;
    else out << ' ' << *v ;
  }

  return out;
}

template<class CharType, class CharTrait, class T>
inline
std::basic_istream<CharType, CharTrait>&
operator>>(std::basic_istream<CharType, CharTrait>& in, optional<T>& v)
{
  if (in.good())
  {
    int d = in.get();
    if (d == ' ')
    {
      T x;
      in >> x;
#ifndef  HBOOST_OPTIONAL_DETAIL_NO_RVALUE_REFERENCES
      v = hboost::move(x);
#else
      v = x;
#endif
    }
    else
    {
      if (d == '-')
      {
        d = in.get();

        if (d == '-')
        {
          v = none;
          return in;
        }
      }

      in.setstate( std::ios::failbit );
    }
  }

  return in;
}

} // namespace hboost

#endif

