// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/support/pair_type_by.hpp
/// \brief pair_type_by<tag,relation> metafunction

#ifndef HBOOST_BIMAP_RELATION_SUPPORT_PAIR_TYPE_BY_HPP
#define HBOOST_BIMAP_RELATION_SUPPORT_PAIR_TYPE_BY_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/relation/detail/metadata_access_builder.hpp>

/** \struct hboost::bimaps::relation::support::pair_type_by

\brief Metafunction to obtain the view type indexed by one of the sides.

\code

template< class Tag, class Relation >
struct pair_type_by
{
        typedef {signature-compatible with std::pair} type;
};

\endcode

See also member_at, pair_by().
\ingroup relation_group
                                                                    **/

namespace hboost {
namespace bimaps {
namespace relation {
namespace support {

// Implementation of pair type by metafunction

HBOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    pair_type_by,
    left_pair,
    right_pair
)

} // namespace support
} // namespace relation
} // namespace bimaps
} // namespace hboost


#endif // HBOOST_BIMAP_RELATION_SUPPORT_PAIR_BY_TYPE_HPP
