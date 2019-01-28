
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

#if HBOOST_FT_ARITY_LOOP_PREFIX

#   ifndef HBOOST_FT_DETAIL_CLASSIFIER_IMPL_MASTER_HPP_INCLUDED
#   define HBOOST_FT_DETAIL_CLASSIFIER_IMPL_MASTER_HPP_INCLUDED
#     include <hboost/preprocessor/facilities/identity.hpp>
#   endif

#   define HBOOST_FT_type_name

#elif HBOOST_FT_ARITY_LOOP_IS_ITERATING

template< HBOOST_FT_tplargs(HBOOST_PP_IDENTITY(typename)) >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,HBOOST_FT_arity>::type
classifier_impl(HBOOST_FT_type);

#elif HBOOST_FT_ARITY_LOOP_SUFFIX

#   undef HBOOST_FT_type_name

#else
#   error "attempt to use arity loop master file without loop"
#endif

