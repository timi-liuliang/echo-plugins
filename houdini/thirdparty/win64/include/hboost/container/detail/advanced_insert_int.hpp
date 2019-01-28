//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2008-2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_ADVANCED_INSERT_INT_HPP
#define HBOOST_CONTAINER_ADVANCED_INSERT_INT_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>

// container
#include <hboost/container/allocator_traits.hpp>
// container/detail
#include <hboost/container/detail/copy_move_algo.hpp>
#include <hboost/container/detail/destroyers.hpp>
#include <hboost/container/detail/mpl.hpp>
#include <hboost/container/detail/type_traits.hpp>
#include <hboost/container/detail/iterator.hpp>
#include <hboost/container/detail/iterators.hpp>
#include <hboost/container/detail/iterator_to_raw_pointer.hpp>
#if defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#include <hboost/move/detail/fwd_macros.hpp>
#endif
// move
#include <hboost/move/utility_core.hpp>
// other
#include <hboost/assert.hpp>
#include <hboost/core/no_exceptions_support.hpp>

namespace hboost { namespace container { namespace container_detail {

template<class Allocator, class FwdIt, class Iterator>
struct move_insert_range_proxy
{
   typedef typename allocator_traits<Allocator>::size_type size_type;
   typedef typename allocator_traits<Allocator>::value_type value_type;

   explicit move_insert_range_proxy(FwdIt first)
      :  first_(first)
   {}

   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n)
   {
      this->first_ = ::hboost::container::uninitialized_move_alloc_n_source
         (a, this->first_, n, p);
   }

   void copy_n_and_update(Allocator &, Iterator p, size_type n)
   {
      this->first_ = ::hboost::container::move_n_source(this->first_, n, p);
   }

   FwdIt first_;
};


template<class Allocator, class FwdIt, class Iterator>
struct insert_range_proxy
{
   typedef typename allocator_traits<Allocator>::size_type size_type;
   typedef typename allocator_traits<Allocator>::value_type value_type;

   explicit insert_range_proxy(FwdIt first)
      :  first_(first)
   {}

   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n)
   {
      this->first_ = ::hboost::container::uninitialized_copy_alloc_n_source(a, this->first_, n, p);
   }

   void copy_n_and_update(Allocator &, Iterator p, size_type n)
   {
      this->first_ = ::hboost::container::copy_n_source(this->first_, n, p);
   }

   FwdIt first_;
};


template<class Allocator, class Iterator>
struct insert_n_copies_proxy
{
   typedef typename allocator_traits<Allocator>::size_type size_type;
   typedef typename allocator_traits<Allocator>::value_type value_type;

   explicit insert_n_copies_proxy(const value_type &v)
      :  v_(v)
   {}

   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n) const
   {  hboost::container::uninitialized_fill_alloc_n(a, v_, n, p);  }

   void copy_n_and_update(Allocator &, Iterator p, size_type n) const
   {
	   for (; 0 < n; --n, ++p){
		   *p = v_;
      }
   }

   const value_type &v_;
};

template<class Allocator, class Iterator>
struct insert_value_initialized_n_proxy
{
   typedef ::hboost::container::allocator_traits<Allocator> alloc_traits;
   typedef typename allocator_traits<Allocator>::size_type size_type;
   typedef typename allocator_traits<Allocator>::value_type value_type;

   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n) const
   {  hboost::container::uninitialized_value_init_alloc_n(a, n, p);  }

   void copy_n_and_update(Allocator &, Iterator, size_type) const
   {  HBOOST_ASSERT(false); }
};

template<class Allocator, class Iterator>
struct insert_default_initialized_n_proxy
{
   typedef ::hboost::container::allocator_traits<Allocator> alloc_traits;
   typedef typename allocator_traits<Allocator>::size_type size_type;
   typedef typename allocator_traits<Allocator>::value_type value_type;

   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n) const
   {  hboost::container::uninitialized_default_init_alloc_n(a, n, p);  }

   void copy_n_and_update(Allocator &, Iterator, size_type) const
   {  HBOOST_ASSERT(false); }
};

template<class Allocator, class Iterator>
struct insert_copy_proxy
{
   typedef hboost::container::allocator_traits<Allocator> alloc_traits;
   typedef typename alloc_traits::size_type size_type;
   typedef typename alloc_traits::value_type value_type;

