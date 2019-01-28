// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/support/data_extractor.hpp
/// \brief Data extraction functor.

#ifndef HBOOST_BIMAP_RELATION_SUPPORT_DATA_EXTRACTOR_HPP
#define HBOOST_BIMAP_RELATION_SUPPORT_DATA_EXTRACTOR_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/relation/detail/metadata_access_builder.hpp>
#include <functional>

/** \struct hboost::bimaps::relation::support::data_extractor

\brief Data extraction functor.

\ingroup relation_group
                                                                    **/

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace hboost {
namespace bimaps {
namespace relation {
namespace support {

template< class Tag, class Relation >
struct data_extractor_implementation;

template< class Relation >
struct data_extractor_implementation< member_at::left, Relation > :
    public std::unary_function<Relation,HBOOST_DEDUCED_TYPENAME Relation::left_value_type>
{
    HBOOST_DEDUCED_TYPENAME Relation::left_value_type const &
        operator()(Relation const & rel) const
    {
        return rel.left;
    }

    HBOOST_DEDUCED_TYPENAME Relation::left_value_type &
        operator()(Relation       & rel) const
    {
        return rel.left;
    }
};

template< class Relation >
struct data_extractor_implementation< member_at::right, Relation > :
    public std::unary_function<Relation,HBOOST_DEDUCED_TYPENAME Relation::right_value_type>
{
    HBOOST_DEDUCED_TYPENAME Relation::right_value_type const & 
        operator()(Relation const & rel) const
    {
        return rel.right;
    }

    HBOOST_DEDUCED_TYPENAME Relation::right_value_type & 
        operator()(Relation       & rel) const
    {
        return rel.right;
    }
};

template< class Tag, class Relation >
struct data_extractor
{
    typedef data_extractor_implementation
    <
        HBOOST_DEDUCED_TYPENAME member_with_tag<Tag,Relation>::type,
        Relation

    > type;
};

template< class Relation >
struct both_keys_extractor
{
    typedef HBOOST_DEDUCED_TYPENAME Relation::storage_base result_type;

     const result_type & operator()(const Relation & rel) const
    {
        return rel;
    }

    result_type & operator()( Relation & rel) const
    {
        return rel;
    }
};

} // namespace support
} // namespace relation
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // HBOOST_BIMAP_RELATION_SUPPORT_DATA_EXTRACTOR_HPP

