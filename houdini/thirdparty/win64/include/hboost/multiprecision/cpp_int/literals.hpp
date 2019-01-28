///////////////////////////////////////////////////////////////
//  Copyright 2013 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_

#ifndef HBOOST_MP_CPP_INT_LITERALS_HPP
#define HBOOST_MP_CPP_INT_LITERALS_HPP

#include <hboost/multiprecision/cpp_int/cpp_int_config.hpp>

namespace hboost{ namespace multiprecision{

namespace literals{ namespace detail{

template <char> struct hex_value;
template <> struct hex_value<'0'> { static constexpr limb_type value = 0; };
template <> struct hex_value<'1'> { static constexpr limb_type value = 1; };
template <> struct hex_value<'2'> { static constexpr limb_type value = 2; };
template <> struct hex_value<'3'> { static constexpr limb_type value = 3; };
template <> struct hex_value<'4'> { static constexpr limb_type value = 4; };
template <> struct hex_value<'5'> { static constexpr limb_type value = 5; };
template <> struct hex_value<'6'> { static constexpr limb_type value = 6; };
template <> struct hex_value<'7'> { static constexpr limb_type value = 7; };
template <> struct hex_value<'8'> { static constexpr limb_type value = 8; };
template <> struct hex_value<'9'> { static constexpr limb_type value = 9; };
template <> struct hex_value<'a'> { static constexpr limb_type value = 10; };
template <> struct hex_value<'b'> { static constexpr limb_type value = 11; };
template <> struct hex_value<'c'> { static constexpr limb_type value = 12; };
template <> struct hex_value<'d'> { static constexpr limb_type value = 13; };
template <> struct hex_value<'e'> { static constexpr limb_type value = 14; };
template <> struct hex_value<'f'> { static constexpr limb_type value = 15; };
template <> struct hex_value<'A'> { static constexpr limb_type value = 10; };
template <> struct hex_value<'B'> { static constexpr limb_type value = 11; };
template <> struct hex_value<'C'> { static constexpr limb_type value = 12; };
template <> struct hex_value<'D'> { static constexpr limb_type value = 13; };
template <> struct hex_value<'E'> { static constexpr limb_type value = 14; };
template <> struct hex_value<'F'> { static constexpr limb_type value = 15; };

template <class Pack, limb_type value>
struct combine_value_to_pack;
template <limb_type first, limb_type...ARGS, limb_type value>
struct combine_value_to_pack<value_pack<first, ARGS...>, value>
{
   typedef value_pack<first | value, ARGS...> type;
};

template <char NextChar, char...CHARS>
struct pack_values
{
   static constexpr unsigned chars_per_limb = sizeof(limb_type) * CHAR_BIT / 4;
   static constexpr unsigned shift = ((sizeof...(CHARS)) % chars_per_limb) * 4;
   static constexpr limb_type value_to_add = shift ? hex_value<NextChar>::value << shift : hex_value<NextChar>::value;

   typedef typename pack_values<CHARS...>::type recursive_packed_type;
   typedef typename hboost::mpl::if_c<shift == 0,
      typename recursive_packed_type::next_type,
      recursive_packed_type>::type pack_type;
   typedef typename combine_value_to_pack<pack_type, value_to_add>::type type;
};
template <char NextChar>
struct pack_values<NextChar>
{
   static constexpr limb_type value_to_add = hex_value<NextChar>::value;

