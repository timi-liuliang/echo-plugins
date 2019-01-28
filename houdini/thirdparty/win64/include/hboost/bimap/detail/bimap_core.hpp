// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/bimap_core.hpp
/// \brief Bimap base definition.

#ifndef HBOOST_BIMAP_DETAIL_BIMAP_CORE_HPP
#define HBOOST_BIMAP_DETAIL_BIMAP_CORE_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/mpl/placeholders.hpp>
#include <hboost/mpl/push_front.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/mpl/vector.hpp>

#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/is_same.hpp>

#include <hboost/concept_check.hpp>

// Boost.MultiIndex
#include <hboost/multi_index_container.hpp>
#include <hboost/multi_index/member.hpp>

// Boost.Bimap
#include <hboost/bimap/relation/mutant_relation.hpp>
#include <hboost/bimap/relation/member_at.hpp>
#include <hboost/bimap/relation/support/data_extractor.hpp>
#include <hboost/bimap/tags/support/default_tagged.hpp>
#include <hboost/bimap/tags/tagged.hpp>
#include <hboost/bimap/detail/manage_bimap_key.hpp>
#include <hboost/bimap/detail/manage_additional_parameters.hpp>
#include <hboost/bimap/detail/map_view_iterator.hpp>
#include <hboost/bimap/detail/set_view_iterator.hpp>

#include <hboost/bimap/set_of.hpp>
#include <hboost/bimap/unconstrained_set_of.hpp>

namespace hboost {
namespace bimaps {

/// \brief Library details

namespace detail {

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class Type >
struct get_value_type
{
    typedef HBOOST_DEDUCED_TYPENAME Type::value_type type;
};

struct independent_index_tag {};

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES


/// \brief Base for the bimap class.
/**


See also bimap.
                                                                        **/


template< class LeftSetType, class RightSetType, class AP1, class AP2, class AP3 >
class bimap_core
{
    // Manage bimap key instantiation
    // --------------------------------------------------------------------
    public:

    typedef HBOOST_DEDUCED_TYPENAME manage_bimap_key
    <
        LeftSetType

    >::type left_set_type;

    typedef HBOOST_DEDUCED_TYPENAME manage_bimap_key
    <
        RightSetType

    >::type right_set_type;


    private:

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::tags::support::default_tagged
    <
        HBOOST_DEDUCED_TYPENAME left_set_type::user_type,
        ::hboost::bimaps::relation::member_at::left

    >::type left_tagged_type;

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::tags::support::default_tagged
    <
        HBOOST_DEDUCED_TYPENAME right_set_type::user_type,
        ::hboost::bimaps::relation::member_at::right

    >::type right_tagged_type;

    public:

    //@{

        typedef HBOOST_DEDUCED_TYPENAME  left_tagged_type::tag  left_tag;
        typedef HBOOST_DEDUCED_TYPENAME right_tagged_type::tag right_tag;

    //@}

    //@{

        typedef HBOOST_DEDUCED_TYPENAME  left_set_type::value_type  left_key_type;
        typedef HBOOST_DEDUCED_TYPENAME right_set_type::value_type right_key_type;

    //@}

    //@{

        typedef right_key_type  left_data_type;
        typedef  left_key_type right_data_type;

    //@}

    // Manage the additional parameters
    // --------------------------------------------------------------------
    private:

    typedef HBOOST_DEDUCED_TYPENAME manage_additional_parameters<AP1,AP2,AP3>::type parameters;

    /// \brief Relation type stored by the bimap.
    // --------------------------------------------------------------------
    public:

    typedef ::hboost::bimaps::relation::mutant_relation
    <

        ::hboost::bimaps::tags::tagged<
            HBOOST_DEDUCED_TYPENAME mpl::if_<
                mpl::and_
                <
                    HBOOST_DEDUCED_TYPENAME left_set_type::mutable_key,
                    HBOOST_DEDUCED_TYPENAME parameters::set_type_of_relation::left_mutable_key
                >,
            // {
                    left_key_type,
            // }
            // else
            // {
                    HBOOST_DEDUCED_TYPENAME ::hboost::add_const< left_key_type >::type
            // }

            >::type,
            left_tag
        >,

