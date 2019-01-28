/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_OPERATOR_BITWISE_HPP
#define HBOOST_PHOENIX_OPERATOR_BITWISE_HPP

#include <hboost/phoenix/operator/detail/define_operator.hpp>
#include <hboost/phoenix/core/expression.hpp>
#include <hboost/proto/operators.hpp>

namespace hboost { namespace hboostphoenix
{
    HBOOST_PHOENIX_UNARY_OPERATORS(
        (complement)
    )
    
    HBOOST_PHOENIX_BINARY_OPERATORS(
        (bitwise_and_assign)
        (bitwise_or_assign)
        (bitwise_xor_assign)
        (shift_left_assign)
        (shift_right_assign)
        (bitwise_and)
        (bitwise_or)
        (bitwise_xor)
        (shift_left)
        (shift_right)
    )

    using proto::exprns_::operator~;
    using proto::exprns_::operator&=;
    using proto::exprns_::operator|=;
    using proto::exprns_::operator^=;
    using proto::exprns_::operator<<=;
    using proto::exprns_::operator>>=;
    using proto::exprns_::operator&;
    using proto::exprns_::operator|;
    using proto::exprns_::operator^;
    using proto::exprns_::operator<<;
    using proto::exprns_::operator>>;
}}

#include <hboost/phoenix/operator/detail/undef_operator.hpp>

#endif
