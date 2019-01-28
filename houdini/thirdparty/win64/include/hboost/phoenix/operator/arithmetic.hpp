/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_OPERATOR_ARITHMETIC_HPP
#define HBOOST_PHOENIX_OPERATOR_ARITHMETIC_HPP

#include <hboost/phoenix/operator/detail/define_operator.hpp>
#include <hboost/phoenix/core/expression.hpp>
#include <hboost/proto/operators.hpp>

namespace hboost { namespace hboostphoenix
{
    HBOOST_PHOENIX_UNARY_OPERATORS(
        (negate)
        (unary_plus)
        (pre_inc)
        (pre_dec)
        (post_inc)
        (post_dec)
    )
    
    HBOOST_PHOENIX_BINARY_OPERATORS(
        (plus_assign)
        (minus_assign)
        (multiplies_assign)
        (divides_assign)
        (modulus_assign)
        (plus)
        (minus)
        (multiplies)
        (divides)
        (modulus)
    )

    using proto::exprns_::operator++;
    using proto::exprns_::operator--;
    using proto::exprns_::operator+=;
    using proto::exprns_::operator-=;
    using proto::exprns_::operator*=;
    using proto::exprns_::operator/=;
    using proto::exprns_::operator%=;
    using proto::exprns_::operator+;
    using proto::exprns_::operator-;
    using proto::exprns_::operator*;
    using proto::exprns_::operator/;
    using proto::exprns_::operator%;
}}

#include <hboost/phoenix/operator/detail/undef_operator.hpp>

#endif
