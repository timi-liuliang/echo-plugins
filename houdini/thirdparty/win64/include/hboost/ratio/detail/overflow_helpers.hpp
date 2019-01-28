//  ratio.hpp  ---------------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes
//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

/*

This code was derived by Beman Dawes from Howard Hinnant's time2_demo prototype.
Many thanks to Howard for making his code available under the Boost license.
The original code was modified to conform to Boost conventions and to section
20.4 Compile-time rational arithmetic [ratio], of the C++ committee working
paper N2798.
See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2798.pdf.

time2_demo contained this comment:

    Much thanks to Andrei Alexandrescu,
                   Walter Brown,
                   Peter Dimov,
                   Jeff Garland,
                   Terry Golubiewski,
                   Daniel Krugler,
                   Anthony Williams.
*/

// The way overflow is managed for ratio_less is taken from llvm/libcxx/include/ratio

#ifndef HBOOST_RATIO_DETAIL_RATIO_OPERATIONS_HPP
#define HBOOST_RATIO_DETAIL_RATIO_OPERATIONS_HPP

#include <hboost/ratio/config.hpp>
#include <hboost/ratio/detail/mpl/abs.hpp>
#include <hboost/ratio/detail/mpl/sign.hpp>
#include <cstdlib>
#include <climits>
#include <limits>
#include <hboost/cstdint.hpp>
#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/core/enable_if.hpp>
#include <hboost/integer_traits.hpp>

//
// We simply cannot include this header on gcc without getting copious warnings of the kind:
//
// hboost/integer.hpp:77:30: warning: use of C99 long long integer constant
//
// And yet there is no other reasonable implementation, so we declare this a system header
// to suppress these warnings.
//
#if defined(__GNUC__) && (__GNUC__ >= 4)
#pragma GCC system_header
#endif

namespace hboost
{

//----------------------------------------------------------------------------//
//                                 helpers                                    //
//----------------------------------------------------------------------------//

namespace ratio_detail
{

  template <hboost::intmax_t X, hboost::intmax_t Y, hboost::intmax_t = mpl::sign_c<hboost::intmax_t, Y>::value>
  class br_add;

  template <hboost::intmax_t X, hboost::intmax_t Y>
  class br_add<X, Y, 1>
  {
      static const hboost::intmax_t min = hboost::integer_traits<hboost::intmax_t>::const_min;
      static const hboost::intmax_t max = hboost::integer_traits<hboost::intmax_t>::const_max;

      HBOOST_RATIO_STATIC_ASSERT(X <= max - Y , HBOOST_RATIO_OVERFLOW_IN_ADD, ());
  public:
      static const hboost::intmax_t value = X + Y;
  };

  template <hboost::intmax_t X, hboost::intmax_t Y>
  class br_add<X, Y, 0>
  {
  public:
      static const hboost::intmax_t value = X;
  };

  template <hboost::intmax_t X, hboost::intmax_t Y>
  class br_add<X, Y, -1>
  {
      static const hboost::intmax_t min = hboost::integer_traits<hboost::intmax_t>::const_min;
      static const hboost::intmax_t max = hboost::integer_traits<hboost::intmax_t>::const_max;

      HBOOST_RATIO_STATIC_ASSERT(min - Y <= X, HBOOST_RATIO_OVERFLOW_IN_ADD, ());
  public:
      static const hboost::intmax_t value = X + Y;
  };

  template <hboost::intmax_t X, hboost::intmax_t Y, hboost::intmax_t = mpl::sign_c<hboost::intmax_t, Y>::value>
  class br_sub;

  template <hboost::intmax_t X, hboost::intmax_t Y>
  class br_sub<X, Y, 1>
  {
      static const hboost::intmax_t min = hboost::integer_traits<hboost::intmax_t>::const_min;
      static const hboost::intmax_t max = hboost::integer_traits<hboost::intmax_t>::const_max;

      HBOOST_RATIO_STATIC_ASSERT(min + Y <= X, HBOOST_RATIO_OVERFLOW_IN_SUB, ());
  public:
      static const hboost::intmax_t value = X - Y;
  };

  template <hboost::intmax_t X, hboost::intmax_t Y>
  class br_sub<X, Y, 0>
  {
  public:
      static const hboost::intmax_t value = X;
  };

  template <hboost::intmax_t X, hboost::intmax_t Y>
  class br_sub<X, Y, -1>
  {
      static const hboost::intmax_t min = hboost::integer_traits<hboost::intmax_t>::const_min;
      static const hboost::intmax_t max = hboost::integer_traits<hboost::intmax_t>::const_max;

      HBOOST_RATIO_STATIC_ASSERT(X <= max + Y, HBOOST_RATIO_OVERFLOW_IN_SUB, ());
  public:
      static const hboost::intmax_t value = X - Y;
  };

