// Copyright (C) 2012 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// 2013/04 Vicente J. Botet Escriba
//    Provide implementation up to 9 parameters when HBOOST_NO_CXX11_VARIADIC_TEMPLATES is defined.
//    Make use of Boost.Move
//    Make use of Boost.Tuple (movable)
// 2012/11 Vicente J. Botet Escriba
//    Adapt to boost libc++ implementation

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// The invoker code is based on the one from libcxx.
//===----------------------------------------------------------------------===//

#ifndef HBOOST_THREAD_DETAIL_INVOKER_HPP
#define HBOOST_THREAD_DETAIL_INVOKER_HPP

#include <hboost/config.hpp>

#include <hboost/utility/result_of.hpp>
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/detail/invoke.hpp>
#include <hboost/thread/detail/make_tuple_indices.hpp>
#include <hboost/thread/csbl/tuple.hpp>
#include <hboost/tuple/tuple.hpp>

#include <hboost/thread/detail/variadic_header.hpp>

namespace hboost
{
  namespace detail
  {

#if defined(HBOOST_THREAD_PROVIDES_INVOKE) && ! defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && ! defined(HBOOST_NO_CXX11_HDR_TUPLE)

    template <class Fp, class ... Args>
    class invoker
    {
      //typedef typename decay<Fp>::type Fpd;
      //typedef tuple<typename decay<Args>::type...> Argsd;

      //csbl::tuple<Fpd, Argsd...> f_;
      csbl::tuple<Fp, Args...> f_;

    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE( invoker)
      //typedef typename invoke_of<_Fp, _Args...>::type Rp;
      typedef typename result_of<Fp(Args...)>::type result_type;

      template <class F, class ... As>
      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(As)... args)
      : f_(hboost::forward<F>(f), hboost::forward<As>(args)...)
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f) : f_(hboost::move(HBOOST_THREAD_RV(f).f_))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker( const invoker& f) : f_(f.f_)
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker& operator=(HBOOST_THREAD_RV_REF(invoker) f)
      {
        if (this != &f)
        {
          f_ = hboost::move(HBOOST_THREAD_RV(f).f_);
        }
        return *this;
      }

      HBOOST_SYMBOL_VISIBLE
      invoker& operator=( HBOOST_THREAD_COPY_ASSIGN_REF(invoker) f)
      {
        if (this != &f)
        {
          f_ = f.f_;
        }
        return *this;
      }

      result_type operator()()
      {
        typedef typename make_tuple_indices<1+sizeof...(Args), 1>::type Index;
        return execute(Index());
      }
    private:
      template <size_t ...Indices>
      result_type
      execute(tuple_indices<Indices...>)
      {
        return detail::invoke(hboost::move(csbl::get<0>(f_)), hboost::move(csbl::get<Indices>(f_))...);
      }
    };

    template <class R, class Fp, class ... Args>
    class invoker_ret
    {
      //typedef typename decay<Fp>::type Fpd;
      //typedef tuple<typename decay<Args>::type...> Argsd;

      //csbl::tuple<Fpd, Argsd...> f_;
      csbl::tuple<Fp, Args...> f_;

    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE( invoker_ret)
      typedef R result_type;

      template <class F, class ... As>
      HBOOST_SYMBOL_VISIBLE
      explicit invoker_ret(HBOOST_THREAD_FWD_REF(F) f, HBOOST_THREAD_FWD_REF(As)... args)
      : f_(hboost::forward<F>(f), hboost::forward<As>(args)...)
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker_ret(HBOOST_THREAD_RV_REF(invoker_ret) f) : f_(hboost::move(HBOOST_THREAD_RV(f).f_))
      {}

      result_type operator()()
      {
        typedef typename make_tuple_indices<1+sizeof...(Args), 1>::type Index;
        return execute(Index());
      }
    private:
      template <size_t ...Indices>
      result_type
      execute(tuple_indices<Indices...>)
      {
        return detail::invoke<R>(hboost::move(csbl::get<0>(f_)), hboost::move(csbl::get<Indices>(f_))...);
      }
    };
  //HBOOST_THREAD_DCL_MOVABLE_BEG(X) invoker<Fp> HBOOST_THREAD_DCL_MOVABLE_END
