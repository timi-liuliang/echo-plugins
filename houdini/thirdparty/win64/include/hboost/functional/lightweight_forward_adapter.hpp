/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef HBOOST_FUNCTIONAL_LIGHTWEIGHT_FORWARD_ADAPTER_HPP_INCLUDED
#   ifndef HBOOST_PP_IS_ITERATING

#   include <hboost/config.hpp>
#   include <hboost/detail/workaround.hpp>

#   include <hboost/preprocessor/cat.hpp>
#   include <hboost/preprocessor/iteration/iterate.hpp>
#   include <hboost/preprocessor/repetition/enum.hpp>
#   include <hboost/preprocessor/repetition/enum_params.hpp>
#   include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#   include <hboost/preprocessor/facilities/intercept.hpp>

#   include <hboost/utility/result_of.hpp>
#   include <hboost/ref.hpp>

#   ifndef HBOOST_FUNCTIONAL_LIGHTWEIGHT_FORWARD_ADAPTER_MAX_ARITY
#     define HBOOST_FUNCTIONAL_LIGHTWEIGHT_FORWARD_ADAPTER_MAX_ARITY 10
#   elif HBOOST_FUNCTIONAL_FORDWARD_ADAPTER_MAX_ARITY < 3
#     undef  HBOOST_FUNCTIONAL_LIGHTWEIGHT_FORWARD_ADAPTER_MAX_ARITY
#     define HBOOST_FUNCTIONAL_LIGHTWEIGHT_FORWARD_ADAPTER_MAX_ARITY 3
#   endif

namespace hboost 
{
    template< typename Function, int Arity_Or_MinArity = -1, int MaxArity = -1 >
    class lightweight_forward_adapter;

    //----- ---- --- -- - -  -   -

    namespace detail
    {
        template< class MostDerived, typename Function, typename FunctionConst, 
            int Arity, int MinArity >
        struct lightweight_forward_adapter_impl;

        struct lightweight_forward_adapter_result
        {
            template< typename Sig > struct apply;

            // Utility metafunction for argument transform
            template< typename T > struct x  { typedef T const& t; };
            template< typename T > struct x< hboost::reference_wrapper<T> >
            { typedef T& t; };
            template< typename T > struct x<T&>       : x<T> { };
            template< typename T > struct x<T const&> : x<T> { };
            template< typename T > struct x<T const>  : x<T> { };

            // Utility metafunction to choose target function qualification
            template< typename T > struct c
            { typedef typename T::target_function_t t; };
            template< typename T > struct c<T&      >
            { typedef typename T::target_function_t t; };
            template< typename T > struct c<T const >
            { typedef typename T::target_function_const_t t; };
            template< typename T > struct c<T const&>
            { typedef typename T::target_function_const_t t; };
        };
    }

#   define HBOOST_TMP_MACRO(f,fn,fc) \
        hboost::detail::lightweight_forward_adapter_impl< \
            lightweight_forward_adapter<f,Arity_Or_MinArity,MaxArity>, fn, fc, \
            (MaxArity!=-1? MaxArity :Arity_Or_MinArity!=-1? Arity_Or_MinArity \
                :HBOOST_FUNCTIONAL_LIGHTWEIGHT_FORWARD_ADAPTER_MAX_ARITY), \
            (Arity_Or_MinArity!=-1? Arity_Or_MinArity : 0) >

    template< typename Function, int Arity_Or_MinArity, int MaxArity >
    class lightweight_forward_adapter
        : public HBOOST_TMP_MACRO(Function,Function,Function const)
        , private Function
    {
      public:
        lightweight_forward_adapter(Function const& f = Function()) 
          : Function(f) 
        { }

        typedef Function        target_function_t;
        typedef Function const  target_function_const_t;

        Function       & target_function()       { return *this; }
        Function const & target_function() const { return *this; }

        template< typename Sig > struct result
            : detail::lightweight_forward_adapter_result::template apply<Sig>
        { };

        using HBOOST_TMP_MACRO(Function,Function, Function const)::operator();
    };
    template< typename Function, int Arity_Or_MinArity, int MaxArity >
    class lightweight_forward_adapter< Function const, Arity_Or_MinArity, 
        MaxArity >
        : public HBOOST_TMP_MACRO(Function const, Function const, Function const)
        , private Function
    {
      public:
        lightweight_forward_adapter(Function const& f = Function())
          : Function(f) 
        { }

        typedef Function const target_function_t;
        typedef Function const target_function_const_t;

        Function const & target_function() const { return *this; }

        template< typename Sig > struct result
            : detail::lightweight_forward_adapter_result::template apply<Sig>
        { };

        using HBOOST_TMP_MACRO(Function const,Function const, Function const)
            ::operator();
    };
    template< typename Function, int Arity_Or_MinArity, int MaxArity >
    class lightweight_forward_adapter< Function &, Arity_Or_MinArity, MaxArity >
        : public HBOOST_TMP_MACRO(Function&, Function, Function)
    {
        Function& ref_function;
      public:
        lightweight_forward_adapter(Function& f)
          : ref_function(f) 
        { }

        typedef Function target_function_t;
        typedef Function target_function_const_t;

        Function & target_function() const { return this->ref_function; }

        template< typename Sig > struct result
            : detail::lightweight_forward_adapter_result::template apply<Sig>
        { };

        using HBOOST_TMP_MACRO(Function&, Function, Function)::operator();
    }; 

