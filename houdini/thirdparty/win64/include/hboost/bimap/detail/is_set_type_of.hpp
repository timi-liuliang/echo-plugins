// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/is_set_type_of.hpp
/// \brief Is set type of and is set type of relation metafunctions.

#ifndef HBOOST_BIMAP_DETAIL_IS_SET_TYPE_OF_HPP
#define HBOOST_BIMAP_DETAIL_IS_SET_TYPE_OF_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/type_traits/is_base_of.hpp>
#include <hboost/bimap/detail/concept_tags.hpp>

/** \struct hboost::bimaps::detail::is_set_type_of

\brief Type trait to check if a class is a set_type_of specification

\code
template< class Type >
struct is_set_type_of : {true_|false_} {};
\endcode
                                                                        **/

/** \struct hboost::bimaps::detail::is_set_type_of_relation

\brief Type trait to check if a class is a set_type_of_relation specification

\code
template< class Type >
struct is_set_type_of_relation : {true_|false_} {};
\endcode

                                                                               **/

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace hboost {
namespace bimaps {
namespace detail {

template< class Type >
struct is_set_type_of :
    is_base_of< set_type_of_tag, Type > {};

template< class Type >
struct is_set_type_of_relation :
    is_base_of< set_type_of_relation_tag, Type > {};

} // namespace detail
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // HBOOST_BIMAP_DETAIL_IS_SET_TYPE_OF_HPP

