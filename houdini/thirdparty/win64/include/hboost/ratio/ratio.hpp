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

#ifndef HBOOST_RATIO_RATIO_HPP
#define HBOOST_RATIO_RATIO_HPP

#include <hboost/ratio/config.hpp>
#include <hboost/ratio/detail/mpl/abs.hpp>
#include <hboost/ratio/detail/mpl/sign.hpp>
#include <hboost/ratio/detail/mpl/gcd.hpp>
#include <hboost/ratio/detail/mpl/lcm.hpp>
#include <cstdlib>
#include <climits>
#include <limits>
#include <hboost/cstdint.hpp>
#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/core/enable_if.hpp>
#include <hboost/integer_traits.hpp>
#include <hboost/ratio/ratio_fwd.hpp>
#include <hboost/ratio/detail/overflow_helpers.hpp>
#ifdef HBOOST_RATIO_EXTENSIONS
#include <hboost/rational.hpp>
#include <hboost/ratio/mpl/rational_c_tag.hpp>
#endif

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
//                                                                            //
//                20.6.1 Class template ratio [ratio.ratio]                   //
//                                                                            //
//----------------------------------------------------------------------------//

template <hboost::intmax_t N, hboost::intmax_t D>
class ratio
{
    static const hboost::intmax_t ABS_N = mpl::abs_c<hboost::intmax_t, N>::value;
    static const hboost::intmax_t ABS_D = mpl::abs_c<hboost::intmax_t, D>::value;
    HBOOST_RATIO_STATIC_ASSERT(ABS_N >= 0, HBOOST_RATIO_NUMERATOR_IS_OUT_OF_RANGE, ());
    HBOOST_RATIO_STATIC_ASSERT(ABS_D > 0, HBOOST_RATIO_DENOMINATOR_IS_OUT_OF_RANGE, ());
    HBOOST_RATIO_STATIC_ASSERT(D != 0, HBOOST_RATIO_DIVIDE_BY_0 , ());
    static const hboost::intmax_t SIGN_N = mpl::sign_c<hboost::intmax_t,N>::value
      * mpl::sign_c<hboost::intmax_t,D>::value;
    static const hboost::intmax_t GCD = mpl::gcd_c<hboost::intmax_t, ABS_N, ABS_D>::value;
public:
    HBOOST_STATIC_CONSTEXPR hboost::intmax_t num = SIGN_N * ABS_N / GCD;
    HBOOST_STATIC_CONSTEXPR hboost::intmax_t den = ABS_D / GCD;

#ifdef HBOOST_RATIO_EXTENSIONS
    typedef mpl::rational_c_tag tag;
    typedef hboost::rational<hboost::intmax_t> value_type;
    typedef hboost::intmax_t num_type;
    typedef hboost::intmax_t den_type;
    ratio()
    {}
    template <hboost::intmax_t _N2, hboost::intmax_t _D2>
    ratio(const ratio<_N2, _D2>&,
        typename enable_if_c
            <
                (ratio<_N2, _D2>::num == num &&
                ratio<_N2, _D2>::den == den)
            >::type* = 0)
    {}

    template <hboost::intmax_t _N2, hboost::intmax_t _D2>
        typename enable_if_c
        <
            (ratio<_N2, _D2>::num == num &&
            ratio<_N2, _D2>::den == den),
            ratio&
        >::type
    operator=(const ratio<_N2, _D2>&) {return *this;}

    static value_type value() {return value_type(num,den);}
    value_type operator()() const {return value();}
#endif
    typedef ratio<num, den> type;
};

#if defined(HBOOST_NO_CXX11_CONSTEXPR)
template <hboost::intmax_t N, hboost::intmax_t D>
const    hboost::intmax_t ratio<N, D>::num;
template <hboost::intmax_t N, hboost::intmax_t D>
const    hboost::intmax_t ratio<N, D>::den;
#endif

//----------------------------------------------------------------------------//
//                                                                            //
//                20.6.2 Arithmetic on ratio types [ratio.arithmetic]         //
//                                                                            //
//----------------------------------------------------------------------------//

template <class R1, class R2>
struct ratio_add
: hboost::ratio_detail::ratio_add<R1, R2>::type
{
};

