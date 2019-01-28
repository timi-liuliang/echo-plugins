/* Copyright 2003-2015 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_COMPOSITE_KEY_HPP
#define HBOOST_MULTI_INDEX_COMPOSITE_KEY_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <hboost/functional/hash_fwd.hpp>
#include <hboost/multi_index/detail/access_specifier.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/or.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/control/expr_if.hpp>
#include <hboost/preprocessor/list/at.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp> 
#include <hboost/static_assert.hpp>
#include <hboost/tuple/tuple.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/utility/enable_if.hpp>
#include <functional>

#if !defined(HBOOST_NO_FUNCTION_TEMPLATE_ORDERING)
#include <hboost/ref.hpp>
#endif

#if !defined(HBOOST_NO_SFINAE)
#include <hboost/type_traits/is_convertible.hpp>
#endif

#if !defined(HBOOST_NO_CXX11_HDR_TUPLE)&&\
    !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#include <hboost/multi_index/detail/cons_stdtuple.hpp>
#endif

/* A composite key stores n key extractors and "computes" the
 * result on a given value as a packed reference to the value and
 * the composite key itself. Actual invocations to the component
 * key extractors are lazily performed when executing an operation
 * on composite_key results (equality, comparison, hashing.)
 * As the other key extractors in Boost.MultiIndex, composite_key<T,...>
 * is  overloaded to work on chained pointers to T and reference_wrappers
 * of T.
 */

/* This user_definable macro limits the number of elements of a composite
 * key; useful for shortening resulting symbol names (MSVC++ 6.0, for
 * instance has problems coping with very long symbol names.)
 * NB: This cannot exceed the maximum number of arguments of
 * hboost::tuple. In Boost 1.32, the limit is 10.
 */

#if !defined(HBOOST_MULTI_INDEX_LIMIT_COMPOSITE_KEY_SIZE)
#define HBOOST_MULTI_INDEX_LIMIT_COMPOSITE_KEY_SIZE 10
#endif

/* maximum number of key extractors in a composite key */

#if HBOOST_MULTI_INDEX_LIMIT_COMPOSITE_KEY_SIZE<10 /* max length of a tuple */
#define HBOOST_MULTI_INDEX_COMPOSITE_KEY_SIZE \
  HBOOST_MULTI_INDEX_LIMIT_COMPOSITE_KEY_SIZE
#else
#define HBOOST_MULTI_INDEX_COMPOSITE_KEY_SIZE 10
#endif

/* HBOOST_PP_ENUM of HBOOST_MULTI_INDEX_COMPOSITE_KEY_SIZE elements */

#define HBOOST_MULTI_INDEX_CK_ENUM(macro,data)                                \
  HBOOST_PP_ENUM(HBOOST_MULTI_INDEX_COMPOSITE_KEY_SIZE,macro,data)

/* HBOOST_PP_ENUM_PARAMS of HBOOST_MULTI_INDEX_COMPOSITE_KEY_SIZE elements */

#define HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(param)                              \
  HBOOST_PP_ENUM_PARAMS(HBOOST_MULTI_INDEX_COMPOSITE_KEY_SIZE,param)

/* if n==0 ->   text0
 * otherwise -> textn=tuples::null_type
 */

#define HBOOST_MULTI_INDEX_CK_TEMPLATE_PARM(z,n,text)                         \
  typename HBOOST_PP_CAT(text,n) HBOOST_PP_EXPR_IF(n,=tuples::null_type)

/* const textn& kn=textn() */

#define HBOOST_MULTI_INDEX_CK_CTOR_ARG(z,n,text)                              \
  const HBOOST_PP_CAT(text,n)& HBOOST_PP_CAT(k,n) = HBOOST_PP_CAT(text,n)()

/* typename list(0)<list(1),n>::type */

#define HBOOST_MULTI_INDEX_CK_APPLY_METAFUNCTION_N(z,n,list)                  \
  HBOOST_DEDUCED_TYPENAME HBOOST_PP_LIST_AT(list,0)<                           \
    HBOOST_PP_LIST_AT(list,1),n                                               \
  >::type

