#if !defined(HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/proto/detail/preprocessed/poly_function_traits.hpp>

#elif !defined(HBOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/poly_function_traits.hpp")
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    // poly_function_traits.hpp
    // Contains specializations of poly_function_traits and as_mono_function
    //
    //  Copyright 2008 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define HBOOST_PP_ITERATION_PARAMS_1                                                             \
        (3, (1, HBOOST_PROTO_MAX_ARITY, <hboost/proto/detail/poly_function_traits.hpp>))
    #include HBOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(HBOOST_PROTO_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else

    #define N HBOOST_PP_ITERATION()

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename PolyFun HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct poly_function_traits<PolyFun, PolyFun(HBOOST_PP_ENUM_PARAMS(N, A)), mpl::size_t<sizeof(poly_function_t)> >
    {
        typedef typename PolyFun::template impl<HBOOST_PP_ENUM_PARAMS(N, const A)> function_type;
        typedef typename function_type::result_type result_type;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename PolyFun HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct as_mono_function_impl<PolyFun(HBOOST_PP_ENUM_PARAMS(N, A)), true>
    {
        typedef typename PolyFun::template impl<HBOOST_PP_ENUM_PARAMS(N, const A)> type;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename PolyFun HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct as_mono_function_impl<PolyFun(HBOOST_PP_ENUM_PARAMS(N, A)), false>
    {
        typedef PolyFun type;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename PolyFun HBOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct as_mono_function<PolyFun(HBOOST_PP_ENUM_PARAMS(N, A))>
      : as_mono_function_impl<PolyFun(HBOOST_PP_ENUM_PARAMS(N, A)), is_poly_function<PolyFun>::value>
    {};

    #undef N

#endif // HBOOST_PROTO_DONT_USE_PREPROCESSED_FILES