   explicit insert_copy_proxy(const value_type &v)
      :  v_(v)
   {}

   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n) const
   {
      HBOOST_ASSERT(n == 1);  (void)n;
      alloc_traits::construct( a, iterator_to_raw_pointer(p), v_);
   }

   void copy_n_and_update(Allocator &, Iterator p, size_type n) const
   {
      HBOOST_ASSERT(n == 1);  (void)n;
      *p =v_;
   }

   const value_type &v_;
};


template<class Allocator, class Iterator>
struct insert_move_proxy
{
   typedef hboost::container::allocator_traits<Allocator> alloc_traits;
   typedef typename alloc_traits::size_type size_type;
   typedef typename alloc_traits::value_type value_type;

   explicit insert_move_proxy(value_type &v)
      :  v_(v)
   {}

   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n) const
   {
      HBOOST_ASSERT(n == 1);  (void)n;
      alloc_traits::construct( a, iterator_to_raw_pointer(p), ::hboost::move(v_) );
   }

   void copy_n_and_update(Allocator &, Iterator p, size_type n) const
   {
      HBOOST_ASSERT(n == 1);  (void)n;
      *p = ::hboost::move(v_);
   }

   value_type &v_;
};

template<class It, class Allocator>
insert_move_proxy<Allocator, It> get_insert_value_proxy(HBOOST_RV_REF(typename hboost::container::iterator_traits<It>::value_type) v)
{
   return insert_move_proxy<Allocator, It>(v);
}

template<class It, class Allocator>
insert_copy_proxy<Allocator, It> get_insert_value_proxy(const typename hboost::container::iterator_traits<It>::value_type &v)
{
   return insert_copy_proxy<Allocator, It>(v);
}

}}}   //namespace hboost { namespace container { namespace container_detail {

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

#include <hboost/container/detail/variadic_templates_tools.hpp>
#include <hboost/move/utility_core.hpp>

namespace hboost {
namespace container {
namespace container_detail {

template<class Allocator, class Iterator, class ...Args>
struct insert_nonmovable_emplace_proxy
{
   typedef hboost::container::allocator_traits<Allocator>   alloc_traits;
   typedef typename alloc_traits::size_type        size_type;
   typedef typename alloc_traits::value_type       value_type;

   typedef typename build_number_seq<sizeof...(Args)>::type index_tuple_t;

   explicit insert_nonmovable_emplace_proxy(HBOOST_FWD_REF(Args)... args)
      : args_(args...)
   {}

   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n)
   {  this->priv_uninitialized_copy_some_and_update(a, index_tuple_t(), p, n);  }

   private:
   template<int ...IdxPack>
   void priv_uninitialized_copy_some_and_update(Allocator &a, const index_tuple<IdxPack...>&, Iterator p, size_type n)
   {
      HBOOST_ASSERT(n == 1); (void)n;
      alloc_traits::construct( a, iterator_to_raw_pointer(p), ::hboost::forward<Args>(get<IdxPack>(this->args_))... );
   }

