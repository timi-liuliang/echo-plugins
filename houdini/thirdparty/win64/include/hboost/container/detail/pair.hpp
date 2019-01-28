//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_CONTAINER_DETAIL_PAIR_HPP
#define HBOOST_CONTAINER_CONTAINER_DETAIL_PAIR_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>

#include <hboost/container/detail/mpl.hpp>
#include <hboost/container/detail/type_traits.hpp>
#include <hboost/container/detail/mpl.hpp>
#include <hboost/container/detail/std_fwd.hpp>
#include <hboost/move/adl_move_swap.hpp> //swap

#include <hboost/intrusive/detail/minimal_pair_header.hpp>      //pair
#include <hboost/move/utility_core.hpp>

namespace hboost {
namespace container {
namespace container_detail {

template <class T1, class T2>
struct pair;

template <class T>
struct is_pair
{
   static const bool value = false;
};

template <class T1, class T2>
struct is_pair< pair<T1, T2> >
{
   static const bool value = true;
};

template <class T1, class T2>
struct is_pair< std::pair<T1, T2> >
{
   static const bool value = true;
};

template <class T>
struct is_not_pair
{
   static const bool value = !is_pair<T>::value;
};

template <class T>
struct is_std_pair
{
   static const bool value = false;
};

template <class T1, class T2>
struct is_std_pair< std::pair<T1, T2> >
{
   static const bool value = true;
};

struct pair_nat;

template <class T1, class T2>
struct pair
{
   private:
   HBOOST_COPYABLE_AND_MOVABLE(pair)

   public:
   typedef T1 first_type;
   typedef T2 second_type;

   T1 first;
   T2 second;

   //Default constructor
   pair()
      : first(), second()
   {}

   //pair copy assignment
   pair(const pair& x)
      : first(x.first), second(x.second)
   {}

   //pair move constructor
   pair(HBOOST_RV_REF(pair) p)
      : first(::hboost::move(p.first)), second(::hboost::move(p.second))
   {}

   template <class D, class S>
   pair(const pair<D, S> &p)
      : first(p.first), second(p.second)
   {}

   template <class D, class S>
   pair(HBOOST_RV_REF_BEG pair<D, S> HBOOST_RV_REF_END p)
      : first(::hboost::move(p.first)), second(::hboost::move(p.second))
   {}

   //pair from two values
   pair(const T1 &t1, const T2 &t2)
      : first(t1)
      , second(t2)
   {}

   template<class U, class V>
   pair(HBOOST_FWD_REF(U) u, HBOOST_FWD_REF(V) v)
      : first(::hboost::forward<U>(u))
      , second(::hboost::forward<V>(v))
   {}

   //And now compatibility with std::pair
   pair(const std::pair<T1, T2>& x)
      : first(x.first), second(x.second)
   {}

   template <class D, class S>
   pair(const std::pair<D, S>& p)
      : first(p.first), second(p.second)
   {}

   pair(HBOOST_RV_REF_BEG std::pair<T1, T2> HBOOST_RV_REF_END p)
      : first(::hboost::move(p.first)), second(::hboost::move(p.second))
   {}

   template <class D, class S>
   pair(HBOOST_RV_REF_BEG std::pair<D, S> HBOOST_RV_REF_END p)
      : first(::hboost::move(p.first)), second(::hboost::move(p.second))
   {}

   //piecewise_construct missing
   //template <class U, class V> pair(pair<U, V>&& p);
   //template <class... Args1, class... Args2>
   //   pair(piecewise_construct_t, tuple<Args1...> first_args,
   //        tuple<Args2...> second_args);

   //pair copy assignment
   pair& operator=(HBOOST_COPY_ASSIGN_REF(pair) p)
   {
      first  = p.first;
      second = p.second;
      return *this;
   }

   //pair move assignment
   pair& operator=(HBOOST_RV_REF(pair) p)
   {
      first  = ::hboost::move(p.first);
      second = ::hboost::move(p.second);
      return *this;
   }

   template <class D, class S>
   typename ::hboost::container::container_detail::disable_if_or
      < pair &
      , ::hboost::container::container_detail::is_same<T1, D>
      , ::hboost::container::container_detail::is_same<T2, S>
      >::type
      operator=(const pair<D, S>&p)
   {
      first  = p.first;
      second = p.second;
      return *this;
   }