#else

#if ! defined HBOOST_MSVC && defined(HBOOST_THREAD_PROVIDES_INVOKE)

#define HBOOST_THREAD_RV_REF_ARG_T(z, n, unused) HBOOST_PP_COMMA_IF(n) HBOOST_THREAD_RV_REF(Arg##n)
#define HBOOST_THREAD_RV_REF_A_T(z, n, unused) HBOOST_PP_COMMA_IF(n) HBOOST_THREAD_RV_REF(A##n)
#define HBOOST_THREAD_RV_REF_ARG(z, n, unused) , HBOOST_THREAD_RV_REF(Arg##n) arg##n
#define HBOOST_THREAD_FWD_REF_A(z, n, unused)   , HBOOST_THREAD_FWD_REF(A##n) arg##n
#define HBOOST_THREAD_FWD_REF_ARG(z, n, unused) , HBOOST_THREAD_FWD_REF(Arg##n) arg##n
#define HBOOST_THREAD_FWD_PARAM(z, n, unused) , hboost::forward<Arg##n>(arg##n)
#define HBOOST_THREAD_FWD_PARAM_A(z, n, unused) , hboost::forward<A##n>(arg##n)
#define HBOOST_THREAD_DCL(z, n, unused) Arg##n v##n;
#define HBOOST_THREAD_MOVE_PARAM(z, n, unused) , v##n(hboost::move(arg##n))
#define HBOOST_THREAD_FORWARD_PARAM_A(z, n, unused) , v##n(hboost::forward<A##n>(arg##n))
#define HBOOST_THREAD_MOVE_RHS_PARAM(z, n, unused) , v##n(hboost::move(x.v##n))
#define HBOOST_THREAD_MOVE_DCL(z, n, unused) , hboost::move(v##n)
#define HBOOST_THREAD_MOVE_DCL_T(z, n, unused) HBOOST_PP_COMMA_IF(n) hboost::move(v##n)
#define HBOOST_THREAD_ARG_DEF(z, n, unused) , class Arg##n = tuples::null_type

  template  <class Fp, class Arg = tuples::null_type
    HBOOST_PP_REPEAT(HBOOST_THREAD_MAX_ARGS, HBOOST_THREAD_ARG_DEF, ~)
    >
    class invoker;

