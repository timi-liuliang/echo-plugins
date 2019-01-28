#ifndef HBOOST_BIND_MEM_FN_HPP_INCLUDED
#define HBOOST_BIND_MEM_FN_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  mem_fn.hpp - a generalization of std::mem_fun[_ref]
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2001 David Abrahams
//  Copyright (c) 2003-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/bind/mem_fn.html for documentation.
//

#include <hboost/config.hpp>
#include <hboost/get_pointer.hpp>
#include <hboost/detail/workaround.hpp>

namespace hboost
{

#if defined(HBOOST_NO_VOID_RETURNS)

#define HBOOST_MEM_FN_CLASS_F , class F
#define HBOOST_MEM_FN_TYPEDEF(X)

namespace _mfi // mem_fun_impl
{

template<class V> struct mf
{

#define HBOOST_MEM_FN_RETURN return

#define HBOOST_MEM_FN_NAME(X) inner_##X
#define HBOOST_MEM_FN_CC

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#ifdef HBOOST_MEM_FN_ENABLE_CDECL

#define HBOOST_MEM_FN_NAME(X) inner_##X##_cdecl
#define HBOOST_MEM_FN_CC __cdecl

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#ifdef HBOOST_MEM_FN_ENABLE_STDCALL

#define HBOOST_MEM_FN_NAME(X) inner_##X##_stdcall
#define HBOOST_MEM_FN_CC __stdcall

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#ifdef HBOOST_MEM_FN_ENABLE_FASTCALL

#define HBOOST_MEM_FN_NAME(X) inner_##X##_fastcall
#define HBOOST_MEM_FN_CC __fastcall

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#undef HBOOST_MEM_FN_RETURN

}; // struct mf<V>

template<> struct mf<void>
{

#define HBOOST_MEM_FN_RETURN

#define HBOOST_MEM_FN_NAME(X) inner_##X
#define HBOOST_MEM_FN_CC

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#ifdef HBOOST_MEM_FN_ENABLE_CDECL

#define HBOOST_MEM_FN_NAME(X) inner_##X##_cdecl
#define HBOOST_MEM_FN_CC __cdecl

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#ifdef HBOOST_MEM_FN_ENABLE_STDCALL

#define HBOOST_MEM_FN_NAME(X) inner_##X##_stdcall
#define HBOOST_MEM_FN_CC __stdcall

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#ifdef HBOOST_MEM_FN_ENABLE_FASTCALL

#define HBOOST_MEM_FN_NAME(X) inner_##X##_fastcall
#define HBOOST_MEM_FN_CC __fastcall

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#undef HBOOST_MEM_FN_RETURN

}; // struct mf<void>

#undef HBOOST_MEM_FN_CLASS_F
#undef HBOOST_MEM_FN_TYPEDEF_F

#define HBOOST_MEM_FN_NAME(X) X
#define HBOOST_MEM_FN_NAME2(X) inner_##X
#define HBOOST_MEM_FN_CC

#include <hboost/bind/mem_fn_vw.hpp>

#undef HBOOST_MEM_FN_NAME
#undef HBOOST_MEM_FN_NAME2
#undef HBOOST_MEM_FN_CC

#ifdef HBOOST_MEM_FN_ENABLE_CDECL

#define HBOOST_MEM_FN_NAME(X) X##_cdecl
#define HBOOST_MEM_FN_NAME2(X) inner_##X##_cdecl
#define HBOOST_MEM_FN_CC __cdecl

#include <hboost/bind/mem_fn_vw.hpp>

#undef HBOOST_MEM_FN_NAME
#undef HBOOST_MEM_FN_NAME2
#undef HBOOST_MEM_FN_CC

#endif

#ifdef HBOOST_MEM_FN_ENABLE_STDCALL

#define HBOOST_MEM_FN_NAME(X) X##_stdcall
#define HBOOST_MEM_FN_NAME2(X) inner_##X##_stdcall
#define HBOOST_MEM_FN_CC __stdcall

#include <hboost/bind/mem_fn_vw.hpp>

#undef HBOOST_MEM_FN_NAME
#undef HBOOST_MEM_FN_NAME2
#undef HBOOST_MEM_FN_CC

#endif

#ifdef HBOOST_MEM_FN_ENABLE_FASTCALL

#define HBOOST_MEM_FN_NAME(X) X##_fastcall
#define HBOOST_MEM_FN_NAME2(X) inner_##X##_fastcall
#define HBOOST_MEM_FN_CC __fastcall

#include <hboost/bind/mem_fn_vw.hpp>

#undef HBOOST_MEM_FN_NAME
#undef HBOOST_MEM_FN_NAME2
#undef HBOOST_MEM_FN_CC

#endif

} // namespace _mfi

#else // #ifdef HBOOST_NO_VOID_RETURNS

#define HBOOST_MEM_FN_CLASS_F
#define HBOOST_MEM_FN_TYPEDEF(X) typedef X;

namespace _mfi
{

#define HBOOST_MEM_FN_RETURN return

#define HBOOST_MEM_FN_NAME(X) X
#define HBOOST_MEM_FN_CC

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#ifdef HBOOST_MEM_FN_ENABLE_CDECL

#define HBOOST_MEM_FN_NAME(X) X##_cdecl
#define HBOOST_MEM_FN_CC __cdecl

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#ifdef HBOOST_MEM_FN_ENABLE_STDCALL

#define HBOOST_MEM_FN_NAME(X) X##_stdcall
#define HBOOST_MEM_FN_CC __stdcall

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#ifdef HBOOST_MEM_FN_ENABLE_FASTCALL

#define HBOOST_MEM_FN_NAME(X) X##_fastcall
#define HBOOST_MEM_FN_CC __fastcall

#include <hboost/bind/mem_fn_template.hpp>

#undef HBOOST_MEM_FN_CC
#undef HBOOST_MEM_FN_NAME

#endif

#undef HBOOST_MEM_FN_RETURN

} // namespace _mfi

#undef HBOOST_MEM_FN_CLASS_F
#undef HBOOST_MEM_FN_TYPEDEF

#endif // #ifdef HBOOST_NO_VOID_RETURNS

#define HBOOST_MEM_FN_NAME(X) X
#define HBOOST_MEM_FN_CC

#include <hboost/bind/mem_fn_cc.hpp>

#undef HBOOST_MEM_FN_NAME
#undef HBOOST_MEM_FN_CC

#ifdef HBOOST_MEM_FN_ENABLE_CDECL

#define HBOOST_MEM_FN_NAME(X) X##_cdecl
#define HBOOST_MEM_FN_CC __cdecl

#include <hboost/bind/mem_fn_cc.hpp>

#undef HBOOST_MEM_FN_NAME
#undef HBOOST_MEM_FN_CC

#endif

#ifdef HBOOST_MEM_FN_ENABLE_STDCALL

#define HBOOST_MEM_FN_NAME(X) X##_stdcall
#define HBOOST_MEM_FN_CC __stdcall

#include <hboost/bind/mem_fn_cc.hpp>

#undef HBOOST_MEM_FN_NAME
#undef HBOOST_MEM_FN_CC

#endif

#ifdef HBOOST_MEM_FN_ENABLE_FASTCALL

#define HBOOST_MEM_FN_NAME(X) X##_fastcall
#define HBOOST_MEM_FN_CC __fastcall

#include <hboost/bind/mem_fn_cc.hpp>

#undef HBOOST_MEM_FN_NAME
#undef HBOOST_MEM_FN_CC

#endif

// data member support

namespace _mfi
{

template<class R, class T> class dm
{
public:

    typedef R const & result_type;
    typedef T const * argument_type;

private:
    
    typedef R (T::*F);
    F f_;

    template<class U> R const & call(U & u, T const *) const
    {
        return (u.*f_);
    }

    template<class U> R const & call(U & u, void const *) const
    {
        return (get_pointer(u)->*f_);
    }

public:
    
    explicit dm(F f): f_(f) {}

    R & operator()(T * p) const
    {
        return (p->*f_);
    }

    R const & operator()(T const * p) const
    {
        return (p->*f_);
    }

    template<class U> R const & operator()(U const & u) const
    {
        return call(u, &u);
    }

#if !HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) && !HBOOST_WORKAROUND(__MWERKS__, < 0x3200)

    R & operator()(T & t) const
    {
        return (t.*f_);
    }

    R const & operator()(T const & t) const
    {
        return (t.*f_);
    }

#endif

    bool operator==(dm const & rhs) const
    {
        return f_ == rhs.f_;
    }

    bool operator!=(dm const & rhs) const
    {
        return f_ != rhs.f_;
    }
};

} // namespace _mfi

template<class R, class T> _mfi::dm<R, T> mem_fn(R T::*f)
{
    return _mfi::dm<R, T>(f);
}

} // namespace hboost

#endif // #ifndef HBOOST_BIND_MEM_FN_HPP_INCLUDED