   template <class D, class S>
   typename ::hboost::container::container_detail::disable_if_or
      < pair &
      , ::hboost::container::container_detail::is_same<T1, D>
      , ::hboost::container::container_detail::is_same<T2, S>
      >::type
      operator=(HBOOST_RV_REF_BEG pair<D, S> HBOOST_RV_REF_END p)
   {
      first  = ::hboost::move(p.first);
      second = ::hboost::move(p.second);
      return *this;
   }
//std::pair copy assignment
   pair& operator=(const std::pair<T1, T2> &p)
   {
      first  = p.first;
      second = p.second;
      return *this;
   }

   template <class D, class S>
   pair& operator=(const std::pair<D, S> &p)
   {
      first  = ::hboost::move(p.first);
      second = ::hboost::move(p.second);
      return *this;
   }

   //std::pair move assignment
   pair& operator=(HBOOST_RV_REF_BEG std::pair<T1, T2> HBOOST_RV_REF_END p)
   {
      first  = ::hboost::move(p.first);
      second = ::hboost::move(p.second);
      return *this;
   }

   template <class D, class S>
   pair& operator=(HBOOST_RV_REF_BEG std::pair<D, S> HBOOST_RV_REF_END p)
   {
      first  = ::hboost::move(p.first);
      second = ::hboost::move(p.second);
      return *this;
   }

   //swap
   void swap(pair& p)
   {
      ::hboost::adl_move_swap(this->first, p.first);
      ::hboost::adl_move_swap(this->second, p.second);
   }
};

template <class T1, class T2>
inline bool operator==(const pair<T1,T2>& x, const pair<T1,T2>& y)
{  return static_cast<bool>(x.first == y.first && x.second == y.second);  }

template <class T1, class T2>
inline bool operator< (const pair<T1,T2>& x, const pair<T1,T2>& y)
{  return static_cast<bool>(x.first < y.first ||
                         (!(y.first < x.first) && x.second < y.second)); }

template <class T1, class T2>
inline bool operator!=(const pair<T1,T2>& x, const pair<T1,T2>& y)
{  return static_cast<bool>(!(x == y));  }

template <class T1, class T2>
inline bool operator> (const pair<T1,T2>& x, const pair<T1,T2>& y)
{  return y < x;  }

template <class T1, class T2>
inline bool operator>=(const pair<T1,T2>& x, const pair<T1,T2>& y)
{  return static_cast<bool>(!(x < y)); }

template <class T1, class T2>
inline bool operator<=(const pair<T1,T2>& x, const pair<T1,T2>& y)
{  return static_cast<bool>(!(y < x)); }

template <class T1, class T2>
inline pair<T1, T2> make_pair(T1 x, T2 y)
{  return pair<T1, T2>(x, y); }

template <class T1, class T2>
inline void swap(pair<T1, T2>& x, pair<T1, T2>& y)
{  x.swap(y);  }

}  //namespace container_detail {
}  //namespace container {


//Without this specialization recursive flat_(multi)map instantiation fails
//because is_enum needs to instantiate the recursive pair, leading to a compilation error).
//This breaks the cycle clearly stating that pair is not an enum avoiding any instantiation.
template<class T>
struct is_enum;

template<class T, class U>
struct is_enum< ::hboost::container::container_detail::pair<T, U> >
{
   static const bool value = false;
};

template <class T>
struct is_class;

//This specialization is needed to avoid instantiation of pair in
//is_class, and allow recursive maps.
template <class T1, class T2>
struct is_class< ::hboost::container::container_detail::pair<T1, T2> >
{
   static const bool value = true;
};

#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES

template<class T1, class T2>
struct has_move_emulation_enabled< ::hboost::container::container_detail::pair<T1, T2> >
{
   static const bool value = true;
};

#endif

namespace move_detail{

template<class T>
struct is_class_or_union;

template <class T1, class T2>
struct is_class_or_union< ::hboost::container::container_detail::pair<T1, T2> >
//This specialization is needed to avoid instantiation of pair in
//is_class, and allow recursive maps.
{
   static const bool value = true;
};

template <class T1, class T2>
struct is_class_or_union< std::pair<T1, T2> >
//This specialization is needed to avoid instantiation of pair in
//is_class, and allow recursive maps.
{
   static const bool value = true;
};




}  //namespace move_detail{

}  //namespace hboost {

#include <hboost/container/detail/config_end.hpp>

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_PAIR_HPP
