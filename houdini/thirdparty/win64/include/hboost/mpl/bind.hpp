
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_BIND_HPP_INCLUDED
#define HBOOST_MPL_BIND_HPP_INCLUDED

// Copyright Peter Dimov 2001
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/bind_fwd.hpp>
#   include <hboost/mpl/placeholders.hpp>
#   include <hboost/mpl/next.hpp>
#   include <hboost/mpl/protect.hpp>
#   include <hboost/mpl/apply_wrap.hpp>
#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/na.hpp>
#   include <hboost/mpl/aux_/arity_spec.hpp>
#   include <hboost/mpl/aux_/type_wrapper.hpp>
#   include <hboost/mpl/aux_/yes_no.hpp>
#   if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#       include <hboost/type_traits/is_reference.hpp>
#   endif 
#endif

#include <hboost/mpl/aux_/config/bind.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>
#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   if defined(HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT)
#       define HBOOST_MPL_PREPROCESSED_HEADER basic_bind.hpp
#   else
#       define HBOOST_MPL_PREPROCESSED_HEADER bind.hpp
#   endif
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/def_params_tail.hpp>
#   include <hboost/mpl/aux_/preprocessor/partial_spec_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/ext_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/repeat.hpp>
#   include <hboost/mpl/aux_/preprocessor/enum.hpp>
#   include <hboost/mpl/aux_/preprocessor/add.hpp>
#   include <hboost/mpl/aux_/config/dmc_ambiguous_ctps.hpp>
#   include <hboost/mpl/aux_/config/ctps.hpp>
#   include <hboost/mpl/aux_/config/ttp.hpp>
#   include <hboost/mpl/aux_/config/dtp.hpp>
#   include <hboost/mpl/aux_/nttp_decl.hpp>

#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/cat.hpp>
#   include <hboost/preprocessor/inc.hpp>

