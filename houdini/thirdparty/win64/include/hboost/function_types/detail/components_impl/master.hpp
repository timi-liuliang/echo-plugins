
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

#if   HBOOST_FT_ARITY_LOOP_PREFIX

#   ifndef HBOOST_FT_DETAIL_COMPONENTS_IMPL_MASTER_HPP_INCLUDED
#   define HBOOST_FT_DETAIL_COMPONENTS_IMPL_MASTER_HPP_INCLUDED
#     include <hboost/preprocessor/cat.hpp>
#     include <hboost/preprocessor/facilities/empty.hpp>
#     include <hboost/preprocessor/facilities/identity.hpp>
#     include <hboost/preprocessor/arithmetic/dec.hpp>
#     include <hboost/preprocessor/punctuation/comma_if.hpp>
#   endif

#   define HBOOST_FT_type_name

#   if !HBOOST_FT_mfp

#     define HBOOST_FT_types \
          R HBOOST_PP_COMMA_IF(HBOOST_FT_arity) HBOOST_FT_params(HBOOST_PP_EMPTY)
#   else

#     define HBOOST_FT_types \
          R, typename class_transform<T0 HBOOST_FT_cv, L>::type \
          HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(HBOOST_FT_arity)) \
          HBOOST_FT_params(HBOOST_PP_EMPTY)

#   endif

#elif HBOOST_FT_ARITY_LOOP_IS_ITERATING

template< HBOOST_FT_tplargs(HBOOST_PP_IDENTITY(typename)), typename L>
struct components_impl<HBOOST_FT_type, L>
{
  typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
  typedef constant<HBOOST_FT_full_mask> mask;

  typedef function_types::components<HBOOST_FT_type, L> type;
  typedef components_mpl_sequence_tag tag;

  typedef mpl::integral_c<std::size_t,HBOOST_FT_arity> function_arity;

  typedef HBOOST_PP_CAT(mpl::vector,HBOOST_FT_n)< HBOOST_FT_types > types;
};

#elif HBOOST_FT_ARITY_LOOP_SUFFIX

#   undef HBOOST_FT_types
#   undef HBOOST_FT_type_name

#else
#   error "attempt to use arity loop master file without loop"
#endif