   protected:
   tuple<Args&...> args_;
};

template<class Allocator, class Iterator, class ...Args>
struct insert_emplace_proxy
   :  public insert_nonmovable_emplace_proxy<Allocator, Iterator, Args...>
{
   typedef insert_nonmovable_emplace_proxy<Allocator, Iterator, Args...> base_t;
   typedef hboost::container::allocator_traits<Allocator>   alloc_traits;
   typedef typename base_t::value_type             value_type;
   typedef typename base_t::size_type              size_type;
   typedef typename base_t::index_tuple_t          index_tuple_t;

   explicit insert_emplace_proxy(HBOOST_FWD_REF(Args)... args)
      : base_t(::hboost::forward<Args>(args)...)
   {}

   void copy_n_and_update(Allocator &a, Iterator p, size_type n)
   {  this->priv_copy_some_and_update(a, index_tuple_t(), p, n);  }

   private:

   template<int ...IdxPack>
   void priv_copy_some_and_update(Allocator &a, const index_tuple<IdxPack...>&, Iterator p, size_type n)
   {
      HBOOST_ASSERT(n ==1); (void)n;
      typename aligned_storage<sizeof(value_type), alignment_of<value_type>::value>::type v;
      value_type *vp = static_cast<value_type *>(static_cast<void *>(&v));
      alloc_traits::construct(a, vp,
         ::hboost::forward<Args>(get<IdxPack>(this->args_))...);
      HBOOST_TRY{
         *p = ::hboost::move(*vp);
      }
      HBOOST_CATCH(...){
         alloc_traits::destroy(a, vp);
         HBOOST_RETHROW
      }
      HBOOST_CATCH_END
      alloc_traits::destroy(a, vp);
   }
};

//Specializations to avoid an unneeded temporary when emplacing from a single argument o type value_type
template<class Allocator, class Iterator>
struct insert_emplace_proxy<Allocator, Iterator, typename hboost::container::allocator_traits<Allocator>::value_type>
   : public insert_move_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy(typename hboost::container::allocator_traits<Allocator>::value_type &&v)
   : insert_move_proxy<Allocator, Iterator>(v)
   {}
};

//We use "add_const" here as adding "const" only confuses MSVC12(and maybe later) provoking
//compiler error C2752 ("more than one partial specialization matches").
//Any problem is solvable with an extra layer of indirection? ;-)
template<class Allocator, class Iterator>
struct insert_emplace_proxy<Allocator, Iterator
   , typename hboost::container::container_detail::add_const<typename hboost::container::allocator_traits<Allocator>::value_type>::type
   >
   : public insert_copy_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy(const typename hboost::container::allocator_traits<Allocator>::value_type &v)
   : insert_copy_proxy<Allocator, Iterator>(v)
   {}
};

template<class Allocator, class Iterator>
struct insert_emplace_proxy<Allocator, Iterator, typename hboost::container::allocator_traits<Allocator>::value_type &>
   : public insert_copy_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy(const typename hboost::container::allocator_traits<Allocator>::value_type &v)
   : insert_copy_proxy<Allocator, Iterator>(v)
   {}
};

template<class Allocator, class Iterator>
struct insert_emplace_proxy<Allocator, Iterator
   , typename hboost::container::container_detail::add_const<typename hboost::container::allocator_traits<Allocator>::value_type>::type &
   >
   : public insert_copy_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy(const typename hboost::container::allocator_traits<Allocator>::value_type &v)
   : insert_copy_proxy<Allocator, Iterator>(v)
   {}
};

}}}   //namespace hboost { namespace container { namespace container_detail {

#else // !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

#include <hboost/container/detail/value_init.hpp>