namespace hboost{

template<class T> class reference_wrapper; /* fwd decl. */

namespace multi_index{

namespace detail{

/* n-th key extractor of a composite key */

template<typename CompositeKey,int N>
struct nth_key_from_value
{
  typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
  typedef typename mpl::eval_if_c<
    N<tuples::length<key_extractor_tuple>::value,
    tuples::element<N,key_extractor_tuple>,
    mpl::identity<tuples::null_type>
  >::type                                            type;
};

/* nth_composite_key_##name<CompositeKey,N>::type yields
 * functor<nth_key_from_value<CompositeKey,N> >, or tuples::null_type
 * if N exceeds the length of the composite key.
 */

#define HBOOST_MULTI_INDEX_CK_NTH_COMPOSITE_KEY_FUNCTOR(name,functor)         \
template<typename KeyFromValue>                                              \
struct HBOOST_PP_CAT(key_,name)                                               \
{                                                                            \
  typedef functor<typename KeyFromValue::result_type> type;                  \
};                                                                           \
                                                                             \
template<>                                                                   \
struct HBOOST_PP_CAT(key_,name)<tuples::null_type>                            \
{                                                                            \
  typedef tuples::null_type type;                                            \
};                                                                           \
                                                                             \
template<typename CompositeKey,int  N>                                       \
struct HBOOST_PP_CAT(nth_composite_key_,name)                                 \
{                                                                            \
  typedef typename nth_key_from_value<CompositeKey,N>::type key_from_value;  \
  typedef typename HBOOST_PP_CAT(key_,name)<key_from_value>::type type;       \
};

/* nth_composite_key_equal_to
 * nth_composite_key_less
 * nth_composite_key_greater
 * nth_composite_key_hash
 */

HBOOST_MULTI_INDEX_CK_NTH_COMPOSITE_KEY_FUNCTOR(equal_to,std::equal_to)
HBOOST_MULTI_INDEX_CK_NTH_COMPOSITE_KEY_FUNCTOR(less,std::less)
HBOOST_MULTI_INDEX_CK_NTH_COMPOSITE_KEY_FUNCTOR(greater,std::greater)
HBOOST_MULTI_INDEX_CK_NTH_COMPOSITE_KEY_FUNCTOR(hash,hboost::hash)

/* used for defining equality and comparison ops of composite_key_result */

#define HBOOST_MULTI_INDEX_CK_IDENTITY_ENUM_MACRO(z,n,text) text

struct generic_operator_equal
{
  template<typename T,typename Q>
  bool operator()(const T& x,const Q& y)const{return x==y;}
};

typedef tuple<
  HBOOST_MULTI_INDEX_CK_ENUM(
    HBOOST_MULTI_INDEX_CK_IDENTITY_ENUM_MACRO,
    detail::generic_operator_equal)>          generic_operator_equal_tuple;

struct generic_operator_less
{
  template<typename T,typename Q>
  bool operator()(const T& x,const Q& y)const{return x<y;}
};

typedef tuple<
  HBOOST_MULTI_INDEX_CK_ENUM(
    HBOOST_MULTI_INDEX_CK_IDENTITY_ENUM_MACRO,
    detail::generic_operator_less)>           generic_operator_less_tuple;

/* Metaprogramming machinery for implementing equality, comparison and
 * hashing operations of composite_key_result.
 *
 * equal_* checks for equality between composite_key_results and
 * between those and tuples, accepting a tuple of basic equality functors.
 * compare_* does lexicographical comparison.
 * hash_* computes a combination of elementwise hash values.
 */

template
<
  typename KeyCons1,typename Value1,
  typename KeyCons2, typename Value2,
  typename EqualCons
>
struct equal_ckey_ckey; /* fwd decl. */

template
<
  typename KeyCons1,typename Value1,
  typename KeyCons2, typename Value2,
  typename EqualCons
>
struct equal_ckey_ckey_terminal
{
  static bool compare(
    const KeyCons1&,const Value1&,
    const KeyCons2&,const Value2&,
    const EqualCons&)
  {
    return true;
  }
};

template
<
  typename KeyCons1,typename Value1,
  typename KeyCons2, typename Value2,
  typename EqualCons
>
struct equal_ckey_ckey_normal
{
  static bool compare(
    const KeyCons1& c0,const Value1& v0,
    const KeyCons2& c1,const Value2& v1,
    const EqualCons& eq)
  {
    if(!eq.get_head()(c0.get_head()(v0),c1.get_head()(v1)))return false;
    return equal_ckey_ckey<
      HBOOST_DEDUCED_TYPENAME KeyCons1::tail_type,Value1,
      HBOOST_DEDUCED_TYPENAME KeyCons2::tail_type,Value2,
      HBOOST_DEDUCED_TYPENAME EqualCons::tail_type
    >::compare(c0.get_tail(),v0,c1.get_tail(),v1,eq.get_tail());
  }
};

template
<
  typename KeyCons1,typename Value1,
  typename KeyCons2, typename Value2,
  typename EqualCons
>
struct equal_ckey_ckey:
  mpl::if_<
    mpl::or_<
      is_same<KeyCons1,tuples::null_type>,
      is_same<KeyCons2,tuples::null_type>
    >,
    equal_ckey_ckey_terminal<KeyCons1,Value1,KeyCons2,Value2,EqualCons>,
    equal_ckey_ckey_normal<KeyCons1,Value1,KeyCons2,Value2,EqualCons>
  >::type
{
};

template
<
  typename KeyCons,typename Value,
  typename ValCons,typename EqualCons
>
struct equal_ckey_cval; /* fwd decl. */

template
<
  typename KeyCons,typename Value,
  typename ValCons,typename EqualCons
>
struct equal_ckey_cval_terminal
{
  static bool compare(
    const KeyCons&,const Value&,const ValCons&,const EqualCons&)
  {
    return true;
  }

  static bool compare(
    const ValCons&,const KeyCons&,const Value&,const EqualCons&)
  {
    return true;
  }
};

template
<
  typename KeyCons,typename Value,
  typename ValCons,typename EqualCons
>
struct equal_ckey_cval_normal
{
  static bool compare(
    const KeyCons& c,const Value& v,const ValCons& vc,
    const EqualCons& eq)
  {
    if(!eq.get_head()(c.get_head()(v),vc.get_head()))return false;
    return equal_ckey_cval<
      HBOOST_DEDUCED_TYPENAME KeyCons::tail_type,Value,
      HBOOST_DEDUCED_TYPENAME ValCons::tail_type,
      HBOOST_DEDUCED_TYPENAME EqualCons::tail_type
    >::compare(c.get_tail(),v,vc.get_tail(),eq.get_tail());
  }

  static bool compare(
    const ValCons& vc,const KeyCons& c,const Value& v,
    const EqualCons& eq)
  {
    if(!eq.get_head()(vc.get_head(),c.get_head()(v)))return false;
    return equal_ckey_cval<
      HBOOST_DEDUCED_TYPENAME KeyCons::tail_type,Value,
      HBOOST_DEDUCED_TYPENAME ValCons::tail_type,
      HBOOST_DEDUCED_TYPENAME EqualCons::tail_type
    >::compare(vc.get_tail(),c.get_tail(),v,eq.get_tail());
  }
};

template
<
  typename KeyCons,typename Value,
  typename ValCons,typename EqualCons
>
struct equal_ckey_cval:
  mpl::if_<
    mpl::or_<
      is_same<KeyCons,tuples::null_type>,
      is_same<ValCons,tuples::null_type>
    >,
    equal_ckey_cval_terminal<KeyCons,Value,ValCons,EqualCons>,
    equal_ckey_cval_normal<KeyCons,Value,ValCons,EqualCons>
  >::type
{
};

template
<
  typename KeyCons1,typename Value1,
  typename KeyCons2, typename Value2,
  typename CompareCons
>
struct compare_ckey_ckey; /* fwd decl. */

template
<
  typename KeyCons1,typename Value1,
  typename KeyCons2, typename Value2,
  typename CompareCons
>
struct compare_ckey_ckey_terminal
{
  static bool compare(
    const KeyCons1&,const Value1&,
    const KeyCons2&,const Value2&,
    const CompareCons&)
  {
    return false;
  }
};

template
<
  typename KeyCons1,typename Value1,
  typename KeyCons2, typename Value2,
  typename CompareCons
>
struct compare_ckey_ckey_normal
{
  static bool compare(
    const KeyCons1& c0,const Value1& v0,
    const KeyCons2& c1,const Value2& v1,
    const CompareCons& comp)
  {
    if(comp.get_head()(c0.get_head()(v0),c1.get_head()(v1)))return true;
    if(comp.get_head()(c1.get_head()(v1),c0.get_head()(v0)))return false;
    return compare_ckey_ckey<
      HBOOST_DEDUCED_TYPENAME KeyCons1::tail_type,Value1,
      HBOOST_DEDUCED_TYPENAME KeyCons2::tail_type,Value2,
      HBOOST_DEDUCED_TYPENAME CompareCons::tail_type
    >::compare(c0.get_tail(),v0,c1.get_tail(),v1,comp.get_tail());
  }
};

template
<
  typename KeyCons1,typename Value1,
  typename KeyCons2, typename Value2,
  typename CompareCons
>
struct compare_ckey_ckey:
  mpl::if_<
    mpl::or_<
      is_same<KeyCons1,tuples::null_type>,
      is_same<KeyCons2,tuples::null_type>
    >,
    compare_ckey_ckey_terminal<KeyCons1,Value1,KeyCons2,Value2,CompareCons>,
    compare_ckey_ckey_normal<KeyCons1,Value1,KeyCons2,Value2,CompareCons>
  >::type
{
};

template
<
  typename KeyCons,typename Value,
  typename ValCons,typename CompareCons
>
struct compare_ckey_cval; /* fwd decl. */

template
<
  typename KeyCons,typename Value,
  typename ValCons,typename CompareCons
>
struct compare_ckey_cval_terminal
{
  static bool compare(
    const KeyCons&,const Value&,const ValCons&,const CompareCons&)
  {
    return false;
  }

