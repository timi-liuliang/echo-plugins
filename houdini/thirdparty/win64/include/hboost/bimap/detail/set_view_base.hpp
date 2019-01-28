// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/set_view_base.hpp
/// \brief Helper base for the construction of the bimap views types.

#ifndef HBOOST_BIMAP_DETAIL_SET_VIEW_BASE_HPP
#define HBOOST_BIMAP_DETAIL_SET_VIEW_BASE_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/relation/member_at.hpp>
#include <hboost/bimap/relation/support/data_extractor.hpp>
#include <hboost/bimap/detail/modifier_adaptor.hpp>
#include <hboost/bimap/detail/set_view_iterator.hpp>
#include <hboost/bimap/relation/support/get_pair_functor.hpp>
#include <hboost/bimap/relation/detail/to_mutable_relation_functor.hpp>
#include <hboost/bimap/relation/mutant_relation.hpp>
#include <hboost/bimap/container_adaptor/support/iterator_facade_converters.hpp>

namespace hboost {
namespace bimaps {
namespace detail {

template< class Key, class Value, class KeyToBase >
class set_view_key_to_base
{
    public:
    const Key operator()( const Value & v ) const
    {
        return keyToBase( v );
    }
    private:
    KeyToBase keyToBase;
};

template< class MutantRelationStorage, class KeyToBase >
class set_view_key_to_base<MutantRelationStorage,MutantRelationStorage,KeyToBase>
{
    typedef HBOOST_DEDUCED_TYPENAME MutantRelationStorage::non_mutable_storage non_mutable_storage;
    public:
    const MutantRelationStorage & operator()( const non_mutable_storage & k ) const
    {
        return ::hboost::bimaps::relation::detail::mutate<MutantRelationStorage>(k);
    }
    const MutantRelationStorage & operator()( const MutantRelationStorage & k ) const
    {
        return k;
    }
};


// The next macro can be converted in a metafunctor to gain code robustness.
/*===========================================================================*/
#define HBOOST_BIMAP_SET_VIEW_CONTAINER_ADAPTOR(                               \
    CONTAINER_ADAPTOR, CORE_INDEX, OTHER_ITER, CONST_OTHER_ITER               \
)                                                                             \
::hboost::bimaps::container_adaptor::CONTAINER_ADAPTOR                         \
<                                                                             \
    CORE_INDEX,                                                               \
    ::hboost::bimaps::detail::                                                 \
              set_view_iterator<                                              \
                    HBOOST_DEDUCED_TYPENAME CORE_INDEX::iterator         >,    \
    ::hboost::bimaps::detail::                                                 \
        const_set_view_iterator<                                              \
                    HBOOST_DEDUCED_TYPENAME CORE_INDEX::const_iterator   >,    \
    ::hboost::bimaps::detail::                                                 \
              set_view_iterator<                                              \
                    HBOOST_DEDUCED_TYPENAME CORE_INDEX::OTHER_ITER       >,    \
    ::hboost::bimaps::detail::                                                 \
        const_set_view_iterator<                                              \
                    HBOOST_DEDUCED_TYPENAME CORE_INDEX::CONST_OTHER_ITER >,    \
    ::hboost::bimaps::container_adaptor::support::iterator_facade_to_base      \
    <                                                                         \
        ::hboost::bimaps::detail::      set_view_iterator<                     \
            HBOOST_DEDUCED_TYPENAME CORE_INDEX::iterator>,                     \
        ::hboost::bimaps::detail::const_set_view_iterator<                     \
            HBOOST_DEDUCED_TYPENAME CORE_INDEX::const_iterator>                \
                                                                              \
    >,                                                                        \
    ::hboost::mpl::na,                                                         \
    ::hboost::mpl::na,                                                         \
    ::hboost::bimaps::relation::detail::                                       \
        get_mutable_relation_functor<                                         \
            HBOOST_DEDUCED_TYPENAME CORE_INDEX::value_type >,                  \
    ::hboost::bimaps::relation::support::                                      \
        get_above_view_functor<                                               \
            HBOOST_DEDUCED_TYPENAME CORE_INDEX::value_type >,                  \
    ::hboost::bimaps::detail::set_view_key_to_base<                            \
        HBOOST_DEDUCED_TYPENAME CORE_INDEX::key_type,                          \
        HBOOST_DEDUCED_TYPENAME CORE_INDEX::value_type,                        \
        HBOOST_DEDUCED_TYPENAME CORE_INDEX::key_from_value                     \
    >                                                                         \
>
/*===========================================================================*/


/*===========================================================================*/
#define HBOOST_BIMAP_SEQUENCED_SET_VIEW_CONTAINER_ADAPTOR(                     \
    CONTAINER_ADAPTOR, CORE_INDEX, OTHER_ITER, CONST_OTHER_ITER               \
)                                                                             \
::hboost::bimaps::container_adaptor::CONTAINER_ADAPTOR                         \
<                                                                             \
    CORE_INDEX,                                                               \
    ::hboost::bimaps::detail::                                                 \
              set_view_iterator<                                              \
                    HBOOST_DEDUCED_TYPENAME CORE_INDEX::iterator         >,    \
    ::hboost::bimaps::detail::                                                 \
        const_set_view_iterator<                                              \
                    HBOOST_DEDUCED_TYPENAME CORE_INDEX::const_iterator   >,    \
    ::hboost::bimaps::detail::                                                 \
              set_view_iterator<                                              \
                    HBOOST_DEDUCED_TYPENAME CORE_INDEX::OTHER_ITER       >,    \
    ::hboost::bimaps::detail::                                                 \
        const_set_view_iterator<                                              \
                    HBOOST_DEDUCED_TYPENAME CORE_INDEX::CONST_OTHER_ITER >,    \
    ::hboost::bimaps::container_adaptor::support::iterator_facade_to_base      \
    <                                                                         \
        ::hboost::bimaps::detail::      set_view_iterator<                     \
            HBOOST_DEDUCED_TYPENAME CORE_INDEX::iterator>,                     \
        ::hboost::bimaps::detail::const_set_view_iterator<                     \
            HBOOST_DEDUCED_TYPENAME CORE_INDEX::const_iterator>                \
                                                                              \
    >,                                                                        \
    ::hboost::mpl::na,                                                         \
    ::hboost::mpl::na,                                                         \
    ::hboost::bimaps::relation::detail::                                       \
        get_mutable_relation_functor<                                         \
            HBOOST_DEDUCED_TYPENAME CORE_INDEX::value_type >,                  \
    ::hboost::bimaps::relation::support::                                      \
        get_above_view_functor<                                               \
            HBOOST_DEDUCED_TYPENAME CORE_INDEX::value_type >                   \
>
/*===========================================================================*/


#if defined(HBOOST_MSVC)
/*===========================================================================*/
#define HBOOST_BIMAP_SET_VIEW_BASE_FRIEND(TYPE,INDEX_TYPE)                     \
    typedef ::hboost::bimaps::detail::set_view_base<                           \
        TYPE< INDEX_TYPE >, INDEX_TYPE > template_class_friend;               \
    friend class template_class_friend;
/*===========================================================================*/
#else
/*===========================================================================*/
#define HBOOST_BIMAP_SET_VIEW_BASE_FRIEND(TYPE,INDEX_TYPE)                     \
    friend class ::hboost::bimaps::detail::set_view_base<                      \
        TYPE< INDEX_TYPE >, INDEX_TYPE >;
/*===========================================================================*/
#endif


/// \brief Common base for set views.

template< class Derived, class Index >
class set_view_base
{
    typedef ::hboost::bimaps::container_adaptor::support::
    iterator_facade_to_base
    <
        ::hboost::bimaps::detail::
                  set_view_iterator<HBOOST_DEDUCED_TYPENAME Index::      iterator>,
        ::hboost::bimaps::detail::
            const_set_view_iterator<HBOOST_DEDUCED_TYPENAME Index::const_iterator>

