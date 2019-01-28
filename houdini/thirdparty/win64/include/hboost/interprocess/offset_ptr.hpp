//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2015. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_OFFSET_PTR_HPP
#define HBOOST_INTERPROCESS_OFFSET_PTR_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/interprocess/detail/workaround.hpp>

#include <hboost/interprocess/interprocess_fwd.hpp>
#include <hboost/interprocess/detail/utilities.hpp>
#include <hboost/interprocess/detail/cast_tags.hpp>
#include <hboost/interprocess/detail/mpl.hpp>
#include <hboost/container/detail/type_traits.hpp>  //alignment_of, aligned_storage
#include <hboost/assert.hpp>
#include <iosfwd>

//!\file
//!Describes a smart pointer that stores the offset between this pointer and
//!target pointee, called offset_ptr.

namespace hboost {

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

//Predeclarations
template <class T>
struct has_trivial_destructor;

#endif   //#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

namespace interprocess {

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
namespace ipcdetail {

   template<class OffsetType, std::size_t OffsetAlignment>
   union offset_ptr_internal
   {
      HBOOST_STATIC_ASSERT(sizeof(OffsetType) >= sizeof(uintptr_t));

      explicit offset_ptr_internal(OffsetType off)
         : m_offset(off)
      {}

      OffsetType m_offset; //Distance between this object and pointee address

      typename ::hboost::container::container_detail::aligned_storage
         < sizeof(OffsetType)//for offset_type_alignment m_offset will be enough
         , (OffsetAlignment == offset_type_alignment) ? 1u : OffsetAlignment
         >::type alignment_helper;
   };

   //Note: using the address of a local variable to point to another address
   //is not standard conforming and this can be optimized-away by the compiler.
   //Non-inlining is a method to remain illegal but correct

   //Undef HBOOST_INTERPROCESS_OFFSET_PTR_INLINE_XXX if your compiler can inline
   //this code without breaking the library

   ////////////////////////////////////////////////////////////////////////
   //
   //                      offset_ptr_to_raw_pointer
   //
   ////////////////////////////////////////////////////////////////////////
   #define HBOOST_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_PTR
   HBOOST_INTERPROCESS_FORCEINLINE void * offset_ptr_to_raw_pointer(const volatile void *this_ptr, uintptr_t offset)
   {
      typedef pointer_uintptr_caster<void*> caster_t;
      #ifndef HBOOST_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_PTR
         if(offset == 1){
            return 0;
         }
         else{
            return caster_t(caster_t(this_ptr).uintptr() + offset).pointer();
         }
      #else
         uintptr_t mask = offset == 1;
         --mask;
         uintptr_t target_offset = caster_t(this_ptr).uintptr() + offset;
         target_offset &= mask;
         return caster_t(target_offset).pointer();
      #endif
   }

   ////////////////////////////////////////////////////////////////////////
   //
   //                      offset_ptr_to_offset
   //
   ////////////////////////////////////////////////////////////////////////
   #define HBOOST_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_OFF
   HBOOST_INTERPROCESS_FORCEINLINE uintptr_t offset_ptr_to_offset(const volatile void *ptr, const volatile void *this_ptr)
   {
      typedef pointer_uintptr_caster<void*> caster_t;
      #ifndef HBOOST_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_OFF
         //offset == 1 && ptr != 0 is not legal for this pointer
         if(!ptr){
            return 1;
         }
         else{
            uintptr_t offset = caster_t(ptr).uintptr() - caster_t(this_ptr).uintptr();
            HBOOST_ASSERT(offset != 1);
            return offset;
         }
      #else
         //const uintptr_t other = -uintptr_t(ptr != 0);
         //const uintptr_t offset = (caster_t(ptr).uintptr() - caster_t(this_ptr).uintptr()) & other;
         //return offset + uintptr_t(!other);
         //
         uintptr_t offset = caster_t(ptr).uintptr() - caster_t(this_ptr).uintptr();
         --offset;
         uintptr_t mask = uintptr_t(ptr == 0);
         --mask;
         offset &= mask;
         return ++offset;
      #endif
   }