  static bool compare(
    const ValCons&,const KeyCons&,const Value&,const CompareCons&)
  {
    return false;
  }
};

template
<
  typename KeyCons,typename Value,
  typename ValCons,typename CompareCons
>
struct compare_ckey_cval_normal
{
  static bool compare(
    const KeyCons& c,const Value& v,const ValCons& vc,
    const CompareCons& comp)
  {
    if(comp.get_head()(c.get_head()(v),vc.get_head()))return true;
    if(comp.get_head()(vc.get_head(),c.get_head()(v)))return false;
    return compare_ckey_cval<
      HBOOST_DEDUCED_TYPENAME KeyCons::tail_type,Value,
      HBOOST_DEDUCED_TYPENAME ValCons::tail_type,
      HBOOST_DEDUCED_TYPENAME CompareCons::tail_type
    >::compare(c.get_tail(),v,vc.get_tail(),comp.get_tail());
  }

  static bool compare(
    const ValCons& vc,const KeyCons& c,const Value& v,
    const CompareCons& comp)
  {
    if(comp.get_head()(vc.get_head(),c.get_head()(v)))return true;
    if(comp.get_head()(c.get_head()(v),vc.get_head()))return false;
    return compare_ckey_cval<
      HBOOST_DEDUCED_TYPENAME KeyCons::tail_type,Value,
      HBOOST_DEDUCED_TYPENAME ValCons::tail_type,
      HBOOST_DEDUCED_TYPENAME CompareCons::tail_type
    >::compare(vc.get_tail(),c.get_tail(),v,comp.get_tail());
  }
};

template
<
  typename KeyCons,typename Value,
  typename ValCons,typename CompareCons
>
struct compare_ckey_cval:
  mpl::if_<
    mpl::or_<
      is_same<KeyCons,tuples::null_type>,
      is_same<ValCons,tuples::null_type>
    >,
    compare_ckey_cval_terminal<KeyCons,Value,ValCons,CompareCons>,
    compare_ckey_cval_normal<KeyCons,Value,ValCons,CompareCons>
  >::type
{
};

template<typename KeyCons,typename Value,typename HashCons>
struct hash_ckey; /* fwd decl. */

template<typename KeyCons,typename Value,typename HashCons>
struct hash_ckey_terminal
{
  static std::size_t hash(
    const KeyCons&,const Value&,const HashCons&,std::size_t carry)
  {
    return carry;
  }
};

template<typename KeyCons,typename Value,typename HashCons>
struct hash_ckey_normal
{
  static std::size_t hash(
    const KeyCons& c,const Value& v,const HashCons& h,std::size_t carry=0)
  {
    /* same hashing formula as hboost::hash_combine */

    carry^=h.get_head()(c.get_head()(v))+0x9e3779b9+(carry<<6)+(carry>>2);
    return hash_ckey<
      HBOOST_DEDUCED_TYPENAME KeyCons::tail_type,Value,
      HBOOST_DEDUCED_TYPENAME HashCons::tail_type
    >::hash(c.get_tail(),v,h.get_tail(),carry);
  }
};

template<typename KeyCons,typename Value,typename HashCons>
struct hash_ckey:
  mpl::if_<
    is_same<KeyCons,tuples::null_type>,
    hash_ckey_terminal<KeyCons,Value,HashCons>,
    hash_ckey_normal<KeyCons,Value,HashCons>
  >::type
{
};

template<typename ValCons,typename HashCons>
struct hash_cval; /* fwd decl. */

template<typename ValCons,typename HashCons>
struct hash_cval_terminal
{
  static std::size_t hash(const ValCons&,const HashCons&,std::size_t carry)
  {
    return carry;
  }
};

template<typename ValCons,typename HashCons>
struct hash_cval_normal
{
  static std::size_t hash(
    const ValCons& vc,const HashCons& h,std::size_t carry=0)
  {
    carry^=h.get_head()(vc.get_head())+0x9e3779b9+(carry<<6)+(carry>>2);
    return hash_cval<
      HBOOST_DEDUCED_TYPENAME ValCons::tail_type,
      HBOOST_DEDUCED_TYPENAME HashCons::tail_type
    >::hash(vc.get_tail(),h.get_tail(),carry);
  }
};

template<typename ValCons,typename HashCons>
struct hash_cval:
  mpl::if_<
    is_same<ValCons,tuples::null_type>,
    hash_cval_terminal<ValCons,HashCons>,
    hash_cval_normal<ValCons,HashCons>
  >::type
{
};

} /* namespace multi_index::detail */

/* composite_key_result */

#if defined(HBOOST_MSVC)
#pragma warning(push)
#pragma warning(disable:4512)
#endif

template<typename CompositeKey>
struct composite_key_result
{
  typedef CompositeKey                            composite_key_type;
  typedef typename composite_key_type::value_type value_type;

  composite_key_result(
    const composite_key_type& composite_key_,const value_type& value_):
    composite_key(composite_key_),value(value_)
  {}

