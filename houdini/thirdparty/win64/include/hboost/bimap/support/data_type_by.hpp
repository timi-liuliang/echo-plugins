// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file support/data_type_by.hpp
/// \brief Metafunction to access the data types of a bimap

#ifndef HBOOST_BIMAP_SUPPORT_DATA_TYPE_BY_HPP
#define HBOOST_BIMAP_SUPPORT_DATA_TYPE_BY_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/relation/detail/metadata_access_builder.hpp>

/** \struct hboost::bimaps::support::data_type_by

\brief Metafunction to obtain the data type of one of the sides in a bimap

The tag parameter can be either a user defined tag or \c member_at::{side}.
This is the actual data type stored in the bimap.
\code

template< class Tag, class Bimap >
struct data_type_by
{
    typedef typename Bimap::{side}_data_type type;
};

\endcode

The following holds:

\code

HBOOST_STATIC_ASSERT
(
    is_same< data_type_by< member_at::left, bimap<A,B> >::type, A >::value
)

\endcode

See also member_at.
\ingroup bimap_group
                                                                    **/

namespace hboost {
namespace bimaps {
namespace support {

// Implementation of data type of metafunction

HBOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    data_type_by,
    left_data_type,
    right_data_type
)

} // namespace support
} // namespace bimaps
} // namespace hboost


#endif // HBOOST_BIMAP_SUPPORT_DATA_TYPE_BY_HPP

