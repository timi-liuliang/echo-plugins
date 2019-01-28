//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2010-2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_MOVE_MOVE_HELPERS_HPP
#define HBOOST_MOVE_MOVE_HELPERS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/move/core.hpp>
#include <hboost/move/utility_core.hpp>
#include <hboost/move/detail/type_traits.hpp>

#if defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)

#define HBOOST_MOVE_CATCH_CONST(U)  \
   typename ::hboost::move_detail::if_< ::hboost::move_detail::is_class<U>, HBOOST_CATCH_CONST_RLVALUE(U), const U &>::type
#define HBOOST_MOVE_CATCH_RVALUE(U)\
   typename ::hboost::move_detail::if_< ::hboost::move_detail::is_class<U>, HBOOST_RV_REF(U), ::hboost::move_detail::nat>::type
#define HBOOST_MOVE_CATCH_FWD(U) HBOOST_FWD_REF(U)
#else
#define HBOOST_MOVE_CATCH_CONST(U)  const U &
#define HBOOST_MOVE_CATCH_RVALUE(U) U &&
#define HBOOST_MOVE_CATCH_FWD(U)    U &&
#endif

////////////////////////////////////////
//
// HBOOST_MOVE_CONVERSION_AWARE_CATCH
//
////////////////////////////////////////

#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES

   template<class RETURN_VALUE, class HBOOST_MOVE_TEMPL_PARAM, class TYPE>
   struct hboost_move_conversion_aware_catch_1
      : public ::hboost::move_detail::enable_if_and
                        < RETURN_VALUE
                        , ::hboost::move_detail::is_same<TYPE, HBOOST_MOVE_TEMPL_PARAM>
                        , ::hboost::move_detail::is_class<TYPE>
                        , ::hboost::has_move_emulation_disabled<HBOOST_MOVE_TEMPL_PARAM>
                        >
   {};

   template<class RETURN_VALUE, class HBOOST_MOVE_TEMPL_PARAM, class TYPE>
   struct hboost_move_conversion_aware_catch_2
      : public ::hboost::move_detail::disable_if_or
                        < RETURN_VALUE
                        , ::hboost::move_detail::is_same<TYPE, HBOOST_MOVE_TEMPL_PARAM> 
                        , ::hboost::move_detail::is_rv_impl<HBOOST_MOVE_TEMPL_PARAM>
                        , ::hboost::move_detail::and_
                                    < ::hboost::move_detail::is_rv_impl<HBOOST_MOVE_TEMPL_PARAM>
                                    , ::hboost::move_detail::is_class<HBOOST_MOVE_TEMPL_PARAM>
                                    >
                        >
   {};

   #define HBOOST_MOVE_CONVERSION_AWARE_CATCH_COMMON(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION)\
      RETURN_VALUE PUB_FUNCTION(HBOOST_MOVE_CATCH_CONST(TYPE) x)\
      {  return FWD_FUNCTION(static_cast<const TYPE&>(x)); }\
      \
      RETURN_VALUE PUB_FUNCTION(HBOOST_MOVE_CATCH_RVALUE(TYPE) x) \
      {  return FWD_FUNCTION(::hboost::move(x));  }\
      \
      RETURN_VALUE PUB_FUNCTION(TYPE &x)\
      {  return FWD_FUNCTION(const_cast<const TYPE &>(x)); }\
   //
   #if defined(HBOOST_MOVE_HELPERS_RETURN_SFINAE_BROKEN)
      #define HBOOST_MOVE_CONVERSION_AWARE_CATCH(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION)\
         HBOOST_MOVE_CONVERSION_AWARE_CATCH_COMMON(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION)\
         \
         template<class HBOOST_MOVE_TEMPL_PARAM>\
         RETURN_VALUE PUB_FUNCTION(const HBOOST_MOVE_TEMPL_PARAM &u,\
            typename hboost_move_conversion_aware_catch_1< ::hboost::move_detail::nat, HBOOST_MOVE_TEMPL_PARAM, TYPE>::type* = 0)\
         { return FWD_FUNCTION(u); }\
         \
         template<class HBOOST_MOVE_TEMPL_PARAM>\
         RETURN_VALUE PUB_FUNCTION(const HBOOST_MOVE_TEMPL_PARAM &u,\
            typename hboost_move_conversion_aware_catch_2< ::hboost::move_detail::nat, HBOOST_MOVE_TEMPL_PARAM, TYPE>::type* = 0)\
         {\
            TYPE t((u));\
            return FWD_FUNCTION(::hboost::move(t));\
         }\
      //
   #else
      #define HBOOST_MOVE_CONVERSION_AWARE_CATCH(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION)\
         HBOOST_MOVE_CONVERSION_AWARE_CATCH_COMMON(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION)\
         \
         template<class HBOOST_MOVE_TEMPL_PARAM>\
         typename hboost_move_conversion_aware_catch_1<RETURN_VALUE, HBOOST_MOVE_TEMPL_PARAM, TYPE>::type\
            PUB_FUNCTION(const HBOOST_MOVE_TEMPL_PARAM &u)\
         { return FWD_FUNCTION(u); }\
         \
         template<class HBOOST_MOVE_TEMPL_PARAM>\
         typename hboost_move_conversion_aware_catch_2<RETURN_VALUE, HBOOST_MOVE_TEMPL_PARAM, TYPE>::type\
            PUB_FUNCTION(const HBOOST_MOVE_TEMPL_PARAM &u)\
         {\
            TYPE t((u));\
            return FWD_FUNCTION(::hboost::move(t));\
         }\
      //
   #endif
