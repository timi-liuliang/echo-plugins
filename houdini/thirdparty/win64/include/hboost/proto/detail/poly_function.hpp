///////////////////////////////////////////////////////////////////////////////
/// \file poly_function.hpp
/// A wrapper that makes a tr1-style function object that handles const
/// and non-const refs and reference_wrapper arguments, too, and forwards
/// the arguments on to the specified implementation.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_DETAIL_POLY_FUNCTION_EAN_2008_05_02
#define HBOOST_PROTO_DETAIL_POLY_FUNCTION_EAN_2008_05_02

#include <hboost/ref.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/void.hpp>
#include <hboost/mpl/size_t.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp>
#include <hboost/preprocessor/iteration/iterate.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/proto/proto_fwd.hpp>
#include <hboost/proto/detail/is_noncopyable.hpp>

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4181) // const applied to reference type
#endif

namespace hboost { namespace proto { namespace detail
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    struct normalize_arg
    {
        typedef typename mpl::if_c<is_noncopyable<T>::value, T &, T>::type type;
        typedef T &reference;
    };

    template<typename T>
    struct normalize_arg<T const>
    {
        typedef typename mpl::if_c<is_noncopyable<T>::value, T const &, T>::type type;
        typedef T const &reference;
    };

    template<typename T>
    struct normalize_arg<T &>
    {
        typedef typename mpl::if_c<is_noncopyable<T>::value, T &, T>::type type;
        typedef T &reference;
    };

    template<typename T>
    struct normalize_arg<T const &>
    {
        typedef typename mpl::if_c<is_noncopyable<T>::value, T const &, T>::type type;
        typedef T const &reference;
    };

    template<typename T>
    struct normalize_arg<hboost::reference_wrapper<T> >
    {
        typedef T &type;
        typedef T &reference;
    };

    template<typename T>
    struct normalize_arg<hboost::reference_wrapper<T> const>
    {
        typedef T &type;
        typedef T &reference;
    };

    template<typename T>
    struct normalize_arg<hboost::reference_wrapper<T> &>
    {
        typedef T &type;
        typedef T &reference;
    };

    template<typename T>
    struct normalize_arg<hboost::reference_wrapper<T> const &>
    {
        typedef T &type;
        typedef T &reference;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    struct arg
    {
        typedef T const &type;

        arg(type t)
          : value(t)
        {}

        operator type() const
        {
            return this->value;
        }

        type operator()() const
        {
            return this->value;
        }

    private:
        arg &operator =(arg const &);
        type value;
    };

    template<typename T>
    struct arg<T &>
    {
        typedef T &type;

        arg(type t)
          : value(t)
        {}

        operator type() const
        {
            return this->value;
        }

        type operator()() const
        {
            return this->value;
        }

    private:
        arg &operator =(arg const &);
        type value;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename T, typename Void = void>
    struct is_poly_function
      : mpl::false_
    {};

    template<typename T>
    struct is_poly_function<T, typename T::is_poly_function_base_>
      : mpl::true_
    {};

    ////////////////////////////////////////////////////////////////////////////////////////////////
    #define HBOOST_PROTO_POLY_FUNCTION()                                                             \
        typedef void is_poly_function_base_;                                                        \
        /**/

    ////////////////////////////////////////////////////////////////////////////////////////////////
    struct poly_function_base
    {
        /// INTERNAL ONLY
        HBOOST_PROTO_POLY_FUNCTION()
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename Derived, typename NullaryResult = void>
    struct poly_function
      : poly_function_base
    {
        template<typename Sig>
        struct result;

        template<typename This>
        struct result<This()>
          : Derived::template impl<>
        {
            typedef typename result::result_type type;
        };

        NullaryResult operator()() const
        {
            result<Derived const()> impl;
            return impl();
        }

        #include <hboost/proto/detail/poly_function_funop.hpp>
    };

    template<typename T>
    struct wrap_t;

    typedef char poly_function_t;
    typedef char (&mono_function_t)[2];
    typedef char (&unknown_function_t)[3];

    template<typename T> poly_function_t test_poly_function(T *, wrap_t<typename T::is_poly_function_base_> * = 0);
    template<typename T> mono_function_t test_poly_function(T *, wrap_t<typename T::result_type> * = 0);
    template<typename T> unknown_function_t test_poly_function(T *, ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename Fun, typename Sig, typename Switch = mpl::size_t<sizeof(test_poly_function<Fun>(0,0))> >
    struct poly_function_traits
    {
        typedef typename Fun::template result<Sig>::type result_type;
        typedef Fun function_type;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename Fun, typename Sig>
    struct poly_function_traits<Fun, Sig, mpl::size_t<sizeof(mono_function_t)> >
    {
        typedef typename Fun::result_type result_type;
        typedef Fun function_type;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename PolyFunSig, bool IsPolyFunction>
    struct as_mono_function_impl;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename PolyFunSig>
    struct as_mono_function;

    #include <hboost/proto/detail/poly_function_traits.hpp>

}}} // namespace hboost::proto::detail

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#endif

