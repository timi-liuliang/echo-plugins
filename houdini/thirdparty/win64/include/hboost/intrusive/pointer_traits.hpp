//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Pablo Halpern 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2011-2014. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_POINTER_TRAITS_HPP
#define HBOOST_INTRUSIVE_POINTER_TRAITS_HPP

#include <hboost/intrusive/detail/config_begin.hpp>
#include <hboost/intrusive/detail/workaround.hpp>
#include <hboost/intrusive/pointer_rebind.hpp>
#include <hboost/intrusive/detail/pointer_element.hpp>
#include <hboost/intrusive/detail/mpl.hpp>
#include <cstddef>

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace hboost {
namespace intrusive {
namespace detail {

#if !defined(HBOOST_MSVC) || (HBOOST_MSVC > 1310)
HBOOST_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_pointer_to, pointer_to)
HBOOST_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_dynamic_cast_from, dynamic_cast_from)
HBOOST_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_static_cast_from, static_cast_from)
HBOOST_INTRUSIVE_HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_const_cast_from, const_cast_from)
#else
HBOOST_INTRUSIVE_HAS_MEMBER_FUNC_CALLED_IGNORE_SIGNATURE(has_member_function_callable_with_pointer_to, pointer_to)
HBOOST_INTRUSIVE_HAS_MEMBER_FUNC_CALLED_IGNORE_SIGNATURE(has_member_function_callable_with_dynamic_cast_from, dynamic_cast_from)
HBOOST_INTRUSIVE_HAS_MEMBER_FUNC_CALLED_IGNORE_SIGNATURE(has_member_function_callable_with_static_cast_from, static_cast_from)
HBOOST_INTRUSIVE_HAS_MEMBER_FUNC_CALLED_IGNORE_SIGNATURE(has_member_function_callable_with_const_cast_from, const_cast_from)
#endif

HBOOST_INTRUSIVE_INSTANTIATE_EVAL_DEFAULT_TYPE_TMPLT(element_type)
HBOOST_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(difference_type)
HBOOST_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(reference)
HBOOST_INTRUSIVE_INSTANTIATE_DEFAULT_TYPE_TMPLT(value_traits_ptr)

}  //namespace detail {


//! pointer_traits is the implementation of C++11 std::pointer_traits class with some
//! extensions like castings.
//!
//! pointer_traits supplies a uniform interface to certain attributes of pointer-like types.
//!
//! <b>Note</b>: When defining a custom family of pointers or references to be used with BI
//! library, make sure the public static conversion functions accessed through
//! the `pointer_traits` interface (`*_cast_from` and `pointer_to`) can
//! properly convert between const and nonconst referred member types
//! <b>without the use of implicit constructor calls</b>. It is suggested these
//! conversions be implemented as function templates, where the template
//! argument is the type of the object being converted from.
template <typename Ptr>
struct pointer_traits
{
   #ifdef HBOOST_INTRUSIVE_DOXYGEN_INVOKED
      //!The pointer type
      //!queried by this pointer_traits instantiation
      typedef Ptr             pointer;

      //!Ptr::element_type if such a type exists; otherwise, T if Ptr is a class
      //!template instantiation of the form SomePointer<T, Args>, where Args is zero or
      //!more type arguments ; otherwise , the specialization is ill-formed.
      typedef unspecified_type element_type;

      //!Ptr::difference_type if such a type exists; otherwise,
      //!std::ptrdiff_t.
      typedef unspecified_type difference_type;

      //!Ptr::rebind<U> if such a type exists; otherwise, SomePointer<U, Args> if Ptr is
      //!a class template instantiation of the form SomePointer<T, Args>, where Args is zero or
      //!more type arguments ; otherwise, the instantiation of rebind is ill-formed.
      //!
      //!For portable code for C++03 and C++11, <pre>typename rebind_pointer<U>::type</pre>
      //!shall be used instead of rebind<U> to obtain a pointer to U.
      template <class U> using rebind = unspecified;

      //!Ptr::reference if such a type exists (non-standard extension); otherwise, element_type &
      //!
      typedef unspecified_type reference;
   #else
      typedef Ptr                                                             pointer;
      //
      typedef HBOOST_INTRUSIVE_OBTAIN_TYPE_WITH_EVAL_DEFAULT
         ( hboost::intrusive::detail::, Ptr, element_type
         , hboost::intrusive::detail::first_param<Ptr>)                        element_type;
      //
      typedef HBOOST_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT
         (hboost::intrusive::detail::, Ptr, difference_type, std::ptrdiff_t)   difference_type;