#elif (defined(_MSC_VER) && (_MSC_VER == 1600))

   #define HBOOST_MOVE_CONVERSION_AWARE_CATCH(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION)\
      RETURN_VALUE PUB_FUNCTION(HBOOST_MOVE_CATCH_CONST(TYPE) x)\
      {  return FWD_FUNCTION(static_cast<const TYPE&>(x)); }\
      \
      RETURN_VALUE PUB_FUNCTION(HBOOST_MOVE_CATCH_RVALUE(TYPE) x) \
      {  return FWD_FUNCTION(::hboost::move(x));  }\
      \
      template<class HBOOST_MOVE_TEMPL_PARAM>\
      typename ::hboost::move_detail::enable_if_c\
                        < !::hboost::move_detail::is_same<TYPE, HBOOST_MOVE_TEMPL_PARAM>::value\
                        , RETURN_VALUE >::type\
      PUB_FUNCTION(const HBOOST_MOVE_TEMPL_PARAM &u)\
      {\
         TYPE t((u));\
         return FWD_FUNCTION(::hboost::move(t));\
      }\
   //

#else    //HBOOST_NO_CXX11_RVALUE_REFERENCES

   #define HBOOST_MOVE_CONVERSION_AWARE_CATCH(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION)\
      RETURN_VALUE PUB_FUNCTION(HBOOST_MOVE_CATCH_CONST(TYPE) x)\
      {  return FWD_FUNCTION(static_cast<const TYPE&>(x)); }\
      \
      RETURN_VALUE PUB_FUNCTION(HBOOST_MOVE_CATCH_RVALUE(TYPE) x) \
      {  return FWD_FUNCTION(::hboost::move(x));  }\
   //

#endif   //HBOOST_NO_CXX11_RVALUE_REFERENCES

////////////////////////////////////////
//
// HBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG
//
////////////////////////////////////////