        ::hboost::bimaps::tags::tagged<
            HBOOST_DEDUCED_TYPENAME mpl::if_<
                mpl::and_
                <
                    HBOOST_DEDUCED_TYPENAME right_set_type::mutable_key,
                    HBOOST_DEDUCED_TYPENAME parameters::set_type_of_relation::right_mutable_key
                >,
            // {
                    right_key_type,
            // }
            // else
            // {
                    HBOOST_DEDUCED_TYPENAME ::hboost::add_const< right_key_type >::type
            // }

            >::type,
            right_tag
        >,

        // It is ::hboost::mpl::na if no info_hook was included
        HBOOST_DEDUCED_TYPENAME parameters::additional_info,

        // Force mutable keys
        true

    > relation;

    //@{

        typedef HBOOST_DEDUCED_TYPENAME relation::left_pair  left_value_type;
        typedef HBOOST_DEDUCED_TYPENAME relation::right_pair right_value_type;

    //@}

    // Bind the member of the relation, so multi_index can manage them
    // --------------------------------------------------------------------
    private:

    typedef HBOOST_DEDUCED_TYPENAME relation::storage_base relation_storage_base;

    typedef HBOOST_MULTI_INDEX_MEMBER(relation_storage_base, left_key_type, left)
        left_member_extractor;

    typedef HBOOST_MULTI_INDEX_MEMBER(relation_storage_base,right_key_type,right)
        right_member_extractor;

    // The core indices are somewhat complicated to calculate, because they
    // can be zero, one, two or three indices, depending on the use of
    // {side}_based set type of relations and unconstrained_set_of and
    // unconstrained_set_of_relation specifications.

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<
        ::hboost::bimaps::detail::is_unconstrained_set_of< left_set_type >,
    // {
            mpl::vector<>,
    // }
    // else
    // {
            mpl::vector
            <
                HBOOST_DEDUCED_TYPENAME left_set_type::
                HBOOST_NESTED_TEMPLATE index_bind
                <
                    left_member_extractor,
                    left_tag

                >::type
            >
    // }
    >::type left_core_indices;

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<
        ::hboost::bimaps::detail::is_unconstrained_set_of< right_set_type >,
    // {
            left_core_indices,
    // }
    // else
    // {
            HBOOST_DEDUCED_TYPENAME mpl::push_front
            <
                left_core_indices,

                HBOOST_DEDUCED_TYPENAME right_set_type::
                HBOOST_NESTED_TEMPLATE index_bind
                <
                    right_member_extractor,
                    right_tag

                >::type

            >::type
    // }
    >::type basic_core_indices;

    // If it is based either on the left or on the right, then only the side
    // indices are needed. But the set type of the relation can be completely
    // diferent from the one used for the sides in wich case we have to add yet
    // another index to the core.

    // TODO
    // If all the set types are unsconstrained there must be readable compile
    // time error.

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<

        is_same< HBOOST_DEDUCED_TYPENAME parameters::set_type_of_relation, left_based >,
    // {
            ::hboost::bimaps::tags::tagged< left_set_type, left_tag >,
    // }
    /* else */ HBOOST_DEDUCED_TYPENAME mpl::if_<
            is_same< HBOOST_DEDUCED_TYPENAME parameters::set_type_of_relation, right_based >,
    // {
            ::hboost::bimaps::tags::tagged< right_set_type, right_tag >,
    // }
    // else
    // {
            tags::tagged
            <
                HBOOST_DEDUCED_TYPENAME parameters::
                set_type_of_relation::HBOOST_NESTED_TEMPLATE bind_to
                <
                    relation

                >::type,
                independent_index_tag
            >
    // }
    >::type
    >::type tagged_set_of_relation_type;

    protected:

    typedef HBOOST_DEDUCED_TYPENAME tagged_set_of_relation_type::tag
                        relation_set_tag;

    typedef HBOOST_DEDUCED_TYPENAME tagged_set_of_relation_type::value_type
                        relation_set_type_of;

    // Logic tags
    // This is a necesary extra level of indirection to allow unconstrained
    // sets to be plug in the design. The bimap constructors use this logic
    // tags.

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<
        ::hboost::bimaps::detail::is_unconstrained_set_of< left_set_type >,

        HBOOST_DEDUCED_TYPENAME mpl::if_<
            ::hboost::bimaps::detail::is_unconstrained_set_of< right_set_type >,

            independent_index_tag,
            right_tag