   ////////////////////////////////////////////////////////////////////////
   //
   //                      offset_ptr_to_offset_from_other
   //
   ////////////////////////////////////////////////////////////////////////
   #define HBOOST_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_OFF_FROM_OTHER
   HBOOST_INTERPROCESS_FORCEINLINE uintptr_t offset_ptr_to_offset_from_other
      (const volatile void *this_ptr, const volatile void *other_ptr, uintptr_t other_offset)
   {
      typedef pointer_uintptr_caster<void*> caster_t;
      #ifndef HBOOST_INTERPROCESS_OFFSET_PTR_BRANCHLESS_TO_OFF_FROM_OTHER
      if(other_offset == 1){
         return 1;
      }
      else{
         uintptr_t offset = caster_t(other_ptr).uintptr() - caster_t(this_ptr).uintptr() + other_offset;
         HBOOST_ASSERT(offset != 1);
         return offset;
      }
      #else
      uintptr_t mask = other_offset == 1;
      --mask;
      uintptr_t offset = caster_t(other_ptr).uintptr() - caster_t(this_ptr).uintptr();
      offset &= mask;
      return offset + other_offset;

      //uintptr_t mask = -uintptr_t(other_offset != 1);
      //uintptr_t offset = caster_t(other_ptr).uintptr() - caster_t(this_ptr).uintptr();
      //offset &= mask;
      //return offset + other_offset;
      #endif
   }

   ////////////////////////////////////////////////////////////////////////
   //
   // Let's assume cast to void and cv cast don't change any target address
   //
   ////////////////////////////////////////////////////////////////////////
   template<class From, class To>
   struct offset_ptr_maintains_address
   {
      static const bool value =    ipcdetail::is_cv_same<From, To>::value
                                || ipcdetail::is_cv_same<void, To>::value
                                || ipcdetail::is_cv_same<char, To>::value
                                ;
   };

   template<class From, class To, class Ret = void>
   struct enable_if_convertible_equal_address
      : enable_if_c< is_convertible<From*, To*>::value
                     && offset_ptr_maintains_address<From, To>::value
                  , Ret>
   {};

   template<class From, class To, class Ret = void>
   struct enable_if_convertible_unequal_address
      : enable_if_c< is_convertible<From*, To*>::value
                     && !offset_ptr_maintains_address<From, To>::value
                   , Ret>
   {};

}  //namespace ipcdetail {
#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

//!A smart pointer that stores the offset between between the pointer and the
//!the object it points. This allows offset allows special properties, since
//!the pointer is independent from the address of the pointee, if the
//!pointer and the pointee are still separated by the same offset. This feature
//!converts offset_ptr in a smart pointer that can be placed in shared memory and
//!memory mapped files mapped in different addresses in every process.
//!
//! \tparam PointedType The type of the pointee.
//! \tparam DifferenceType A signed integer type that can represent the arithmetic operations on the pointer
//! \tparam OffsetType An unsigned integer type that can represent the
//!   distance between two pointers reinterpret_cast-ed as unsigned integers. This type
//!   should be at least of the same size of std::uintptr_t. In some systems it's possible to communicate
//!   between 32 and 64 bit processes using 64 bit offsets.
//! \tparam OffsetAlignment Alignment of the OffsetType stored inside. In some systems might be necessary
//!   to align it to 64 bits in order to communicate 32 and 64 bit processes using 64 bit offsets.
//!
//!<b>Note</b>: offset_ptr uses implementation defined properties, present in most platforms, for
//!performance reasons:
//!   - Assumes that uintptr_t representation of nullptr is (uintptr_t)zero.
//!   - Assumes that incrementing a uintptr_t obtained from a pointer is equivalent
//!     to incrementing the pointer and then converting it back to uintptr_t.
template <class PointedType, class DifferenceType, class OffsetType, std::size_t OffsetAlignment>
class offset_ptr
{
   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   typedef offset_ptr<PointedType, DifferenceType, OffsetType, OffsetAlignment>   self_t;
   void unspecified_bool_type_func() const {}
   typedef void (self_t::*unspecified_bool_type)() const;
   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