#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES

   template<class RETURN_VALUE, class HBOOST_MOVE_TEMPL_PARAM, class UNLESS_CONVERTIBLE_TO, class TYPE>
   struct hboost_move_conversion_aware_catch_1arg_1
      : public ::hboost::move_detail::enable_if_and
                        < RETURN_VALUE
                        , ::hboost::move_detail::not_< ::hboost::move_detail::is_same_or_convertible<HBOOST_MOVE_TEMPL_PARAM, UNLESS_CONVERTIBLE_TO> >
                        , ::hboost::move_detail::is_same<TYPE, HBOOST_MOVE_TEMPL_PARAM>
                        , ::hboost::has_move_emulation_disabled<HBOOST_MOVE_TEMPL_PARAM>
                        >
   {};

   template<class RETURN_VALUE, class HBOOST_MOVE_TEMPL_PARAM, class UNLESS_CONVERTIBLE_TO, class TYPE>
   struct hboost_move_conversion_aware_catch_1arg_2
      : public ::hboost::move_detail::disable_if_or
                        < RETURN_VALUE
                        , ::hboost::move_detail::is_same_or_convertible< HBOOST_MOVE_TEMPL_PARAM, UNLESS_CONVERTIBLE_TO>
                        , ::hboost::move_detail::is_rv_impl<HBOOST_MOVE_TEMPL_PARAM>
                        , ::hboost::move_detail::is_same<TYPE, HBOOST_MOVE_TEMPL_PARAM>
                        >
   {};

   #define HBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG_COMMON(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION, ARG1, UNLESS_CONVERTIBLE_TO)\
      RETURN_VALUE PUB_FUNCTION(ARG1 arg1, HBOOST_MOVE_CATCH_CONST(TYPE) x)\
      {  return FWD_FUNCTION(arg1, static_cast<const TYPE&>(x)); }\
      \
      RETURN_VALUE PUB_FUNCTION(ARG1 arg1, HBOOST_MOVE_CATCH_RVALUE(TYPE) x) \
      {  return FWD_FUNCTION(arg1, ::hboost::move(x));  }\
      \
      RETURN_VALUE PUB_FUNCTION(ARG1 arg1, TYPE &x)\
      {  return FWD_FUNCTION(arg1, const_cast<const TYPE &>(x)); }\
   //
   #if defined(HBOOST_MOVE_HELPERS_RETURN_SFINAE_BROKEN)
      #define HBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION, ARG1, UNLESS_CONVERTIBLE_TO)\
         HBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG_COMMON(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION, ARG1, UNLESS_CONVERTIBLE_TO)\
         \
         template<class HBOOST_MOVE_TEMPL_PARAM>\
         RETURN_VALUE PUB_FUNCTION(ARG1 arg1, const HBOOST_MOVE_TEMPL_PARAM &u,\
            typename hboost_move_conversion_aware_catch_1arg_1<void, HBOOST_MOVE_TEMPL_PARAM, UNLESS_CONVERTIBLE_TO, TYPE>::type* = 0)\
         { return FWD_FUNCTION(arg1, u); }\
         \
         template<class HBOOST_MOVE_TEMPL_PARAM>\
         RETURN_VALUE PUB_FUNCTION(ARG1 arg1, const HBOOST_MOVE_TEMPL_PARAM &u,\
            typename hboost_move_conversion_aware_catch_1arg_2<void, HBOOST_MOVE_TEMPL_PARAM, UNLESS_CONVERTIBLE_TO, TYPE>::type* = 0)\
         {\
            TYPE t((u));\
            return FWD_FUNCTION(arg1, ::hboost::move(t));\
         }\
      //
   #else
      #define HBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION, ARG1, UNLESS_CONVERTIBLE_TO)\
         HBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG_COMMON(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION, ARG1, UNLESS_CONVERTIBLE_TO)\
         \
         template<class HBOOST_MOVE_TEMPL_PARAM>\
         typename hboost_move_conversion_aware_catch_1arg_1<RETURN_VALUE, HBOOST_MOVE_TEMPL_PARAM, UNLESS_CONVERTIBLE_TO, TYPE>::type\
            PUB_FUNCTION(ARG1 arg1, const HBOOST_MOVE_TEMPL_PARAM &u)\
         { return FWD_FUNCTION(arg1, u); }\
         \
         template<class HBOOST_MOVE_TEMPL_PARAM>\
         typename hboost_move_conversion_aware_catch_1arg_2<RETURN_VALUE, HBOOST_MOVE_TEMPL_PARAM, UNLESS_CONVERTIBLE_TO, TYPE>::type\
            PUB_FUNCTION(ARG1 arg1, const HBOOST_MOVE_TEMPL_PARAM &u)\
         {\
            TYPE t((u));\
            return FWD_FUNCTION(arg1, ::hboost::move(t));\
         }\
      //
   #endif

#elif (defined(_MSC_VER) && (_MSC_VER == 1600))

   #define HBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION, ARG1, UNLESS_CONVERTIBLE_TO)\
      RETURN_VALUE PUB_FUNCTION(ARG1 arg1, HBOOST_MOVE_CATCH_CONST(TYPE) x)\
      {  return FWD_FUNCTION(arg1, static_cast<const TYPE&>(x)); }\
      \
      RETURN_VALUE PUB_FUNCTION(ARG1 arg1, HBOOST_MOVE_CATCH_RVALUE(TYPE) x) \
      {  return FWD_FUNCTION(arg1, ::hboost::move(x));  }\
      \
      template<class HBOOST_MOVE_TEMPL_PARAM>\
      typename ::hboost::move_detail::disable_if_or\
                        < RETURN_VALUE \
                        , ::hboost::move_detail::is_same<TYPE, HBOOST_MOVE_TEMPL_PARAM> \
                        , ::hboost::move_detail::is_same_or_convertible<HBOOST_MOVE_TEMPL_PARAM, UNLESS_CONVERTIBLE_TO> \
                        >::type\
      PUB_FUNCTION(ARG1 arg1, const HBOOST_MOVE_TEMPL_PARAM &u)\
      {\
         TYPE t((u));\
         return FWD_FUNCTION(arg1, ::hboost::move(t));\
      }\
   //

#else

   #define HBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG(PUB_FUNCTION, TYPE, RETURN_VALUE, FWD_FUNCTION, ARG1, UNLESS_CONVERTIBLE_TO)\
      RETURN_VALUE PUB_FUNCTION(ARG1 arg1, HBOOST_MOVE_CATCH_CONST(TYPE) x)\
      {  return FWD_FUNCTION(arg1, static_cast<const TYPE&>(x)); }\
      \
      RETURN_VALUE PUB_FUNCTION(ARG1 arg1, HBOOST_MOVE_CATCH_RVALUE(TYPE) x) \
      {  return FWD_FUNCTION(arg1, ::hboost::move(x));  }\
   //

#endif

#endif //#ifndef HBOOST_MOVE_MOVE_HELPERS_HPP