namespace hboost { namespace mpl {

// local macros, #undef-ined at the end of the header
#   define AUX778076_APPLY \
    HBOOST_PP_CAT(apply_wrap,HBOOST_MPL_LIMIT_METAFUNCTION_ARITY) \
    /**/

#   if defined(HBOOST_MPL_CFG_DMC_AMBIGUOUS_CTPS)
#       define AUX778076_DMC_PARAM() , int dummy_
#   else
#       define AUX778076_DMC_PARAM()
#   endif

#   define AUX778076_BIND_PARAMS(param) \
    HBOOST_MPL_PP_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        ) \
    /**/

#   define AUX778076_BIND_DEFAULT_PARAMS(param, value) \
    HBOOST_MPL_PP_DEFAULT_PARAMS( \
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        , value \
        ) \
    /**/

#   define AUX778076_BIND_N_PARAMS(n, param) \
    HBOOST_PP_COMMA_IF(n) HBOOST_MPL_PP_PARAMS(n, param) \
    /**/

#   define AUX778076_BIND_N_SPEC_PARAMS(n, param, def) \
    HBOOST_PP_COMMA_IF(n) \
    HBOOST_MPL_PP_PARTIAL_SPEC_PARAMS(n, param, def) \
    /**/

#if !defined(HBOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#   define AUX778076_BIND_NESTED_DEFAULT_PARAMS(param, value) \
    AUX778076_BIND_DEFAULT_PARAMS(param, value) \
    /**/
#else
#   define AUX778076_BIND_NESTED_DEFAULT_PARAMS(param, value) \
    AUX778076_BIND_PARAMS(param) \
    /**/
#endif

namespace aux {

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template<
      typename T, AUX778076_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg
{
    typedef T type;
};

#   if !defined(HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT)

template<
      typename T
    , typename Arg
    >
struct replace_unnamed_arg
{
    typedef Arg next;
    typedef T type;
};

template<
      typename Arg
    >
struct replace_unnamed_arg< arg<-1>,Arg >
{
    typedef typename Arg::next next;
    typedef Arg type;
};

#   endif // HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT

template<
      HBOOST_MPL_AUX_NTTP_DECL(int, N), AUX778076_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg< arg<N>,AUX778076_BIND_PARAMS(U) >
{
    typedef typename AUX778076_APPLY<mpl::arg<N>, AUX778076_BIND_PARAMS(U)>::type type;
};

#if !defined(HBOOST_MPL_CFG_NO_BIND_TEMPLATE)
template<
      typename F, AUX778076_BIND_PARAMS(typename T), AUX778076_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg< bind<F,AUX778076_BIND_PARAMS(T)>,AUX778076_BIND_PARAMS(U) >
{
    typedef bind<F,AUX778076_BIND_PARAMS(T)> f_;
    typedef typename AUX778076_APPLY<f_, AUX778076_BIND_PARAMS(U)>::type type;
};
#endif

#else // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

// agurt, 15/jan/02: it's not a intended to be used as a function class, and 
// MSVC6.5 has problems with 'apply' name here (the code compiles, but doesn't
// work), so I went with the 'result_' here, and in all other similar cases
template< bool >
struct resolve_arg_impl
{
    template< typename T, AUX778076_BIND_PARAMS(typename U) > struct result_
    {
        typedef T type;
    };
};

template<> 
struct resolve_arg_impl<true>
{
    template< typename T, AUX778076_BIND_PARAMS(typename U) > struct result_
    {
        typedef typename AUX778076_APPLY<
              T
            , AUX778076_BIND_PARAMS(U)
            >::type type;
    };
};

// for 'resolve_bind_arg'
template< typename T > struct is_bind_template;

template< 
      typename T, AUX778076_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg
    : resolve_arg_impl< is_bind_template<T>::value >
            ::template result_< T,AUX778076_BIND_PARAMS(U) >
{
};

#   if !defined(HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT)

template< typename T > 
struct replace_unnamed_arg_impl
{
    template< typename Arg > struct result_
    {
        typedef Arg next;
        typedef T type;
    };
};

template<> 
struct replace_unnamed_arg_impl< arg<-1> >
{
    template< typename Arg > struct result_
    {
        typedef typename next<Arg>::type next;
        typedef Arg type;
    };
};

template< typename T, typename Arg > 
struct replace_unnamed_arg
    : replace_unnamed_arg_impl<T>::template result_<Arg>
{
};

#   endif // HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT

// agurt, 10/mar/02: the forward declaration has to appear before any of
// 'is_bind_helper' overloads, otherwise MSVC6.5 issues an ICE on it
template< HBOOST_MPL_AUX_NTTP_DECL(int, arity_) > struct bind_chooser;

aux::no_tag is_bind_helper(...);
template< typename T > aux::no_tag is_bind_helper(protect<T>*);

// overload for "main" form
// agurt, 15/mar/02: MSVC 6.5 fails to properly resolve the overload 
// in case if we use 'aux::type_wrapper< bind<...> >' here, and all 
// 'bind' instantiations form a complete type anyway
#if !defined(HBOOST_MPL_CFG_NO_BIND_TEMPLATE)
template<
      typename F, AUX778076_BIND_PARAMS(typename T)
    >
aux::yes_tag is_bind_helper(bind<F,AUX778076_BIND_PARAMS(T)>*);
#endif

template< HBOOST_MPL_AUX_NTTP_DECL(int, N) >
aux::yes_tag is_bind_helper(arg<N>*);

template< bool is_ref_ = true >
struct is_bind_template_impl
{
    template< typename T > struct result_
    {
        HBOOST_STATIC_CONSTANT(bool, value = false);
    };
};

template<>
struct is_bind_template_impl<false>
{
    template< typename T > struct result_
    {
        HBOOST_STATIC_CONSTANT(bool, value = 
              sizeof(aux::is_bind_helper(static_cast<T*>(0))) 
                == sizeof(aux::yes_tag)
            );
    };
};

template< typename T > struct is_bind_template
    : is_bind_template_impl< ::hboost::detail::is_reference_impl<T>::value >
        ::template result_<T>
{
};

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace aux


#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, <hboost/mpl/bind.hpp>))
#include HBOOST_PP_ITERATE()

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
    && !defined(HBOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS)
/// if_/eval_if specializations
#   define AUX778076_SPEC_NAME if_
#   define HBOOST_PP_ITERATION_PARAMS_1 (3,(3, 3, <hboost/mpl/bind.hpp>))
#   include HBOOST_PP_ITERATE()

#if !defined(HBOOST_MPL_CFG_DMC_AMBIGUOUS_CTPS)
#   define AUX778076_SPEC_NAME eval_if
#   define HBOOST_PP_ITERATION_PARAMS_1 (3,(3, 3, <hboost/mpl/bind.hpp>))
#   include HBOOST_PP_ITERATE()
#endif
#endif

// real C++ version is already taken care of
#if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
    && !defined(HBOOST_MPL_CFG_NO_BIND_TEMPLATE)

namespace aux {
// apply_count_args
#define AUX778076_COUNT_ARGS_PREFIX bind
#define AUX778076_COUNT_ARGS_DEFAULT na
#define AUX778076_COUNT_ARGS_ARITY HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
#include <hboost/mpl/aux_/count_args.hpp>
}

// bind
template<
      typename F, AUX778076_BIND_PARAMS(typename T) AUX778076_DMC_PARAM()
    >
struct bind
    : aux::bind_chooser<
          aux::bind_count_args<AUX778076_BIND_PARAMS(T)>::value
        >::template result_< F,AUX778076_BIND_PARAMS(T) >::type
{
};

HBOOST_MPL_AUX_ARITY_SPEC(
      HBOOST_PP_INC(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY)
    , bind
    )

HBOOST_MPL_AUX_TEMPLATE_ARITY_SPEC(
      HBOOST_PP_INC(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY)
    , bind
    )


#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#   undef AUX778076_BIND_NESTED_DEFAULT_PARAMS
#   undef AUX778076_BIND_N_SPEC_PARAMS
#   undef AUX778076_BIND_N_PARAMS
#   undef AUX778076_BIND_DEFAULT_PARAMS
#   undef AUX778076_BIND_PARAMS
#   undef AUX778076_DMC_PARAM
#   undef AUX778076_APPLY

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_BIND_HPP_INCLUDED

///// iteration, depth == 1

// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1

#   define i_ HBOOST_PP_FRAME_ITERATION(1)

#if defined(AUX778076_SPEC_NAME)

// lazy metafunction specialization
template< template< HBOOST_MPL_PP_PARAMS(i_, typename T) > class F, typename Tag >
struct HBOOST_PP_CAT(quote,i_);

template< HBOOST_MPL_PP_PARAMS(i_, typename T) > struct AUX778076_SPEC_NAME;

template<
      typename Tag AUX778076_BIND_N_PARAMS(i_, typename T)
    >
struct HBOOST_PP_CAT(bind,i_)< 
      HBOOST_PP_CAT(quote,i_)<AUX778076_SPEC_NAME,Tag>
    AUX778076_BIND_N_PARAMS(i_,T)
    >
{
    template<
          AUX778076_BIND_NESTED_DEFAULT_PARAMS(typename U, na)
        >
    struct apply
    {
     private:
        typedef mpl::arg<1> n1;
#       define HBOOST_PP_ITERATION_PARAMS_2 (3,(1, i_, <hboost/mpl/bind.hpp>))
#       include HBOOST_PP_ITERATE()

        typedef typename AUX778076_SPEC_NAME<
              typename t1::type
            , HBOOST_MPL_PP_EXT_PARAMS(2, HBOOST_PP_INC(i_), t)
            >::type f_;

     public:
        typedef typename f_::type type;
    };
};

#undef AUX778076_SPEC_NAME

#else // AUX778076_SPEC_NAME

template<
      typename F AUX778076_BIND_N_PARAMS(i_, typename T) AUX778076_DMC_PARAM()
    >
struct HBOOST_PP_CAT(bind,i_)
{
    template<
          AUX778076_BIND_NESTED_DEFAULT_PARAMS(typename U, na)
        >
    struct apply
    {
     private:
#   if !defined(HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT)

