/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_PHOENIX_OPERATOR_COMPARISON_HPP
#define HBOOST_PHOENIX_OPERATOR_COMPARISON_HPP

#include <hboost/phoenix/operator/detail/define_operator.hpp>
#include <hboost/phoenix/core/expression.hpp>
#include <hboost/proto/operators.hpp>
#include <hboost/proto/fusion.hpp> // Added to solve bug 6268

namespace hboost { namespace hboostphoenix
{
    HBOOST_PHOENIX_BINARY_OPERATORS(
        (equal_to)
        (not_equal_to)
        (less_equal)
        (greater_equal)
        (less)
        (greater)
    )
    using proto::exprns_::operator==;
    using proto::exprns_::operator!=;
    using proto::exprns_::operator<=;
    using proto::exprns_::operator>=;
    using proto::exprns_::operator<;
    using proto::exprns_::operator>;
}}

#include <hboost/phoenix/operator/detail/undef_operator.hpp>

#endif
