//  Boost integer/integer_mask.hpp header file  ------------------------------//

//  (C) Copyright Daryle Walker 2001.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef HBOOST_INTEGER_INTEGER_MASK_HPP
#define HBOOST_INTEGER_INTEGER_MASK_HPP

#include <hboost/integer_fwd.hpp>  // self include

#include <hboost/config.hpp>   // for HBOOST_STATIC_CONSTANT
#include <hboost/integer.hpp>  // for hboost::uint_t

#include <climits>  // for UCHAR_MAX, etc.
#include <cstddef>  // for std::size_t

#include <hboost/limits.hpp>  // for std::numeric_limits

//
// We simply cannot include this header on gcc without getting copious warnings of the kind:
//
// hboost/integer/integer_mask.hpp:93:35: warning: use of C99 long long integer constant
//
// And yet there is no other reasonable implementation, so we declare this a system header
// to suppress these warnings.
//
#if defined(__GNUC__) && (__GNUC__ >= 4)
#pragma GCC system_header
#endif

namespace hboost
{


//  Specified single-bit mask class declaration  -----------------------------//
//  (Lowest bit starts counting at 0.)

template < std::size_t Bit >
struct high_bit_mask_t
{
    typedef typename uint_t<(Bit + 1)>::least  least;
    typedef typename uint_t<(Bit + 1)>::fast   fast;

    HBOOST_STATIC_CONSTANT( least, high_bit = (least( 1u ) << Bit) );
    HBOOST_STATIC_CONSTANT( fast, high_bit_fast = (fast( 1u ) << Bit) );

    HBOOST_STATIC_CONSTANT( std::size_t, bit_position = Bit );

};  // hboost::high_bit_mask_t


//  Specified bit-block mask class declaration  ------------------------------//
//  Makes masks for the lowest N bits
//  (Specializations are needed when N fills up a type.)

template < std::size_t Bits >
struct low_bits_mask_t
{
    typedef typename uint_t<Bits>::least  least;
    typedef typename uint_t<Bits>::fast   fast;

    HBOOST_STATIC_CONSTANT( least, sig_bits = (~( ~(least( 0u )) << Bits )) );
    HBOOST_STATIC_CONSTANT( fast, sig_bits_fast = fast(sig_bits) );

    HBOOST_STATIC_CONSTANT( std::size_t, bit_count = Bits );

};  // hboost::low_bits_mask_t


#define HBOOST_LOW_BITS_MASK_SPECIALIZE( Type )                                  \
  template <  >  struct low_bits_mask_t< std::numeric_limits<Type>::digits >  { \
      typedef std::numeric_limits<Type>           limits_type;                  \
      typedef uint_t<limits_type::digits>::least  least;                        \
      typedef uint_t<limits_type::digits>::fast   fast;                         \
      HBOOST_STATIC_CONSTANT( least, sig_bits = (~( least(0u) )) );              \
      HBOOST_STATIC_CONSTANT( fast, sig_bits_fast = fast(sig_bits) );            \
      HBOOST_STATIC_CONSTANT( std::size_t, bit_count = limits_type::digits );    \
  }

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4245)  // 'initializing' : conversion from 'int' to 'const hboost::low_bits_mask_t<8>::least', signed/unsigned mismatch
#endif

HBOOST_LOW_BITS_MASK_SPECIALIZE( unsigned char );

#if USHRT_MAX > UCHAR_MAX
HBOOST_LOW_BITS_MASK_SPECIALIZE( unsigned short );
#endif

#if UINT_MAX > USHRT_MAX
HBOOST_LOW_BITS_MASK_SPECIALIZE( unsigned int );
#endif

#if ULONG_MAX > UINT_MAX
HBOOST_LOW_BITS_MASK_SPECIALIZE( unsigned long );
#endif

#if defined(HBOOST_HAS_LONG_LONG)
    #if ((defined(ULLONG_MAX) && (ULLONG_MAX > ULONG_MAX)) ||\
        (defined(ULONG_LONG_MAX) && (ULONG_LONG_MAX > ULONG_MAX)) ||\
        (defined(ULONGLONG_MAX) && (ULONGLONG_MAX > ULONG_MAX)) ||\
        (defined(_ULLONG_MAX) && (_ULLONG_MAX > ULONG_MAX)))
    HBOOST_LOW_BITS_MASK_SPECIALIZE( hboost::ulong_long_type );
    #endif
#elif defined(HBOOST_HAS_MS_INT64)
    #if 18446744073709551615ui64 > ULONG_MAX
    HBOOST_LOW_BITS_MASK_SPECIALIZE( unsigned __int64 );
    #endif
#endif

#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#undef HBOOST_LOW_BITS_MASK_SPECIALIZE


}  // namespace hboost


#endif  // HBOOST_INTEGER_INTEGER_MASK_HPP