      typedef HBOOST_INTRUSIVE_OBTAIN_TYPE_WITH_DEFAULT
         (hboost::intrusive::detail::, Ptr, reference, typename hboost::intrusive::detail::unvoid_ref<element_type>::type)   reference;
      //
      template <class U> struct rebind_pointer
      {
         typedef typename hboost::intrusive::pointer_rebind<Ptr, U>::type  type;
      };

      #if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)
         template <class U> using rebind = typename hboost::intrusive::pointer_rebind<Ptr, U>::type;
      #endif
   #endif   //#if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)

   //! <b>Remark</b>: If element_type is (possibly cv-qualified) void, r type is unspecified; otherwise,
   //!   it is element_type &.
   //!
   //! <b>Returns</b>: A dereferenceable pointer to r obtained by calling Ptr::pointer_to(reference).
   //!   Non-standard extension: If such function does not exist, returns pointer(addressof(r));
   //!
   //! <b>Note</b>: For non-conforming compilers only the existence of a member function called
   //!   <code>pointer_to</code> is checked.
   static pointer pointer_to(reference r)
   {
      //Non-standard extension, it does not require Ptr::pointer_to. If not present
      //tries to converts &r to pointer.
      const bool value = hboost::intrusive::detail::
         has_member_function_callable_with_pointer_to
            <Ptr, Ptr (*)(reference)>::value;
      hboost::intrusive::detail::bool_<value> flag;
      return pointer_traits::priv_pointer_to(flag, r);
   }

   //! <b>Remark</b>: Non-standard extension.
   //!
   //! <b>Returns</b>: A dereferenceable pointer to r obtained by calling the static template function
   //!   Ptr::static_cast_from(UPpr/const UPpr &).
   //!   If such function does not exist, returns pointer_to(static_cast<element_type&>(*uptr))
   //!
   //! <b>Note</b>: For non-conforming compilers only the existence of a member function called
   //!   <code>static_cast_from</code> is checked.
   template<class UPtr>
   static pointer static_cast_from(const UPtr &uptr)
   {
      typedef const UPtr &RefArg;
      const bool value = hboost::intrusive::detail::
         has_member_function_callable_with_static_cast_from
            <pointer, pointer(*)(RefArg)>::value
         || hboost::intrusive::detail::
               has_member_function_callable_with_static_cast_from
                  <pointer, pointer(*)(UPtr)>::value;
      return pointer_traits::priv_static_cast_from(hboost::intrusive::detail::bool_<value>(), uptr);
   }

   //! <b>Remark</b>: Non-standard extension.
   //!
   //! <b>Returns</b>: A dereferenceable pointer to r obtained by calling the static template function
   //!   Ptr::const_cast_from<UPtr>(UPpr/const UPpr &).
   //!   If such function does not exist, returns pointer_to(const_cast<element_type&>(*uptr))
   //!
   //! <b>Note</b>: For non-conforming compilers only the existence of a member function called
   //!   <code>const_cast_from</code> is checked.
   template<class UPtr>
   static pointer const_cast_from(const UPtr &uptr)
   {
      typedef const UPtr &RefArg;
      const bool value = hboost::intrusive::detail::
         has_member_function_callable_with_const_cast_from
            <pointer, pointer(*)(RefArg)>::value
         || hboost::intrusive::detail::
               has_member_function_callable_with_const_cast_from
                  <pointer, pointer(*)(UPtr)>::value;
      return pointer_traits::priv_const_cast_from(hboost::intrusive::detail::bool_<value>(), uptr);
   }

   //! <b>Remark</b>: Non-standard extension.
   //!
   //! <b>Returns</b>: A dereferenceable pointer to r obtained by calling the static template function
   //!   Ptr::dynamic_cast_from<UPtr>(UPpr/const UPpr &).
   //!   If such function does not exist, returns pointer_to(*dynamic_cast<element_type*>(&*uptr))
   //!
   //! <b>Note</b>: For non-conforming compilers only the existence of a member function called
   //!   <code>dynamic_cast_from</code> is checked.
   template<class UPtr>
   static pointer dynamic_cast_from(const UPtr &uptr)
   {
      typedef const UPtr &RefArg;
      const bool value = hboost::intrusive::detail::
         has_member_function_callable_with_dynamic_cast_from
            <pointer, pointer(*)(RefArg)>::value
         || hboost::intrusive::detail::
               has_member_function_callable_with_dynamic_cast_from
                  <pointer, pointer(*)(UPtr)>::value;
      return pointer_traits::priv_dynamic_cast_from(hboost::intrusive::detail::bool_<value>(), uptr);
   }

   ///@cond
   private:
   //priv_to_raw_pointer
   template <class T>
   static T* to_raw_pointer(T* p)
   {  return p; }