    > iterator_to_base_;

    typedef HBOOST_DEDUCED_TYPENAME Index::value_type::left_value_type          left_type_;

    typedef HBOOST_DEDUCED_TYPENAME Index::value_type::right_value_type        right_type_;

    typedef HBOOST_DEDUCED_TYPENAME Index::value_type                          value_type_;

    typedef ::hboost::bimaps::detail::
                    set_view_iterator<HBOOST_DEDUCED_TYPENAME Index::iterator>   iterator_;

    public:

    bool replace(iterator_ position,
                 const value_type_ & x)
    {
        return derived().base().replace(
            derived().template functor<iterator_to_base_>()(position),x
        );
    }

    template< class CompatibleLeftType >
    bool replace_left(iterator_ position,
                      const CompatibleLeftType & l)
    {
        return derived().base().replace(
            derived().template functor<iterator_to_base_>()(position),
            ::hboost::bimaps::relation::detail::copy_with_left_replaced(*position,l)
        );
    }

    template< class CompatibleRightType >
    bool replace_right(iterator_ position,
                       const CompatibleRightType & r)
    {
        return derived().base().replace(
            derived().template functor<iterator_to_base_>()(position),
            ::hboost::bimaps::relation::detail::copy_with_right_replaced(*position,r)
        );
    }