   public:
   typedef PointedType                       element_type;
   typedef PointedType *                     pointer;
   typedef typename ipcdetail::
      add_reference<PointedType>::type       reference;
   typedef typename ipcdetail::
      remove_volatile<typename ipcdetail::
         remove_const<PointedType>::type
            >::type                          value_type;
   typedef DifferenceType                    difference_type;
   typedef std::random_access_iterator_tag   iterator_category;
   typedef OffsetType                        offset_type;

   public:   //Public Functions

   //!Default constructor (null pointer).
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr() HBOOST_NOEXCEPT
      : internal(1)
   {}

   //!Constructor from raw pointer (allows "0" pointer conversion).
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr(pointer ptr) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>(ipcdetail::offset_ptr_to_offset(ptr, this)))
   {}

   //!Constructor from other pointer.
   //!Never throws.
   template <class T>
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr( T *ptr
             , typename ipcdetail::enable_if< ipcdetail::is_convertible<T*, PointedType*> >::type * = 0) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>
         (ipcdetail::offset_ptr_to_offset(static_cast<PointedType*>(ptr), this)))
   {}

   //!Constructor from other offset_ptr
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr& ptr) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>
         (ipcdetail::offset_ptr_to_offset_from_other(this, &ptr, ptr.internal.m_offset)))
   {}

   //!Constructor from other offset_ptr. If pointers of pointee types are
   //!convertible, offset_ptrs will be convertibles. Never throws.
   template<class T2>
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr( const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr
             #ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
             , typename ipcdetail::enable_if_convertible_equal_address<T2, PointedType>::type* = 0
             #endif
             ) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>
         (ipcdetail::offset_ptr_to_offset_from_other(this, &ptr, ptr.get_offset())))
   {}

   #ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

   //!Constructor from other offset_ptr. If pointers of pointee types are
   //!convertible, offset_ptrs will be convertibles. Never throws.
   template<class T2>
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr( const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr
             , typename ipcdetail::enable_if_convertible_unequal_address<T2, PointedType>::type* = 0) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>
         (ipcdetail::offset_ptr_to_offset(static_cast<PointedType*>(ptr.get()), this)))
   {}

   #endif

   //!Emulates static_cast operator.
   //!Never throws.
   template<class T2, class P2, class O2, std::size_t A2>
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr<T2, P2, O2, A2> & r, ipcdetail::static_cast_tag) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>
         (ipcdetail::offset_ptr_to_offset(static_cast<PointedType*>(r.get()), this)))
   {}

   //!Emulates const_cast operator.
   //!Never throws.
   template<class T2, class P2, class O2, std::size_t A2>
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr<T2, P2, O2, A2> & r, ipcdetail::const_cast_tag) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>
         (ipcdetail::offset_ptr_to_offset(const_cast<PointedType*>(r.get()), this)))
   {}

   //!Emulates dynamic_cast operator.
   //!Never throws.
   template<class T2, class P2, class O2, std::size_t A2>
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr<T2, P2, O2, A2> & r, ipcdetail::dynamic_cast_tag) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>
         (ipcdetail::offset_ptr_to_offset(dynamic_cast<PointedType*>(r.get()), this)))
   {}

   //!Emulates reinterpret_cast operator.
   //!Never throws.
   template<class T2, class P2, class O2, std::size_t A2>
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr(const offset_ptr<T2, P2, O2, A2> & r, ipcdetail::reinterpret_cast_tag) HBOOST_NOEXCEPT
      : internal(static_cast<OffsetType>
      (ipcdetail::offset_ptr_to_offset(reinterpret_cast<PointedType*>(r.get()), this)))
   {}

   //!Obtains raw pointer from offset.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE pointer get() const HBOOST_NOEXCEPT
   {  return (pointer)ipcdetail::offset_ptr_to_raw_pointer(this, this->internal.m_offset);   }

   HBOOST_INTERPROCESS_FORCEINLINE offset_type get_offset() const HBOOST_NOEXCEPT
   {  return this->internal.m_offset;  }

   //!Pointer-like -> operator. It can return 0 pointer.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE pointer operator->() const HBOOST_NOEXCEPT
   {  return this->get(); }

   //!Dereferencing operator, if it is a null offset_ptr behavior
   //!   is undefined. Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE reference operator* () const HBOOST_NOEXCEPT
   {
      pointer p = this->get();
      reference r = *p;
      return r;
   }

   //!Indexing operator.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE reference operator[](difference_type idx) const HBOOST_NOEXCEPT
   {  return this->get()[idx];  }

   //!Assignment from pointer (saves extra conversion).
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr& operator= (pointer from) HBOOST_NOEXCEPT
   {
      this->internal.m_offset =
         static_cast<OffsetType>(ipcdetail::offset_ptr_to_offset(from, this));
      return *this;
   }

   //!Assignment from other offset_ptr.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr& operator= (const offset_ptr & ptr) HBOOST_NOEXCEPT
   {
      this->internal.m_offset =
         static_cast<OffsetType>(ipcdetail::offset_ptr_to_offset_from_other(this, &ptr, ptr.internal.m_offset));
      return *this;
   }

   //!Assignment from related offset_ptr. If pointers of pointee types
   //!   are assignable, offset_ptrs will be assignable. Never throws.
   template<class T2> HBOOST_INTERPROCESS_FORCEINLINE 
   #ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
   typename ipcdetail::enable_if_c
      < ipcdetail::is_convertible<T2*, PointedType*>::value, offset_ptr&>::type
   #else
   offset_ptr&
   #endif
      operator= (const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr) HBOOST_NOEXCEPT
   {
      this->assign(ptr, ipcdetail::bool_<ipcdetail::offset_ptr_maintains_address<T2, PointedType>::value>());
      return *this;
   }

   public:

   //!offset_ptr += difference_type.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr &operator+= (difference_type offset) HBOOST_NOEXCEPT
   {  this->inc_offset(offset * sizeof (PointedType));   return *this;  }

   //!offset_ptr -= difference_type.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr &operator-= (difference_type offset) HBOOST_NOEXCEPT
   {  this->dec_offset(offset * sizeof (PointedType));   return *this;  }

   //!++offset_ptr.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr& operator++ (void) HBOOST_NOEXCEPT
   {  this->inc_offset(sizeof (PointedType));   return *this;  }

   //!offset_ptr++.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr operator++ (int) HBOOST_NOEXCEPT
   {
      offset_ptr tmp(*this);
      this->inc_offset(sizeof (PointedType));
      return tmp;
   }

   //!--offset_ptr.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr& operator-- (void) HBOOST_NOEXCEPT
   {  this->dec_offset(sizeof (PointedType));   return *this;  }

   //!offset_ptr--.
   //!Never throws.
   HBOOST_INTERPROCESS_FORCEINLINE offset_ptr operator-- (int) HBOOST_NOEXCEPT
   {
      offset_ptr tmp(*this);
      this->dec_offset(sizeof (PointedType));
      return tmp;
   }

   //!safe bool conversion operator.
   //!Never throws.
   #if defined(HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
   HBOOST_INTERPROCESS_FORCEINLINE operator unspecified_bool_type() const HBOOST_NOEXCEPT
   {  return this->internal.m_offset != 1? &self_t::unspecified_bool_type_func : 0;   }
   #else
   explicit operator bool() const HBOOST_NOEXCEPT
   {  return this->internal.m_offset != 1;  }
   #endif
   
   //!Not operator. Not needed in theory, but improves portability.
   //!Never throws
   HBOOST_INTERPROCESS_FORCEINLINE bool operator! () const HBOOST_NOEXCEPT
   {  return this->internal.m_offset == 1;   }

   //!Compatibility with pointer_traits
   //!
   template <class U>
   struct rebind
   {  typedef offset_ptr<U, DifferenceType, OffsetType, OffsetAlignment> other;  };

   //!Compatibility with pointer_traits
   //!
   HBOOST_INTERPROCESS_FORCEINLINE static offset_ptr pointer_to(reference r) HBOOST_NOEXCEPT
   { return offset_ptr(&r); }

   //!difference_type + offset_ptr
   //!operation
   HBOOST_INTERPROCESS_FORCEINLINE friend offset_ptr operator+(difference_type diff, offset_ptr right) HBOOST_NOEXCEPT
   {  right += diff;  return right;  }

   //!offset_ptr + difference_type
   //!operation
   HBOOST_INTERPROCESS_FORCEINLINE friend offset_ptr operator+(offset_ptr left, difference_type diff) HBOOST_NOEXCEPT
   {  left += diff;  return left; }

   //!offset_ptr - diff
   //!operation
   HBOOST_INTERPROCESS_FORCEINLINE friend offset_ptr operator-(offset_ptr left, difference_type diff) HBOOST_NOEXCEPT
   {  left -= diff;  return left; }

   //!offset_ptr - diff
   //!operation
   HBOOST_INTERPROCESS_FORCEINLINE friend offset_ptr operator-(difference_type diff, offset_ptr right) HBOOST_NOEXCEPT
   {  right -= diff; return right; }

   //!offset_ptr - offset_ptr
   //!operation
   HBOOST_INTERPROCESS_FORCEINLINE friend difference_type operator-(const offset_ptr &pt, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return difference_type(pt.get()- pt2.get());   }

   //Comparison
   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator== (const offset_ptr &pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1.get() == pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator!= (const offset_ptr &pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1.get() != pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator<(const offset_ptr &pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1.get() < pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator<=(const offset_ptr &pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1.get() <= pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator>(const offset_ptr &pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1.get() > pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator>=(const offset_ptr &pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1.get() >= pt2.get();  }

   //Comparison to raw ptr to support literal 0
   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator== (pointer pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1 == pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator!= (pointer pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1 != pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator<(pointer pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1 < pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator<=(pointer pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1 <= pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator>(pointer pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1 > pt2.get();  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator>=(pointer pt1, const offset_ptr &pt2) HBOOST_NOEXCEPT
   {  return pt1 >= pt2.get();  }

   //Comparison
   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator== (const offset_ptr &pt1, pointer pt2) HBOOST_NOEXCEPT
   {  return pt1.get() == pt2;  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator!= (const offset_ptr &pt1, pointer pt2) HBOOST_NOEXCEPT
   {  return pt1.get() != pt2;  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator<(const offset_ptr &pt1, pointer pt2) HBOOST_NOEXCEPT
   {  return pt1.get() < pt2;  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator<=(const offset_ptr &pt1, pointer pt2) HBOOST_NOEXCEPT
   {  return pt1.get() <= pt2;  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator>(const offset_ptr &pt1, pointer pt2) HBOOST_NOEXCEPT
   {  return pt1.get() > pt2;  }

   HBOOST_INTERPROCESS_FORCEINLINE friend bool operator>=(const offset_ptr &pt1, pointer pt2) HBOOST_NOEXCEPT
   {  return pt1.get() >= pt2;  }

   HBOOST_INTERPROCESS_FORCEINLINE friend void swap(offset_ptr &left, offset_ptr &right) HBOOST_NOEXCEPT
   {
      pointer ptr = right.get();
      right = left;
      left = ptr;
   }

   private:
   template<class T2>
   HBOOST_INTERPROCESS_FORCEINLINE void assign(const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr, ipcdetail::bool_<true>) HBOOST_NOEXCEPT
   {  //no need to pointer adjustment
      this->internal.m_offset =
         static_cast<OffsetType>(ipcdetail::offset_ptr_to_offset_from_other(this, &ptr, ptr.get_offset()));
   }

   template<class T2>
   HBOOST_INTERPROCESS_FORCEINLINE void assign(const offset_ptr<T2, DifferenceType, OffsetType, OffsetAlignment> &ptr, ipcdetail::bool_<false>) HBOOST_NOEXCEPT
   {  //we must convert to raw before calculating the offset
      this->internal.m_offset =
         static_cast<OffsetType>(ipcdetail::offset_ptr_to_offset(static_cast<PointedType*>(ptr.get()), this));
   }

   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   HBOOST_INTERPROCESS_FORCEINLINE void inc_offset(DifferenceType bytes) HBOOST_NOEXCEPT
   {  internal.m_offset += bytes;   }

   HBOOST_INTERPROCESS_FORCEINLINE void dec_offset(DifferenceType bytes) HBOOST_NOEXCEPT
   {  internal.m_offset -= bytes;   }

   ipcdetail::offset_ptr_internal<OffsetType, OffsetAlignment> internal;

   public:
   HBOOST_INTERPROCESS_FORCEINLINE const OffsetType &priv_offset() const HBOOST_NOEXCEPT
   {  return internal.m_offset;   }

   HBOOST_INTERPROCESS_FORCEINLINE       OffsetType &priv_offset() HBOOST_NOEXCEPT
   {  return internal.m_offset;   }

   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
};

//!operator<<
//!for offset ptr
template<class E, class T, class W, class X, class Y, std::size_t Z>
inline std::basic_ostream<E, T> & operator<<
   (std::basic_ostream<E, T> & os, offset_ptr<W, X, Y, Z> const & p)
{  return os << p.get_offset();   }

//!operator>>
//!for offset ptr
template<class E, class T, class W, class X, class Y, std::size_t Z>
inline std::basic_istream<E, T> & operator>>
   (std::basic_istream<E, T> & is, offset_ptr<W, X, Y, Z> & p)
{  return is >> p.get_offset();  }

//!Simulation of static_cast between pointers. Never throws.
template<class T1, class P1, class O1, std::size_t A1, class T2, class P2, class O2, std::size_t A2>
HBOOST_INTERPROCESS_FORCEINLINE hboost::interprocess::offset_ptr<T1, P1, O1, A1>
   static_pointer_cast(const hboost::interprocess::offset_ptr<T2, P2, O2, A2> & r) HBOOST_NOEXCEPT
{
   return hboost::interprocess::offset_ptr<T1, P1, O1, A1>
            (r, hboost::interprocess::ipcdetail::static_cast_tag());
}

//!Simulation of const_cast between pointers. Never throws.
template<class T1, class P1, class O1, std::size_t A1, class T2, class P2, class O2, std::size_t A2>
HBOOST_INTERPROCESS_FORCEINLINE hboost::interprocess::offset_ptr<T1, P1, O1, A1>
   const_pointer_cast(const hboost::interprocess::offset_ptr<T2, P2, O2, A2> & r) HBOOST_NOEXCEPT
{
   return hboost::interprocess::offset_ptr<T1, P1, O1, A1>
            (r, hboost::interprocess::ipcdetail::const_cast_tag());
}

//!Simulation of dynamic_cast between pointers. Never throws.
template<class T1, class P1, class O1, std::size_t A1, class T2, class P2, class O2, std::size_t A2>
HBOOST_INTERPROCESS_FORCEINLINE hboost::interprocess::offset_ptr<T1, P1, O1, A1>
   dynamic_pointer_cast(const hboost::interprocess::offset_ptr<T2, P2, O2, A2> & r) HBOOST_NOEXCEPT
{
   return hboost::interprocess::offset_ptr<T1, P1, O1, A1>
            (r, hboost::interprocess::ipcdetail::dynamic_cast_tag());
}

//!Simulation of reinterpret_cast between pointers. Never throws.
template<class T1, class P1, class O1, std::size_t A1, class T2, class P2, class O2, std::size_t A2>
HBOOST_INTERPROCESS_FORCEINLINE hboost::interprocess::offset_ptr<T1, P1, O1, A1>
   reinterpret_pointer_cast(const hboost::interprocess::offset_ptr<T2, P2, O2, A2> & r) HBOOST_NOEXCEPT
{
   return hboost::interprocess::offset_ptr<T1, P1, O1, A1>
            (r, hboost::interprocess::ipcdetail::reinterpret_cast_tag());
}

}  //namespace interprocess {

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

///has_trivial_destructor<> == true_type specialization for optimizations
template <class T, class P, class O, std::size_t A>
struct has_trivial_destructor< ::hboost::interprocess::offset_ptr<T, P, O, A> >
{
   static const bool value = true;
};

namespace move_detail {

///has_trivial_destructor<> == true_type specialization for optimizations
template <class T, class P, class O, std::size_t A>
struct is_trivially_destructible< ::hboost::interprocess::offset_ptr<T, P, O, A> >
{
   static const bool value = true;
};

}  //namespace move_detail {

namespace interprocess {

//!to_raw_pointer() enables hboost::mem_fn to recognize offset_ptr.
//!Never throws.
template <class T, class P, class O, std::size_t A>
HBOOST_INTERPROCESS_FORCEINLINE T * to_raw_pointer(hboost::interprocess::offset_ptr<T, P, O, A> const & p) HBOOST_NOEXCEPT
{  return ipcdetail::to_raw_pointer(p);   }

}  //namespace interprocess


#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED
}  //namespace hboost {

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

namespace hboost{

//This is to support embedding a bit in the pointer
//for intrusive containers, saving space
namespace intrusive {

//Predeclaration to avoid including header
template<class VoidPointer, std::size_t N>
struct max_pointer_plus_bits;

template<std::size_t OffsetAlignment, class P, class O, std::size_t A>
struct max_pointer_plus_bits<hboost::interprocess::offset_ptr<void, P, O, A>, OffsetAlignment>
{
   //The offset ptr can embed one bit less than the alignment since it
   //uses offset == 1 to store the null pointer.
   static const std::size_t value = ::hboost::interprocess::ipcdetail::ls_zeros<OffsetAlignment>::value - 1;
};

//Predeclaration
template<class Pointer, std::size_t NumBits>
struct pointer_plus_bits;

template<class T, class P, class O, std::size_t A, std::size_t NumBits>
struct pointer_plus_bits<hboost::interprocess::offset_ptr<T, P, O, A>, NumBits>
{
   typedef hboost::interprocess::offset_ptr<T, P, O, A>      pointer;
   //Bits are stored in the lower bits of the pointer except the LSB,
   //because this bit is used to represent the null pointer.
   static const uintptr_t Mask = ((uintptr_t(1) << uintptr_t(NumBits)) - uintptr_t(1)) << uintptr_t(1);
   HBOOST_STATIC_ASSERT(0 ==(Mask&1));

   //We must ALWAYS take argument "n" by reference as a copy of a null pointer
   //with a bit (e.g. offset == 3) would be incorrectly copied and interpreted as non-null.

   HBOOST_INTERPROCESS_FORCEINLINE static pointer get_pointer(const pointer &n) HBOOST_NOEXCEPT
   {
      pointer p;
      O const tmp_off = n.priv_offset() & O(~Mask);
      p.priv_offset() = hboost::interprocess::ipcdetail::offset_ptr_to_offset_from_other(&p, &n, tmp_off);
      return p;
   }

   HBOOST_INTERPROCESS_FORCEINLINE static void set_pointer(pointer &n, const pointer &p) HBOOST_NOEXCEPT
   {
      HBOOST_ASSERT(0 == (get_bits)(p));
      O const stored_bits = O(n.priv_offset() & Mask);
      n = p;
      n.priv_offset() |= stored_bits;
   }

   HBOOST_INTERPROCESS_FORCEINLINE static std::size_t get_bits(const pointer &n) HBOOST_NOEXCEPT
   {
      return std::size_t((n.priv_offset() & Mask) >> 1u);
   }

   HBOOST_INTERPROCESS_FORCEINLINE static void set_bits(pointer &n, std::size_t const b) HBOOST_NOEXCEPT
   {
      HBOOST_ASSERT(b < (std::size_t(1) << NumBits));
      O tmp = n.priv_offset();
      tmp &= O(~Mask);
      tmp |= O(b << 1u);
      n.priv_offset() = tmp;
   }
};

}  //namespace intrusive

//Predeclaration
template<class T, class U>
struct pointer_to_other;

//Backwards compatibility with pointer_to_other
template <class PointedType, class DifferenceType, class OffsetType, std::size_t OffsetAlignment, class U>
struct pointer_to_other
   < ::hboost::interprocess::offset_ptr<PointedType, DifferenceType, OffsetType, OffsetAlignment>, U >
{
   typedef ::hboost::interprocess::offset_ptr<U, DifferenceType, OffsetType, OffsetAlignment> type;
};

}  //namespace hboost{
#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

#include <hboost/interprocess/detail/config_end.hpp>

#endif //#ifndef HBOOST_INTERPROCESS_OFFSET_PTR_HPP