  const composite_key_type& composite_key;
  const value_type&         value;
};

#if defined(HBOOST_MSVC)
#pragma warning(pop)
#endif

/* composite_key */

template<
  typename Value,
  HBOOST_MULTI_INDEX_CK_ENUM(HBOOST_MULTI_INDEX_CK_TEMPLATE_PARM,KeyFromValue)
>
struct composite_key:
  private tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(KeyFromValue)>
{
private:
  typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(KeyFromValue)> super;

public:
  typedef super                               key_extractor_tuple;
  typedef Value                               value_type;
  typedef composite_key_result<composite_key> result_type;

  composite_key(
    HBOOST_MULTI_INDEX_CK_ENUM(HBOOST_MULTI_INDEX_CK_CTOR_ARG,KeyFromValue)):
    super(HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(k))
  {}

  composite_key(const key_extractor_tuple& x):super(x){}

  const key_extractor_tuple& key_extractors()const{return *this;}
  key_extractor_tuple&       key_extractors(){return *this;}

  template<typename ChainedPtr>

#if !defined(HBOOST_NO_SFINAE)
  typename disable_if<
    is_convertible<const ChainedPtr&,const value_type&>,result_type>::type
#else
  result_type
#endif

  operator()(const ChainedPtr& x)const
  {
    return operator()(*x);
  }

  result_type operator()(const value_type& x)const
  {
    return result_type(*this,x);
  }

  result_type operator()(const reference_wrapper<const value_type>& x)const
  {
    return result_type(*this,x.get());
  }

  result_type operator()(const reference_wrapper<value_type>& x)const
  {
    return result_type(*this,x.get());
  }
};

/* comparison operators */

/* == */

template<typename CompositeKey1,typename CompositeKey2>
inline bool operator==(
  const composite_key_result<CompositeKey1>& x,
  const composite_key_result<CompositeKey2>& y)
{
  typedef typename CompositeKey1::key_extractor_tuple key_extractor_tuple1;
  typedef typename CompositeKey1::value_type          value_type1;
  typedef typename CompositeKey2::key_extractor_tuple key_extractor_tuple2;
  typedef typename CompositeKey2::value_type          value_type2;

  HBOOST_STATIC_ASSERT(
    tuples::length<key_extractor_tuple1>::value==
    tuples::length<key_extractor_tuple2>::value);

  return detail::equal_ckey_ckey<
    key_extractor_tuple1,value_type1,
    key_extractor_tuple2,value_type2,
    detail::generic_operator_equal_tuple
  >::compare(
    x.composite_key.key_extractors(),x.value,
    y.composite_key.key_extractors(),y.value,
    detail::generic_operator_equal_tuple());
}

template<
  typename CompositeKey,
  HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value)
>
inline bool operator==(
  const composite_key_result<CompositeKey>& x,
  const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& y)
{
  typedef typename CompositeKey::key_extractor_tuple     key_extractor_tuple;
  typedef typename CompositeKey::value_type              value_type;
  typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;
  
  HBOOST_STATIC_ASSERT(
    tuples::length<key_extractor_tuple>::value==
    tuples::length<key_tuple>::value);

  return detail::equal_ckey_cval<
    key_extractor_tuple,value_type,
    key_tuple,detail::generic_operator_equal_tuple
  >::compare(
    x.composite_key.key_extractors(),x.value,
    y,detail::generic_operator_equal_tuple());
}

template
<
  HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value),
  typename CompositeKey
>
inline bool operator==(
  const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& x,
  const composite_key_result<CompositeKey>& y)
{
  typedef typename CompositeKey::key_extractor_tuple     key_extractor_tuple;
  typedef typename CompositeKey::value_type              value_type;
  typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;
  
  HBOOST_STATIC_ASSERT(
    tuples::length<key_extractor_tuple>::value==
    tuples::length<key_tuple>::value);

  return detail::equal_ckey_cval<
    key_extractor_tuple,value_type,
    key_tuple,detail::generic_operator_equal_tuple
  >::compare(
    x,y.composite_key.key_extractors(),
    y.value,detail::generic_operator_equal_tuple());
}

#if !defined(HBOOST_NO_CXX11_HDR_TUPLE)&&\
    !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
template<typename CompositeKey,typename... Values>
inline bool operator==(
  const composite_key_result<CompositeKey>& x,
  const std::tuple<Values...>& y)
{
  typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
  typedef typename CompositeKey::value_type          value_type;
  typedef std::tuple<Values...>                      key_tuple;
  typedef typename detail::cons_stdtuple_ctor<
    key_tuple>::result_type                          cons_key_tuple;
  
  HBOOST_STATIC_ASSERT(
    static_cast<std::size_t>(tuples::length<key_extractor_tuple>::value)==
    std::tuple_size<key_tuple>::value);

  return detail::equal_ckey_cval<
    key_extractor_tuple,value_type,
    cons_key_tuple,detail::generic_operator_equal_tuple
  >::compare(
    x.composite_key.key_extractors(),x.value,
    detail::make_cons_stdtuple(y),detail::generic_operator_equal_tuple());
}

template<typename CompositeKey,typename... Values>
inline bool operator==(
  const std::tuple<Values...>& x,
  const composite_key_result<CompositeKey>& y)
{
  typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
  typedef typename CompositeKey::value_type          value_type;
  typedef std::tuple<Values...>                      key_tuple;
  typedef typename detail::cons_stdtuple_ctor<
    key_tuple>::result_type                          cons_key_tuple;

  HBOOST_STATIC_ASSERT(
    static_cast<std::size_t>(tuples::length<key_extractor_tuple>::value)==
    std::tuple_size<key_tuple>::value);

  return detail::equal_ckey_cval<
    key_extractor_tuple,value_type,
    cons_key_tuple,detail::generic_operator_equal_tuple
  >::compare(
    detail::make_cons_stdtuple(x),y.composite_key.key_extractors(),
    y.value,detail::generic_operator_equal_tuple());
}
#endif

/* < */

