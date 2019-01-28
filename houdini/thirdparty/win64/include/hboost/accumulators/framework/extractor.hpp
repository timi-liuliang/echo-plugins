///////////////////////////////////////////////////////////////////////////////
// extractor.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ACCUMULATORS_FRAMEWORK_EXTRACTOR_HPP_EAN_28_10_2005
#define HBOOST_ACCUMULATORS_FRAMEWORK_EXTRACTOR_HPP_EAN_28_10_2005

#include <hboost/preprocessor/tuple/rem.hpp>
#include <hboost/preprocessor/array/size.hpp>
#include <hboost/preprocessor/array/data.hpp>
#include <hboost/preprocessor/array/elem.hpp>
#include <hboost/preprocessor/seq/to_array.hpp>
#include <hboost/preprocessor/seq/transform.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <hboost/parameter/binding.hpp>
#include <hboost/mpl/apply.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/accumulators/accumulators_fwd.hpp>
#include <hboost/accumulators/framework/parameters/accumulator.hpp>

namespace hboost { namespace accumulators
{

namespace detail
{
    template<typename AccumulatorSet, typename Feature>
    struct accumulator_set_result
    {
        typedef typename as_feature<Feature>::type feature_type;
        typedef typename mpl::apply<AccumulatorSet, feature_type>::type::result_type type;
    };

    template<typename Args, typename Feature>
    struct argument_pack_result
      : accumulator_set_result<
            typename remove_reference<
                typename parameter::binding<Args, tag::accumulator>::type
            >::type
          , Feature
        >
    {
    };

    template<typename A, typename Feature>
    struct extractor_result
      : mpl::eval_if<
            detail::is_accumulator_set<A>
          , accumulator_set_result<A, Feature>
          , argument_pack_result<A, Feature>
        >
    {
    };

    template<typename Feature, typename AccumulatorSet>
    typename extractor_result<AccumulatorSet, Feature>::type
    do_extract(AccumulatorSet const &acc, mpl::true_)
    {
        typedef typename as_feature<Feature>::type feature_type;
        return extract_result<feature_type>(acc);
    }

    template<typename Feature, typename Args>
    typename extractor_result<Args, Feature>::type
    do_extract(Args const &args, mpl::false_)
    {
        typedef typename as_feature<Feature>::type feature_type;
        return find_accumulator<feature_type>(args[accumulator]).result(args);
    }

} // namespace detail


///////////////////////////////////////////////////////////////////////////////
/// Extracts the result associated with Feature from the specified accumulator_set.
template<typename Feature>
struct extractor
{
    typedef extractor<Feature> this_type;

    /// The result meta-function for determining the return type of the extractor
    template<typename F>
    struct result;

    template<typename A1>
    struct result<this_type(A1)>
      : detail::extractor_result<A1, Feature>
    {
    };

    /// Extract the result associated with Feature from the accumulator set
    /// \param acc The accumulator set object from which to extract the result
    template<typename Arg1>
    typename detail::extractor_result<Arg1, Feature>::type
    operator ()(Arg1 const &arg1) const
    {
        // Arg1 could be an accumulator_set or an argument pack containing
        // an accumulator_set. Dispatch accordingly.
        return detail::do_extract<Feature>(arg1, detail::is_accumulator_set<Arg1>());
    }

    /// \overload
    ///
    /// \param a1 Optional named parameter to be passed to the accumulator's result() function.
    template<typename AccumulatorSet, typename A1>
    typename detail::extractor_result<AccumulatorSet, Feature>::type
    operator ()(AccumulatorSet const &acc, A1 const &a1) const
    {
        HBOOST_MPL_ASSERT((detail::is_accumulator_set<AccumulatorSet>));
        typedef typename as_feature<Feature>::type feature_type;
        return extract_result<feature_type>(acc, a1);
    }

    // ... other overloads generated by Boost.Preprocessor:

    /// INTERNAL ONLY
    ///
#define HBOOST_ACCUMULATORS_EXTRACTOR_FUN_OP(z, n, _)                                    \
    template<HBOOST_PP_ENUM_PARAMS_Z(z, n, typename A)>                                  \
    struct result<this_type(HBOOST_PP_ENUM_PARAMS_Z(z, n, A))>                           \
      : detail::extractor_result<A1, Feature>                                           \
    {};                                                                                 \
    template<                                                                           \
        typename AccumulatorSet                                                         \
        HBOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, typename A)                               \
    >                                                                                   \
    typename detail::extractor_result<AccumulatorSet, Feature>::type                    \
    operator ()(                                                                        \
        AccumulatorSet const &acc                                                       \
        HBOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, n, A, const &a)                       \
    ) const                                                                             \
    {                                                                                   \
        HBOOST_MPL_ASSERT((detail::is_accumulator_set<AccumulatorSet>));                 \
        typedef typename as_feature<Feature>::type feature_type;                        \
        return extract_result<feature_type>(acc HBOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, a));\
    }

    HBOOST_PP_REPEAT_FROM_TO(
        2
      , HBOOST_PP_INC(HBOOST_ACCUMULATORS_MAX_ARGS)
      , HBOOST_ACCUMULATORS_EXTRACTOR_FUN_OP
      , _
    )

    #ifdef HBOOST_ACCUMULATORS_DOXYGEN_INVOKED
    /// \overload
    ///
    template<typename AccumulatorSet, typename A1, typename A2, ...>
    typename detail::extractor_result<AccumulatorSet, Feature>::type
    operator ()(AccumulatorSet const &acc, A1 const &a1, A2 const &a2, ...);
    #endif
};

/// INTERNAL ONLY
///
#define HBOOST_ACCUMULATORS_ARRAY_REM(Array)                                                         \
    HBOOST_PP_TUPLE_REM_CTOR(HBOOST_PP_ARRAY_SIZE(Array), HBOOST_PP_ARRAY_DATA(Array))

/// INTERNAL ONLY
///
#define HBOOST_ACCUMULATORS_SEQ_REM(Seq)                                                             \
    HBOOST_ACCUMULATORS_ARRAY_REM(HBOOST_PP_SEQ_TO_ARRAY(Seq))

/// INTERNAL ONLY
///
#define HBOOST_ACCUMULATORS_ARGS_OP(s, data, elem)                                                   \
    T ## s

/// INTERNAL ONLY
///
#define HBOOST_ACCUMULATORS_PARAMS_OP(s, data, elem)                                                 \
    elem T ## s

/// INTERNAL ONLY
///
#define HBOOST_ACCUMULATORS_MAKE_FEATURE(Tag, Feature, ParamsSeq)                                    \
    Tag::Feature<                                                                                   \
        HBOOST_ACCUMULATORS_SEQ_REM(                                                                 \
            HBOOST_PP_SEQ_TRANSFORM(HBOOST_ACCUMULATORS_ARGS_OP, ~, ParamsSeq)                        \
        )                                                                                           \
    >

/// INTERNAL ONLY
///
#define HBOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN_IMPL(z, n, Tag, Feature, ParamsSeq)                 \
    template<                                                                                       \
        HBOOST_ACCUMULATORS_SEQ_REM(                                                                 \
            HBOOST_PP_SEQ_TRANSFORM(HBOOST_ACCUMULATORS_PARAMS_OP, ~, ParamsSeq)                      \
        )                                                                                           \
      , typename Arg1                                                                               \
        HBOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, typename A)                                           \
    >                                                                                               \
    typename hboost::accumulators::detail::extractor_result<                                         \
        Arg1                                                                                        \
      , HBOOST_ACCUMULATORS_MAKE_FEATURE(Tag, Feature, ParamsSeq)                                    \
    >::type                                                                                         \
    Feature(Arg1 const &arg1 HBOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, n, A, const &a) )            \
    {                                                                                               \
        typedef HBOOST_ACCUMULATORS_MAKE_FEATURE(Tag, Feature, ParamsSeq) feature_type;              \
        return hboost::accumulators::extractor<feature_type>()(                                      \
            arg1 HBOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, a));                                         \
    }

/// INTERNAL ONLY
///
#define HBOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN(z, n, _)                                            \
    HBOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN_IMPL(                                                   \
        z                                                                                           \
      , n                                                                                           \
      , HBOOST_PP_ARRAY_ELEM(0, _)                                                                   \
      , HBOOST_PP_ARRAY_ELEM(1, _)                                                                   \
      , HBOOST_PP_ARRAY_ELEM(2, _)                                                                   \
    )

#define HBOOST_ACCUMULATORS_DEFINE_EXTRACTOR(Tag, Feature, ParamSeq)                                 \
    HBOOST_PP_REPEAT(                                                                                \
        HBOOST_PP_INC(HBOOST_ACCUMULATORS_MAX_ARGS)                                                   \
      , HBOOST_ACCUMULATORS_DEFINE_EXTRACTOR_FUN                                                     \
      , (3, (Tag, Feature, ParamSeq))                                                               \
    )

}} // namespace hboost::accumulators

#endif