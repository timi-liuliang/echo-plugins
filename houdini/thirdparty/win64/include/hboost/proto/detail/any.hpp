///////////////////////////////////////////////////////////////////////////////
/// \file any.hpp
/// Contains definition the detail::any type
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_DETAIL_ANY_HPP_EAN_18_07_2012
#define HBOOST_PROTO_DETAIL_ANY_HPP_EAN_18_07_2012

#include <hboost/preprocessor/facilities/intercept.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/proto/proto_fwd.hpp>

namespace hboost { namespace proto
{
    namespace detail
    {
        namespace anyns
        {
            ////////////////////////////////////////////////////////////////////////////////////////////
            struct any
            {
                template<typename T> any(T const &) {}
                any operator=(any);
                any operator[](any);
                #define M0(Z, N, DATA) any operator()(HBOOST_PP_ENUM_PARAMS_Z(Z, N, any HBOOST_PP_INTERCEPT));
                HBOOST_PP_REPEAT(HBOOST_PROTO_MAX_ARITY, M0, ~)
                #undef M0

                template<typename T>
                operator T &() const volatile;

                any operator+();
                any operator-();
                any operator*();
                any operator&();
                any operator~();
                any operator!();
                any operator++();
                any operator--();
                any operator++(int);
                any operator--(int);

                friend any operator<<(any, any);
                friend any operator>>(any, any);
                friend any operator*(any, any);
                friend any operator/(any, any);
                friend any operator%(any, any);
                friend any operator+(any, any);
                friend any operator-(any, any);
                friend any operator<(any, any);
                friend any operator>(any, any);
                friend any operator<=(any, any);
                friend any operator>=(any, any);
                friend any operator==(any, any);
                friend any operator!=(any, any);
                friend any operator||(any, any);
                friend any operator&&(any, any);
                friend any operator&(any, any);
                friend any operator|(any, any);
                friend any operator^(any, any);
                friend any operator,(any, any);
                friend any operator->*(any, any);

                friend any operator<<=(any, any);
                friend any operator>>=(any, any);
                friend any operator*=(any, any);
                friend any operator/=(any, any);
                friend any operator%=(any, any);
                friend any operator+=(any, any);
                friend any operator-=(any, any);
                friend any operator&=(any, any);
                friend any operator|=(any, any);
                friend any operator^=(any, any);
            };
        }

        using anyns::any;
    }
}}

#endif