        typedef aux::replace_unnamed_arg< F,mpl::arg<1> > r0;
        typedef typename r0::type a0;
        typedef typename r0::next n1;
        typedef typename aux::resolve_bind_arg<a0,AUX778076_BIND_PARAMS(U)>::type f_;
        ///
#   else
        typedef typename aux::resolve_bind_arg<F,AUX778076_BIND_PARAMS(U)>::type f_;

#   endif // HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT

#   if i_ > 0
#       define HBOOST_PP_ITERATION_PARAMS_2 (3,(1, i_, <hboost/mpl/bind.hpp>))
#       include HBOOST_PP_ITERATE()
#   endif

     public:

#   define AUX778076_ARG(unused, i_, t) \
    HBOOST_PP_COMMA_IF(i_) \
    typename HBOOST_PP_CAT(t,HBOOST_PP_INC(i_))::type \
/**/

        typedef typename HBOOST_PP_CAT(apply_wrap,i_)<
              f_ 
            HBOOST_PP_COMMA_IF(i_) HBOOST_MPL_PP_REPEAT(i_, AUX778076_ARG, t)
            >::type type;

#   undef AUX778076_ARG
    };
};

namespace aux {

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template<
      typename F AUX778076_BIND_N_PARAMS(i_, typename T), AUX778076_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg<
      HBOOST_PP_CAT(bind,i_)<F AUX778076_BIND_N_PARAMS(i_,T)>,AUX778076_BIND_PARAMS(U)
    >
{
    typedef HBOOST_PP_CAT(bind,i_)<F AUX778076_BIND_N_PARAMS(i_,T)> f_;
    typedef typename AUX778076_APPLY<f_, AUX778076_BIND_PARAMS(U)>::type type;
};

#else

template<
      typename F AUX778076_BIND_N_PARAMS(i_, typename T)
    >
aux::yes_tag
is_bind_helper(HBOOST_PP_CAT(bind,i_)<F AUX778076_BIND_N_PARAMS(i_,T)>*);

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace aux

HBOOST_MPL_AUX_ARITY_SPEC(HBOOST_PP_INC(i_), HBOOST_PP_CAT(bind,i_))
HBOOST_MPL_AUX_TEMPLATE_ARITY_SPEC(HBOOST_PP_INC(i_), HBOOST_PP_CAT(bind,i_))

#   if !defined(HBOOST_MPL_CFG_NO_BIND_TEMPLATE)
#   if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    
#if i_ == HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
/// primary template (not a specialization!)
template<
      typename F AUX778076_BIND_N_PARAMS(i_, typename T) AUX778076_DMC_PARAM()
    >
struct bind
    : HBOOST_PP_CAT(bind,i_)<F AUX778076_BIND_N_PARAMS(i_,T) >
{
};
#else
template<
      typename F AUX778076_BIND_N_PARAMS(i_, typename T) AUX778076_DMC_PARAM()
    >
struct bind< F AUX778076_BIND_N_SPEC_PARAMS(i_, T, na) >
    : HBOOST_PP_CAT(bind,i_)<F AUX778076_BIND_N_PARAMS(i_,T) >
{
};
#endif

#   else // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace aux {

template<>
struct bind_chooser<i_>
{
    template<
          typename F, AUX778076_BIND_PARAMS(typename T)
        >
    struct result_
    {
        typedef HBOOST_PP_CAT(bind,i_)< F AUX778076_BIND_N_PARAMS(i_,T) > type;
    };
};

} // namespace aux

#   endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#   endif // HBOOST_MPL_CFG_NO_BIND_TEMPLATE

#endif // AUX778076_SPEC_NAME

#   undef i_

///// iteration, depth == 2

#elif HBOOST_PP_ITERATION_DEPTH() == 2

#   define j_ HBOOST_PP_FRAME_ITERATION(2)
#   if !defined(HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT)

        typedef aux::replace_unnamed_arg< HBOOST_PP_CAT(T,j_),HBOOST_PP_CAT(n,j_) > HBOOST_PP_CAT(r,j_);
        typedef typename HBOOST_PP_CAT(r,j_)::type HBOOST_PP_CAT(a,j_);
        typedef typename HBOOST_PP_CAT(r,j_)::next HBOOST_PP_CAT(n,HBOOST_PP_INC(j_));
        typedef aux::resolve_bind_arg<HBOOST_PP_CAT(a,j_), AUX778076_BIND_PARAMS(U)> HBOOST_PP_CAT(t,j_);
        ///
#   else
        typedef aux::resolve_bind_arg< HBOOST_PP_CAT(T,j_),AUX778076_BIND_PARAMS(U)> HBOOST_PP_CAT(t,j_);

#   endif
#   undef j_

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif // HBOOST_PP_IS_ITERATING