namespace hboost {
namespace container {
namespace container_detail {

#define HBOOST_CONTAINER_ADVANCED_INSERT_INT_CODE(N) \
template< class Allocator, class Iterator HBOOST_MOVE_I##N HBOOST_MOVE_CLASS##N >\
struct insert_nonmovable_emplace_proxy##N\
{\
   typedef hboost::container::allocator_traits<Allocator> alloc_traits;\
   typedef typename alloc_traits::size_type size_type;\
   typedef typename alloc_traits::value_type value_type;\
   \
   explicit insert_nonmovable_emplace_proxy##N(HBOOST_MOVE_UREF##N)\
      HBOOST_MOVE_COLON##N HBOOST_MOVE_FWD_INIT##N {}\
   \
   void uninitialized_copy_n_and_update(Allocator &a, Iterator p, size_type n)\
   {\
      HBOOST_ASSERT(n == 1); (void)n;\
      alloc_traits::construct(a, iterator_to_raw_pointer(p) HBOOST_MOVE_I##N HBOOST_MOVE_MFWD##N);\
   }\
   \
   void copy_n_and_update(Allocator &, Iterator, size_type)\
   {  HBOOST_ASSERT(false);   }\
   \
   protected:\
   HBOOST_MOVE_MREF##N\
};\
\
template< class Allocator, class Iterator HBOOST_MOVE_I##N HBOOST_MOVE_CLASS##N >\
struct insert_emplace_proxy_arg##N\
   : insert_nonmovable_emplace_proxy##N< Allocator, Iterator HBOOST_MOVE_I##N HBOOST_MOVE_TARG##N >\
{\
   typedef insert_nonmovable_emplace_proxy##N\
      < Allocator, Iterator HBOOST_MOVE_I##N HBOOST_MOVE_TARG##N > base_t;\
   typedef typename base_t::value_type value_type;\
   typedef typename base_t::size_type size_type;\
   typedef hboost::container::allocator_traits<Allocator> alloc_traits;\
   \
   explicit insert_emplace_proxy_arg##N(HBOOST_MOVE_UREF##N)\
      : base_t(HBOOST_MOVE_FWD##N){}\
   \
   void copy_n_and_update(Allocator &a, Iterator p, size_type n)\
   {\
      HBOOST_ASSERT(n == 1); (void)n;\
      typename aligned_storage<sizeof(value_type), alignment_of<value_type>::value>::type v;\
      HBOOST_ASSERT((((size_type)(&v)) % alignment_of<value_type>::value) == 0);\
      value_type *vp = static_cast<value_type *>(static_cast<void *>(&v));\
      alloc_traits::construct(a, vp HBOOST_MOVE_I##N HBOOST_MOVE_MFWD##N);\
      HBOOST_TRY{\
         *p = ::hboost::move(*vp);\
      }\
      HBOOST_CATCH(...){\
         alloc_traits::destroy(a, vp);\
         HBOOST_RETHROW\
      }\
      HBOOST_CATCH_END\
      alloc_traits::destroy(a, vp);\
   }\
};\
//
HBOOST_MOVE_ITERATE_0TO9(HBOOST_CONTAINER_ADVANCED_INSERT_INT_CODE)
#undef HBOOST_CONTAINER_ADVANCED_INSERT_INT_CODE

#if defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)

//Specializations to avoid an unneeded temporary when emplacing from a single argument o type value_type
template<class Allocator, class Iterator>
struct insert_emplace_proxy_arg1<Allocator, Iterator, ::hboost::rv<typename hboost::container::allocator_traits<Allocator>::value_type> >
   : public insert_move_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy_arg1(typename hboost::container::allocator_traits<Allocator>::value_type &v)
   : insert_move_proxy<Allocator, Iterator>(v)
   {}
};

template<class Allocator, class Iterator>
struct insert_emplace_proxy_arg1<Allocator, Iterator, typename hboost::container::allocator_traits<Allocator>::value_type>
   : public insert_copy_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy_arg1(const typename hboost::container::allocator_traits<Allocator>::value_type &v)
   : insert_copy_proxy<Allocator, Iterator>(v)
   {}
};

#else //e.g. MSVC10 & MSVC11

//Specializations to avoid an unneeded temporary when emplacing from a single argument o type value_type
template<class Allocator, class Iterator>
struct insert_emplace_proxy_arg1<Allocator, Iterator, typename hboost::container::allocator_traits<Allocator>::value_type>
   : public insert_move_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy_arg1(typename hboost::container::allocator_traits<Allocator>::value_type &&v)
   : insert_move_proxy<Allocator, Iterator>(v)
   {}
};

//We use "add_const" here as adding "const" only confuses MSVC10&11 provoking
//compiler error C2752 ("more than one partial specialization matches").
//Any problem is solvable with an extra layer of indirection? ;-)
template<class Allocator, class Iterator>
struct insert_emplace_proxy_arg1<Allocator, Iterator
   , typename hboost::container::container_detail::add_const<typename hboost::container::allocator_traits<Allocator>::value_type>::type
   >
   : public insert_copy_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy_arg1(const typename hboost::container::allocator_traits<Allocator>::value_type &v)
   : insert_copy_proxy<Allocator, Iterator>(v)
   {}
};

template<class Allocator, class Iterator>
struct insert_emplace_proxy_arg1<Allocator, Iterator, typename hboost::container::allocator_traits<Allocator>::value_type &>
   : public insert_copy_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy_arg1(const typename hboost::container::allocator_traits<Allocator>::value_type &v)
   : insert_copy_proxy<Allocator, Iterator>(v)
   {}
};

template<class Allocator, class Iterator>
struct insert_emplace_proxy_arg1<Allocator, Iterator
   , typename hboost::container::container_detail::add_const<typename hboost::container::allocator_traits<Allocator>::value_type>::type &
   >
   : public insert_copy_proxy<Allocator, Iterator>
{
   explicit insert_emplace_proxy_arg1(const typename hboost::container::allocator_traits<Allocator>::value_type &v)
   : insert_copy_proxy<Allocator, Iterator>(v)
   {}
};

#endif

}}}   //namespace hboost { namespace container { namespace container_detail {

#endif   // !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

#include <hboost/container/detail/config_end.hpp>

#endif //#ifndef HBOOST_CONTAINER_ADVANCED_INSERT_INT_HPP
