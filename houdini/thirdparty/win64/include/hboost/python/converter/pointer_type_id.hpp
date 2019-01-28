// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_POINTER_TYPE_ID_DWA2002222_HPP
# define HBOOST_POINTER_TYPE_ID_DWA2002222_HPP

# include <hboost/python/type_id.hpp>
# include <hboost/type_traits/composite_traits.hpp>

namespace hboost { namespace python { namespace converter { 

namespace detail
{
  template <bool is_ref = false>
  struct pointer_typeid_select
  {
      template <class T>
      static inline type_info execute(T*(*)() = 0)
      {
          return type_id<T>();
      }
  };

  template <>
  struct pointer_typeid_select<true>
  {
      template <class T>
      static inline type_info execute(T* const volatile&(*)() = 0)
      {
          return type_id<T>();
      }
    
      template <class T>
      static inline type_info execute(T*volatile&(*)() = 0)
      {
          return type_id<T>();
      }
    
      template <class T>
      static inline type_info execute(T*const&(*)() = 0)
      {
          return type_id<T>();
      }

      template <class T>
      static inline type_info execute(T*&(*)() = 0)
      {
          return type_id<T>();
      }
  };
}

// Usage: pointer_type_id<T>()
//
// Returns a type_info associated with the type pointed
// to by T, which may be a pointer or a reference to a pointer.
template <class T>
type_info pointer_type_id(T(*)() = 0)
{
    return detail::pointer_typeid_select<
          is_reference<T>::value
        >::execute((T(*)())0);
}

}}} // namespace hboost::python::converter

#endif // HBOOST_POINTER_TYPE_ID_DWA2002222_HPP