#define HBOOST_THREAD_ASYNC_FUNCT(z, n, unused) \
    template <class Fp HBOOST_PP_COMMA_IF(n) HBOOST_PP_ENUM_PARAMS(n, class Arg) > \
    class invoker<Fp HBOOST_PP_COMMA_IF(n) HBOOST_PP_ENUM_PARAMS(n, Arg)> \
    { \
      Fp fp_; \
      HBOOST_PP_REPEAT(n, HBOOST_THREAD_DCL, ~) \
    public: \
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker) \
      typedef typename result_of<Fp(HBOOST_PP_ENUM_PARAMS(n, Arg))>::type result_type; \
      \
      template <class F HBOOST_PP_COMMA_IF(n) HBOOST_PP_ENUM_PARAMS(n, class A) > \
      HBOOST_SYMBOL_VISIBLE \
      explicit invoker(HBOOST_THREAD_FWD_REF(F) f \
          HBOOST_PP_REPEAT(n, HBOOST_THREAD_FWD_REF_A, ~) \
      ) \
      : fp_(hboost::forward<F>(f)) \
      HBOOST_PP_REPEAT(n, HBOOST_THREAD_FORWARD_PARAM_A, ~) \
      {} \
      \
      HBOOST_SYMBOL_VISIBLE \
      invoker(HBOOST_THREAD_RV_REF(invoker) x) \
      : fp_(hboost::move(x.fp_)) \
      HBOOST_PP_REPEAT(n, HBOOST_THREAD_MOVE_RHS_PARAM, ~) \
      {} \
      \
      result_type operator()() { \
        return detail::invoke(hboost::move(fp_) \
            HBOOST_PP_REPEAT(n, HBOOST_THREAD_MOVE_DCL, ~) \
        ); \
      } \
    }; \
    \
    template <class R HBOOST_PP_COMMA_IF(n) HBOOST_PP_ENUM_PARAMS(n, class Arg) > \
    class invoker<R(*)(HBOOST_PP_REPEAT(n, HBOOST_THREAD_RV_REF_ARG_T, ~)) HBOOST_PP_COMMA_IF(n) HBOOST_PP_ENUM_PARAMS(n, Arg)> \
    { \
      typedef R(*Fp)(HBOOST_PP_REPEAT(n, HBOOST_THREAD_RV_REF_ARG_T, ~)); \
      Fp fp_; \
      HBOOST_PP_REPEAT(n, HBOOST_THREAD_DCL, ~) \
    public: \
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker) \
      typedef typename result_of<Fp(HBOOST_PP_ENUM_PARAMS(n, Arg))>::type result_type; \
      \
      template <class R2 HBOOST_PP_COMMA_IF(n) HBOOST_PP_ENUM_PARAMS(n, class A) > \
      HBOOST_SYMBOL_VISIBLE \
      explicit invoker(R2(*f)(HBOOST_PP_REPEAT(n, HBOOST_THREAD_RV_REF_A_T, ~))  \
          HBOOST_PP_REPEAT(n, HBOOST_THREAD_FWD_REF_A, ~) \
      ) \
      : fp_(f) \
      HBOOST_PP_REPEAT(n, HBOOST_THREAD_FORWARD_PARAM_A, ~) \
      {} \
      \
      HBOOST_SYMBOL_VISIBLE \
      invoker(HBOOST_THREAD_RV_REF(invoker) x) \
      : fp_(x.fp_) \
      HBOOST_PP_REPEAT(n, HBOOST_THREAD_MOVE_RHS_PARAM, ~) \
      {} \
      \
      result_type operator()() { \
        return fp_( \
            HBOOST_PP_REPEAT(n, HBOOST_THREAD_MOVE_DCL_T, ~) \
        ); \
      } \
    };

    HBOOST_PP_REPEAT(HBOOST_THREAD_MAX_ARGS, HBOOST_THREAD_ASYNC_FUNCT, ~)

    #undef HBOOST_THREAD_RV_REF_ARG_T
    #undef HBOOST_THREAD_RV_REF_ARG
    #undef HBOOST_THREAD_FWD_REF_ARG
    #undef HBOOST_THREAD_FWD_REF_A
    #undef HBOOST_THREAD_FWD_PARAM
    #undef HBOOST_THREAD_FWD_PARAM_A
    #undef HBOOST_THREAD_DCL
    #undef HBOOST_THREAD_MOVE_PARAM
    #undef HBOOST_THREAD_MOVE_RHS_PARAM
    #undef HBOOST_THREAD_MOVE_DCL
    #undef HBOOST_THREAD_ARG_DEF
    #undef HBOOST_THREAD_ASYNC_FUNCT

