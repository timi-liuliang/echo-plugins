
// Copyright (C) 2009-2012 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt)
// Home at http://www.boost.org/libs/functional/overloaded_function

#if !HBOOST_PP_IS_ITERATING
#   ifndef HBOOST_FUNCTIONAL_OVERLOADED_FUNCTION_DETAIL_BASE_HPP_
#       define HBOOST_FUNCTIONAL_OVERLOADED_FUNCTION_DETAIL_BASE_HPP_

#       include <hboost/functional/overloaded_function/config.hpp>
#       include <hboost/function.hpp>
#       include <hboost/preprocessor/iteration/iterate.hpp>
#       include <hboost/preprocessor/repetition/enum.hpp>
#       include <hboost/preprocessor/cat.hpp>
#       include <hboost/preprocessor/comma_if.hpp>

#define HBOOST_FUNCTIONAL_DETAIL_arg_type(z, n, unused) \
    HBOOST_PP_CAT(A, n)

#define HBOOST_FUNCTIONAL_DETAIL_arg_name(z, n, unused) \
    HBOOST_PP_CAT(a, n)

#define HBOOST_FUNCTIONAL_DETAIL_arg_tparam(z, n, unused) \
    typename HBOOST_FUNCTIONAL_DETAIL_arg_type(z, n, unused)

#define HBOOST_FUNCTIONAL_DETAIL_arg(z, n, unused) \
    HBOOST_FUNCTIONAL_DETAIL_arg_type(z, n, unused) \
    HBOOST_FUNCTIONAL_DETAIL_arg_name(z, n, unused)

#define HBOOST_FUNCTIONAL_DETAIL_f \
    R (HBOOST_PP_ENUM(HBOOST_FUNCTIONAL_DETAIL_arity, \
            HBOOST_FUNCTIONAL_DETAIL_arg_type, ~))

// Do not use namespace ::detail because overloaded_function is already a class.
namespace hboost { namespace overloaded_function_detail {

template<typename F>
class base {}; // Empty template cannot be used directly (only its spec).

#       define HBOOST_PP_ITERATION_PARAMS_1 \
                (3, (0, HBOOST_FUNCTIONAL_OVERLOADED_FUNCTION_CONFIG_ARITY_MAX, \
                "hboost/functional/overloaded_function/detail/base.hpp"))
#       include HBOOST_PP_ITERATE() // Iterate over funciton arity.

} } // namespace

#undef HBOOST_FUNCTIONAL_DETAIL_arg_type
#undef HBOOST_FUNCTIONAL_DETAIL_arg_name
#undef HBOOST_FUNCTIONAL_DETAIL_arg_tparam
#undef HBOOST_FUNCTIONAL_DETAIL_arg
#undef HBOOST_FUNCTIONAL_DETAIL_f

#   endif // #include guard

#elif HBOOST_PP_ITERATION_DEPTH() == 1
#   define HBOOST_FUNCTIONAL_DETAIL_arity HBOOST_PP_FRAME_ITERATION(1)

template<
    typename R
    HBOOST_PP_COMMA_IF(HBOOST_FUNCTIONAL_DETAIL_arity)
    HBOOST_PP_ENUM(HBOOST_FUNCTIONAL_DETAIL_arity,
            HBOOST_FUNCTIONAL_DETAIL_arg_tparam, ~)
>
class base< HBOOST_FUNCTIONAL_DETAIL_f > {
public:
    /* implicit */ inline base(
            // This requires specified type to be implicitly convertible to
            // a hboost::function<> functor.
            hboost::function< HBOOST_FUNCTIONAL_DETAIL_f > const& f): f_(f)
    {}

    inline R operator()(HBOOST_PP_ENUM(HBOOST_FUNCTIONAL_DETAIL_arity,
            HBOOST_FUNCTIONAL_DETAIL_arg, ~)) const {
        return f_(HBOOST_PP_ENUM(HBOOST_FUNCTIONAL_DETAIL_arity,
                HBOOST_FUNCTIONAL_DETAIL_arg_name, ~));
    }

private:
    hboost::function< HBOOST_FUNCTIONAL_DETAIL_f > const f_;
};

#   undef HBOOST_FUNCTIONAL_DETAIL_arity
#endif // iteration