template<typename CompositeKey1,typename CompositeKey2>
inline bool operator<(
  const composite_key_result<CompositeKey1>& x,
  const composite_key_result<CompositeKey2>& y)
{
  typedef typename CompositeKey1::key_extractor_tuple key_extractor_tuple1;
  typedef typename CompositeKey1::value_type          value_type1;
  typedef typename CompositeKey2::key_extractor_tuple key_extractor_tuple2;
  typedef typename CompositeKey2::value_type          value_type2;

  return detail::compare_ckey_ckey<
   key_extractor_tuple1,value_type1,
   key_extractor_tuple2,value_type2,
   detail::generic_operator_less_tuple
  >::compare(
    x.composite_key.key_extractors(),x.value,
    y.composite_key.key_extractors(),y.value,
    detail::generic_operator_less_tuple());
}

template
<
  typename CompositeKey,
  HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value)
>
inline bool operator<(
  const composite_key_result<CompositeKey>& x,
  const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& y)
{
  typedef typename CompositeKey::key_extractor_tuple     key_extractor_tuple;
  typedef typename CompositeKey::value_type              value_type;
  typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;
  
  return detail::compare_ckey_cval<
    key_extractor_tuple,value_type,
    key_tuple,detail::generic_operator_less_tuple
  >::compare(
    x.composite_key.key_extractors(),x.value,
    y,detail::generic_operator_less_tuple());
}

template
<
  HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value),
  typename CompositeKey
>
inline bool operator<(
  const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& x,
  const composite_key_result<CompositeKey>& y)
{
  typedef typename CompositeKey::key_extractor_tuple     key_extractor_tuple;
  typedef typename CompositeKey::value_type              value_type;
  typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;
  
  return detail::compare_ckey_cval<
    key_extractor_tuple,value_type,
    key_tuple,detail::generic_operator_less_tuple
  >::compare(
    x,y.composite_key.key_extractors(),
    y.value,detail::generic_operator_less_tuple());
}

#if !defined(HBOOST_NO_CXX11_HDR_TUPLE)&&\
    !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
template<typename CompositeKey,typename... Values>
inline bool operator<(
  const composite_key_result<CompositeKey>& x,
  const std::tuple<Values...>& y)
{
  typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
  typedef typename CompositeKey::value_type          value_type;
  typedef std::tuple<Values...>                      key_tuple;
  typedef typename detail::cons_stdtuple_ctor<
    key_tuple>::result_type                          cons_key_tuple;
  
  return detail::compare_ckey_cval<
    key_extractor_tuple,value_type,
    cons_key_tuple,detail::generic_operator_less_tuple
  >::compare(
    x.composite_key.key_extractors(),x.value,
    detail::make_cons_stdtuple(y),detail::generic_operator_less_tuple());
}

template<typename CompositeKey,typename... Values>
inline bool operator<(
  const std::tuple<Values...>& x,
  const composite_key_result<CompositeKey>& y)
{
  typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
  typedef typename CompositeKey::value_type          value_type;
  typedef std::tuple<Values...>                      key_tuple;
  typedef typename detail::cons_stdtuple_ctor<
    key_tuple>::result_type                          cons_key_tuple;
  
  return detail::compare_ckey_cval<
    key_extractor_tuple,value_type,
    cons_key_tuple,detail::generic_operator_less_tuple
  >::compare(
    detail::make_cons_stdtuple(x),y.composite_key.key_extractors(),
    y.value,detail::generic_operator_less_tuple());
}
#endif

/* rest of comparison operators */

#define HBOOST_MULTI_INDEX_CK_COMPLETE_COMP_OPS(t1,t2,a1,a2)                  \
template<t1,t2> inline bool operator!=(const a1& x,const a2& y)              \
{                                                                            \
  return !(x==y);                                                            \
}                                                                            \
                                                                             \
template<t1,t2> inline bool operator>(const a1& x,const a2& y)               \
{                                                                            \
  return y<x;                                                                \
}                                                                            \
                                                                             \
template<t1,t2> inline bool operator>=(const a1& x,const a2& y)              \
{                                                                            \
  return !(x<y);                                                             \
}                                                                            \
                                                                             \
template<t1,t2> inline bool operator<=(const a1& x,const a2& y)              \
{                                                                            \
  return !(y<x);                                                             \
}

HBOOST_MULTI_INDEX_CK_COMPLETE_COMP_OPS(
  typename CompositeKey1,
  typename CompositeKey2,
  composite_key_result<CompositeKey1>,
  composite_key_result<CompositeKey2>
)

HBOOST_MULTI_INDEX_CK_COMPLETE_COMP_OPS(
  typename CompositeKey,
  HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value),
  composite_key_result<CompositeKey>,
  tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>
)

HBOOST_MULTI_INDEX_CK_COMPLETE_COMP_OPS(
  HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value),
  typename CompositeKey,
  tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>,
  composite_key_result<CompositeKey>
)

#if !defined(HBOOST_NO_CXX11_HDR_TUPLE)&&\
    !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
HBOOST_MULTI_INDEX_CK_COMPLETE_COMP_OPS(
  typename CompositeKey,
  typename... Values,
  composite_key_result<CompositeKey>,
  std::tuple<Values...>
)

HBOOST_MULTI_INDEX_CK_COMPLETE_COMP_OPS(
  typename CompositeKey,
  typename... Values,
  std::tuple<Values...>,
  composite_key_result<CompositeKey>
)
#endif

/* composite_key_equal_to */

template
<
  HBOOST_MULTI_INDEX_CK_ENUM(HBOOST_MULTI_INDEX_CK_TEMPLATE_PARM,Pred)
>
struct composite_key_equal_to:
  private tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Pred)>
{
private:
  typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Pred)> super;

