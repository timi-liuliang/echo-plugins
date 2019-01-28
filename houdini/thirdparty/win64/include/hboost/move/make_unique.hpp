//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2006-2014. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_MOVE_MAKE_UNIQUE_HPP_INCLUDED
#define HBOOST_MOVE_MAKE_UNIQUE_HPP_INCLUDED

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/move/detail/config_begin.hpp>
#include <hboost/move/detail/workaround.hpp>
#include <hboost/move/utility_core.hpp>
#include <hboost/move/unique_ptr.hpp>
#include <cstddef>   //for std::size_t
#include <hboost/move/detail/unique_ptr_meta_utils.hpp>
#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
#  include <hboost/move/detail/fwd_macros.hpp>
#endif

//!\file
//! Defines "make_unique" functions, which are factories to create instances
//! of unique_ptr depending on the passed arguments.
//!
//! This header can be a bit heavyweight in C++03 compilers due to the use of the
//! preprocessor library, that's why it's a a separate header from <tt>unique_ptr.hpp</tt>
 
#if !defined(HBOOST_MOVE_DOXYGEN_INVOKED)

namespace std {   //no namespace versioning in clang+libc++

struct nothrow_t;

}  //namespace std {

namespace hboost{
namespace move_upmu {

//Compile time switch between
//single element, unknown bound array
//and known bound array
template<class T>
struct unique_ptr_if
{
   typedef ::hboost::movelib::unique_ptr<T> t_is_not_array;
};

template<class T>
struct unique_ptr_if<T[]>
{
   typedef ::hboost::movelib::unique_ptr<T[]> t_is_array_of_unknown_bound;
};

template<class T, std::size_t N>
struct unique_ptr_if<T[N]>
{
   typedef void t_is_array_of_known_bound;
};

template <int Dummy = 0>
struct nothrow_holder
{
   static std::nothrow_t *pnothrow;   
};

template <int Dummy>
std::nothrow_t *nothrow_holder<Dummy>::pnothrow;

}  //namespace move_upmu {
}  //namespace hboost{

#endif   //!defined(HBOOST_MOVE_DOXYGEN_INVOKED)

