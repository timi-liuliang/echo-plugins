// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_KEYWORDS_DWA2002323_HPP
# define HBOOST_KEYWORDS_DWA2002323_HPP

# include <hboost/python/detail/prefix.hpp>

# include <hboost/python/args_fwd.hpp>
# include <hboost/config.hpp>
# include <hboost/python/detail/preprocessor.hpp>
# include <hboost/python/detail/type_list.hpp>

# include <hboost/type_traits/is_reference.hpp>
# include <hboost/type_traits/remove_reference.hpp>
# include <hboost/type_traits/remove_cv.hpp>

# include <hboost/preprocessor/enum_params.hpp>
# include <hboost/preprocessor/repeat.hpp>
# include <hboost/preprocessor/facilities/intercept.hpp>
# include <hboost/preprocessor/iteration/local.hpp>

# include <hboost/python/detail/mpl_lambda.hpp>
# include <hboost/python/object_core.hpp>

# include <hboost/mpl/bool.hpp>

# include <cstddef>
# include <algorithm>

namespace hboost { namespace python {

typedef detail::keywords<1> arg;
typedef arg arg_; // gcc 2.96 workaround

namespace detail
{
  template <std::size_t nkeywords>
  struct keywords_base
  {
      HBOOST_STATIC_CONSTANT(std::size_t, size = nkeywords);
      
      keyword_range range() const
      {
          return keyword_range(elements, elements + nkeywords);
      }

      keyword elements[nkeywords];

      keywords<nkeywords+1>
      operator,(python::arg const &k) const;

      keywords<nkeywords + 1>
      operator,(char const *name) const;
  };
  
  template <std::size_t nkeywords>
  struct keywords : keywords_base<nkeywords>
  {
  };

  template <>
  struct keywords<1> : keywords_base<1>
  {
      explicit keywords(char const *name)
      {
          elements[0].name = name;
      }
    
      template <class T>
      python::arg& operator=(T const& value)
      {
          object z(value);
          elements[0].default_value = handle<>(python::borrowed(object(value).ptr()));
          return *this;
      }
    
      operator detail::keyword const&() const
      {
          return elements[0];
      }
  };

  template <std::size_t nkeywords>
  inline
  keywords<nkeywords+1>
  keywords_base<nkeywords>::operator,(python::arg const &k) const
  {
      keywords<nkeywords> const& l = *static_cast<keywords<nkeywords> const*>(this);
      python::detail::keywords<nkeywords+1> res;
      std::copy(l.elements, l.elements+nkeywords, res.elements);
      res.elements[nkeywords] = k.elements[0];
      return res;
  }

  template <std::size_t nkeywords>
  inline
  keywords<nkeywords + 1>
  keywords_base<nkeywords>::operator,(char const *name) const
  {
      return this->operator,(python::arg(name));
  }

  template<typename T>
  struct is_keywords
  {
      HBOOST_STATIC_CONSTANT(bool, value = false); 
  };

  template<std::size_t nkeywords>
  struct is_keywords<keywords<nkeywords> >
  {
      HBOOST_STATIC_CONSTANT(bool, value = true);
  };
  template <class T>
  struct is_reference_to_keywords
  {
      HBOOST_STATIC_CONSTANT(bool, is_ref = is_reference<T>::value);
      typedef typename remove_reference<T>::type deref;
      typedef typename remove_cv<deref>::type key_t;
      HBOOST_STATIC_CONSTANT(bool, is_key = is_keywords<key_t>::value);
      HBOOST_STATIC_CONSTANT(bool, value = (is_ref & is_key));
      
      typedef mpl::bool_<value> type;
      HBOOST_PYTHON_MPL_LAMBDA_SUPPORT(1,is_reference_to_keywords,(T))
  };
}

inline detail::keywords<1> args(char const* name)
{ 
    return detail::keywords<1>(name);
}

#  define HBOOST_PYTHON_ASSIGN_NAME(z, n, _) result.elements[n].name = name##n;
#  define HBOOST_PP_LOCAL_MACRO(n)                                               \
inline detail::keywords<n> args(HBOOST_PP_ENUM_PARAMS_Z(1, n, char const* name)) \
{                                                                               \
    detail::keywords<n> result;                                                 \
    HBOOST_PP_REPEAT_1(n, HBOOST_PYTHON_ASSIGN_NAME, _)                           \
    return result;                                                              \
}
#  define HBOOST_PP_LOCAL_LIMITS (2, HBOOST_PYTHON_MAX_ARITY)
#  include HBOOST_PP_LOCAL_ITERATE()

}} // namespace hboost::python


# endif // HBOOST_KEYWORDS_DWA2002323_HPP
