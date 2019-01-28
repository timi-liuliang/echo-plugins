//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2012-2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
//////////////////////////////////////////////////////////////////////////////

//! \file
//! This header includes core utilities from <tt><hboost/move/utility_core.hpp></tt> and defines
//! some more advanced utilities such as:

#ifndef HBOOST_MOVE_MOVE_UTILITY_HPP
#define HBOOST_MOVE_MOVE_UTILITY_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/move/detail/config_begin.hpp>
#include <hboost/move/detail/workaround.hpp>  //forceinline
#include <hboost/move/utility_core.hpp>
#include <hboost/move/traits.hpp>

#if defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(HBOOST_MOVE_DOXYGEN_INVOKED)

   namespace hboost {

   //////////////////////////////////////////////////////////////////////////////
   //
   //                            move_if_noexcept()
   //
   //////////////////////////////////////////////////////////////////////////////

   template <class T>
   HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
      < enable_move_utility_emulation<T>::value && !has_move_emulation_enabled<T>::value
      , typename ::hboost::move_detail::add_const<T>::type &
      >::type
         move_if_noexcept(T& x) HBOOST_NOEXCEPT
   {
      return x;
   }

   template <class T>
   HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
      < enable_move_utility_emulation<T>::value && has_move_emulation_enabled<T>::value
            && ::hboost::move_detail::is_nothrow_move_constructible_or_uncopyable<T>::value, rv<T>&>::type
         move_if_noexcept(T& x) HBOOST_NOEXCEPT
   {
      return *static_cast<rv<T>* >(::hboost::move_detail::addressof(x));
   }

   template <class T>
   HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
      < enable_move_utility_emulation<T>::value && has_move_emulation_enabled<T>::value
            && ::hboost::move_detail::is_nothrow_move_constructible_or_uncopyable<T>::value
      , rv<T>&
      >::type
         move_if_noexcept(rv<T>& x) HBOOST_NOEXCEPT
   {
      return x;
   }

   template <class T>
   HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
      < enable_move_utility_emulation<T>::value && has_move_emulation_enabled<T>::value
            && !::hboost::move_detail::is_nothrow_move_constructible_or_uncopyable<T>::value
      , typename ::hboost::move_detail::add_const<T>::type &
      >::type
         move_if_noexcept(T& x) HBOOST_NOEXCEPT
   {
      return x;
   }

   template <class T>
   HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
      < enable_move_utility_emulation<T>::value && has_move_emulation_enabled<T>::value
            && !::hboost::move_detail::is_nothrow_move_constructible_or_uncopyable<T>::value
      , typename ::hboost::move_detail::add_const<T>::type &
      >::type
         move_if_noexcept(rv<T>& x) HBOOST_NOEXCEPT
   {
      return x;
   }

   }  //namespace hboost

#else    //#if defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(HBOOST_MOVE_DOXYGEN_INVOKED)

   #if defined(HBOOST_MOVE_USE_STANDARD_LIBRARY_MOVE)
      #include <utility>

      namespace hboost{

      using ::std::move_if_noexcept;

      }  //namespace hboost

   #else //!HBOOST_MOVE_USE_STANDARD_LIBRARY_MOVE

      namespace hboost {

      //////////////////////////////////////////////////////////////////////////////
      //
      //                            move_if_noexcept()
      //
      //////////////////////////////////////////////////////////////////////////////
      #if defined(HBOOST_MOVE_DOXYGEN_INVOKED)
         //! This function provides a way to convert a reference into a rvalue reference
         //! in compilers with rvalue references. For other compilers converts T & into
         //! <i>::hboost::rv<T> &</i> so that move emulation is activated. Reference
         //! would be converted to rvalue reference only if input type is nothrow move
         //! constructible or if it has no copy constructor. In all other cases const
         //! reference would be returned
         template <class T>
         rvalue_reference_or_const_lvalue_reference move_if_noexcept(input_reference) noexcept;

      #else //HBOOST_MOVE_DOXYGEN_INVOKED

         template <class T>
         HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
            < ::hboost::move_detail::is_nothrow_move_constructible_or_uncopyable<T>::value, T&&>::type
               move_if_noexcept(T& x) HBOOST_NOEXCEPT
         {  return ::hboost::move(x);   }

         template <class T>
         HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
            < !::hboost::move_detail::is_nothrow_move_constructible_or_uncopyable<T>::value, const T&>::type
               move_if_noexcept(T& x) HBOOST_NOEXCEPT
         {  return x;  }

      #endif //HBOOST_MOVE_DOXYGEN_INVOKED

      }  //namespace hboost {

   #endif   //#if defined(HBOOST_MOVE_USE_STANDARD_LIBRARY_MOVE)

#endif   //HBOOST_NO_CXX11_RVALUE_REFERENCES

#include <hboost/move/detail/config_end.hpp>

#endif //#ifndef HBOOST_MOVE_MOVE_UTILITY_HPP