   typedef value_pack<value_to_add> type;
};

template <class T>
struct strip_leading_zeros_from_pack;
template <limb_type...PACK>
struct strip_leading_zeros_from_pack<value_pack<PACK...> >
{
   typedef value_pack<PACK...> type;
};
template <limb_type...PACK>
struct strip_leading_zeros_from_pack<value_pack<0u, PACK...> >
{
   typedef typename strip_leading_zeros_from_pack<value_pack<PACK...> >::type type;
};

template <limb_type v, class PACK>
struct append_value_to_pack;
template <limb_type v, limb_type...PACK>
struct append_value_to_pack<v, value_pack<PACK...> >
{
   typedef value_pack<PACK..., v> type;
};

template <class T>
struct reverse_value_pack;
template <limb_type v, limb_type...VALUES>
struct reverse_value_pack<value_pack<v, VALUES...> >
{
   typedef typename reverse_value_pack<value_pack<VALUES...> >::type lead_values;
   typedef typename append_value_to_pack<v, lead_values>::type type;
};
template <limb_type v>
struct reverse_value_pack<value_pack<v> >
{
   typedef value_pack<v> type;
};
template <>
struct reverse_value_pack<value_pack<> >
{
   typedef value_pack<> type;
};

template <char l1, char l2, char...STR>
struct make_packed_value_from_str
{
   HBOOST_STATIC_ASSERT_MSG(l1 == '0', "Multi-precision integer literals must be in hexadecimal notation.");
   HBOOST_STATIC_ASSERT_MSG((l2 == 'X') || (l2 == 'x'), "Multi-precision integer literals must be in hexadecimal notation.");
   typedef typename pack_values<STR...>::type packed_type;
   typedef typename strip_leading_zeros_from_pack<packed_type>::type stripped_type;
   typedef typename reverse_value_pack<stripped_type>::type type;
};

template <class Pack, class B>
struct make_backend_from_pack
{
   static constexpr Pack p = {};
   static constexpr B value = p;
};

template <class Pack, class B>
constexpr B make_backend_from_pack<Pack, B>::value;

template <unsigned Digits>
struct signed_cpp_int_literal_result_type
{
   static constexpr unsigned bits = Digits * 4;
   typedef hboost::multiprecision::backends::cpp_int_backend<bits, bits, signed_magnitude, unchecked, void> backend_type;
   typedef number<backend_type, et_off> number_type;
};

template <unsigned Digits>
struct unsigned_cpp_int_literal_result_type
{
   static constexpr unsigned bits = Digits * 4;
   typedef hboost::multiprecision::backends::cpp_int_backend<bits, bits, unsigned_magnitude, unchecked, void> backend_type;
   typedef number<backend_type, et_off> number_type;
};

}

template <char... STR>
constexpr typename hboost::multiprecision::literals::detail::signed_cpp_int_literal_result_type<(sizeof...(STR)) - 2>::number_type operator "" _cppi()
{
   typedef typename hboost::multiprecision::literals::detail::make_packed_value_from_str<STR...>::type pt;
   return hboost::multiprecision::literals::detail::make_backend_from_pack<pt, typename hboost::multiprecision::literals::detail::signed_cpp_int_literal_result_type<(sizeof...(STR)) - 2>::backend_type>::value;
}

template <char... STR>
constexpr typename hboost::multiprecision::literals::detail::unsigned_cpp_int_literal_result_type<(sizeof...(STR)) - 2>::number_type operator "" _cppui()
{
   typedef typename hboost::multiprecision::literals::detail::make_packed_value_from_str<STR...>::type pt;
   return hboost::multiprecision::literals::detail::make_backend_from_pack<pt, typename hboost::multiprecision::literals::detail::unsigned_cpp_int_literal_result_type<(sizeof...(STR)) - 2>::backend_type>::value;
}

#define HBOOST_MP_DEFINE_SIZED_CPP_INT_LITERAL(Bits)\
template <char... STR> \
constexpr hboost::multiprecision::number<hboost::multiprecision::backends::cpp_int_backend<Bits, Bits, hboost::multiprecision::signed_magnitude, hboost::multiprecision::unchecked, void> > operator "" HBOOST_JOIN(_cppi, Bits)()\
{\
   typedef typename hboost::multiprecision::literals::detail::make_packed_value_from_str<STR...>::type pt;\
   return hboost::multiprecision::literals::detail::make_backend_from_pack<\
      pt, \
      hboost::multiprecision::backends::cpp_int_backend<Bits, Bits, hboost::multiprecision::signed_magnitude, hboost::multiprecision::unchecked, void> \
   >::value;\
}\
template <char... STR> \
constexpr hboost::multiprecision::number<hboost::multiprecision::backends::cpp_int_backend<Bits, Bits, hboost::multiprecision::unsigned_magnitude, hboost::multiprecision::unchecked, void> > operator "" HBOOST_JOIN(_cppui, Bits)()\
{\
   typedef typename hboost::multiprecision::literals::detail::make_packed_value_from_str<STR...>::type pt;\
   return hboost::multiprecision::literals::detail::make_backend_from_pack<\
      pt, \
      hboost::multiprecision::backends::cpp_int_backend<Bits, Bits, hboost::multiprecision::unsigned_magnitude, hboost::multiprecision::unchecked, void>\
   >::value;\
}\

HBOOST_MP_DEFINE_SIZED_CPP_INT_LITERAL(128)
HBOOST_MP_DEFINE_SIZED_CPP_INT_LITERAL(256)
HBOOST_MP_DEFINE_SIZED_CPP_INT_LITERAL(512)
HBOOST_MP_DEFINE_SIZED_CPP_INT_LITERAL(1024)

}

//
// Overload unary minus operator for constexpr use:
//
template <unsigned MinBits, cpp_int_check_type Checked>
constexpr number<cpp_int_backend<MinBits, MinBits, signed_magnitude, Checked, void>, et_off>
   operator - (const number<cpp_int_backend<MinBits, MinBits, signed_magnitude, Checked, void>, et_off>& a)
{
   return cpp_int_backend<MinBits, MinBits, signed_magnitude, Checked, void>(a.backend(), hboost::multiprecision::literals::detail::make_negate_tag());
}
template <unsigned MinBits, cpp_int_check_type Checked>
constexpr number<cpp_int_backend<MinBits, MinBits, signed_magnitude, Checked, void>, et_off>
   operator - (number<cpp_int_backend<MinBits, MinBits, signed_magnitude, Checked, void>, et_off>&& a)
{
   return cpp_int_backend<MinBits, MinBits, signed_magnitude, Checked, void>(static_cast<const number<cpp_int_backend<MinBits, MinBits, signed_magnitude, Checked, void>, et_off>&>(a).backend(), hboost::multiprecision::literals::detail::make_negate_tag());
}

}} // namespaces

#endif // HBOOST_MP_CPP_INT_CORE_HPP