   template <class Pointer>
   static typename pointer_traits<Pointer>::element_type*
      to_raw_pointer(const Pointer &p)
   {  return pointer_traits::to_raw_pointer(p.operator->());  }

   //priv_pointer_to
   static pointer priv_pointer_to(hboost::intrusive::detail::true_, reference r)
   { return Ptr::pointer_to(r); }

   static pointer priv_pointer_to(hboost::intrusive::detail::false_, reference r)
   { return pointer(hboost::intrusive::detail::addressof(r)); }

   //priv_static_cast_from
   template<class UPtr>
   static pointer priv_static_cast_from(hboost::intrusive::detail::true_, const UPtr &uptr)
   { return Ptr::static_cast_from(uptr); }

   template<class UPtr>
   static pointer priv_static_cast_from(hboost::intrusive::detail::false_, const UPtr &uptr)
   {  return uptr ? pointer_to(*static_cast<element_type*>(to_raw_pointer(uptr))) : pointer();  }

   //priv_const_cast_from
   template<class UPtr>
   static pointer priv_const_cast_from(hboost::intrusive::detail::true_, const UPtr &uptr)
   { return Ptr::const_cast_from(uptr); }

   template<class UPtr>
   static pointer priv_const_cast_from(hboost::intrusive::detail::false_, const UPtr &uptr)
   {  return uptr ? pointer_to(const_cast<element_type&>(*uptr)) : pointer();  }

   //priv_dynamic_cast_from
   template<class UPtr>
   static pointer priv_dynamic_cast_from(hboost::intrusive::detail::true_, const UPtr &uptr)
   { return Ptr::dynamic_cast_from(uptr); }

   template<class UPtr>
   static pointer priv_dynamic_cast_from(hboost::intrusive::detail::false_, const UPtr &uptr)
   {  return uptr ? pointer_to(dynamic_cast<element_type&>(*uptr)) : pointer();  }
   ///@endcond
};

///@cond

// Remove cv qualification from Ptr parameter to pointer_traits:
template <typename Ptr>
struct pointer_traits<const Ptr> : pointer_traits<Ptr> {};
template <typename Ptr>
struct pointer_traits<volatile Ptr> : pointer_traits<Ptr> { };
template <typename Ptr>
struct pointer_traits<const volatile Ptr> : pointer_traits<Ptr> { };
// Remove reference from Ptr parameter to pointer_traits:
template <typename Ptr>
struct pointer_traits<Ptr&> : pointer_traits<Ptr> { };

///@endcond

//! Specialization of pointer_traits for raw pointers
//!
template <typename T>
struct pointer_traits<T*>
{
   typedef T            element_type;
   typedef T*           pointer;
   typedef std::ptrdiff_t difference_type;

   #ifdef HBOOST_INTRUSIVE_DOXYGEN_INVOKED
      typedef T &          reference;
      //!typedef for <pre>U *</pre>
      //!
      //!For portable code for C++03 and C++11, <pre>typename rebind_pointer<U>::type</pre>
      //!shall be used instead of rebind<U> to obtain a pointer to U.
      template <class U> using rebind = U*;
   #else
      typedef typename hboost::intrusive::detail::unvoid_ref<element_type>::type reference;
      #if !defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)
         template <class U> using rebind = U*;
      #endif
   #endif

   template <class U> struct rebind_pointer
   {  typedef U* type;  };

   //! <b>Returns</b>: addressof(r)
   //!
   HBOOST_INTRUSIVE_FORCEINLINE static pointer pointer_to(reference r)
   { return hboost::intrusive::detail::addressof(r); }

   //! <b>Returns</b>: static_cast<pointer>(uptr)
   //!
   template<class U>
   HBOOST_INTRUSIVE_FORCEINLINE static pointer static_cast_from(U *uptr)
   {  return static_cast<pointer>(uptr);  }

   //! <b>Returns</b>: const_cast<pointer>(uptr)
   //!
   template<class U>
   HBOOST_INTRUSIVE_FORCEINLINE static pointer const_cast_from(U *uptr)
   {  return const_cast<pointer>(uptr);  }

   //! <b>Returns</b>: dynamic_cast<pointer>(uptr)
   //!
   template<class U>
   HBOOST_INTRUSIVE_FORCEINLINE static pointer dynamic_cast_from(U *uptr)
   {  return dynamic_cast<pointer>(uptr);  }
};

}  //namespace container {
}  //namespace hboost {

#include <hboost/intrusive/detail/config_end.hpp>

#endif // ! defined(HBOOST_INTRUSIVE_POINTER_TRAITS_HPP)