#else

    template <class Fp,
    class T0 = tuples::null_type, class T1 = tuples::null_type, class T2 = tuples::null_type,
    class T3 = tuples::null_type, class T4 = tuples::null_type, class T5 = tuples::null_type,
    class T6 = tuples::null_type, class T7 = tuples::null_type, class T8 = tuples::null_type
    , class T9 = tuples::null_type
    >
    class invoker;

    template <class Fp,
    class T0 , class T1 , class T2 ,
    class T3 , class T4 , class T5 ,
    class T6 , class T7 , class T8 >
    class invoker<Fp, T0, T1, T2, T3, T4, T5, T6, T7, T8>
    {
      Fp fp_;
      T0 v0_;
      T1 v1_;
      T2 v2_;
      T3 v3_;
      T4 v4_;
      T5 v5_;
      T6 v6_;
      T7 v7_;
      T8 v8_;
      //::hboost::tuple<Fp, T0, T1, T2, T3, T4, T5, T6, T7, T8> f_;

    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0, T1, T2, T3, T4, T5, T6, T7, T8)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
          , HBOOST_THREAD_RV_REF(T1) a1
          , HBOOST_THREAD_RV_REF(T2) a2
          , HBOOST_THREAD_RV_REF(T3) a3
          , HBOOST_THREAD_RV_REF(T4) a4
          , HBOOST_THREAD_RV_REF(T5) a5
          , HBOOST_THREAD_RV_REF(T6) a6
          , HBOOST_THREAD_RV_REF(T7) a7
          , HBOOST_THREAD_RV_REF(T8) a8
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      , v1_(hboost::move(a1))
      , v2_(hboost::move(a2))
      , v3_(hboost::move(a3))
      , v4_(hboost::move(a4))
      , v5_(hboost::move(a5))
      , v6_(hboost::move(a6))
      , v7_(hboost::move(a7))
      , v8_(hboost::move(a8))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      , v1_(hboost::move(HBOOST_THREAD_RV(f).v1_))
      , v2_(hboost::move(HBOOST_THREAD_RV(f).v2_))
      , v3_(hboost::move(HBOOST_THREAD_RV(f).v3_))
      , v4_(hboost::move(HBOOST_THREAD_RV(f).v4_))
      , v5_(hboost::move(HBOOST_THREAD_RV(f).v5_))
      , v6_(hboost::move(HBOOST_THREAD_RV(f).v6_))
      , v7_(hboost::move(HBOOST_THREAD_RV(f).v7_))
      , v8_(hboost::move(HBOOST_THREAD_RV(f).v8_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
            , hboost::move(v1_)
            , hboost::move(v2_)
            , hboost::move(v3_)
            , hboost::move(v4_)
            , hboost::move(v5_)
            , hboost::move(v6_)
            , hboost::move(v7_)
            , hboost::move(v8_)
        );
      }
    };
    template <class Fp, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
    class invoker<Fp, T0, T1, T2, T3, T4, T5, T6, T7>
    {
      Fp fp_;
      T0 v0_;
      T1 v1_;
      T2 v2_;
      T3 v3_;
      T4 v4_;
      T5 v5_;
      T6 v6_;
      T7 v7_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0, T1, T2, T3, T4, T5, T6, T7)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
          , HBOOST_THREAD_RV_REF(T1) a1
          , HBOOST_THREAD_RV_REF(T2) a2
          , HBOOST_THREAD_RV_REF(T3) a3
          , HBOOST_THREAD_RV_REF(T4) a4
          , HBOOST_THREAD_RV_REF(T5) a5
          , HBOOST_THREAD_RV_REF(T6) a6
          , HBOOST_THREAD_RV_REF(T7) a7
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      , v1_(hboost::move(a1))
      , v2_(hboost::move(a2))
      , v3_(hboost::move(a3))
      , v4_(hboost::move(a4))
      , v5_(hboost::move(a5))
      , v6_(hboost::move(a6))
      , v7_(hboost::move(a7))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      , v1_(hboost::move(HBOOST_THREAD_RV(f).v1_))
      , v2_(hboost::move(HBOOST_THREAD_RV(f).v2_))
      , v3_(hboost::move(HBOOST_THREAD_RV(f).v3_))
      , v4_(hboost::move(HBOOST_THREAD_RV(f).v4_))
      , v5_(hboost::move(HBOOST_THREAD_RV(f).v5_))
      , v6_(hboost::move(HBOOST_THREAD_RV(f).v6_))
      , v7_(hboost::move(HBOOST_THREAD_RV(f).v7_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
            , hboost::move(v1_)
            , hboost::move(v2_)
            , hboost::move(v3_)
            , hboost::move(v4_)
            , hboost::move(v5_)
            , hboost::move(v6_)
            , hboost::move(v7_)
        );
      }
    };
    template <class Fp, class T0, class T1, class T2, class T3, class T4, class T5, class T6>
    class invoker<Fp, T0, T1, T2, T3, T4, T5, T6>
    {
      Fp fp_;
      T0 v0_;
      T1 v1_;
      T2 v2_;
      T3 v3_;
      T4 v4_;
      T5 v5_;
      T6 v6_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0, T1, T2, T3, T4, T5, T6)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
          , HBOOST_THREAD_RV_REF(T1) a1
          , HBOOST_THREAD_RV_REF(T2) a2
          , HBOOST_THREAD_RV_REF(T3) a3
          , HBOOST_THREAD_RV_REF(T4) a4
          , HBOOST_THREAD_RV_REF(T5) a5
          , HBOOST_THREAD_RV_REF(T6) a6
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      , v1_(hboost::move(a1))
      , v2_(hboost::move(a2))
      , v3_(hboost::move(a3))
      , v4_(hboost::move(a4))
      , v5_(hboost::move(a5))
      , v6_(hboost::move(a6))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      , v1_(hboost::move(HBOOST_THREAD_RV(f).v1_))
      , v2_(hboost::move(HBOOST_THREAD_RV(f).v2_))
      , v3_(hboost::move(HBOOST_THREAD_RV(f).v3_))
      , v4_(hboost::move(HBOOST_THREAD_RV(f).v4_))
      , v5_(hboost::move(HBOOST_THREAD_RV(f).v5_))
      , v6_(hboost::move(HBOOST_THREAD_RV(f).v6_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
            , hboost::move(v1_)
            , hboost::move(v2_)
            , hboost::move(v3_)
            , hboost::move(v4_)
            , hboost::move(v5_)
            , hboost::move(v6_)
        );
      }
    };
    template <class Fp, class T0, class T1, class T2, class T3, class T4, class T5>
    class invoker<Fp, T0, T1, T2, T3, T4, T5>
    {
      Fp fp_;
      T0 v0_;
      T1 v1_;
      T2 v2_;
      T3 v3_;
      T4 v4_;
      T5 v5_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0, T1, T2, T3, T4, T5)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
          , HBOOST_THREAD_RV_REF(T1) a1
          , HBOOST_THREAD_RV_REF(T2) a2
          , HBOOST_THREAD_RV_REF(T3) a3
          , HBOOST_THREAD_RV_REF(T4) a4
          , HBOOST_THREAD_RV_REF(T5) a5
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      , v1_(hboost::move(a1))
      , v2_(hboost::move(a2))
      , v3_(hboost::move(a3))
      , v4_(hboost::move(a4))
      , v5_(hboost::move(a5))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      , v1_(hboost::move(HBOOST_THREAD_RV(f).v1_))
      , v2_(hboost::move(HBOOST_THREAD_RV(f).v2_))
      , v3_(hboost::move(HBOOST_THREAD_RV(f).v3_))
      , v4_(hboost::move(HBOOST_THREAD_RV(f).v4_))
      , v5_(hboost::move(HBOOST_THREAD_RV(f).v5_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
            , hboost::move(v1_)
            , hboost::move(v2_)
            , hboost::move(v3_)
            , hboost::move(v4_)
            , hboost::move(v5_)
        );
      }
    };
    template <class Fp, class T0, class T1, class T2, class T3, class T4>
    class invoker<Fp, T0, T1, T2, T3, T4>
    {
      Fp fp_;
      T0 v0_;
      T1 v1_;
      T2 v2_;
      T3 v3_;
      T4 v4_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0, T1, T2, T3, T4)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
          , HBOOST_THREAD_RV_REF(T1) a1
          , HBOOST_THREAD_RV_REF(T2) a2
          , HBOOST_THREAD_RV_REF(T3) a3
          , HBOOST_THREAD_RV_REF(T4) a4
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      , v1_(hboost::move(a1))
      , v2_(hboost::move(a2))
      , v3_(hboost::move(a3))
      , v4_(hboost::move(a4))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      , v1_(hboost::move(HBOOST_THREAD_RV(f).v1_))
      , v2_(hboost::move(HBOOST_THREAD_RV(f).v2_))
      , v3_(hboost::move(HBOOST_THREAD_RV(f).v3_))
      , v4_(hboost::move(HBOOST_THREAD_RV(f).v4_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
            , hboost::move(v1_)
            , hboost::move(v2_)
            , hboost::move(v3_)
            , hboost::move(v4_)
        );
      }
    };
    template <class Fp, class T0, class T1, class T2, class T3>
    class invoker<Fp, T0, T1, T2, T3>
    {
      Fp fp_;
      T0 v0_;
      T1 v1_;
      T2 v2_;
      T3 v3_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0, T1, T2, T3)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
          , HBOOST_THREAD_RV_REF(T1) a1
          , HBOOST_THREAD_RV_REF(T2) a2
          , HBOOST_THREAD_RV_REF(T3) a3
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      , v1_(hboost::move(a1))
      , v2_(hboost::move(a2))
      , v3_(hboost::move(a3))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      , v1_(hboost::move(HBOOST_THREAD_RV(f).v1_))
      , v2_(hboost::move(HBOOST_THREAD_RV(f).v2_))
      , v3_(hboost::move(HBOOST_THREAD_RV(f).v3_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
            , hboost::move(v1_)
            , hboost::move(v2_)
            , hboost::move(v3_)
        );
      }
    };
    template <class Fp, class T0, class T1, class T2>
    class invoker<Fp, T0, T1, T2>
    {
      Fp fp_;
      T0 v0_;
      T1 v1_;
      T2 v2_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0, T1, T2)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
          , HBOOST_THREAD_RV_REF(T1) a1
          , HBOOST_THREAD_RV_REF(T2) a2
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      , v1_(hboost::move(a1))
      , v2_(hboost::move(a2))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      , v1_(hboost::move(HBOOST_THREAD_RV(f).v1_))
      , v2_(hboost::move(HBOOST_THREAD_RV(f).v2_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
            , hboost::move(v1_)
            , hboost::move(v2_)
        );
      }
    };
    template <class Fp, class T0, class T1>
    class invoker<Fp, T0, T1>
    {
      Fp fp_;
      T0 v0_;
      T1 v1_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0, T1)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
          , HBOOST_THREAD_RV_REF(T1) a1
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      , v1_(hboost::move(a1))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      , v1_(hboost::move(HBOOST_THREAD_RV(f).v1_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
            , hboost::move(v1_)
        );
      }
    };
    template <class Fp, class T0>
    class invoker<Fp, T0>
    {
      Fp fp_;
      T0 v0_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp(T0)>::type result_type;

      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f
          , HBOOST_THREAD_RV_REF(T0) a0
      )
      : fp_(hboost::move(f))
      , v0_(hboost::move(a0))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(HBOOST_THREAD_RV(f).fp))
      , v0_(hboost::move(HBOOST_THREAD_RV(f).v0_))
      {}

      result_type operator()()
      {
        return detail::invoke(hboost::move(fp_)
            , hboost::move(v0_)
        );
      }
    };
    template <class Fp>
    class invoker<Fp>
    {
      Fp fp_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp()>::type result_type;
      HBOOST_SYMBOL_VISIBLE
      explicit invoker(HBOOST_THREAD_FWD_REF(Fp) f)
      : fp_(hboost::move(f))
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(hboost::move(f.fp_))
      {}
      result_type operator()()
      {
        return fp_();
      }
    };
    template <class R>
    class invoker<R(*)()>
    {
      typedef R(*Fp)();
      Fp fp_;
    public:
      HBOOST_THREAD_COPYABLE_AND_MOVABLE(invoker)
      typedef typename result_of<Fp()>::type result_type;
      HBOOST_SYMBOL_VISIBLE
      explicit invoker(Fp f)
      : fp_(f)
      {}

      HBOOST_SYMBOL_VISIBLE
      invoker(HBOOST_THREAD_RV_REF(invoker) f)
      : fp_(f.fp_)
      {}
      result_type operator()()
      {
        return fp_();
      }
    };
#endif
#endif

  }
}

#include <hboost/thread/detail/variadic_footer.hpp>

#endif // header
