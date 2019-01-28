/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2014-2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_DETAIL_SIZE_HOLDER_HPP
#define HBOOST_INTRUSIVE_DETAIL_SIZE_HOLDER_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/intrusive/detail/workaround.hpp>

namespace hboost {
namespace intrusive {
namespace detail {

template<bool ConstantSize, class SizeType, class Tag = void>
struct size_holder
{
   static const bool constant_time_size = ConstantSize;
   typedef SizeType  size_type;

   HBOOST_INTRUSIVE_FORCEINLINE SizeType get_size() const
   {  return size_;  }

   HBOOST_INTRUSIVE_FORCEINLINE void set_size(SizeType size)
   {  size_ = size; }

   HBOOST_INTRUSIVE_FORCEINLINE void decrement()
   {  --size_; }

   HBOOST_INTRUSIVE_FORCEINLINE void increment()
   {  ++size_; }

   HBOOST_INTRUSIVE_FORCEINLINE void increase(SizeType n)
   {  size_ += n; }

   HBOOST_INTRUSIVE_FORCEINLINE void decrease(SizeType n)
   {  size_ -= n; }

   SizeType size_;
};

template<class SizeType, class Tag>
struct size_holder<false, SizeType, Tag>
{
   static const bool constant_time_size = false;
   typedef SizeType  size_type;

   HBOOST_INTRUSIVE_FORCEINLINE size_type get_size() const
   {  return 0;  }

   HBOOST_INTRUSIVE_FORCEINLINE void set_size(size_type)
   {}

   HBOOST_INTRUSIVE_FORCEINLINE void decrement()
   {}

   HBOOST_INTRUSIVE_FORCEINLINE void increment()
   {}

   HBOOST_INTRUSIVE_FORCEINLINE void increase(SizeType)
   {}

   HBOOST_INTRUSIVE_FORCEINLINE void decrease(SizeType)
   {}
};

}  //namespace detail{
}  //namespace intrusive{
}  //namespace hboost{

#endif //HBOOST_INTRUSIVE_DETAIL_SIZE_HOLDER_HPP
