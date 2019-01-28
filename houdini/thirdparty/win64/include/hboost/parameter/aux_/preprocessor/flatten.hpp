// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PARAMETER_FLATTEN_051217_HPP
# define HBOOST_PARAMETER_FLATTEN_051217_HPP

# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/seq/for_each.hpp>
# include <hboost/preprocessor/seq/for_each_i.hpp>
# include <hboost/preprocessor/identity.hpp>
# include <hboost/preprocessor/selection/max.hpp>
# include <hboost/preprocessor/arithmetic/sub.hpp>
# include <hboost/preprocessor/repetition/enum_trailing.hpp>
# include <hboost/parameter/aux_/preprocessor/for_each.hpp>

# define HBOOST_PARAMETER_FLATTEN_SPLIT_required required,
# define HBOOST_PARAMETER_FLATTEN_SPLIT_optional optional,
# define HBOOST_PARAMETER_FLATTEN_SPLIT_deduced deduced,

# define HBOOST_PARAMETER_FLATTEN_SPLIT(sub) \
    HBOOST_PP_CAT(HBOOST_PARAMETER_FLATTEN_SPLIT_, sub)

# define HBOOST_PARAMETER_FLATTEN_QUALIFIER(sub) \
    HBOOST_PP_SPLIT(0, HBOOST_PARAMETER_FLATTEN_SPLIT(sub))

# define HBOOST_PARAMETER_FLATTEN_ARGS(sub) \
    HBOOST_PP_SPLIT(1, HBOOST_PARAMETER_FLATTEN_SPLIT(sub))

# define HBOOST_PARAMETER_FLATTEN_ARITY_optional(arities) \
    HBOOST_PP_TUPLE_ELEM(3,0,arities)

# define HBOOST_PARAMETER_FLATTEN_ARITY_required(arities) \
    HBOOST_PP_TUPLE_ELEM(3,1,arities)

# define HBOOST_PARAMETER_FLATTEN_SPEC0_DUMMY_ELEM(z, n, data) ~
# define HBOOST_PARAMETER_FLATTEN_SPEC0(r, n, elem, data) \
    (( \
        HBOOST_PP_TUPLE_ELEM(3,2,data) \
      , HBOOST_PP_TUPLE_REM(HBOOST_PP_TUPLE_ELEM(3,0,data)) elem \
        HBOOST_PP_ENUM_TRAILING( \
            HBOOST_PP_SUB( \
                HBOOST_PP_TUPLE_ELEM(3,1,data) \
              , HBOOST_PP_TUPLE_ELEM(3,0,data) \
            ) \
          , HBOOST_PARAMETER_FLATTEN_SPEC0_DUMMY_ELEM \
          , ~ \
        ) \
    ))

# define HBOOST_PARAMETER_FLATTEN_SPEC_AUX(r, arity, max_arity, spec, transform) \
    HBOOST_PARAMETER_FOR_EACH_R( \
        r \
      , arity \
      , HBOOST_PARAMETER_FLATTEN_ARGS(spec) \
      , (arity, max_arity, transform(HBOOST_PARAMETER_FLATTEN_QUALIFIER(spec))) \
      , HBOOST_PARAMETER_FLATTEN_SPEC0 \
    )

# define HBOOST_PARAMETER_FLATTEN_IDENTITY(x) x

# define HBOOST_PARAMETER_FLATTEN_SPEC_optional(r, arities, spec) \
    HBOOST_PARAMETER_FLATTEN_SPEC_AUX( \
        r \
      , HBOOST_PP_CAT( \
            HBOOST_PARAMETER_FLATTEN_ARITY_, HBOOST_PARAMETER_FLATTEN_QUALIFIER(spec) \
        )(arities) \
      , HBOOST_PP_TUPLE_ELEM(3,2,arities) \
      , spec \
      , HBOOST_PARAMETER_FLATTEN_IDENTITY \
    )

# define HBOOST_PARAMETER_FLATTEN_SPEC_required(r, arities, spec) \
    HBOOST_PARAMETER_FLATTEN_SPEC_optional(r, arities, spec)

# define HBOOST_PARAMETER_FLATTEN_SPEC_AS_DEDUCED(x) HBOOST_PP_CAT(deduced_,x)

# define HBOOST_PARAMETER_FLATTEN_SPEC_deduced_M(r, arities, n, spec) \
    HBOOST_PARAMETER_FLATTEN_SPEC_AUX( \
        r \
      , HBOOST_PP_CAT( \
            HBOOST_PARAMETER_FLATTEN_ARITY_, HBOOST_PARAMETER_FLATTEN_QUALIFIER(spec) \
        )(arities) \
      , HBOOST_PP_TUPLE_ELEM(3,2,arities) \
      , spec \
      , HBOOST_PARAMETER_FLATTEN_SPEC_AS_DEDUCED \
    )

# define HBOOST_PARAMETER_FLATTEN_SPEC_deduced(r, arities, spec) \
    HBOOST_PP_SEQ_FOR_EACH_I_R( \
        r \
      , HBOOST_PARAMETER_FLATTEN_SPEC_deduced_M \
      , arities \
      , HBOOST_PARAMETER_FLATTEN_ARGS(spec) \
    )

# define HBOOST_PARAMETER_FLATTEN_SPEC(r, arities, spec) \
    HBOOST_PP_CAT( \
        HBOOST_PARAMETER_FLATTEN_SPEC_, HBOOST_PARAMETER_FLATTEN_QUALIFIER(spec) \
    )(r, arities, spec)

# define HBOOST_PARAMETER_FLATTEN(optional_arity, required_arity, wanted_arity, specs) \
    HBOOST_PP_SEQ_FOR_EACH( \
        HBOOST_PARAMETER_FLATTEN_SPEC \
      , ( \
            optional_arity, required_arity \
          , wanted_arity \
        ) \
      , specs \
    )

#endif // HBOOST_PARAMETER_FLATTEN_051217_HPP

