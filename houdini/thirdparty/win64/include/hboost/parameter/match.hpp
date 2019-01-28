// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PARAMETER_MATCH_DWA2005714_HPP
# define HBOOST_PARAMETER_MATCH_DWA2005714_HPP

# include <hboost/detail/workaround.hpp>
# include <hboost/preprocessor/seq/enum.hpp>

# if HBOOST_WORKAROUND(__MWERKS__, <= 0x3003)
// Temporary version of HBOOST_PP_SEQ_ENUM until Paul M. integrates the workaround.
#  define HBOOST_PARAMETER_SEQ_ENUM_I(size,seq) HBOOST_PP_CAT(HBOOST_PP_SEQ_ENUM_, size) seq
#  define HBOOST_PARAMETER_SEQ_ENUM(seq) HBOOST_PARAMETER_SEQ_ENUM_I(HBOOST_PP_SEQ_SIZE(seq), seq)
# else
#  define HBOOST_PARAMETER_SEQ_ENUM(seq) HBOOST_PP_SEQ_ENUM(seq)
# endif 

# if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))

#  include <hboost/parameter/config.hpp>
#  include <hboost/parameter/aux_/void.hpp>
#  include <hboost/preprocessor/arithmetic/sub.hpp>
#  include <hboost/preprocessor/facilities/intercept.hpp>
#  include <hboost/preprocessor/repetition/enum_trailing_params.hpp>

#  define HBOOST_PARAMETER_MATCH_DEFAULTS(ArgTypes)              \
        HBOOST_PP_ENUM_TRAILING_PARAMS(                          \
            HBOOST_PP_SUB(                                       \
                HBOOST_PARAMETER_MAX_ARITY                       \
              , HBOOST_PP_SEQ_SIZE(ArgTypes)                     \
            )                                                   \
          , ::hboost::parameter::void_ HBOOST_PP_INTERCEPT   \
        )

# else

#  define HBOOST_PARAMETER_MATCH_DEFAULTS(ArgTypes)

# endif 

//
// Generates, e.g.
//
//    typename dfs_params::match<A1,A2>::type name = dfs_params()
//
// with workarounds for Borland compatibility.
//

# define HBOOST_PARAMETER_MATCH(ParameterSpec, ArgTypes, name)   \
    typename ParameterSpec ::match<                             \
        HBOOST_PARAMETER_SEQ_ENUM(ArgTypes)                      \
        HBOOST_PARAMETER_MATCH_DEFAULTS(ArgTypes)                \
    >::type name = ParameterSpec ()

#endif // HBOOST_PARAMETER_MATCH_DWA2005714_HPP
