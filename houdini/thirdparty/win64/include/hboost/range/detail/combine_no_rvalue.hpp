// Boost.Range library
//
//  Copyright Neil Groves 2014. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#define HBOOST_RANGE_combined_exp_pred(d, data) HBOOST_PP_TUPLE_ELEM(3, 0, data)

#define HBOOST_RANGE_combined_exp_op(d, data) \
 ( \
    HBOOST_PP_DEC( \
       HBOOST_PP_TUPLE_ELEM(3, 0, data) \
    ), \
    HBOOST_PP_TUPLE_ELEM(3, 1, data), \
    HBOOST_PP_MUL_D( \
       d, \
       HBOOST_PP_TUPLE_ELEM(3, 2, data), \
       HBOOST_PP_TUPLE_ELEM(3, 1, data) \
    ) \
 )

#define HBOOST_RANGE_combined_exp(x, n) \
  HBOOST_PP_TUPLE_ELEM(3, 2, \
  HBOOST_PP_WHILE(HBOOST_RANGE_combined_exp_pred, \
                 HBOOST_RANGE_combined_exp_op, (n, x, 1)))

#define HBOOST_RANGE_combined_bitset_pred(n, state) \
    HBOOST_PP_TUPLE_ELEM(2,1,state)

#define HBOOST_RANGE_combined_bitset_op(d, state) \
    (HBOOST_PP_DIV_D(d, HBOOST_PP_TUPLE_ELEM(2,0,state), 2), \
     HBOOST_PP_DEC(HBOOST_PP_TUPLE_ELEM(2,1,state)))

#define HBOOST_RANGE_combined_bitset(i, n) \
HBOOST_PP_MOD(HBOOST_PP_TUPLE_ELEM(2, 0, \
      HBOOST_PP_WHILE(HBOOST_RANGE_combined_bitset_pred, \
                     HBOOST_RANGE_combined_bitset_op, (i,n))), 2)

#define HBOOST_RANGE_combined_range_iterator(z, n, i) \
  typename range_iterator< \
      HBOOST_PP_CAT(R,n)          \
      HBOOST_PP_IF( \
          HBOOST_RANGE_combined_bitset(i,n), \
          HBOOST_PP_IDENTITY(const), \
          HBOOST_PP_EMPTY)() \
  >::type

#define HBOOST_RANGE_combined_args(z, n, i) \
  HBOOST_PP_CAT(R, n) \
  HBOOST_PP_IF(HBOOST_RANGE_combined_bitset(i,n), const&, &)  \
  HBOOST_PP_CAT(r, n)

#define HBOOST_RANGE_combine_impl(z, i, n)\
    template<HBOOST_PP_ENUM_PARAMS(n, typename R)> \
    inline range::combined_range< \
        hboost::tuple<HBOOST_PP_ENUM(n, HBOOST_RANGE_combined_range_iterator, i)> \
    > \
    combine(HBOOST_PP_ENUM(n, HBOOST_RANGE_combined_args, i)) \
    { \
        typedef tuple< \
            HBOOST_PP_ENUM(n, HBOOST_RANGE_combined_range_iterator, i) \
        > rng_tuple_t;   \
        return range::combined_range<rng_tuple_t>( \
            rng_tuple_t(HBOOST_PP_ENUM(n, HBOOST_RANGE_combined_seq, begin)), \
            rng_tuple_t(HBOOST_PP_ENUM(n, HBOOST_RANGE_combined_seq, end))); \
    }


#define HBOOST_RANGE_combine(z, n, data) \
  HBOOST_PP_REPEAT(HBOOST_RANGE_combined_exp(2,n), HBOOST_RANGE_combine_impl, n)