  template <hboost::intmax_t X, hboost::intmax_t Y>
  class br_mul
  {
      static const hboost::intmax_t nan =
          hboost::intmax_t(HBOOST_RATIO_UINTMAX_C(1) << (sizeof(hboost::intmax_t) * CHAR_BIT - 1));
      static const hboost::intmax_t min = hboost::integer_traits<hboost::intmax_t>::const_min;
      static const hboost::intmax_t max = hboost::integer_traits<hboost::intmax_t>::const_max;

      static const hboost::intmax_t a_x = mpl::abs_c<hboost::intmax_t, X>::value;
      static const hboost::intmax_t a_y = mpl::abs_c<hboost::intmax_t, Y>::value;

      HBOOST_RATIO_STATIC_ASSERT(X != nan, HBOOST_RATIO_OVERFLOW_IN_MUL, ());
      HBOOST_RATIO_STATIC_ASSERT(Y != nan, HBOOST_RATIO_OVERFLOW_IN_MUL, ());
      HBOOST_RATIO_STATIC_ASSERT(a_x <= max / a_y, HBOOST_RATIO_OVERFLOW_IN_MUL, ());
  public:
      static const hboost::intmax_t value = X * Y;
  };

  template <hboost::intmax_t Y>
  class br_mul<0, Y>
  {
  public:
      static const hboost::intmax_t value = 0;
  };

  template <hboost::intmax_t X>
  class br_mul<X, 0>
  {
  public:
      static const hboost::intmax_t value = 0;
  };

  template <>
  class br_mul<0, 0>
  {
  public:
      static const hboost::intmax_t value = 0;
  };

  // Not actually used but left here in case needed in future maintenance
  template <hboost::intmax_t X, hboost::intmax_t Y>
  class br_div
  {
      static const hboost::intmax_t nan = hboost::intmax_t(HBOOST_RATIO_UINTMAX_C(1) << (sizeof(hboost::intmax_t) * CHAR_BIT - 1));
      static const hboost::intmax_t min = hboost::integer_traits<hboost::intmax_t>::const_min;
      static const hboost::intmax_t max = hboost::integer_traits<hboost::intmax_t>::const_max;

      HBOOST_RATIO_STATIC_ASSERT(X != nan, HBOOST_RATIO_OVERFLOW_IN_DIV, ());
      HBOOST_RATIO_STATIC_ASSERT(Y != nan, HBOOST_RATIO_OVERFLOW_IN_DIV, ());
      HBOOST_RATIO_STATIC_ASSERT(Y != 0, HBOOST_RATIO_DIVIDE_BY_0, ());
  public:
      static const hboost::intmax_t value = X / Y;
  };

  // ratio arithmetic
  template <class R1, class R2> struct ratio_add;
  template <class R1, class R2> struct ratio_subtract;
  template <class R1, class R2> struct ratio_multiply;
  template <class R1, class R2> struct ratio_divide;

  template <class R1, class R2>
  struct ratio_add
  {
      //The nested typedef type shall be a synonym for ratio<T1, T2>::type where T1 has the value R1::num *
      //R2::den + R2::num * R1::den and T2 has the value R1::den * R2::den.
      // As the preceding doesn't works because of overflow on hboost::intmax_t we need something more elaborated.
  private:
      static const hboost::intmax_t gcd_n1_n2 = mpl::gcd_c<hboost::intmax_t, R1::num, R2::num>::value;
      static const hboost::intmax_t gcd_d1_d2 = mpl::gcd_c<hboost::intmax_t, R1::den, R2::den>::value;
  public:
      // No need to normalize as ratio_multiply is already normalized
      typedef typename ratio_multiply
         <
             ratio<gcd_n1_n2, R1::den / gcd_d1_d2>,
             ratio
             <
                 hboost::ratio_detail::br_add
                 <
                     hboost::ratio_detail::br_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
                     hboost::ratio_detail::br_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
                 >::value,
                 R2::den
             >
         >::type type;
  };
  template <class R, hboost::intmax_t D>
  struct ratio_add<R, ratio<0,D> >
  {
    typedef R type;
  };

  template <class R1, class R2>
  struct ratio_subtract
  {
      //The nested typedef type shall be a synonym for ratio<T1, T2>::type where T1 has the value
      // R1::num *R2::den - R2::num * R1::den and T2 has the value R1::den * R2::den.
      // As the preceding doesn't works because of overflow on hboost::intmax_t we need something more elaborated.
  private:
      static const hboost::intmax_t gcd_n1_n2 = mpl::gcd_c<hboost::intmax_t, R1::num, R2::num>::value;
      static const hboost::intmax_t gcd_d1_d2 = mpl::gcd_c<hboost::intmax_t, R1::den, R2::den>::value;
  public:
      // No need to normalize as ratio_multiply is already normalized
      typedef typename ratio_multiply
         <
             ratio<gcd_n1_n2, R1::den / gcd_d1_d2>,
             ratio
             <
                 hboost::ratio_detail::br_sub
                 <
                     hboost::ratio_detail::br_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
                     hboost::ratio_detail::br_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
                 >::value,
                 R2::den
             >
         >::type type;
  };

