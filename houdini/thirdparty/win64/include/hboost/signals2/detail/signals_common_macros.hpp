/*
  Author: Frank Mori Hess <fmhess@users.sourceforge.net>
  Begin: 2007-01-23
*/
// Copyright Frank Mori Hess 2007-2008
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_SIGNALS2_SIGNALS_COMMON_MACROS_HPP
#define HBOOST_SIGNALS2_SIGNALS_COMMON_MACROS_HPP

#include <hboost/config.hpp>

#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES

#ifndef HBOOST_SIGNALS2_MAX_ARGS
#define HBOOST_SIGNALS2_MAX_ARGS 9
#endif

// signaln
#define HBOOST_SIGNALS2_SIGNAL_CLASS_NAME(arity) HBOOST_PP_CAT(signal, arity)
// weak_signaln
#define HBOOST_SIGNALS2_WEAK_SIGNAL_CLASS_NAME(arity) HBOOST_PP_CAT(weak_, HBOOST_SIGNALS2_SIGNAL_CLASS_NAME(arity))
// signaln_impl
#define HBOOST_SIGNALS2_SIGNAL_IMPL_CLASS_NAME(arity) HBOOST_PP_CAT(HBOOST_SIGNALS2_SIGNAL_CLASS_NAME(arity), _impl)
// argn
#define HBOOST_SIGNALS2_SIGNATURE_ARG_NAME(z, n, data) HBOOST_PP_CAT(arg, HBOOST_PP_INC(n))
// Tn argn
#define HBOOST_SIGNALS2_SIGNATURE_FULL_ARG(z, n, data) \
  HBOOST_PP_CAT(T, HBOOST_PP_INC(n)) HBOOST_SIGNALS2_SIGNATURE_ARG_NAME(~, n, ~)
// T1 arg1, T2 arg2, ..., Tn argn
#define HBOOST_SIGNALS2_SIGNATURE_FULL_ARGS(arity) \
  HBOOST_PP_ENUM(arity, HBOOST_SIGNALS2_SIGNATURE_FULL_ARG, ~)
// arg1, arg2, ..., argn
#define HBOOST_SIGNALS2_SIGNATURE_ARG_NAMES(arity) HBOOST_PP_ENUM(arity, HBOOST_SIGNALS2_SIGNATURE_ARG_NAME, ~)
// T1, T2, ..., TN
#define HBOOST_SIGNALS2_ARGS_TEMPLATE_INSTANTIATION(arity) \
  HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_INC(arity), T)
// R (T1, T2, ..., TN)
#define HBOOST_SIGNALS2_SIGNATURE_FUNCTION_TYPE(arity) \
  R ( HBOOST_SIGNALS2_ARGS_TEMPLATE_INSTANTIATION(arity) )
// typename prefixR, typename prefixT1, typename prefixT2, ..., typename prefixTN
#define HBOOST_SIGNALS2_PREFIXED_SIGNATURE_TEMPLATE_DECL(arity, prefix) \
  typename HBOOST_PP_CAT(prefix, R) HBOOST_PP_COMMA_IF(arity) \
  HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_INC(arity), typename HBOOST_PP_CAT(prefix, T))
// typename R, typename T1, typename T2, ..., typename TN
#define HBOOST_SIGNALS2_SIGNATURE_TEMPLATE_DECL(arity) \
  typename R HBOOST_PP_COMMA_IF(arity) \
  HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_INC(arity), typename T)
// typename prefixT1, typename prefixT2, ..., typename prefixTN
#define HBOOST_SIGNALS2_PREFIXED_ARGS_TEMPLATE_DECL(arity, prefix) \
  HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_INC(arity), typename HBOOST_PP_CAT(prefix, T))
// typename T1, typename T2, ..., typename TN
#define HBOOST_SIGNALS2_ARGS_TEMPLATE_DECL(arity) \
  HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_INC(arity), typename T)
