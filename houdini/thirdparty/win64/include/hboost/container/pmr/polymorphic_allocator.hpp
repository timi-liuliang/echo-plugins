//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2015-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_CONTAINER_PMR_POLYMORPHIC_ALLOCATOR_HPP
#define HBOOST_CONTAINER_PMR_POLYMORPHIC_ALLOCATOR_HPP

#if defined (_MSC_VER)
#  pragma once 
#endif

#include <hboost/config.hpp>
#include <hboost/move/detail/type_traits.hpp>
#include <hboost/move/utility_core.hpp>
#include <hboost/container/detail/dispatch_uses_allocator.hpp>
#include <hboost/container/new_allocator.hpp>
#include <hboost/container/pmr/memory_resource.hpp>
#include <hboost/container/pmr/global_resource.hpp>

#include <cstddef>

namespace hboost {
namespace container {
namespace pmr {

//! A specialization of class template `polymorphic_allocator` conforms to the Allocator requirements.
//! Constructed with different memory resources, different instances of the same specialization of
//! `polymorphic_allocator` can exhibit entirely different allocation behavior. This runtime
//! polymorphism allows objects that use polymorphic_allocator to behave as if they used different
//! allocator types at run time even though they use the same static allocator type.
template <class T>
class polymorphic_allocator
{
   public:
   typedef T value_type;

   //! <b>Effects</b>: Sets m_resource to
   //! `get_default_resource()`.
   polymorphic_allocator() HBOOST_NOEXCEPT
      : m_resource(::hboost::container::pmr::get_default_resource())
   {}

   //! <b>Requires</b>: r is non-null.
   //!
   //! <b>Effects</b>: Sets m_resource to r.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Notes</b>: This constructor provides an implicit conversion from memory_resource*.
   //!   Non-standard extension: if r is null m_resource is set to get_default_resource().
   polymorphic_allocator(memory_resource* r)
      : m_resource(r ? r : ::hboost::container::pmr::get_default_resource())
   {}

   //! <b>Effects</b>: Sets m_resource to
   //!   other.resource().
   polymorphic_allocator(const polymorphic_allocator& other)
      : m_resource(other.m_resource)
   {}

   //! <b>Effects</b>: Sets m_resource to
   //!   other.resource().
   template <class U>
   polymorphic_allocator(const polymorphic_allocator<U>& other) HBOOST_NOEXCEPT
      : m_resource(other.resource())
   {}

   //! <b>Effects</b>: Sets m_resource to
   //!   other.resource().
   polymorphic_allocator& operator=(const polymorphic_allocator& other)
   {  m_resource = other.m_resource;   return *this;  }

   //! <b>Returns</b>: Equivalent to
   //!   `static_cast<T*>(m_resource->allocate(n * sizeof(T), alignof(T)))`.
   T* allocate(size_t n)
   {  return static_cast<T*>(m_resource->allocate(n*sizeof(T), ::hboost::move_detail::alignment_of<T>::value));  }

   //! <b>Requires</b>: p was allocated from a memory resource, x, equal to *m_resource,
   //! using `x.allocate(n * sizeof(T), alignof(T))`.
   //!
   //! <b>Effects</b>: Equivalent to m_resource->deallocate(p, n * sizeof(T), alignof(T)).
   //!
   //! <b>Throws</b>: Nothing.
   void deallocate(T* p, size_t n)
   {  m_resource->deallocate(p, n*sizeof(T), ::hboost::move_detail::alignment_of<T>::value);  }

   #if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)
   //! <b>Requires</b>: Uses-allocator construction of T with allocator
   //!   `this->resource()` and constructor arguments `std::forward<Args>(args)...`
   //!   is well-formed. [Note: uses-allocator construction is always well formed for
   //!   types that do not use allocators. - end note]
   //!
   //! <b>Effects</b>: Construct a T object at p by uses-allocator construction with allocator
   //!   `this->resource()` and constructor arguments `std::forward<Args>(args)...`.
   //!
   //! <b>Throws</b>: Nothing unless the constructor for T throws.
   template < typename U, class ...Args>
   void construct(U* p, HBOOST_FWD_REF(Args)...args)
   {
      new_allocator<U> na;
      container_detail::dispatch_uses_allocator
         (na, this->resource(), p, ::hboost::forward<Args>(args)...);
   }

   #else // #if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //Disable this overload if the first argument is pair as some compilers have
   //overload selection problems when the first parameter is a pair.
   #define HBOOST_CONTAINER_PMR_POLYMORPHIC_ALLOCATOR_CONSTRUCT_CODE(N) \
   template < typename U HBOOST_MOVE_I##N HBOOST_MOVE_CLASSQ##N >\
   void construct(U* p HBOOST_MOVE_I##N HBOOST_MOVE_UREFQ##N)\
   {\
      new_allocator<U> na;\
      container_detail::dispatch_uses_allocator\
         (na, this->resource(), p HBOOST_MOVE_I##N HBOOST_MOVE_FWDQ##N);\
   }\
   //
   HBOOST_MOVE_ITERATE_0TO9(HBOOST_CONTAINER_PMR_POLYMORPHIC_ALLOCATOR_CONSTRUCT_CODE)
   #undef HBOOST_CONTAINER_PMR_POLYMORPHIC_ALLOCATOR_CONSTRUCT_CODE

   #endif   //#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>:
   //!   p->~U().
   template <class U>
   void destroy(U* p)
   {  (void)p; p->~U(); }

   //! <b>Returns</b>: Equivalent to
   //!   `polymorphic_allocator()`.
   polymorphic_allocator select_on_container_copy_construction() const
   {  return polymorphic_allocator();  }

   //! <b>Returns</b>:
   //!   m_resource.
   memory_resource* resource() const
   {  return m_resource;  }

   private:
   memory_resource* m_resource;
};

//! <b>Returns</b>:
//!   `*a.resource() == *b.resource()`.
template <class T1, class T2>
bool operator==(const polymorphic_allocator<T1>& a, const polymorphic_allocator<T2>& b) HBOOST_NOEXCEPT
{  return *a.resource() == *b.resource();  }


//! <b>Returns</b>:
//!   `! (a == b)`.
template <class T1, class T2>
bool operator!=(const polymorphic_allocator<T1>& a, const polymorphic_allocator<T2>& b) HBOOST_NOEXCEPT
{  return *a.resource() != *b.resource();  }

}  //namespace pmr {
}  //namespace container {
}  //namespace hboost {

#endif   //HBOOST_CONTAINER_PMR_POLYMORPHIC_ALLOCATOR_HPP