    /* This function may be provided in the future

    template< class Modifier >
    bool modify(iterator_ position,
                Modifier mod)
    {
        return derived().base().modify(

            derived().template functor<iterator_to_base_>()(position),

            ::hboost::bimaps::detail::relation_modifier_adaptor
            <
                Modifier,
                HBOOST_DEDUCED_TYPENAME Index::value_type,
                HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
                data_extractor
                <
                    ::hboost::bimaps::relation::member_at::left,
                    HBOOST_DEDUCED_TYPENAME Index::value_type

                >::type,
                HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
                data_extractor
                <
                    ::hboost::bimaps::relation::member_at::right,
                    HBOOST_DEDUCED_TYPENAME Index::value_type

                >::type

            >(mod)
        );
    }
    */
    /*
    template< class Modifier >
    bool modify_left(iterator_ position, Modifier mod)
    {
        typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
        data_extractor
        <
            HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::member_at::right,
            HBOOST_DEDUCED_TYPENAME Index::value_type

        >::type left_data_extractor_;

        return derived().base().modify(

            derived().template functor<iterator_to_base_>()(position),

            // this may be replaced later by
            // ::hboost::bind( mod, ::hboost::bind(data_extractor_(),_1) )

            ::hboost::bimaps::detail::unary_modifier_adaptor
            <
                Modifier,
                HBOOST_DEDUCED_TYPENAME Index::value_type,
                left_data_extractor_

            >(mod)
        );
    }

    template< class Modifier >
    bool modify_right(iterator_ position, Modifier mod)
    {
        typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
        data_extractor
        <
            HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::member_at::right,
            HBOOST_DEDUCED_TYPENAME Index::value_type

        >::type right_data_extractor_;

        return derived().base().modify(

            derived().template functor<iterator_to_base_>()(position),

            // this may be replaced later by
            // ::hboost::bind( mod, ::hboost::bind(data_extractor_(),_1) )

            ::hboost::bimaps::detail::unary_modifier_adaptor
            <
                Modifier,
                HBOOST_DEDUCED_TYPENAME Index::value_type,
                right_data_extractor_

            >(mod)
        );
    }
    */
    protected:

    typedef set_view_base set_view_base_;

    private:

    // Curiously Recurring Template interface.

    Derived& derived()
    {
        return *static_cast<Derived*>(this);
    }

    Derived const& derived() const
    {
        return *static_cast<Derived const*>(this);
    }
};



} // namespace detail
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_DETAIL_SET_VIEW_BASE_HPP
