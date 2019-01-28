//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013.
// (C) Copyright Gennaro Prota 2003 - 2004.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_DETAIL_ITERATORS_HPP
#define HBOOST_CONTAINER_DETAIL_ITERATORS_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>
#include <hboost/container/allocator_traits.hpp>
#include <hboost/container/detail/type_traits.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/move/utility_core.hpp>
#include <hboost/intrusive/detail/reverse_iterator.hpp>

#if defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#include <hboost/move/detail/fwd_macros.hpp>
#else
#include <hboost/container/detail/variadic_templates_tools.hpp>
#endif
#include <hboost/container/detail/iterator.hpp>

namespace hboost {
namespace container {

template <class T, class Difference = std::ptrdiff_t>
class constant_iterator
  : public ::hboost::container::iterator
      <std::random_access_iterator_tag, T, Difference, const T*, const T &>
{
   typedef  constant_iterator<T, Difference> this_type;

   public:
   explicit constant_iterator(const T &ref, Difference range_size)
      :  m_ptr(&ref), m_num(range_size){}

   //Constructors
   constant_iterator()
      :  m_ptr(0), m_num(0){}

   constant_iterator& operator++()
   { increment();   return *this;   }

   constant_iterator operator++(int)
   {
      constant_iterator result (*this);
      increment();
      return result;
   }

   constant_iterator& operator--()
   { decrement();   return *this;   }

   constant_iterator operator--(int)
   {
      constant_iterator result (*this);
      decrement();
      return result;
   }

   friend bool operator== (const constant_iterator& i, const constant_iterator& i2)
   { return i.equal(i2); }

   friend bool operator!= (const constant_iterator& i, const constant_iterator& i2)
   { return !(i == i2); }

   friend bool operator< (const constant_iterator& i, const constant_iterator& i2)
   { return i.less(i2); }

   friend bool operator> (const constant_iterator& i, const constant_iterator& i2)
   { return i2 < i; }

   friend bool operator<= (const constant_iterator& i, const constant_iterator& i2)
   { return !(i > i2); }

   friend bool operator>= (const constant_iterator& i, const constant_iterator& i2)
   { return !(i < i2); }

   friend Difference operator- (const constant_iterator& i, const constant_iterator& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   constant_iterator& operator+=(Difference off)
   {  this->advance(off); return *this;   }

   constant_iterator operator+(Difference off) const
   {
      constant_iterator other(*this);
      other.advance(off);
      return other;
   }

   friend constant_iterator operator+(Difference off, const constant_iterator& right)
   {  return right + off; }

   constant_iterator& operator-=(Difference off)
   {  this->advance(-off); return *this;   }

   constant_iterator operator-(Difference off) const
   {  return *this + (-off);  }

   const T& operator*() const
   { return dereference(); }

   const T& operator[] (Difference ) const
   { return dereference(); }

   const T* operator->() const
   { return &(dereference()); }

   private:
   const T *   m_ptr;
   Difference  m_num;

   void increment()
   { --m_num; }

   void decrement()
   { ++m_num; }

   bool equal(const this_type &other) const
   {  return m_num == other.m_num;   }

   bool less(const this_type &other) const
   {  return other.m_num < m_num;   }

   const T & dereference() const
   { return *m_ptr; }

   void advance(Difference n)
   {  m_num -= n; }

   Difference distance_to(const this_type &other)const
   {  return m_num - other.m_num;   }
};

template <class T, class Difference>
class value_init_construct_iterator
  : public ::hboost::container::iterator
      <std::random_access_iterator_tag, T, Difference, const T*, const T &>
{
   typedef  value_init_construct_iterator<T, Difference> this_type;

   public:
   explicit value_init_construct_iterator(Difference range_size)
      :  m_num(range_size){}

   //Constructors
   value_init_construct_iterator()
      :  m_num(0){}

   value_init_construct_iterator& operator++()
   { increment();   return *this;   }

   value_init_construct_iterator operator++(int)
   {
      value_init_construct_iterator result (*this);
      increment();
      return result;
   }

   value_init_construct_iterator& operator--()
   { decrement();   return *this;   }

   value_init_construct_iterator operator--(int)
   {
      value_init_construct_iterator result (*this);
      decrement();
      return result;
   }

   friend bool operator== (const value_init_construct_iterator& i, const value_init_construct_iterator& i2)
   { return i.equal(i2); }

   friend bool operator!= (const value_init_construct_iterator& i, const value_init_construct_iterator& i2)
   { return !(i == i2); }

   friend bool operator< (const value_init_construct_iterator& i, const value_init_construct_iterator& i2)
   { return i.less(i2); }

   friend bool operator> (const value_init_construct_iterator& i, const value_init_construct_iterator& i2)
   { return i2 < i; }

   friend bool operator<= (const value_init_construct_iterator& i, const value_init_construct_iterator& i2)
   { return !(i > i2); }

   friend bool operator>= (const value_init_construct_iterator& i, const value_init_construct_iterator& i2)
   { return !(i < i2); }

   friend Difference operator- (const value_init_construct_iterator& i, const value_init_construct_iterator& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   value_init_construct_iterator& operator+=(Difference off)
   {  this->advance(off); return *this;   }

   value_init_construct_iterator operator+(Difference off) const
   {
      value_init_construct_iterator other(*this);
      other.advance(off);
      return other;
   }

   friend value_init_construct_iterator operator+(Difference off, const value_init_construct_iterator& right)
   {  return right + off; }

   value_init_construct_iterator& operator-=(Difference off)
   {  this->advance(-off); return *this;   }

   value_init_construct_iterator operator-(Difference off) const
   {  return *this + (-off);  }

   //This pseudo-iterator's dereference operations have no sense since value is not
   //constructed until ::hboost::container::construct_in_place is called.
   //So comment them to catch bad uses
   //const T& operator*() const;
   //const T& operator[](difference_type) const;
   //const T* operator->() const;

   private:
   Difference  m_num;

   void increment()
   { --m_num; }

   void decrement()
   { ++m_num; }

   bool equal(const this_type &other) const
   {  return m_num == other.m_num;   }

   bool less(const this_type &other) const
   {  return other.m_num < m_num;   }

   const T & dereference() const
   {
      static T dummy;
      return dummy;
   }

   void advance(Difference n)
   {  m_num -= n; }

   Difference distance_to(const this_type &other)const
   {  return m_num - other.m_num;   }
};

template <class T, class Difference>
class default_init_construct_iterator
  : public ::hboost::container::iterator
      <std::random_access_iterator_tag, T, Difference, const T*, const T &>
{
   typedef  default_init_construct_iterator<T, Difference> this_type;

   public:
   explicit default_init_construct_iterator(Difference range_size)
      :  m_num(range_size){}

   //Constructors
   default_init_construct_iterator()
      :  m_num(0){}

   default_init_construct_iterator& operator++()
   { increment();   return *this;   }

   default_init_construct_iterator operator++(int)
   {
      default_init_construct_iterator result (*this);
      increment();
      return result;
   }

   default_init_construct_iterator& operator--()
   { decrement();   return *this;   }

   default_init_construct_iterator operator--(int)
   {
      default_init_construct_iterator result (*this);
      decrement();
      return result;
   }

   friend bool operator== (const default_init_construct_iterator& i, const default_init_construct_iterator& i2)
   { return i.equal(i2); }

   friend bool operator!= (const default_init_construct_iterator& i, const default_init_construct_iterator& i2)
   { return !(i == i2); }

   friend bool operator< (const default_init_construct_iterator& i, const default_init_construct_iterator& i2)
   { return i.less(i2); }

   friend bool operator> (const default_init_construct_iterator& i, const default_init_construct_iterator& i2)
   { return i2 < i; }

   friend bool operator<= (const default_init_construct_iterator& i, const default_init_construct_iterator& i2)
   { return !(i > i2); }

   friend bool operator>= (const default_init_construct_iterator& i, const default_init_construct_iterator& i2)
   { return !(i < i2); }

   friend Difference operator- (const default_init_construct_iterator& i, const default_init_construct_iterator& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   default_init_construct_iterator& operator+=(Difference off)
   {  this->advance(off); return *this;   }

   default_init_construct_iterator operator+(Difference off) const
   {
      default_init_construct_iterator other(*this);
      other.advance(off);
      return other;
   }

   friend default_init_construct_iterator operator+(Difference off, const default_init_construct_iterator& right)
   {  return right + off; }

   default_init_construct_iterator& operator-=(Difference off)
   {  this->advance(-off); return *this;   }

   default_init_construct_iterator operator-(Difference off) const
   {  return *this + (-off);  }

   //This pseudo-iterator's dereference operations have no sense since value is not
   //constructed until ::hboost::container::construct_in_place is called.
   //So comment them to catch bad uses
   //const T& operator*() const;
   //const T& operator[](difference_type) const;
   //const T* operator->() const;

   private:
   Difference  m_num;

   void increment()
   { --m_num; }

   void decrement()
   { ++m_num; }

   bool equal(const this_type &other) const
   {  return m_num == other.m_num;   }

   bool less(const this_type &other) const
   {  return other.m_num < m_num;   }

   const T & dereference() const
   {
      static T dummy;
      return dummy;
   }

   void advance(Difference n)
   {  m_num -= n; }

   Difference distance_to(const this_type &other)const
   {  return m_num - other.m_num;   }
};


template <class T, class Difference = std::ptrdiff_t>
class repeat_iterator
  : public ::hboost::container::iterator
      <std::random_access_iterator_tag, T, Difference, T*, T&>
{
   typedef repeat_iterator<T, Difference> this_type;
   public:
   explicit repeat_iterator(T &ref, Difference range_size)
      :  m_ptr(&ref), m_num(range_size){}

   //Constructors
   repeat_iterator()
      :  m_ptr(0), m_num(0){}

   this_type& operator++()
   { increment();   return *this;   }

   this_type operator++(int)
   {
      this_type result (*this);
      increment();
      return result;
   }

   this_type& operator--()
   { increment();   return *this;   }

   this_type operator--(int)
   {
      this_type result (*this);
      increment();
      return result;
   }

   friend bool operator== (const this_type& i, const this_type& i2)
   { return i.equal(i2); }

   friend bool operator!= (const this_type& i, const this_type& i2)
   { return !(i == i2); }

   friend bool operator< (const this_type& i, const this_type& i2)
   { return i.less(i2); }

   friend bool operator> (const this_type& i, const this_type& i2)
   { return i2 < i; }

   friend bool operator<= (const this_type& i, const this_type& i2)
   { return !(i > i2); }

   friend bool operator>= (const this_type& i, const this_type& i2)
   { return !(i < i2); }

   friend Difference operator- (const this_type& i, const this_type& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   this_type& operator+=(Difference off)
   {  this->advance(off); return *this;   }

   this_type operator+(Difference off) const
   {
      this_type other(*this);
      other.advance(off);
      return other;
   }

   friend this_type operator+(Difference off, const this_type& right)
   {  return right + off; }

   this_type& operator-=(Difference off)
   {  this->advance(-off); return *this;   }

   this_type operator-(Difference off) const
   {  return *this + (-off);  }

   T& operator*() const
   { return dereference(); }

   T& operator[] (Difference ) const
   { return dereference(); }

   T *operator->() const
   { return &(dereference()); }

   private:
   T *         m_ptr;
   Difference  m_num;

   void increment()
   { --m_num; }

   void decrement()
   { ++m_num; }

   bool equal(const this_type &other) const
   {  return m_num == other.m_num;   }

   bool less(const this_type &other) const
   {  return other.m_num < m_num;   }

   T & dereference() const
   { return *m_ptr; }

   void advance(Difference n)
   {  m_num -= n; }

   Difference distance_to(const this_type &other)const
   {  return m_num - other.m_num;   }
};

template <class T, class EmplaceFunctor, class Difference /*= std::ptrdiff_t*/>
class emplace_iterator
  : public ::hboost::container::iterator
      <std::random_access_iterator_tag, T, Difference, const T*, const T &>
{
   typedef emplace_iterator this_type;

   public:
   typedef Difference difference_type;
   HBOOST_CONTAINER_FORCEINLINE explicit emplace_iterator(EmplaceFunctor&e)
      :  m_num(1), m_pe(&e){}

   HBOOST_CONTAINER_FORCEINLINE emplace_iterator()
      :  m_num(0), m_pe(0){}

   HBOOST_CONTAINER_FORCEINLINE this_type& operator++()
   { increment();   return *this;   }

   this_type operator++(int)
   {
      this_type result (*this);
      increment();
      return result;
   }

   HBOOST_CONTAINER_FORCEINLINE this_type& operator--()
   { decrement();   return *this;   }

   this_type operator--(int)
   {
      this_type result (*this);
      decrement();
      return result;
   }

   HBOOST_CONTAINER_FORCEINLINE friend bool operator== (const this_type& i, const this_type& i2)
   { return i.equal(i2); }

   HBOOST_CONTAINER_FORCEINLINE friend bool operator!= (const this_type& i, const this_type& i2)
   { return !(i == i2); }

   HBOOST_CONTAINER_FORCEINLINE friend bool operator< (const this_type& i, const this_type& i2)
   { return i.less(i2); }

   HBOOST_CONTAINER_FORCEINLINE friend bool operator> (const this_type& i, const this_type& i2)
   { return i2 < i; }

   HBOOST_CONTAINER_FORCEINLINE friend bool operator<= (const this_type& i, const this_type& i2)
   { return !(i > i2); }

   HBOOST_CONTAINER_FORCEINLINE friend bool operator>= (const this_type& i, const this_type& i2)
   { return !(i < i2); }

   HBOOST_CONTAINER_FORCEINLINE friend difference_type operator- (const this_type& i, const this_type& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   HBOOST_CONTAINER_FORCEINLINE this_type& operator+=(difference_type off)
   {  this->advance(off); return *this;   }

   this_type operator+(difference_type off) const
   {
      this_type other(*this);
      other.advance(off);
      return other;
   }

   HBOOST_CONTAINER_FORCEINLINE friend this_type operator+(difference_type off, const this_type& right)
   {  return right + off; }

   HBOOST_CONTAINER_FORCEINLINE this_type& operator-=(difference_type off)
   {  this->advance(-off); return *this;   }

   HBOOST_CONTAINER_FORCEINLINE this_type operator-(difference_type off) const
   {  return *this + (-off);  }

   //This pseudo-iterator's dereference operations have no sense since value is not
   //constructed until ::hboost::container::construct_in_place is called.
   //So comment them to catch bad uses
   //const T& operator*() const;
   //const T& operator[](difference_type) const;
   //const T* operator->() const;

   template<class Allocator>
   void construct_in_place(Allocator &a, T* ptr)
   {  (*m_pe)(a, ptr);  }

   private:
   difference_type m_num;
   EmplaceFunctor *            m_pe;

   HBOOST_CONTAINER_FORCEINLINE void increment()
   { --m_num; }

   HBOOST_CONTAINER_FORCEINLINE void decrement()
   { ++m_num; }

   HBOOST_CONTAINER_FORCEINLINE bool equal(const this_type &other) const
   {  return m_num == other.m_num;   }

   HBOOST_CONTAINER_FORCEINLINE bool less(const this_type &other) const
   {  return other.m_num < m_num;   }

   HBOOST_CONTAINER_FORCEINLINE const T & dereference() const
   {
      static T dummy;
      return dummy;
   }

   HBOOST_CONTAINER_FORCEINLINE void advance(difference_type n)
   {  m_num -= n; }

   HBOOST_CONTAINER_FORCEINLINE difference_type distance_to(const this_type &other)const
   {  return difference_type(m_num - other.m_num);   }
};

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

template<class ...Args>
struct emplace_functor
{
   typedef typename container_detail::build_number_seq<sizeof...(Args)>::type index_tuple_t;

   emplace_functor(HBOOST_FWD_REF(Args)... args)
      : args_(args...)
   {}

   template<class Allocator, class T>
   void operator()(Allocator &a, T *ptr)
   {  emplace_functor::inplace_impl(a, ptr, index_tuple_t());  }

   template<class Allocator, class T, int ...IdxPack>
   HBOOST_CONTAINER_FORCEINLINE void inplace_impl(Allocator &a, T* ptr, const container_detail::index_tuple<IdxPack...>&)
   {
      allocator_traits<Allocator>::construct
         (a, ptr, ::hboost::forward<Args>(container_detail::get<IdxPack>(args_))...);
   }

   container_detail::tuple<Args&...> args_;
};

#else // !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define HBOOST_MOVE_ITERATOR_EMPLACE_FUNCTOR_CODE(N) \
HBOOST_MOVE_TMPL_LT##N HBOOST_MOVE_CLASS##N HBOOST_MOVE_GT##N \
struct emplace_functor##N\
{\
   explicit emplace_functor##N( HBOOST_MOVE_UREF##N )\
      HBOOST_MOVE_COLON##N HBOOST_MOVE_FWD_INIT##N{}\
   \
   template<class Allocator, class T>\
   void operator()(Allocator &a, T *ptr)\
   {  allocator_traits<Allocator>::construct(a, ptr HBOOST_MOVE_I##N HBOOST_MOVE_MFWD##N);  }\
   \
   HBOOST_MOVE_MREF##N\
};\
//
HBOOST_MOVE_ITERATE_0TO9(HBOOST_MOVE_ITERATOR_EMPLACE_FUNCTOR_CODE)
#undef HBOOST_MOVE_ITERATOR_EMPLACE_FUNCTOR_CODE

#endif

namespace container_detail {

template<class T>
struct has_iterator_category
{
   struct two { char _[2]; };

   template <typename X>
   static char test(int, typename X::iterator_category*);

   template <typename X>
   static two test(int, ...);

   static const bool value = (1 == sizeof(test<T>(0, 0)));
};


template<class T, bool = has_iterator_category<T>::value >
struct is_input_iterator
{
   static const bool value = is_same<typename T::iterator_category, std::input_iterator_tag>::value;
};

template<class T>
struct is_input_iterator<T, false>
{
   static const bool value = false;
};

template<class T>
struct is_not_input_iterator
{
   static const bool value = !is_input_iterator<T>::value;
};

template<class T, bool = has_iterator_category<T>::value >
struct is_forward_iterator
{
   static const bool value = is_same<typename T::iterator_category, std::forward_iterator_tag>::value;
};

template<class T>
struct is_forward_iterator<T, false>
{
   static const bool value = false;
};

template<class T, bool = has_iterator_category<T>::value >
struct is_bidirectional_iterator
{
   static const bool value = is_same<typename T::iterator_category, std::bidirectional_iterator_tag>::value;
};

template<class T>
struct is_bidirectional_iterator<T, false>
{
   static const bool value = false;
};

template<class IINodeType>
struct iiterator_node_value_type {
  typedef typename IINodeType::value_type type;
};

template<class IIterator>
struct iiterator_types
{
   typedef typename IIterator::value_type                            it_value_type;
   typedef typename iiterator_node_value_type<it_value_type>::type   value_type;
   typedef typename hboost::container::iterator_traits<IIterator>::pointer         it_pointer;
   typedef typename hboost::container::iterator_traits<IIterator>::difference_type difference_type;
   typedef typename ::hboost::intrusive::pointer_traits<it_pointer>::
      template rebind_pointer<value_type>::type                      pointer;
   typedef typename ::hboost::intrusive::pointer_traits<it_pointer>::
      template rebind_pointer<const value_type>::type                const_pointer;
   typedef typename ::hboost::intrusive::
      pointer_traits<pointer>::reference                             reference;
   typedef typename ::hboost::intrusive::
      pointer_traits<const_pointer>::reference                       const_reference;
   typedef typename IIterator::iterator_category                     iterator_category;
};

template<class IIterator, bool IsConst>
struct iterator_types
{
   typedef typename ::hboost::container::iterator
      < typename iiterator_types<IIterator>::iterator_category
      , typename iiterator_types<IIterator>::value_type
      , typename iiterator_types<IIterator>::difference_type
      , typename iiterator_types<IIterator>::const_pointer
      , typename iiterator_types<IIterator>::const_reference> type;
};

template<class IIterator>
struct iterator_types<IIterator, false>
{
   typedef typename ::hboost::container::iterator
      < typename iiterator_types<IIterator>::iterator_category
      , typename iiterator_types<IIterator>::value_type
      , typename iiterator_types<IIterator>::difference_type
      , typename iiterator_types<IIterator>::pointer
      , typename iiterator_types<IIterator>::reference> type;
};

template<class IIterator, bool IsConst>
class iterator_from_iiterator
{
   typedef typename iterator_types<IIterator, IsConst>::type types_t;

   public:
   typedef typename types_t::pointer             pointer;
   typedef typename types_t::reference           reference;
   typedef typename types_t::difference_type     difference_type;
   typedef typename types_t::iterator_category   iterator_category;
   typedef typename types_t::value_type          value_type;

   HBOOST_CONTAINER_FORCEINLINE iterator_from_iiterator()
   {}

   HBOOST_CONTAINER_FORCEINLINE explicit iterator_from_iiterator(IIterator iit) HBOOST_NOEXCEPT_OR_NOTHROW
      : m_iit(iit)
   {}

   HBOOST_CONTAINER_FORCEINLINE iterator_from_iiterator(iterator_from_iiterator<IIterator, false> const& other) HBOOST_NOEXCEPT_OR_NOTHROW
      :  m_iit(other.get())
   {}

   HBOOST_CONTAINER_FORCEINLINE iterator_from_iiterator& operator++() HBOOST_NOEXCEPT_OR_NOTHROW
   {  ++this->m_iit;   return *this;  }

   HBOOST_CONTAINER_FORCEINLINE iterator_from_iiterator operator++(int) HBOOST_NOEXCEPT_OR_NOTHROW
   {
      iterator_from_iiterator result (*this);
      ++this->m_iit;
      return result;
   }

   HBOOST_CONTAINER_FORCEINLINE iterator_from_iiterator& operator--() HBOOST_NOEXCEPT_OR_NOTHROW
   {
      //If the iterator_from_iiterator is not a bidirectional iterator, operator-- should not exist
      HBOOST_STATIC_ASSERT((is_bidirectional_iterator<iterator_from_iiterator>::value));
      --this->m_iit;   return *this;
   }

   HBOOST_CONTAINER_FORCEINLINE iterator_from_iiterator operator--(int) HBOOST_NOEXCEPT_OR_NOTHROW
   {
      iterator_from_iiterator result (*this);
      --this->m_iit;
      return result;
   }

   HBOOST_CONTAINER_FORCEINLINE friend bool operator== (const iterator_from_iiterator& l, const iterator_from_iiterator& r) HBOOST_NOEXCEPT_OR_NOTHROW
   {  return l.m_iit == r.m_iit;   }

   HBOOST_CONTAINER_FORCEINLINE friend bool operator!= (const iterator_from_iiterator& l, const iterator_from_iiterator& r) HBOOST_NOEXCEPT_OR_NOTHROW
   {  return !(l == r); }

   HBOOST_CONTAINER_FORCEINLINE reference operator*()  const HBOOST_NOEXCEPT_OR_NOTHROW
   {  return this->m_iit->get_data();  }

   HBOOST_CONTAINER_FORCEINLINE pointer   operator->() const HBOOST_NOEXCEPT_OR_NOTHROW
   {  return ::hboost::intrusive::pointer_traits<pointer>::pointer_to(this->operator*());  }

   HBOOST_CONTAINER_FORCEINLINE const IIterator &get() const HBOOST_NOEXCEPT_OR_NOTHROW
   {  return this->m_iit;   }

   private:
   IIterator m_iit;
};

}  //namespace container_detail {

using ::hboost::intrusive::reverse_iterator;

}  //namespace container {
}  //namespace hboost {

#include <hboost/container/detail/config_end.hpp>

#endif   //#ifndef HBOOST_CONTAINER_DETAIL_ITERATORS_HPP
