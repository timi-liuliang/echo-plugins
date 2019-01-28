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
//! This header implements macros to define movable classes and
//! move-aware functions

#ifndef HBOOST_MOVE_CORE_HPP
#define HBOOST_MOVE_CORE_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/move/detail/config_begin.hpp>
#include <hboost/move/detail/workaround.hpp>

// @cond

//hboost_move_no_copy_constructor_or_assign typedef
//used to detect noncopyable types for other Boost libraries.
#if defined(HBOOST_NO_CXX11_DELETED_FUNCTIONS) || defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
   #define HBOOST_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE) \
      private:\
      TYPE(TYPE &);\
      TYPE& operator=(TYPE &);\
      public:\
      typedef int hboost_move_no_copy_constructor_or_assign; \
      private:\
   //
#else
   #define HBOOST_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE) \
      public:\
      TYPE(TYPE const &) = delete;\
      TYPE& operator=(TYPE const &) = delete;\
      public:\
      typedef int hboost_move_no_copy_constructor_or_assign; \
      private:\
   //
#endif   //HBOOST_NO_CXX11_DELETED_FUNCTIONS

// @endcond

#if defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(HBOOST_MOVE_DOXYGEN_INVOKED)

   #include <hboost/move/detail/type_traits.hpp>

   #if defined(HBOOST_MOVE_ADDRESS_SANITIZER_ON)
      #define HBOOST_MOVE_TO_RV_CAST(RV_TYPE, ARG) reinterpret_cast<RV_TYPE>(ARG)
   #else
      #define HBOOST_MOVE_TO_RV_CAST(RV_TYPE, ARG) static_cast<RV_TYPE>(ARG)
   #endif

   //Move emulation rv breaks standard aliasing rules so add workarounds for some compilers
   #if defined(__GNUC__) && (__GNUC__ >= 4) && \
      (\
         defined(HBOOST_GCC) ||   \
         (defined(HBOOST_INTEL) && (HBOOST_INTEL_CXX_VERSION >= 1300)) \
      )
      #define HBOOST_MOVE_ATTRIBUTE_MAY_ALIAS __attribute__((__may_alias__))
   #else
      #define HBOOST_MOVE_ATTRIBUTE_MAY_ALIAS
   #endif

   namespace hboost {

   //////////////////////////////////////////////////////////////////////////////
   //
   //                            struct rv
   //
   //////////////////////////////////////////////////////////////////////////////
   template <class T>
   class rv
      : public ::hboost::move_detail::if_c
         < ::hboost::move_detail::is_class<T>::value
         , T
         , ::hboost::move_detail::nat
         >::type
   {
      rv();
      ~rv() throw();
      rv(rv const&);
      void operator=(rv const&);
   } HBOOST_MOVE_ATTRIBUTE_MAY_ALIAS;


   //////////////////////////////////////////////////////////////////////////////
   //
   //                            is_rv
   //
   //////////////////////////////////////////////////////////////////////////////

   namespace move_detail {

   template <class T>
   struct is_rv
        //Derive from integral constant because some Boost code assummes it has
        //a "type" internal typedef
      : integral_constant<bool, ::hboost::move_detail::is_rv_impl<T>::value >
   {};

   template <class T>
   struct is_not_rv
   {
      static const bool value = !is_rv<T>::value;
   };

   }  //namespace move_detail {

   //////////////////////////////////////////////////////////////////////////////
   //
   //                               has_move_emulation_enabled
   //
   //////////////////////////////////////////////////////////////////////////////
   template<class T>
   struct has_move_emulation_enabled
      : ::hboost::move_detail::has_move_emulation_enabled_impl<T>
   {};

   template<class T>
   struct has_move_emulation_disabled
   {
      static const bool value = !::hboost::move_detail::has_move_emulation_enabled_impl<T>::value;
   };

   }  //namespace hboost {

   #define HBOOST_RV_REF(TYPE)\
      ::hboost::rv< TYPE >& \
   //

   #define HBOOST_RV_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      ::hboost::rv< TYPE<ARG1, ARG2> >& \
   //

   #define HBOOST_RV_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      ::hboost::rv< TYPE<ARG1, ARG2, ARG3> >& \
   //

   #define HBOOST_RV_REF_BEG\
      ::hboost::rv<   \
   //

   #define HBOOST_RV_REF_END\
      >& \
   //

   #define HBOOST_RV_REF_BEG_IF_CXX11 \
      \
   //

   #define HBOOST_RV_REF_END_IF_CXX11 \
      \
   //

   #define HBOOST_FWD_REF(TYPE)\
      const TYPE & \
   //

   #define HBOOST_COPY_ASSIGN_REF(TYPE)\
      const ::hboost::rv< TYPE >& \
   //

   #define HBOOST_COPY_ASSIGN_REF_BEG \
      const ::hboost::rv<  \
   //

   #define HBOOST_COPY_ASSIGN_REF_END \
      >& \
   //

   #define HBOOST_COPY_ASSIGN_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      const ::hboost::rv< TYPE<ARG1, ARG2> >& \
   //

   #define HBOOST_COPY_ASSIGN_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      const ::hboost::rv< TYPE<ARG1, ARG2, ARG3> >& \
   //

   #define HBOOST_CATCH_CONST_RLVALUE(TYPE)\
      const ::hboost::rv< TYPE >& \
   //

   namespace hboost {
   namespace move_detail {

   template <class Ret, class T>
   HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
      <  ::hboost::move_detail::is_lvalue_reference<Ret>::value ||
        !::hboost::has_move_emulation_enabled<T>::value
      , T&>::type
         move_return(T& x) HBOOST_NOEXCEPT
   {
      return x;
   }

   template <class Ret, class T>
   HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
      < !::hboost::move_detail::is_lvalue_reference<Ret>::value &&
         ::hboost::has_move_emulation_enabled<T>::value
      , ::hboost::rv<T>&>::type
         move_return(T& x) HBOOST_NOEXCEPT
   {
      return *HBOOST_MOVE_TO_RV_CAST(::hboost::rv<T>*, ::hboost::move_detail::addressof(x));
   }

   template <class Ret, class T>
   HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
      < !::hboost::move_detail::is_lvalue_reference<Ret>::value &&
         ::hboost::has_move_emulation_enabled<T>::value
      , ::hboost::rv<T>&>::type
         move_return(::hboost::rv<T>& x) HBOOST_NOEXCEPT
   {
      return x;
   }

   }  //namespace move_detail {
   }  //namespace hboost {

   #define HBOOST_MOVE_RET(RET_TYPE, REF)\
      hboost::move_detail::move_return< RET_TYPE >(REF)
   //

   #define HBOOST_MOVE_BASE(BASE_TYPE, ARG) \
      ::hboost::move((BASE_TYPE&)(ARG))
   //

   //////////////////////////////////////////////////////////////////////////////
   //
   //                         HBOOST_MOVABLE_BUT_NOT_COPYABLE
   //
   //////////////////////////////////////////////////////////////////////////////
   #define HBOOST_MOVABLE_BUT_NOT_COPYABLE(TYPE)\
      HBOOST_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE)\
      public:\
      HBOOST_MOVE_FORCEINLINE operator ::hboost::rv<TYPE>&() \
      {  return *HBOOST_MOVE_TO_RV_CAST(::hboost::rv<TYPE>*, this);  }\
      HBOOST_MOVE_FORCEINLINE operator const ::hboost::rv<TYPE>&() const \
      {  return *HBOOST_MOVE_TO_RV_CAST(const ::hboost::rv<TYPE>*, this);  }\
      private:\
   //

   //////////////////////////////////////////////////////////////////////////////
   //
   //                         HBOOST_COPYABLE_AND_MOVABLE
   //
   //////////////////////////////////////////////////////////////////////////////

   #define HBOOST_COPYABLE_AND_MOVABLE(TYPE)\
      public:\
      TYPE& operator=(TYPE &t)\
      {  this->operator=(const_cast<const TYPE &>(t)); return *this;}\
      public:\
      HBOOST_MOVE_FORCEINLINE operator ::hboost::rv<TYPE>&() \
      {  return *HBOOST_MOVE_TO_RV_CAST(::hboost::rv<TYPE>*, this);  }\
      HBOOST_MOVE_FORCEINLINE operator const ::hboost::rv<TYPE>&() const \
      {  return *HBOOST_MOVE_TO_RV_CAST(const ::hboost::rv<TYPE>*, this);  }\
      private:\
   //

   #define HBOOST_COPYABLE_AND_MOVABLE_ALT(TYPE)\
      public:\
      HBOOST_MOVE_FORCEINLINE operator ::hboost::rv<TYPE>&() \
      {  return *HBOOST_MOVE_TO_RV_CAST(::hboost::rv<TYPE>*, this);  }\
      HBOOST_MOVE_FORCEINLINE operator const ::hboost::rv<TYPE>&() const \
      {  return *HBOOST_MOVE_TO_RV_CAST(const ::hboost::rv<TYPE>*, this);  }\
      private:\
   //

   namespace hboost{
   namespace move_detail{

   template< class T>
   struct forward_type
   { typedef const T &type; };

   template< class T>
   struct forward_type< hboost::rv<T> >
   { typedef T type; };

   }}