namespace hboost{
namespace movelib {

#if defined(HBOOST_MOVE_DOXYGEN_INVOKED) || !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is not an array.
//!
//! <b>Returns</b>: <tt>unique_ptr<T>(new T(std::forward<Args>(args)...))</tt>.
template<class T, class... Args>
inline HBOOST_MOVE_DOC1ST(unique_ptr<T>, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_not_array)
      make_unique(HBOOST_FWD_REF(Args)... args)
{  return unique_ptr<T>(new T(::hboost::forward<Args>(args)...));  }

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is not an array.
//!
//! <b>Returns</b>: <tt>unique_ptr<T>(new T(std::nothrow)(std::forward<Args>(args)...))</tt>.
template<class T, class... Args>
inline HBOOST_MOVE_DOC1ST(unique_ptr<T>, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_not_array)
      make_unique_nothrow(HBOOST_FWD_REF(Args)... args)
{  return unique_ptr<T>(new (*hboost::move_upmu::nothrow_holder<>::pnothrow)T(::hboost::forward<Args>(args)...));  }

#else
   #define HBOOST_MOVE_MAKE_UNIQUE_CODE(N)\
      template<class T HBOOST_MOVE_I##N HBOOST_MOVE_CLASS##N>\
      typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_not_array\
         make_unique( HBOOST_MOVE_UREF##N)\
      {  return unique_ptr<T>( new T( HBOOST_MOVE_FWD##N ) );  }\
      \
      template<class T HBOOST_MOVE_I##N HBOOST_MOVE_CLASS##N>\
      typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_not_array\
         make_unique_nothrow( HBOOST_MOVE_UREF##N)\
      {  return unique_ptr<T>( new (*hboost::move_upmu::nothrow_holder<>::pnothrow)T ( HBOOST_MOVE_FWD##N ) );  }\
      //
   HBOOST_MOVE_ITERATE_0TO9(HBOOST_MOVE_MAKE_UNIQUE_CODE)
   #undef HBOOST_MOVE_MAKE_UNIQUE_CODE

#endif

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is not an array.
//!
//! <b>Returns</b>: <tt>unique_ptr<T>(new T)</tt> (default initialization)
template<class T>
inline HBOOST_MOVE_DOC1ST(unique_ptr<T>, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_not_array)
      make_unique_definit()
{
    return unique_ptr<T>(new T);
}

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is not an array.
//!
//! <b>Returns</b>: <tt>unique_ptr<T>(new T(std::nothrow)</tt> (default initialization)
template<class T>
inline HBOOST_MOVE_DOC1ST(unique_ptr<T>, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_not_array)
      make_unique_nothrow_definit()
{
    return unique_ptr<T>(new (*hboost::move_upmu::nothrow_holder<>::pnothrow)T);
}

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is an array of 
//!   unknown bound.
//!
//! <b>Returns</b>: <tt>unique_ptr<T>(new remove_extent_t<T>[n]())</tt> (value initialization)
template<class T>
inline HBOOST_MOVE_DOC1ST(unique_ptr<T>, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_array_of_unknown_bound)
      make_unique(std::size_t n)
{
    typedef typename ::hboost::move_upmu::remove_extent<T>::type U;
    return unique_ptr<T>(new U[n]());
}

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is an array of 
//!   unknown bound.
//!
//! <b>Returns</b>: <tt>unique_ptr<T>(new (std::nothrow)remove_extent_t<T>[n]())</tt> (value initialization)
template<class T>
inline HBOOST_MOVE_DOC1ST(unique_ptr<T>, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_array_of_unknown_bound)
      make_unique_nothrow(std::size_t n)
{
    typedef typename ::hboost::move_upmu::remove_extent<T>::type U;
    return unique_ptr<T>(new (*hboost::move_upmu::nothrow_holder<>::pnothrow)U[n]());
}

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is an array of 
//!   unknown bound.
//!
//! <b>Returns</b>: <tt>unique_ptr<T>(new remove_extent_t<T>[n])</tt> (default initialization)
template<class T>
inline HBOOST_MOVE_DOC1ST(unique_ptr<T>, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_array_of_unknown_bound)
      make_unique_definit(std::size_t n)
{
    typedef typename ::hboost::move_upmu::remove_extent<T>::type U;
    return unique_ptr<T>(new U[n]);
}

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is an array of 
//!   unknown bound.
//!
//! <b>Returns</b>: <tt>unique_ptr<T>(new (std::nothrow)remove_extent_t<T>[n])</tt> (default initialization)
template<class T>
inline HBOOST_MOVE_DOC1ST(unique_ptr<T>, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_array_of_unknown_bound)
      make_unique_nothrow_definit(std::size_t n)
{
    typedef typename ::hboost::move_upmu::remove_extent<T>::type U;
    return unique_ptr<T>(new (*hboost::move_upmu::nothrow_holder<>::pnothrow) U[n]);
}

#if !defined(HBOOST_NO_CXX11_DELETED_FUNCTIONS)

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is
//!   an array of known bound.
template<class T, class... Args>
inline HBOOST_MOVE_DOC1ST(unspecified, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_array_of_known_bound)
      make_unique(HBOOST_FWD_REF(Args) ...) = delete;

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is
//!   an array of known bound.
template<class T, class... Args>
inline HBOOST_MOVE_DOC1ST(unspecified, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_array_of_known_bound)
      make_unique_definit(HBOOST_FWD_REF(Args) ...) = delete;

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is
//!   an array of known bound.
template<class T, class... Args>
inline HBOOST_MOVE_DOC1ST(unspecified, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_array_of_known_bound)
      make_unique_nothrow(HBOOST_FWD_REF(Args) ...) = delete;

//! <b>Remarks</b>: This function shall not participate in overload resolution unless T is
//!   an array of known bound.
template<class T, class... Args>
inline HBOOST_MOVE_DOC1ST(unspecified, 
   typename ::hboost::move_upmu::unique_ptr_if<T>::t_is_array_of_known_bound)
      make_unique_nothrow_definit(HBOOST_FWD_REF(Args) ...) = delete;

#endif

}  //namespace movelib {

}  //namespace hboost{

#include <hboost/move/detail/config_end.hpp>

#endif   //#ifndef HBOOST_MOVE_MAKE_UNIQUE_HPP_INCLUDED
