
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

template< typename R, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (HBOOST_FT_nullary_param HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (HBOOST_FT_nullary_param HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,0> function_arity;
typedef mpl::vector1< R HBOOST_FT_nullary_param > types;
};
template< typename R , typename T0, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,1> function_arity;
typedef mpl::vector2< R , T0 > types;
};
template< typename R , typename T0 , typename T1, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,2> function_arity;
typedef mpl::vector3< R , T0 , T1 > types;
};
template< typename R , typename T0 , typename T1 , typename T2, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,3> function_arity;
typedef mpl::vector4< R , T0 , T1 , T2 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,4> function_arity;
typedef mpl::vector5< R , T0 , T1 , T2 , T3 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,5> function_arity;
typedef mpl::vector6< R , T0 , T1 , T2 , T3 , T4 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,6> function_arity;
typedef mpl::vector7< R , T0 , T1 , T2 , T3 , T4 , T5 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,7> function_arity;
typedef mpl::vector8< R , T0 , T1 , T2 , T3 , T4 , T5 , T6 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,8> function_arity;
typedef mpl::vector9< R , T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,9> function_arity;
typedef mpl::vector10< R , T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9, typename L>
struct components_impl<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 HBOOST_FT_ell) HBOOST_FT_cv, L>
{
typedef encode_bits<HBOOST_FT_flags,HBOOST_FT_cc_id> bits;
typedef constant<HBOOST_FT_full_mask> mask;
typedef function_types::components<HBOOST_FT_syntax(HBOOST_FT_cc, HBOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 HBOOST_FT_ell) HBOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,10> function_arity;
typedef mpl::vector11< R , T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 > types;
};
