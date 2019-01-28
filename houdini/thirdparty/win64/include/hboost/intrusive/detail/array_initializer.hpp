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

#ifndef HBOOST_INTRUSIVE_DETAIL_ARRAY_INITIALIZER_HPP
#define HBOOST_INTRUSIVE_DETAIL_ARRAY_INITIALIZER_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/config.hpp>
#include <hboost/core/no_exceptions_support.hpp>

namespace hboost {
namespace intrusive {
namespace detail {

//This is not standard, but should work with all compilers
union max_align
{
   char        char_;
   short       short_;
   int         int_;
   long        long_;
   #ifdef HBOOST_HAS_LONG_LONG
   ::hboost::long_long_type  long_long_;
   #endif
   float       float_;
   double      double_;
   long double long_double_;
   void *      void_ptr_;
};

template<class T, std::size_t N>
class array_initializer
{
   public:
   template<class CommonInitializer>
   array_initializer(const CommonInitializer &init)
   {
      char *init_buf = (char*)rawbuf;
      std::size_t i = 0;
      HBOOST_TRY{
         for(; i != N; ++i){
            new(init_buf)T(init);
            init_buf += sizeof(T);
         }
      }
      HBOOST_CATCH(...){
         while(i--){
            init_buf -= sizeof(T);
            ((T*)init_buf)->~T();
         }
         HBOOST_RETHROW;
      }
      HBOOST_CATCH_END
   }

   operator T* ()
   {  return (T*)(rawbuf);  }

   operator const T*() const
   {  return (const T*)(rawbuf);  }

   ~array_initializer()
   {
      char *init_buf = (char*)rawbuf + N*sizeof(T);
      for(std::size_t i = 0; i != N; ++i){
         init_buf -= sizeof(T);
         ((T*)init_buf)->~T();
      }
   }

   private:
   detail::max_align rawbuf[(N*sizeof(T)-1)/sizeof(detail::max_align)+1];
};

}  //namespace detail{
}  //namespace intrusive{
}  //namespace hboost{

#endif //HBOOST_INTRUSIVE_DETAIL_ARRAY_INITIALIZER_HPP