    #undef HBOOST_TMP_MACRO

    namespace detail
    {
        template< class Self >
        struct lightweight_forward_adapter_result::apply< Self() >
            : hboost::result_of< HBOOST_DEDUCED_TYPENAME c<Self>::t() >
        { };

        template< class MD, class F, class FC >
        struct lightweight_forward_adapter_impl<MD,F,FC,0,0>
            : lightweight_forward_adapter_result
        {
            inline typename hboost::result_of< FC() >::type
            operator()() const
            {
                return static_cast<MD const*>(this)->target_function()();
            }

            inline typename hboost::result_of< F() >::type
            operator()()
            {
                return static_cast<MD*>(this)->target_function()();
            }
        };

#       define  HBOOST_PP_FILENAME_1 \
            <hboost/functional/lightweight_forward_adapter.hpp>
#       define  HBOOST_PP_ITERATION_LIMITS                                     \
            (1,HBOOST_FUNCTIONAL_LIGHTWEIGHT_FORWARD_ADAPTER_MAX_ARITY) 
#       include HBOOST_PP_ITERATE()

    } // namespace detail

    template<class F, int A0, int A1>
    struct result_of<hboost::lightweight_forward_adapter<F,A0,A1> const ()>
        : hboost::detail::lightweight_forward_adapter_result::template apply<
            hboost::lightweight_forward_adapter<F,A0,A1> const () >
    { };
    template<class F, int A0, int A1>
    struct result_of<hboost::lightweight_forward_adapter<F,A0,A1>()>
        : hboost::detail::lightweight_forward_adapter_result::template apply<
            hboost::lightweight_forward_adapter<F,A0,A1>() >
    { };
    template<class F, int A0, int A1>
    struct result_of<hboost::lightweight_forward_adapter<F,A0,A1> const& ()>
        : hboost::detail::lightweight_forward_adapter_result::template apply<
            hboost::lightweight_forward_adapter<F,A0,A1> const () >
    { };
    template<class F, int A0, int A1>
    struct result_of<hboost::lightweight_forward_adapter<F,A0,A1>& ()>
        : hboost::detail::lightweight_forward_adapter_result::template apply<
            hboost::lightweight_forward_adapter<F,A0,A1>() >
    { };
}

#     define HBOOST_FUNCTIONAL_LIGHTWEIGHT_FORWARD_ADAPTER_HPP_INCLUDED

#   else // defined(HBOOST_PP_IS_ITERATING)
#     define N HBOOST_PP_ITERATION() 

        template< class Self, HBOOST_PP_ENUM_PARAMS(N,typename T) >
        struct lightweight_forward_adapter_result::apply<
            Self (HBOOST_PP_ENUM_PARAMS(N,T)) >
            : hboost::result_of<
                HBOOST_DEDUCED_TYPENAME c<Self>::t (HBOOST_PP_ENUM_BINARY_PARAMS(N,
                    typename x<T,>::t HBOOST_PP_INTERCEPT)) >
        { };

        template< class MD, class F, class FC >
        struct lightweight_forward_adapter_impl<MD,F,FC,HBOOST_PP_DEC(N),N>
            : lightweight_forward_adapter_result
        {
            template< HBOOST_PP_ENUM_PARAMS(N,typename T) >
            inline typename hboost::result_of< F(HBOOST_PP_ENUM_BINARY_PARAMS(N,
                T,const& HBOOST_PP_INTERCEPT)) >::type
            operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N,T,& HBOOST_PP_INTERCEPT));
        };

        template< class MD, class F, class FC, int MinArity >
        struct lightweight_forward_adapter_impl<MD,F,FC,N,MinArity>
            : lightweight_forward_adapter_impl<MD,F,FC,HBOOST_PP_DEC(N),MinArity>
        {
            using lightweight_forward_adapter_impl<MD,F,FC,HBOOST_PP_DEC(N),
                MinArity>::operator();

#     define M(z,i,d) \
          static_cast<typename d::template x<T##i>::t>(a##i)

            template< HBOOST_PP_ENUM_PARAMS(N,typename T) >
            inline typename lightweight_forward_adapter_result::template apply<
                MD const (HBOOST_PP_ENUM_BINARY_PARAMS(N,
                    T,const& HBOOST_PP_INTERCEPT)) >::type
            operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N,T,const& a)) const
            {
                typedef lightweight_forward_adapter_result _;
                return static_cast<MD const*>(this)->target_function()(
                    HBOOST_PP_ENUM(N,M,_));
            }
            template< HBOOST_PP_ENUM_PARAMS(N,typename T) >
            inline typename lightweight_forward_adapter_result::template apply<
                MD (HBOOST_PP_ENUM_BINARY_PARAMS(N,
                    T,const& HBOOST_PP_INTERCEPT)) >::type
            operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N,T,const& a))
            {
                typedef lightweight_forward_adapter_result _;
                return static_cast<MD*>(this)->target_function()(
                    HBOOST_PP_ENUM(N,M,_));
            }
#     undef M
      };

#     undef N
#   endif // defined(HBOOST_PP_IS_ITERATING)

#endif // include guard