public:
  typedef super key_eq_tuple;

  composite_key_equal_to(
    HBOOST_MULTI_INDEX_CK_ENUM(HBOOST_MULTI_INDEX_CK_CTOR_ARG,Pred)):
    super(HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(k))
  {}

  composite_key_equal_to(const key_eq_tuple& x):super(x){}

  const key_eq_tuple& key_eqs()const{return *this;}
  key_eq_tuple&       key_eqs(){return *this;}

  template<typename CompositeKey1,typename CompositeKey2>
  bool operator()(
    const composite_key_result<CompositeKey1> & x,
    const composite_key_result<CompositeKey2> & y)const
  {
    typedef typename CompositeKey1::key_extractor_tuple key_extractor_tuple1;
    typedef typename CompositeKey1::value_type          value_type1;
    typedef typename CompositeKey2::key_extractor_tuple key_extractor_tuple2;
    typedef typename CompositeKey2::value_type          value_type2;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_extractor_tuple1>::value<=
      tuples::length<key_eq_tuple>::value&&
      tuples::length<key_extractor_tuple1>::value==
      tuples::length<key_extractor_tuple2>::value);

    return detail::equal_ckey_ckey<
      key_extractor_tuple1,value_type1,
      key_extractor_tuple2,value_type2,
      key_eq_tuple
    >::compare(
      x.composite_key.key_extractors(),x.value,
      y.composite_key.key_extractors(),y.value,
      key_eqs());
  }
  
  template
  <
    typename CompositeKey,
    HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value)
  >
  bool operator()(
    const composite_key_result<CompositeKey>& x,
    const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& y)const
  {
    typedef typename CompositeKey::key_extractor_tuple     key_extractor_tuple;
    typedef typename CompositeKey::value_type              value_type;
    typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_extractor_tuple>::value<=
      tuples::length<key_eq_tuple>::value&&
      tuples::length<key_extractor_tuple>::value==
      tuples::length<key_tuple>::value);

    return detail::equal_ckey_cval<
      key_extractor_tuple,value_type,
      key_tuple,key_eq_tuple
    >::compare(x.composite_key.key_extractors(),x.value,y,key_eqs());
  }

  template
  <
    HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value),
    typename CompositeKey
  >
  bool operator()(
    const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& x,
    const composite_key_result<CompositeKey>& y)const
  {
    typedef typename CompositeKey::key_extractor_tuple     key_extractor_tuple;
    typedef typename CompositeKey::value_type              value_type;
    typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_tuple>::value<=
      tuples::length<key_eq_tuple>::value&&
      tuples::length<key_tuple>::value==
      tuples::length<key_extractor_tuple>::value);

    return detail::equal_ckey_cval<
      key_extractor_tuple,value_type,
      key_tuple,key_eq_tuple
    >::compare(x,y.composite_key.key_extractors(),y.value,key_eqs());
  }

#if !defined(HBOOST_NO_CXX11_HDR_TUPLE)&&\
    !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template<typename CompositeKey,typename... Values>
  bool operator()(
    const composite_key_result<CompositeKey>& x,
    const std::tuple<Values...>& y)const
  {
    typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
    typedef typename CompositeKey::value_type          value_type;
    typedef std::tuple<Values...>                      key_tuple;
    typedef typename detail::cons_stdtuple_ctor<
      key_tuple>::result_type                          cons_key_tuple;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_extractor_tuple>::value<=
      tuples::length<key_eq_tuple>::value&&
      static_cast<std::size_t>(tuples::length<key_extractor_tuple>::value)==
      std::tuple_size<key_tuple>::value);

    return detail::equal_ckey_cval<
      key_extractor_tuple,value_type,
      cons_key_tuple,key_eq_tuple
    >::compare(
      x.composite_key.key_extractors(),x.value,
      detail::make_cons_stdtuple(y),key_eqs());
  }

  template<typename CompositeKey,typename... Values>
  bool operator()(
    const std::tuple<Values...>& x,
    const composite_key_result<CompositeKey>& y)const
  {
    typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
    typedef typename CompositeKey::value_type          value_type;
    typedef std::tuple<Values...>                      key_tuple;
    typedef typename detail::cons_stdtuple_ctor<
      key_tuple>::result_type                          cons_key_tuple;

    HBOOST_STATIC_ASSERT(
      std::tuple_size<key_tuple>::value<=
      static_cast<std::size_t>(tuples::length<key_eq_tuple>::value)&&
      std::tuple_size<key_tuple>::value==
      static_cast<std::size_t>(tuples::length<key_extractor_tuple>::value));

    return detail::equal_ckey_cval<
      key_extractor_tuple,value_type,
      cons_key_tuple,key_eq_tuple
    >::compare(
      detail::make_cons_stdtuple(x),y.composite_key.key_extractors(),
      y.value,key_eqs());
  }
#endif
};

/* composite_key_compare */

template
<
  HBOOST_MULTI_INDEX_CK_ENUM(HBOOST_MULTI_INDEX_CK_TEMPLATE_PARM,Compare)
>
struct composite_key_compare:
  private tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Compare)>
{
private:
  typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Compare)> super;

public:
  typedef super key_comp_tuple;

  composite_key_compare(
    HBOOST_MULTI_INDEX_CK_ENUM(HBOOST_MULTI_INDEX_CK_CTOR_ARG,Compare)):
    super(HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(k))
  {}

  composite_key_compare(const key_comp_tuple& x):super(x){}

  const key_comp_tuple& key_comps()const{return *this;}
  key_comp_tuple&       key_comps(){return *this;}

  template<typename CompositeKey1,typename CompositeKey2>
  bool operator()(
    const composite_key_result<CompositeKey1> & x,
    const composite_key_result<CompositeKey2> & y)const
  {
    typedef typename CompositeKey1::key_extractor_tuple key_extractor_tuple1;
    typedef typename CompositeKey1::value_type          value_type1;
    typedef typename CompositeKey2::key_extractor_tuple key_extractor_tuple2;
    typedef typename CompositeKey2::value_type          value_type2;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_extractor_tuple1>::value<=
      tuples::length<key_comp_tuple>::value||
      tuples::length<key_extractor_tuple2>::value<=
      tuples::length<key_comp_tuple>::value);

    return detail::compare_ckey_ckey<
      key_extractor_tuple1,value_type1,
      key_extractor_tuple2,value_type2,
      key_comp_tuple
    >::compare(
      x.composite_key.key_extractors(),x.value,
      y.composite_key.key_extractors(),y.value,
      key_comps());
  }
  
#if !defined(HBOOST_NO_FUNCTION_TEMPLATE_ORDERING)
  template<typename CompositeKey,typename Value>
  bool operator()(
    const composite_key_result<CompositeKey>& x,
    const Value& y)const
  {
    return operator()(x,hboost::make_tuple(hboost::cref(y)));
  }
#endif

  template
  <
    typename CompositeKey,
    HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value)
  >
  bool operator()(
    const composite_key_result<CompositeKey>& x,
    const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& y)const
  {
    typedef typename CompositeKey::key_extractor_tuple     key_extractor_tuple;
    typedef typename CompositeKey::value_type              value_type;
    typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_extractor_tuple>::value<=
      tuples::length<key_comp_tuple>::value||
      tuples::length<key_tuple>::value<=
      tuples::length<key_comp_tuple>::value);

    return detail::compare_ckey_cval<
      key_extractor_tuple,value_type,
      key_tuple,key_comp_tuple
    >::compare(x.composite_key.key_extractors(),x.value,y,key_comps());
  }

