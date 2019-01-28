// Boost.Range library
//
//  Copyright Neil Groves 2014. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_DETAIL_COMBINE_CXX03_HPP
#define HBOOST_RANGE_DETAIL_COMBINE_CXX03_HPP

#ifndef HBOOST_RANGE_MIN_COMBINE_ARGS
#define HBOOST_RANGE_MIN_COMBINE_ARGS 2
#endif

#ifndef HBOOST_RANGE_MAX_COMBINE_ARGS
#define HBOOST_RANGE_MAX_COMBINE_ARGS 5
#endif

#include <hboost/config.hpp>
#include <hboost/iterator/zip_iterator.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/arithmetic/div.hpp>
#include <hboost/preprocessor/arithmetic/mul.hpp>
#include <hboost/preprocessor/control.hpp>
#include <hboost/preprocessor/control/while.hpp>
#include <hboost/preprocessor/facilities/empty.hpp>
#include <hboost/preprocessor/facilities/identity.hpp>
#include <hboost/preprocessor/iteration/local.hpp>
#include <hboost/preprocessor/punctuation/comma.hpp>
#include <hboost/preprocessor/repetition.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/range/iterator_range_core.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/mpl/transform.hpp>
#include <hboost/utility/result_of.hpp>

#include <vector>
#include <list>

namespace hboost
{
    namespace range_detail
    {

template<typename F, typename T, int SIZE>
struct combined_result_impl;

template<typename F, typename T>
struct combined_result
    : combined_result_impl<F, T, tuples::length<T>::value>
{
};

#define HBOOST_RANGE_combined_element(z, n, data) \
    typename tuples::element<n, T>::type

#define HBOOST_RANGE_combined_result(z, n, data) \
    template<typename F, typename T> \
    struct combined_result_impl <F,T,n> \
        : result_of<F(HBOOST_PP_ENUM(n, HBOOST_RANGE_combined_element, ~))> \
    { \
    };

#define HBOOST_PP_LOCAL_MACRO(n) HBOOST_RANGE_combined_result(~,n,~)

#define HBOOST_PP_LOCAL_LIMITS (HBOOST_RANGE_MIN_COMBINE_ARGS, \
                               HBOOST_RANGE_MAX_COMBINE_ARGS)
#include HBOOST_PP_LOCAL_ITERATE()

#define HBOOST_RANGE_combined_get(z, n, data) get<n>(tuple)

#define HBOOST_RANGE_combined_unpack(z, n, data) \
    template<typename F, typename T> inline \
    typename combined_result<F,T>::type \
    unpack_(mpl::int_<n>, F f, const T& tuple) \
    { \
        return f(HBOOST_PP_ENUM(n, HBOOST_RANGE_combined_get, ~)); \
    }

#define HBOOST_PP_LOCAL_MACRO(n) HBOOST_RANGE_combined_unpack(~,n,~)
#define HBOOST_PP_LOCAL_LIMITS (HBOOST_RANGE_MIN_COMBINE_ARGS, \
                               HBOOST_RANGE_MAX_COMBINE_ARGS)
#include HBOOST_PP_LOCAL_ITERATE()

} // namespace range_detail

namespace range
{

#define HBOOST_RANGE_combined_seq(z, n, data) hboost::data(HBOOST_PP_CAT(r,n))

#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES

#include <hboost/range/detail/combine_no_rvalue.hpp>

#else // by using rvalue references we avoid requiring 2^n overloads.

#include <hboost/range/detail/combine_rvalue.hpp>

#endif

#define HBOOST_PP_LOCAL_MACRO(n) HBOOST_RANGE_combine(~,n,~)
#define HBOOST_PP_LOCAL_LIMITS (HBOOST_RANGE_MIN_COMBINE_ARGS, \
                               HBOOST_RANGE_MAX_COMBINE_ARGS)
#include HBOOST_PP_LOCAL_ITERATE()

    } // namespace range

    using hboost::range::combine;

} // namespace hboost

#endif // include guard

#undef HBOOST_RANGE_combined_element
#undef HBOOST_RANGE_combined_result
#undef HBOOST_RANGE_combined_get
#undef HBOOST_RANGE_combined_unpack
#undef HBOOST_RANGE_combined_seq
#undef HBOOST_RANGE_combined_exp_pred
#undef HBOOST_RANGE_combined_exp_op
#undef HBOOST_RANGE_combined_exp
#undef HBOOST_RANGE_combined_bitset_pred
#undef HBOOST_RANGE_combined_bitset_op
#undef HBOOST_RANGE_combined_bitset
#undef HBOOST_RANGE_combined_range_iterator
#undef HBOOST_RANGE_combined_args
#undef HBOOST_RANGE_combine_impl
#undef HBOOST_RANGE_combine