  template <class R, hboost::intmax_t D>
  struct ratio_subtract<R, ratio<0,D> >
  {
    typedef R type;
  };

  template <class R1, class R2>
  struct ratio_multiply
  {
      // The nested typedef type  shall be a synonym for ratio<R1::num * R2::den - R2::num * R1::den, R1::den * R2::den>::type.
      // As the preceding doesn't works because of overflow on hboost::intmax_t we need something more elaborated.
  private:
     static const hboost::intmax_t gcd_n1_d2 = mpl::gcd_c<hboost::intmax_t, R1::num, R2::den>::value;
     static const hboost::intmax_t gcd_d1_n2 = mpl::gcd_c<hboost::intmax_t, R1::den, R2::num>::value;
  public:
      typedef typename ratio
         <
             hboost::ratio_detail::br_mul<R1::num / gcd_n1_d2, R2::num / gcd_d1_n2>::value,
             hboost::ratio_detail::br_mul<R2::den / gcd_n1_d2, R1::den / gcd_d1_n2>::value
         >::type type;
  };

  template <class R1, class R2>
  struct ratio_divide
  {
      // The nested typedef type  shall be a synonym for ratio<R1::num * R2::den, R2::num * R1::den>::type.
      // As the preceding doesn't works because of overflow on hboost::intmax_t we need something more elaborated.
  private:
      static const hboost::intmax_t gcd_n1_n2 = mpl::gcd_c<hboost::intmax_t, R1::num, R2::num>::value;
      static const hboost::intmax_t gcd_d1_d2 = mpl::gcd_c<hboost::intmax_t, R1::den, R2::den>::value;
  public:
      typedef typename ratio
         <
             hboost::ratio_detail::br_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
             hboost::ratio_detail::br_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
         >::type type;
  };
  template <class R1, class R2>
  struct is_evenly_divisible_by
  {
  private:
      static const hboost::intmax_t gcd_n1_n2 = mpl::gcd_c<hboost::intmax_t, R1::num, R2::num>::value;
      static const hboost::intmax_t gcd_d1_d2 = mpl::gcd_c<hboost::intmax_t, R1::den, R2::den>::value;
  public:
      typedef integral_constant<bool,
             ((R2::num / gcd_n1_n2 ==1) && (R1::den / gcd_d1_d2)==1)
      > type;
  };

  template <class T>
  struct is_ratio : public hboost::false_type
  {};
  template <hboost::intmax_t N, hboost::intmax_t D>
  struct is_ratio<ratio<N, D> > : public hboost::true_type
  {};

  template <class R1, class R2,
            hboost::intmax_t Q1 = R1::num / R1::den, hboost::intmax_t M1 = R1::num % R1::den,
            hboost::intmax_t Q2 = R2::num / R2::den, hboost::intmax_t M2 = R2::num % R2::den>
  struct ratio_less1
  {
    static const bool value = Q1 < Q2;
  };

  template <class R1, class R2, hboost::intmax_t Q>
  struct ratio_less1<R1, R2, Q, 0, Q, 0>
  {
    static const bool value = false;
  };

  template <class R1, class R2, hboost::intmax_t Q, hboost::intmax_t M2>
  struct ratio_less1<R1, R2, Q, 0, Q, M2>
  {
    static const bool value = true;
  };

  template <class R1, class R2, hboost::intmax_t Q, hboost::intmax_t M1>
  struct ratio_less1<R1, R2, Q, M1, Q, 0>
  {
    static const bool value = false;
  };

  template <class R1, class R2, hboost::intmax_t Q, hboost::intmax_t M1, hboost::intmax_t M2>
  struct ratio_less1<R1, R2, Q, M1, Q, M2>
  {
    static const bool value = ratio_less1<ratio<R2::den, M2>, ratio<R1::den, M1>
                                            >::value;
  };

  template <
      class R1,
      class R2,
      hboost::intmax_t S1 = mpl::sign_c<hboost::intmax_t, R1::num>::value,
    hboost::intmax_t S2 = mpl::sign_c<hboost::intmax_t, R2::num>::value
>
  struct ratio_less
  {
      static const bool value = S1 < S2;
  };

  template <class R1, class R2>
  struct ratio_less<R1, R2, 1LL, 1LL>
  {
      static const bool value = ratio_less1<R1, R2>::value;
  };

  template <class R1, class R2>
  struct ratio_less<R1, R2, -1LL, -1LL>
  {
      static const bool value = ratio_less1<ratio<-R2::num, R2::den>,
                                            ratio<-R1::num, R1::den> >::value;
  };


}  // namespace ratio_detail

}  // namespace hboost

#endif  // HBOOST_RATIO_HPP