#if !defined(HBOOST_NO_FUNCTION_TEMPLATE_ORDERING)
  template<typename Value,typename CompositeKey>
  bool operator()(
    const Value& x,
    const composite_key_result<CompositeKey>& y)const
  {
    return operator()(hboost::make_tuple(hboost::cref(x)),y);
  }
#endif

  template
  <
    HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value),
    typename CompositeKey
  >
  bool operator()(
    const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& x,
    const composite_key_result<CompositeKey>& y)const
  {
    typedef typename CompositeKey::key_extractor_tuple     key_extractor_tuple;
    typedef typename CompositeKey::value_type              value_type;
    typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_tuple>::value<=
      tuples::length<key_comp_tuple>::value||
      tuples::length<key_extractor_tuple>::value<=
      tuples::length<key_comp_tuple>::value);

    return detail::compare_ckey_cval<
      key_extractor_tuple,value_type,
      key_tuple,key_comp_tuple
    >::compare(x,y.composite_key.key_extractors(),y.value,key_comps());
  }

#if !defined(HBOOST_NO_CXX11_HDR_TUPLE)&&\
    !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template<typename CompositeKey,typename... Values>
  bool operator()(
    const composite_key_result<CompositeKey>& x,
    const std::tuple<Values...>& y)const
  {
    typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
    typedef typename CompositeKey::value_type          value_type;
    typedef std::tuple<Values...>                      key_tuple;
    typedef typename detail::cons_stdtuple_ctor<
      key_tuple>::result_type                          cons_key_tuple;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_extractor_tuple>::value<=
      tuples::length<key_comp_tuple>::value||
      std::tuple_size<key_tuple>::value<=
      static_cast<std::size_t>(tuples::length<key_comp_tuple>::value));

    return detail::compare_ckey_cval<
      key_extractor_tuple,value_type,
      cons_key_tuple,key_comp_tuple
    >::compare(
      x.composite_key.key_extractors(),x.value,
      detail::make_cons_stdtuple(y),key_comps());
  }

  template<typename CompositeKey,typename... Values>
  bool operator()(
    const std::tuple<Values...>& x,
    const composite_key_result<CompositeKey>& y)const
  {
    typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
    typedef typename CompositeKey::value_type          value_type;
    typedef std::tuple<Values...>                      key_tuple;
    typedef typename detail::cons_stdtuple_ctor<
      key_tuple>::result_type                          cons_key_tuple;

    HBOOST_STATIC_ASSERT(
      std::tuple_size<key_tuple>::value<=
      static_cast<std::size_t>(tuples::length<key_comp_tuple>::value)||
      tuples::length<key_extractor_tuple>::value<=
      tuples::length<key_comp_tuple>::value);

    return detail::compare_ckey_cval<
      key_extractor_tuple,value_type,
      cons_key_tuple,key_comp_tuple
    >::compare(
      detail::make_cons_stdtuple(x),y.composite_key.key_extractors(),
      y.value,key_comps());
  }
#endif
};

/* composite_key_hash */

template
<
  HBOOST_MULTI_INDEX_CK_ENUM(HBOOST_MULTI_INDEX_CK_TEMPLATE_PARM,Hash)
>
struct composite_key_hash:
  private tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Hash)>
{
private:
  typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Hash)> super;

public:
  typedef super key_hasher_tuple;

  composite_key_hash(
    HBOOST_MULTI_INDEX_CK_ENUM(HBOOST_MULTI_INDEX_CK_CTOR_ARG,Hash)):
    super(HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(k))
  {}

  composite_key_hash(const key_hasher_tuple& x):super(x){}

  const key_hasher_tuple& key_hash_functions()const{return *this;}
  key_hasher_tuple&       key_hash_functions(){return *this;}

  template<typename CompositeKey>
  std::size_t operator()(const composite_key_result<CompositeKey> & x)const
  {
    typedef typename CompositeKey::key_extractor_tuple key_extractor_tuple;
    typedef typename CompositeKey::value_type          value_type;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_extractor_tuple>::value==
      tuples::length<key_hasher_tuple>::value);

    return detail::hash_ckey<
      key_extractor_tuple,value_type,
      key_hasher_tuple
    >::hash(x.composite_key.key_extractors(),x.value,key_hash_functions());
  }
  
  template<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(typename Value)>
  std::size_t operator()(
    const tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)>& x)const
  {
    typedef tuple<HBOOST_MULTI_INDEX_CK_ENUM_PARAMS(Value)> key_tuple;

    HBOOST_STATIC_ASSERT(
      tuples::length<key_tuple>::value==
      tuples::length<key_hasher_tuple>::value);

    return detail::hash_cval<
      key_tuple,key_hasher_tuple
    >::hash(x,key_hash_functions());
  }

#if !defined(HBOOST_NO_CXX11_HDR_TUPLE)&&\
    !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template<typename... Values>
  std::size_t operator()(const std::tuple<Values...>& x)const
  {
    typedef std::tuple<Values...>                key_tuple;
    typedef typename detail::cons_stdtuple_ctor<
      key_tuple>::result_type                    cons_key_tuple;

    HBOOST_STATIC_ASSERT(
      std::tuple_size<key_tuple>::value==
      static_cast<std::size_t>(tuples::length<key_hasher_tuple>::value));

    return detail::hash_cval<
      cons_key_tuple,key_hasher_tuple
    >::hash(detail::make_cons_stdtuple(x),key_hash_functions());
  }
#endif
};

/* Instantiations of the former functors with "natural" basic components:
 * composite_key_result_equal_to uses std::equal_to of the values.
 * composite_key_result_less     uses std::less.
 * composite_key_result_greater  uses std::greater.
 * composite_key_result_hash     uses hboost::hash.
 */

#define HBOOST_MULTI_INDEX_CK_RESULT_EQUAL_TO_SUPER                           \
composite_key_equal_to<                                                      \
    HBOOST_MULTI_INDEX_CK_ENUM(                                               \
      HBOOST_MULTI_INDEX_CK_APPLY_METAFUNCTION_N,                             \
      /* the argument is a PP list */                                        \
      (detail::nth_composite_key_equal_to,                                   \
        (HBOOST_DEDUCED_TYPENAME CompositeKeyResult::composite_key_type,      \
          HBOOST_PP_NIL)))                                                    \
  >

