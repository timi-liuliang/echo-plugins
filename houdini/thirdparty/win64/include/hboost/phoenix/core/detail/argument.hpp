/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010-2011 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

    #define HBOOST_PHOENIX_ARGUMENT_N_TYPE(_, N, name)                           \
    typedef                                                                     \
        expression::argument<HBOOST_PP_INC(N)>::type                             \
        HBOOST_PP_CAT(HBOOST_PP_CAT(name, HBOOST_PP_INC(N)), _type);               \
    /**/
    
    #define HBOOST_PHOENIX_ARGUMENT_N_INSTANCE(_, N, name)                       \
    expression::argument<HBOOST_PP_INC(N)>::type const                           \
        HBOOST_PP_CAT(name, HBOOST_PP_INC(N)) = {{{}}};                           \
    /**/


    namespace placeholders
    {
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ARG_LIMIT, HBOOST_PHOENIX_ARGUMENT_N_TYPE, arg)
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ARG_LIMIT, HBOOST_PHOENIX_ARGUMENT_N_TYPE, _)
#ifndef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ARG_LIMIT, HBOOST_PHOENIX_ARGUMENT_N_INSTANCE, arg)
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ARG_LIMIT, HBOOST_PHOENIX_ARGUMENT_N_INSTANCE, _)
#endif
    }

    namespace arg_names
    {
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ARG_LIMIT, HBOOST_PHOENIX_ARGUMENT_N_TYPE, arg)
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ARG_LIMIT, HBOOST_PHOENIX_ARGUMENT_N_TYPE, _)
#ifndef HBOOST_PHOENIX_NO_PREDEFINED_TERMINALS
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ARG_LIMIT, HBOOST_PHOENIX_ARGUMENT_N_INSTANCE, arg)
        HBOOST_PP_REPEAT(HBOOST_PHOENIX_ARG_LIMIT, HBOOST_PHOENIX_ARGUMENT_N_INSTANCE, _)
#endif
    }

    #undef HBOOST_PHOENIX_ARGUMENT_N_TYPE
    #undef HBOOST_PHOENIX_ARGUMENT_N_INSTANCE

