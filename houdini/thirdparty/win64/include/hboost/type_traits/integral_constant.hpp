//  (C) Copyright John Maddock 2015. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP
#define HBOOST_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>

#if (HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1400)) \
   || HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) \
   || HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840)) \
   || HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3202)) \
   || HBOOST_WORKAROUND(HBOOST_INTEL_CXX_VERSION, HBOOST_TESTED_AT(810)) )


namespace hboost{
   namespace mpl
   {
      template <bool B> struct bool_;
      template <class I, I val> struct integral_c;
      struct integral_c_tag;
   }
}

#else

namespace hboost_mpl_{

   template <bool B> struct bool_;
   template <class I, I val> struct integral_c;
   struct integral_c_tag;
}

namespace hboost
{
   namespace mpl
   {
      using ::hboost_mpl_::bool_;
      using ::hboost_mpl_::integral_c;
      using ::hboost_mpl_::integral_c_tag;
   }
}

#endif

namespace hboost{

   template <class T, T val>
   struct integral_constant
   {
      typedef mpl::integral_c_tag tag;
      typedef T value_type;
      typedef integral_constant<T, val> type;
      static const T value = val;
      //
      // This helper function is just to disable type-punning 
      // warnings from GCC:
      //
      template <class U>
      static U& dereference(U* p) { return *p; }

      operator const mpl::integral_c<T, val>& ()const
      {
         static const char data[sizeof(long)] = { 0 };
         return dereference(reinterpret_cast<const mpl::integral_c<T, val>*>(&data));
      }
      HBOOST_CONSTEXPR operator T()const { return val; }
   };

   template <class T, T val>
   T const integral_constant<T, val>::value;
      
   template <bool val>
   struct integral_constant<bool, val>
   {
      typedef mpl::integral_c_tag tag;
      typedef bool value_type;
      typedef integral_constant<bool, val> type;
      static const bool value = val;
      //
      // This helper function is just to disable type-punning 
      // warnings from GCC:
      //
      template <class T>
      static T& dereference(T* p) { return *p; }

      operator const mpl::bool_<val>& ()const
      {
         static const char data = 0;
         return dereference(reinterpret_cast<const mpl::bool_<val>*>(&data));
      }
      HBOOST_CONSTEXPR operator bool()const { return val; }
   };

   template <bool val>
   bool const integral_constant<bool, val>::value;

   typedef integral_constant<bool, true> true_type;
   typedef integral_constant<bool, false> false_type;

}

#endif