// prefixR, prefixT1, prefixT2, ..., prefixTN
#define HBOOST_SIGNALS2_PREFIXED_SIGNATURE_TEMPLATE_INSTANTIATION(arity, prefix) \
  HBOOST_PP_CAT(prefix, R) HBOOST_PP_COMMA_IF(arity) HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_INC(arity), HBOOST_PP_CAT(prefix, T))
// R, T1, T2, ..., TN
#define HBOOST_SIGNALS2_SIGNATURE_TEMPLATE_INSTANTIATION(arity) \
  R HBOOST_PP_COMMA_IF(arity) HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_INC(arity), T)
// hboost::functionN<R, T1, T2, ..., TN>
#define HBOOST_SIGNALS2_FUNCTION_N_DECL(arity) HBOOST_PP_CAT(hboost::function, arity)<\
  HBOOST_SIGNALS2_SIGNATURE_TEMPLATE_INSTANTIATION(arity) >
// R, const hboost::signals2::connection&, T1, T2, ..., TN
#define HBOOST_SIGNALS2_EXT_SLOT_TEMPLATE_INSTANTIATION(arity) \
  R, const hboost::signals2::connection&  HBOOST_PP_COMMA_IF(arity) \
  HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_PP_INC(arity), T)
// hboost::functionN<R, const hboost::signals2::connection &, T1, T2, ..., TN>
#define HBOOST_SIGNALS2_EXT_FUNCTION_N_DECL(arity) HBOOST_PP_CAT(hboost::function, HBOOST_PP_INC(arity))<\
  HBOOST_SIGNALS2_EXT_SLOT_TEMPLATE_INSTANTIATION(arity) >
// slotN
#define HBOOST_SIGNALS2_SLOT_CLASS_NAME(arity) HBOOST_PP_CAT(slot, arity)
// slotN+1<R, const connection &, T1, T2, ..., TN, extended_slot_function_type>
#define HBOOST_SIGNALS2_EXTENDED_SLOT_TYPE(arity) \
  HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_PP_INC(arity))< \
  HBOOST_SIGNALS2_EXT_SLOT_TEMPLATE_INSTANTIATION(arity), \
  extended_slot_function_type>
// bound_extended_slot_functionN
#define HBOOST_SIGNALS2_BOUND_EXTENDED_SLOT_FUNCTION_N(arity) HBOOST_PP_CAT(bound_extended_slot_function, arity)
// bound_extended_slot_function_helperN
#define HBOOST_SIGNALS2_BOUND_EXTENDED_SLOT_FUNCTION_INVOKER_N(arity) HBOOST_PP_CAT(bound_extended_slot_function_invoker, arity)
// typename function_traits<Signature>::argn_type
#define HBOOST_SIGNALS2_SIGNATURE_TO_ARGN_TYPE(z, n, Signature) \
  HBOOST_PP_CAT(HBOOST_PP_CAT(typename function_traits<Signature>::arg, HBOOST_PP_INC(n)), _type)
// typename function_traits<Signature>::result_type,
// typename function_traits<Signature>::arg1_type,
// typename function_traits<Signature>::arg2_type,
// ...,
// typename function_traits<Signature>::argn_type
#define HBOOST_SIGNALS2_PORTABLE_SIGNATURE(arity, Signature) \
  typename function_traits<Signature>::result_type \
  HBOOST_PP_COMMA_IF(arity) HBOOST_PP_ENUM(arity, HBOOST_SIGNALS2_SIGNATURE_TO_ARGN_TYPE, Signature)
// prefixTn & argn
#define HBOOST_SIGNALS2_PREFIXED_FULL_REF_ARG(z, n, prefix) \
  HBOOST_PP_CAT(HBOOST_PP_CAT(prefix, T), HBOOST_PP_INC(n)) & HBOOST_SIGNALS2_SIGNATURE_ARG_NAME(~, n, ~)
// prefixT1 & arg1, prefixT2 & arg2, ..., prefixTn & argn
#define HBOOST_SIGNALS2_PREFIXED_FULL_REF_ARGS(arity, prefix) \
  HBOOST_PP_ENUM(arity, HBOOST_SIGNALS2_PREFIXED_FULL_REF_ARG, prefix)
