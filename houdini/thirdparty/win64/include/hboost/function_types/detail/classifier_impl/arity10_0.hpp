
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

// input:  HBOOST_FT_syntax     type macro to use
// input:  HBOOST_FT_cc         empty or cc specifier 
// input:  HBOOST_FT_ell        empty or "..."
// input:  HBOOST_FT_cv         empty or cv qualifiers
// input:  HBOOST_FT_flags      single decimal integer encoding the flags
// output: HBOOST_FT_n          number of component types (arity+1)
// output: HBOOST_FT_arity      current arity
// output: HBOOST_FT_type       macro that expands to the type
// output: HBOOST_FT_tplargs(p) template arguments with given prefix
// output: HBOOST_FT_params(p)  parameters with given prefix

template< typename R >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,0> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (HBOOST_FT_nullary_param HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,1> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,2> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,3> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,4> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,5> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,6> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,7> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,8> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,9> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 HBOOST_FT_ell) HBOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 >
typename encode_charr<HBOOST_FT_flags,HBOOST_FT_cc_id,10> ::type
classifier_impl(HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 HBOOST_FT_ell) HBOOST_FT_cv);