        >::type,

        left_tag

    >::type logic_left_tag;

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<
        ::hboost::bimaps::detail::is_unconstrained_set_of< right_set_type >,

        HBOOST_DEDUCED_TYPENAME mpl::if_< 
            ::hboost::bimaps::detail::is_unconstrained_set_of< left_set_type >,

            independent_index_tag,
            left_tag

        >::type,

        right_tag

    >::type logic_right_tag;

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_< 
        is_same< relation_set_tag, independent_index_tag >,

        HBOOST_DEDUCED_TYPENAME mpl::if_< 
            ::hboost::bimaps::detail::
                is_unconstrained_set_of< relation_set_type_of >,

            logic_left_tag,
            independent_index_tag

        >::type,

        HBOOST_DEDUCED_TYPENAME mpl::if_<
            is_same< HBOOST_DEDUCED_TYPENAME parameters::set_type_of_relation, left_based >,

            logic_left_tag,
            logic_right_tag

        >::type

    >::type logic_relation_set_tag;

    private:

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<
        mpl::and_< is_same< relation_set_tag, independent_index_tag >,
                   mpl::not_<
                       ::hboost::bimaps::detail::
                            is_unconstrained_set_of< relation_set_type_of > 
                   >
        >,
    // {
            HBOOST_DEDUCED_TYPENAME mpl::push_front
            <
                basic_core_indices,

                HBOOST_DEDUCED_TYPENAME relation_set_type_of::
                HBOOST_NESTED_TEMPLATE index_bind
                <
                    ::hboost::bimaps::relation::support::both_keys_extractor<relation>,
                    independent_index_tag

                >::type

            >::type,
    // }
    // else
    // {
            basic_core_indices
    // }

    >::type complete_core_indices;

    struct core_indices : public complete_core_indices {};

    // Define the core using compute_index_type to translate the
    // set type to an multi-index specification
    // --------------------------------------------------------------------
    public:

    typedef multi_index::multi_index_container
    <
        relation,
        core_indices,
        HBOOST_DEDUCED_TYPENAME parameters::allocator::
            HBOOST_NESTED_TEMPLATE rebind<relation>::other

    > core_type;

    // Core metadata
    // --------------------------------------------------------------------
    public:

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::multi_index::
        index<core_type, logic_left_tag>::type  left_index;

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::multi_index::
        index<core_type,logic_right_tag>::type right_index;

    typedef HBOOST_DEDUCED_TYPENAME  left_index::iterator        left_core_iterator;
    typedef HBOOST_DEDUCED_TYPENAME  left_index::const_iterator  left_core_const_iterator;

    typedef HBOOST_DEDUCED_TYPENAME right_index::iterator       right_core_iterator;
    typedef HBOOST_DEDUCED_TYPENAME right_index::const_iterator right_core_const_iterator;

    // Relation set view

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::multi_index::index
    <
        core_type, logic_relation_set_tag

    >::type relation_set_core_index;

    typedef HBOOST_DEDUCED_TYPENAME relation_set_type_of::
    HBOOST_NESTED_TEMPLATE set_view_bind
    <
        relation_set_core_index

    >::type relation_set;

    public:

    typedef bimap_core bimap_core_;
};

// Two auxiliar metafunctions to compute the map view types
// The map view type can not be computed inside the bimap core because a 
// they need the bimap core to be parsed first.

template< class BimapBaseType >
struct left_map_view_type
{
    typedef HBOOST_DEDUCED_TYPENAME BimapBaseType::left_set_type left_set_type;
    typedef HBOOST_DEDUCED_TYPENAME
        left_set_type::HBOOST_NESTED_TEMPLATE map_view_bind<
            HBOOST_DEDUCED_TYPENAME BimapBaseType::left_tag, BimapBaseType
        >::type type;
};

template< class BimapBaseType >
struct right_map_view_type
{
    typedef HBOOST_DEDUCED_TYPENAME BimapBaseType::right_set_type right_set_type;
    typedef HBOOST_DEDUCED_TYPENAME
        right_set_type::HBOOST_NESTED_TEMPLATE map_view_bind<
            HBOOST_DEDUCED_TYPENAME BimapBaseType::right_tag, BimapBaseType
        >::type type;
};


} // namespace detail
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_DETAIL_BIMAP_CORE_HPP