// Tn & argn
#define HBOOST_SIGNALS2_FULL_CREF_ARG(z, n, data) \
  const HBOOST_PP_CAT(T, HBOOST_PP_INC(n)) & HBOOST_SIGNALS2_SIGNATURE_ARG_NAME(~, n, ~)
// const T1 & arg1, const T2 & arg2, ..., const Tn & argn
#define HBOOST_SIGNALS2_FULL_FORWARD_ARGS(arity) \
  HBOOST_PP_ENUM(arity, HBOOST_SIGNALS2_FULL_CREF_ARG, ~)
#define HBOOST_SIGNALS2_FORWARDED_ARGS(arity) \
  HBOOST_SIGNALS2_SIGNATURE_ARG_NAMES(arity)
// preprocessed_arg_typeN
#define HBOOST_SIGNALS2_PREPROCESSED_ARG_N_TYPE_CLASS_NAME(arity) HBOOST_PP_CAT(preprocessed_arg_type, arity)

// typename R, typename T1, typename T2, ..., typename TN, typename SlotFunction
#define HBOOST_SIGNALS2_SLOT_TEMPLATE_SPECIALIZATION_DECL(arity) \
  HBOOST_SIGNALS2_SIGNATURE_TEMPLATE_DECL(arity), \
  typename SlotFunction
#define HBOOST_SIGNALS2_SLOT_TEMPLATE_SPECIALIZATION

// typename R, typename T1, typename T2, ..., typename TN, typename Combiner, ...
#define HBOOST_SIGNALS2_SIGNAL_TEMPLATE_DECL(arity) \
  HBOOST_SIGNALS2_SIGNATURE_TEMPLATE_DECL(arity), \
  typename Combiner, \
  typename Group, \
  typename GroupCompare, \
  typename SlotFunction, \
  typename ExtendedSlotFunction, \
  typename Mutex
// typename R, typename T1, typename T2, ..., typename TN, typename Combiner = optional_last_value<R>, ...
#define HBOOST_SIGNALS2_SIGNAL_TEMPLATE_DEFAULTED_DECL(arity) \
  HBOOST_SIGNALS2_SIGNATURE_TEMPLATE_DECL(arity), \
  typename Combiner = optional_last_value<R>, \
  typename Group = int, \
  typename GroupCompare = std::less<Group>, \
  typename SlotFunction = HBOOST_SIGNALS2_FUNCTION_N_DECL(arity), \
  typename ExtendedSlotFunction = HBOOST_SIGNALS2_EXT_FUNCTION_N_DECL(arity), \
  typename Mutex = signals2::mutex
#define HBOOST_SIGNALS2_SIGNAL_TEMPLATE_SPECIALIZATION_DECL(arity) HBOOST_SIGNALS2_SIGNAL_TEMPLATE_DECL(arity)
#define HBOOST_SIGNALS2_SIGNAL_TEMPLATE_SPECIALIZATION

#define HBOOST_SIGNALS2_STD_FUNCTIONAL_BASE(result_type) std_functional_base

#define HBOOST_SIGNALS2_PP_COMMA_IF(arity) HBOOST_PP_COMMA_IF(arity)

#else // HBOOST_NO_CXX11_VARIADIC_TEMPLATES

