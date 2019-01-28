///////////////////////////////////////////////////////////////
//  Copyright 2013 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_

#ifndef HBOOST_MP_CPP_INT_VP_HPP
#define HBOOST_MP_CPP_INT_VP_HPP

namespace hboost{ namespace multiprecision{

namespace literals{ namespace detail{

template <limb_type...VALUES>
struct value_pack
{
   constexpr value_pack(){}

   typedef value_pack<0, VALUES...> next_type;
};
template <class T>
struct is_value_pack{ static constexpr bool value = false; };
template <limb_type...VALUES>
struct is_value_pack<value_pack<VALUES...> >{ static constexpr bool value = true; };

struct negate_tag{};

constexpr negate_tag make_negate_tag()
{
   return negate_tag();
}


}}}} // namespaces

#endif // HBOOST_MP_CPP_INT_CORE_HPP