#else    //HBOOST_NO_CXX11_RVALUE_REFERENCES

   //! This macro marks a type as movable but not copyable, disabling copy construction
   //! and assignment. The user will need to write a move constructor/assignment as explained
   //! in the documentation to fully write a movable but not copyable class.
   #define HBOOST_MOVABLE_BUT_NOT_COPYABLE(TYPE)\
      HBOOST_MOVE_IMPL_NO_COPY_CTOR_OR_ASSIGN(TYPE)\
      public:\
      typedef int hboost_move_emulation_t;\
      private:\
   //

   //! This macro marks a type as copyable and movable.
   //! The user will need to write a move constructor/assignment and a copy assignment
   //! as explained in the documentation to fully write a copyable and movable class.
   #define HBOOST_COPYABLE_AND_MOVABLE(TYPE)\
   //

   #if !defined(HBOOST_MOVE_DOXYGEN_INVOKED)
   #define HBOOST_COPYABLE_AND_MOVABLE_ALT(TYPE)\
   //
   #endif   //#if !defined(HBOOST_MOVE_DOXYGEN_INVOKED)

   namespace hboost {

   //!This trait yields to a compile-time true boolean if T was marked as
   //!HBOOST_MOVABLE_BUT_NOT_COPYABLE or HBOOST_COPYABLE_AND_MOVABLE and
   //!rvalue references are not available on the platform. False otherwise.
   template<class T>
   struct has_move_emulation_enabled
   {
      static const bool value = false;
   };

   template<class T>
   struct has_move_emulation_disabled
   {
      static const bool value = true;
   };

   }  //namespace hboost{

   //!This macro is used to achieve portable syntax in move
   //!constructors and assignments for classes marked as
   //!HBOOST_COPYABLE_AND_MOVABLE or HBOOST_MOVABLE_BUT_NOT_COPYABLE
   #define HBOOST_RV_REF(TYPE)\
      TYPE && \
   //

   //!This macro is used to achieve portable syntax in move
   //!constructors and assignments for template classes marked as
   //!HBOOST_COPYABLE_AND_MOVABLE or HBOOST_MOVABLE_BUT_NOT_COPYABLE.
   //!As macros have problems with comma-separated template arguments,
   //!the template argument must be preceded with HBOOST_RV_REF_BEG
   //!and ended with HBOOST_RV_REF_END
   #define HBOOST_RV_REF_BEG\
         \
   //

   //!This macro is used to achieve portable syntax in move
   //!constructors and assignments for template classes marked as
   //!HBOOST_COPYABLE_AND_MOVABLE or HBOOST_MOVABLE_BUT_NOT_COPYABLE.
   //!As macros have problems with comma-separated template arguments,
   //!the template argument must be preceded with HBOOST_RV_REF_BEG
   //!and ended with HBOOST_RV_REF_END
   #define HBOOST_RV_REF_END\
      && \
   //

   //!This macro expands to HBOOST_RV_REF_BEG if HBOOST_NO_CXX11_RVALUE_REFERENCES
   //!is not defined, empty otherwise
   #define HBOOST_RV_REF_BEG_IF_CXX11 \
      HBOOST_RV_REF_BEG \
   //

   //!This macro expands to HBOOST_RV_REF_END if HBOOST_NO_CXX11_RVALUE_REFERENCES
   //!is not defined, empty otherwise
   #define HBOOST_RV_REF_END_IF_CXX11 \
      HBOOST_RV_REF_END \
   //

   //!This macro is used to achieve portable syntax in copy
   //!assignment for classes marked as HBOOST_COPYABLE_AND_MOVABLE.
   #define HBOOST_COPY_ASSIGN_REF(TYPE)\
      const TYPE & \
   //

   //! This macro is used to implement portable perfect forwarding
   //! as explained in the documentation.
   #define HBOOST_FWD_REF(TYPE)\
      TYPE && \
   //

   #if !defined(HBOOST_MOVE_DOXYGEN_INVOKED)

   #define HBOOST_RV_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      TYPE<ARG1, ARG2> && \
   //

   #define HBOOST_RV_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      TYPE<ARG1, ARG2, ARG3> && \
   //

   #define HBOOST_COPY_ASSIGN_REF_BEG \
      const \
   //

   #define HBOOST_COPY_ASSIGN_REF_END \
      & \
   //

   #define HBOOST_COPY_ASSIGN_REF_2_TEMPL_ARGS(TYPE, ARG1, ARG2)\
      const TYPE<ARG1, ARG2> & \
   //

   #define HBOOST_COPY_ASSIGN_REF_3_TEMPL_ARGS(TYPE, ARG1, ARG2, ARG3)\
      const TYPE<ARG1, ARG2, ARG3>& \
   //

   #define HBOOST_CATCH_CONST_RLVALUE(TYPE)\
      const TYPE & \
   //

   #endif   //#if !defined(HBOOST_MOVE_DOXYGEN_INVOKED)

   #if !defined(HBOOST_MOVE_MSVC_AUTO_MOVE_RETURN_BUG) || defined(HBOOST_MOVE_DOXYGEN_INVOKED)

      //!This macro is used to achieve portable move return semantics.
      //!The C++11 Standard allows implicit move returns when the object to be returned
      //!is designated by a lvalue and:
      //!   - The criteria for elision of a copy operation are met OR
      //!   - The criteria would be met save for the fact that the source object is a function parameter
      //!
      //!For C++11 conforming compilers this macros only yields to REF:
      //! <code>return HBOOST_MOVE_RET(RET_TYPE, REF);</code> -> <code>return REF;</code>
      //!
      //!For compilers without rvalue references
      //!this macro does an explicit move if the move emulation is activated
      //!and the return type (RET_TYPE) is not a reference.
      //!
      //!For non-conforming compilers with rvalue references like Visual 2010 & 2012,
      //!an explicit move is performed if RET_TYPE is not a reference.
      //!
      //! <b>Caution</b>: When using this macro in non-conforming or C++03
      //!compilers, a move will be performed even if the C++11 standard does not allow it
      //!(e.g. returning a static variable). The user is responsible for using this macro
      //!only to return local objects that met C++11 criteria.
      #define HBOOST_MOVE_RET(RET_TYPE, REF)\
         REF
      //

   #else //!defined(HBOOST_MOVE_MSVC_AUTO_MOVE_RETURN_BUG) || defined(HBOOST_MOVE_DOXYGEN_INVOKED)

      #include <hboost/move/detail/meta_utils.hpp>

      namespace hboost {
      namespace move_detail {

      template <class Ret, class T>
      HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
         <  ::hboost::move_detail::is_lvalue_reference<Ret>::value
         , T&>::type
            move_return(T& x) HBOOST_NOEXCEPT
      {
         return x;
      }

      template <class Ret, class T>
      HBOOST_MOVE_FORCEINLINE typename ::hboost::move_detail::enable_if_c
         < !::hboost::move_detail::is_lvalue_reference<Ret>::value
         , Ret && >::type
            move_return(T&& t) HBOOST_NOEXCEPT
      {
         return static_cast< Ret&& >(t);
      }

      }  //namespace move_detail {
      }  //namespace hboost {

      #define HBOOST_MOVE_RET(RET_TYPE, REF)\
         hboost::move_detail::move_return< RET_TYPE >(REF)
      //

   #endif   //!defined(HBOOST_MOVE_MSVC_AUTO_MOVE_RETURN_BUG) || defined(HBOOST_MOVE_DOXYGEN_INVOKED)

   //!This macro is used to achieve portable optimal move constructors.
   //!
   //!When implementing the move constructor, in C++03 compilers the moved-from argument must be
   //!cast to the base type before calling `::hboost::move()` due to rvalue reference limitations.
   //!
   //!In C++11 compilers the cast from a rvalue reference of a derived type to a rvalue reference of
   //!a base type is implicit.
   #define HBOOST_MOVE_BASE(BASE_TYPE, ARG) \
      ::hboost::move((BASE_TYPE&)(ARG))
   //

   namespace hboost {
   namespace move_detail {

   template< class T> struct forward_type { typedef T type; };

   }}

#endif   //HBOOST_NO_CXX11_RVALUE_REFERENCES

#include <hboost/move/detail/config_end.hpp>

#endif //#ifndef HBOOST_MOVE_CORE_HPP