#define HBOOST_SIGNALS2_SIGNAL_CLASS_NAME(arity) signal
#define HBOOST_SIGNALS2_WEAK_SIGNAL_CLASS_NAME(arity) weak_signal
#define HBOOST_SIGNALS2_SIGNAL_IMPL_CLASS_NAME(arity) signal_impl
#define HBOOST_SIGNALS2_SIGNATURE_TEMPLATE_DECL(arity) typename Signature
#define HBOOST_SIGNALS2_ARGS_TEMPLATE_INSTANTIATION(arity) Args...
#define HBOOST_SIGNALS2_SIGNATURE_TEMPLATE_INSTANTIATION(arity) R (Args...)
#define HBOOST_SIGNALS2_SIGNATURE_FUNCTION_TYPE(arity) R (Args...)
#define HBOOST_SIGNALS2_ARGS_TEMPLATE_DECL(arity) typename ... Args
#define HBOOST_SIGNALS2_FULL_FORWARD_ARGS(arity) Args && ... args
#define HBOOST_SIGNALS2_FORWARDED_ARGS(arity) std::forward<Args>(args)...
#define HBOOST_SIGNALS2_SLOT_CLASS_NAME(arity) slot
#define HBOOST_SIGNALS2_EXTENDED_SLOT_TYPE(arity) slot<R (const connection &, Args...), extended_slot_function_type>
#define HBOOST_SIGNALS2_BOUND_EXTENDED_SLOT_FUNCTION_N(arity) bound_extended_slot_function
#define HBOOST_SIGNALS2_BOUND_EXTENDED_SLOT_FUNCTION_INVOKER_N(arity) bound_extended_slot_function_invoker
#define HBOOST_SIGNALS2_FUNCTION_N_DECL(arity) hboost::function<Signature>
#define HBOOST_SIGNALS2_PREFIXED_SIGNATURE_TEMPLATE_DECL(arity, prefix) typename prefixSignature
#define HBOOST_SIGNALS2_PREFIXED_SIGNATURE_TEMPLATE_INSTANTIATION(arity, prefix) prefixSignature
#define HBOOST_SIGNALS2_SIGNATURE_FULL_ARGS(arity) Args ... args
#define HBOOST_SIGNALS2_SIGNATURE_ARG_NAMES(arity) args...
#define HBOOST_SIGNALS2_PORTABLE_SIGNATURE(arity, Signature) Signature

#define HBOOST_SIGNALS2_SLOT_TEMPLATE_SPECIALIZATION_DECL(arity) \
  typename SlotFunction, \
  typename R, \
  typename ... Args
#define HBOOST_SIGNALS2_SLOT_TEMPLATE_SPECIALIZATION \
  <R (Args...), SlotFunction>

#define HBOOST_SIGNALS2_SIGNAL_TEMPLATE_DECL(arity) \
  typename Signature, \
  typename Combiner, \
  typename Group, \
  typename GroupCompare, \
  typename SlotFunction, \
  typename ExtendedSlotFunction, \
  typename Mutex
#define HBOOST_SIGNALS2_SIGNAL_TEMPLATE_DEFAULTED_DECL(arity) \
  typename Signature, \
  typename Combiner = optional_last_value<typename hboost::function_traits<Signature>::result_type>, \
  typename Group = int, \
  typename GroupCompare = std::less<Group>, \
  typename SlotFunction = hboost::function<Signature>, \
  typename ExtendedSlotFunction = typename detail::variadic_extended_signature<Signature>::function_type, \
  typename Mutex = signals2::mutex
#define HBOOST_SIGNALS2_SIGNAL_TEMPLATE_SPECIALIZATION_DECL(arity) \
  typename Combiner, \
  typename Group, \
  typename GroupCompare, \
  typename SlotFunction, \
  typename ExtendedSlotFunction, \
  typename Mutex, \
  typename R, \
  typename ... Args
#define HBOOST_SIGNALS2_SIGNAL_TEMPLATE_SPECIALIZATION <\
  R (Args...), \
  Combiner, \
  Group, \
  GroupCompare, \
  SlotFunction, \
  ExtendedSlotFunction, \
  Mutex>

#define HBOOST_SIGNALS2_STD_FUNCTIONAL_BASE(result_type) \
  std_functional_base<result_type , Args...>

#define HBOOST_SIGNALS2_PP_COMMA_IF(arity) ,

#endif // HBOOST_NO_CXX11_VARIADIC_TEMPLATES

#endif // HBOOST_SIGNALS2_SIGNALS_COMMON_MACROS_HPP