template<typename CompositeKeyResult>
struct composite_key_result_equal_to:
HBOOST_MULTI_INDEX_PRIVATE_IF_USING_DECL_FOR_TEMPL_FUNCTIONS
HBOOST_MULTI_INDEX_CK_RESULT_EQUAL_TO_SUPER
{
private:
  typedef HBOOST_MULTI_INDEX_CK_RESULT_EQUAL_TO_SUPER super;

public:
  typedef CompositeKeyResult  first_argument_type;
  typedef first_argument_type second_argument_type;
  typedef bool                result_type;

  using super::operator();
};

#define HBOOST_MULTI_INDEX_CK_RESULT_LESS_SUPER                               \
composite_key_compare<                                                       \
    HBOOST_MULTI_INDEX_CK_ENUM(                                               \
      HBOOST_MULTI_INDEX_CK_APPLY_METAFUNCTION_N,                             \
      /* the argument is a PP list */                                        \
      (detail::nth_composite_key_less,                                       \
        (HBOOST_DEDUCED_TYPENAME CompositeKeyResult::composite_key_type,      \
          HBOOST_PP_NIL)))                                                    \
  >

template<typename CompositeKeyResult>
struct composite_key_result_less:
HBOOST_MULTI_INDEX_PRIVATE_IF_USING_DECL_FOR_TEMPL_FUNCTIONS
HBOOST_MULTI_INDEX_CK_RESULT_LESS_SUPER
{
private:
  typedef HBOOST_MULTI_INDEX_CK_RESULT_LESS_SUPER super;

public:
  typedef CompositeKeyResult  first_argument_type;
  typedef first_argument_type second_argument_type;
  typedef bool                result_type;

  using super::operator();
};

#define HBOOST_MULTI_INDEX_CK_RESULT_GREATER_SUPER                            \
composite_key_compare<                                                       \
    HBOOST_MULTI_INDEX_CK_ENUM(                                               \
      HBOOST_MULTI_INDEX_CK_APPLY_METAFUNCTION_N,                             \
      /* the argument is a PP list */                                        \
      (detail::nth_composite_key_greater,                                    \
        (HBOOST_DEDUCED_TYPENAME CompositeKeyResult::composite_key_type,      \
          HBOOST_PP_NIL)))                                                    \
  >

template<typename CompositeKeyResult>
struct composite_key_result_greater:
HBOOST_MULTI_INDEX_PRIVATE_IF_USING_DECL_FOR_TEMPL_FUNCTIONS
HBOOST_MULTI_INDEX_CK_RESULT_GREATER_SUPER
{
private:
  typedef HBOOST_MULTI_INDEX_CK_RESULT_GREATER_SUPER super;

public:
  typedef CompositeKeyResult  first_argument_type;
  typedef first_argument_type second_argument_type;
  typedef bool                result_type;

  using super::operator();
};

#define HBOOST_MULTI_INDEX_CK_RESULT_HASH_SUPER                               \
composite_key_hash<                                                          \
    HBOOST_MULTI_INDEX_CK_ENUM(                                               \
      HBOOST_MULTI_INDEX_CK_APPLY_METAFUNCTION_N,                             \
      /* the argument is a PP list */                                        \
      (detail::nth_composite_key_hash,                                       \
        (HBOOST_DEDUCED_TYPENAME CompositeKeyResult::composite_key_type,      \
          HBOOST_PP_NIL)))                                                    \
  >

template<typename CompositeKeyResult>
struct composite_key_result_hash:
HBOOST_MULTI_INDEX_PRIVATE_IF_USING_DECL_FOR_TEMPL_FUNCTIONS
HBOOST_MULTI_INDEX_CK_RESULT_HASH_SUPER
{
private:
  typedef HBOOST_MULTI_INDEX_CK_RESULT_HASH_SUPER super;

public:
  typedef CompositeKeyResult argument_type;
  typedef std::size_t        result_type;

  using super::operator();
};

} /* namespace multi_index */

} /* namespace hboost */

/* Specializations of std::equal_to, std::less, std::greater and hboost::hash
 * for composite_key_results enabling interoperation with tuples of values.
 */

namespace std{

template<typename CompositeKey>
struct equal_to<hboost::multi_index::composite_key_result<CompositeKey> >:
  hboost::multi_index::composite_key_result_equal_to<
    hboost::multi_index::composite_key_result<CompositeKey>
  >
{
};

template<typename CompositeKey>
struct less<hboost::multi_index::composite_key_result<CompositeKey> >:
  hboost::multi_index::composite_key_result_less<
    hboost::multi_index::composite_key_result<CompositeKey>
  >
{
};

template<typename CompositeKey>
struct greater<hboost::multi_index::composite_key_result<CompositeKey> >:
  hboost::multi_index::composite_key_result_greater<
    hboost::multi_index::composite_key_result<CompositeKey>
  >
{
};

} /* namespace std */

namespace hboost{

template<typename CompositeKey>
struct hash<hboost::multi_index::composite_key_result<CompositeKey> >:
  hboost::multi_index::composite_key_result_hash<
    hboost::multi_index::composite_key_result<CompositeKey>
  >
{
};

} /* namespace hboost */

#undef HBOOST_MULTI_INDEX_CK_RESULT_HASH_SUPER
#undef HBOOST_MULTI_INDEX_CK_RESULT_GREATER_SUPER
#undef HBOOST_MULTI_INDEX_CK_RESULT_LESS_SUPER
#undef HBOOST_MULTI_INDEX_CK_RESULT_EQUAL_TO_SUPER
#undef HBOOST_MULTI_INDEX_CK_COMPLETE_COMP_OPS
#undef HBOOST_MULTI_INDEX_CK_IDENTITY_ENUM_MACRO
#undef HBOOST_MULTI_INDEX_CK_NTH_COMPOSITE_KEY_FUNCTOR
#undef HBOOST_MULTI_INDEX_CK_APPLY_METAFUNCTION_N
#undef HBOOST_MULTI_INDEX_CK_CTOR_ARG
#undef HBOOST_MULTI_INDEX_CK_TEMPLATE_PARM
#undef HBOOST_MULTI_INDEX_CK_ENUM_PARAMS
#undef HBOOST_MULTI_INDEX_CK_ENUM
#undef HBOOST_MULTI_INDEX_COMPOSITE_KEY_SIZE

#endif