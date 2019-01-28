// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_STRING_LITERAL_DWA2002629_HPP
# define HBOOST_STRING_LITERAL_DWA2002629_HPP

# include <cstddef>
# include <hboost/type.hpp>
# include <hboost/type_traits/array_traits.hpp>
# include <hboost/type_traits/same_traits.hpp>
# include <hboost/mpl/bool.hpp>
# include <hboost/detail/workaround.hpp>

namespace hboost { namespace python { namespace detail { 

template <class T>
struct is_string_literal : mpl::false_
{
};

#  if !defined(__MWERKS__) || __MWERKS__ > 0x2407
template <std::size_t n>
struct is_string_literal<char const[n]> : mpl::true_
{
};

#   if HBOOST_WORKAROUND(__DECCXX_VER, HBOOST_TESTED_AT(60590040)) \
  || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730)
// This compiler mistakenly gets the type of string literals as char*
// instead of char[NN].
template <>
struct is_string_literal<char* const> : mpl::true_
{
};
#   endif

#  else

// CWPro7 has trouble with the array type deduction above
template <class T, std::size_t n>
struct is_string_literal<T[n]>
    : is_same<T, char const>
{
};
#  endif 

}}} // namespace hboost::python::detail

#endif // HBOOST_STRING_LITERAL_DWA2002629_HPP
