/*
 * Copyright (c) 2012-2014 Glen Joseph Fernandes 
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License, 
 * Version 1.0. (See accompanying file LICENSE_1_0.txt 
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef HBOOST_SMART_PTR_DETAIL_SP_IF_ARRAY_HPP
#define HBOOST_SMART_PTR_DETAIL_SP_IF_ARRAY_HPP

#include <hboost/smart_ptr/shared_ptr.hpp>

namespace hboost {
    namespace detail {
        template<class T>
        struct sp_if_array;

        template<class T>
        struct sp_if_array<T[]> {
            typedef hboost::shared_ptr<T[]> type;
        };

        template<class T>
        struct sp_if_size_array;

        template<class T, std::size_t N>
        struct sp_if_size_array<T[N]> {
            typedef hboost::shared_ptr<T[N]> type;
        };
    }
}

#endif