template <class R1, class R2>
struct ratio_subtract
: hboost::ratio_detail::ratio_subtract<R1, R2>::type
{
};

template <class R1, class R2>
struct ratio_multiply
: hboost::ratio_detail::ratio_multiply<R1, R2>::type
{
};

template <class R1, class R2>
struct ratio_divide
: hboost::ratio_detail::ratio_divide<R1, R2>::type
{
};

//----------------------------------------------------------------------------//
//                                                                            //
//                20.6.3 Comparision of ratio types [ratio.comparison]        //
//                                                                            //
//----------------------------------------------------------------------------//

// ratio_equal

template <class R1, class R2>
struct ratio_equal
    : public hboost::integral_constant<bool,
                               (R1::num == R2::num && R1::den == R2::den)>
{};

template <class R1, class R2>
struct ratio_not_equal
    : public hboost::integral_constant<bool, !ratio_equal<R1, R2>::value>
{};

// ratio_less

template <class R1, class R2>
struct ratio_less
    : hboost::integral_constant<bool, hboost::ratio_detail::ratio_less<R1, R2>::value>
{};

template <class R1, class R2>
struct ratio_less_equal
    : hboost::integral_constant<bool, !ratio_less<R2, R1>::value>
{};

template <class R1, class R2>
struct ratio_greater
    : hboost::integral_constant<bool, ratio_less<R2, R1>::value>
{};

template <class R1, class R2>
struct ratio_greater_equal
    : hboost::integral_constant<bool, !ratio_less<R1, R2>::value>
{};

template <class R1, class R2>
struct ratio_gcd :
    ratio<mpl::gcd_c<hboost::intmax_t, R1::num, R2::num>::value,
        mpl::lcm_c<hboost::intmax_t, R1::den, R2::den>::value>::type
{
};

    //----------------------------------------------------------------------------//
    //                                                                            //
    //                More arithmetic on ratio types [ratio.arithmetic]           //
    //                                                                            //
    //----------------------------------------------------------------------------//

#ifdef HBOOST_RATIO_EXTENSIONS
template <class R>
struct ratio_negate
    : ratio<-R::num, R::den>::type
{
};
template <class R>
struct ratio_abs
    : ratio<mpl::abs_c<hboost::intmax_t, R::num>::value, R::den>::type
{
};
template <class R>
struct ratio_sign
    : mpl::sign_c<hboost::intmax_t, R::num>
{
};

template <class R>
struct ratio_inverse
    : ratio<R::den, R::num>::type
{
};


template <class R1, class R2>
struct ratio_lcm :
    ratio<mpl::lcm_c<hboost::intmax_t, R1::num, R2::num>::value,
        mpl::gcd_c<hboost::intmax_t, R1::den, R2::den>::value>::type
{
};

template <class R1, class R2>
struct ratio_modulo :
    ratio<(R1::num * R2::den) % (R2::num * R1::den), R1::den * R2::den>::type
{
};

namespace detail {
  template <class R1, class R2, bool r1ltr2>
  struct ratio_min : R1 {};
  template <class R1, class R2>
  struct ratio_min<R1,R2,false> : R2 {};

  template <class R1, class R2, bool r1ltr2>
  struct ratio_max : R2 {};
  template <class R1, class R2>
  struct ratio_max<R1,R2,false> : R1 {};
}

template <class R1, class R2>
struct ratio_min : detail::ratio_min<R1, R2, ratio_less<R1,R2>::value>::type
{
};

template <class R1, class R2>
struct ratio_max : detail::ratio_max<R1, R2, ratio_less<R1,R2>::value>::type
{
};

template<typename R, int p>
struct ratio_power :
  ratio_multiply<
    typename ratio_power<R, p%2>::type,
    typename ratio_power<typename ratio_multiply<R, R>::type, p/2>::type
  >::type
{};

template<typename R>
struct ratio_power<R, 0> : ratio<1>::type {};

template<typename R>
struct ratio_power<R, 1> : R {};

template<typename R>
struct ratio_power<R, -1> : ratio_divide<ratio<1>, R>::type {};

#endif
}  // namespace hboost


#endif  // HBOOST_RATIO_RATIO_HPP
