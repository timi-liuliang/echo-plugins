// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/map_view_base.hpp
/// \brief Helper base for the construction of the bimap views types.

#ifndef HBOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP
#define HBOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <stdexcept>
#include <utility>

#include <hboost/throw_exception.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/mpl/if.hpp>

#include <hboost/bimap/relation/support/get_pair_functor.hpp>
#include <hboost/bimap/relation/detail/to_mutable_relation_functor.hpp>
#include <hboost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <hboost/bimap/relation/support/data_extractor.hpp>
#include <hboost/bimap/relation/support/opposite_tag.hpp>
#include <hboost/bimap/relation/support/pair_type_by.hpp>
//#include <hboost/bimap/support/iterator_type_by.hpp>
#include <hboost/bimap/support/key_type_by.hpp>
#include <hboost/bimap/support/data_type_by.hpp>
#include <hboost/bimap/support/value_type_by.hpp>
#include <hboost/bimap/detail/modifier_adaptor.hpp>
#include <hboost/bimap/detail/debug/static_error.hpp>
#include <hboost/bimap/detail/map_view_iterator.hpp>

namespace hboost {
namespace bimaps {

namespace detail {


// The next macro can be converted in a metafunctor to gain code robustness.
/*===========================================================================*/
#define HBOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(                               \
    CONTAINER_ADAPTOR, TAG, BIMAP, OTHER_ITER, CONST_OTHER_ITER               \
)                                                                             \
::hboost::bimaps::container_adaptor::CONTAINER_ADAPTOR                         \
<                                                                             \
    HBOOST_DEDUCED_TYPENAME BIMAP::core_type::                                 \
        HBOOST_NESTED_TEMPLATE index<TAG>::type,                               \
    ::hboost::bimaps::detail::      map_view_iterator<TAG,BIMAP>,              \
    ::hboost::bimaps::detail::const_map_view_iterator<TAG,BIMAP>,              \
    ::hboost::bimaps::detail::      OTHER_ITER<TAG,BIMAP>,                     \
    ::hboost::bimaps::detail::CONST_OTHER_ITER<TAG,BIMAP>,                     \
    ::hboost::bimaps::container_adaptor::support::iterator_facade_to_base      \
    <                                                                         \
        ::hboost::bimaps::detail::      map_view_iterator<TAG,BIMAP>,          \
        ::hboost::bimaps::detail::const_map_view_iterator<TAG,BIMAP>           \
    >,                                                                        \
    ::hboost::mpl::na,                                                         \
    ::hboost::mpl::na,                                                         \
    ::hboost::bimaps::relation::detail::                                       \
        pair_to_relation_functor<TAG,HBOOST_DEDUCED_TYPENAME BIMAP::relation>, \
    ::hboost::bimaps::relation::support::                                      \
        get_pair_functor<TAG, HBOOST_DEDUCED_TYPENAME BIMAP::relation >        \
>
/*===========================================================================*/


#if defined(HBOOST_MSVC)
/*===========================================================================*/
#define HBOOST_BIMAP_MAP_VIEW_BASE_FRIEND(TYPE,TAG,BIMAP)                      \
    typedef ::hboost::bimaps::detail::map_view_base<                           \
        TYPE<TAG,BIMAP>,TAG,BIMAP > friend_map_view_base;                     \
    friend class friend_map_view_base;
/*===========================================================================*/
#else
/*===========================================================================*/
#define HBOOST_BIMAP_MAP_VIEW_BASE_FRIEND(TYPE,TAG,BIMAP)                      \
    friend class ::hboost::bimaps::detail::map_view_base<                      \
        TYPE<TAG,BIMAP>,TAG,BIMAP >;
/*===========================================================================*/
#endif


/// \brief Common base for map views.

template< class Derived, class Tag, class BimapType>
class map_view_base
{
    typedef ::hboost::bimaps::container_adaptor::support::
        iterator_facade_to_base<
            ::hboost::bimaps::detail::      map_view_iterator<Tag,BimapType>,
            ::hboost::bimaps::detail::const_map_view_iterator<Tag,BimapType>
        > iterator_to_base_;

    typedef ::hboost::bimaps::relation::detail::
        pair_to_relation_functor<Tag,
            HBOOST_DEDUCED_TYPENAME BimapType::relation>      value_to_base_;

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::support::
                           key_type_by<Tag,BimapType>::type       key_type_;

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::support::
                          data_type_by<Tag,BimapType>::type      data_type_;

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
           pair_type_by<Tag,
              HBOOST_DEDUCED_TYPENAME BimapType::relation>::type value_type_;

    typedef 
        ::hboost::bimaps::detail::map_view_iterator<Tag,BimapType> iterator_;

    public:

    bool replace(iterator_ position, const value_type_ & x)
    {
        return derived().base().replace(
            derived().template functor<iterator_to_base_>()(position),
            derived().template functor<value_to_base_>()(x)
        );
    }

    template< class CompatibleKey >
    bool replace_key(iterator_ position, const CompatibleKey & k)
    {
        return derived().base().replace(
            derived().template functor<iterator_to_base_>()(position),
            derived().template functor<value_to_base_>()(
                ::hboost::bimaps::relation::detail::
                    copy_with_first_replaced(*position,k)
            )
        );
    }

    template< class CompatibleData >
    bool replace_data(iterator_ position, const CompatibleData & d)
    {
        return derived().base().replace(
            derived().template functor<iterator_to_base_>()(position),
            derived().template functor<value_to_base_>()(
                ::hboost::bimaps::relation::detail::
                    copy_with_second_replaced(*position,d)
            )
        );
    }

    /* This function may be provided in the future

    template< class Modifier >
    bool modify(iterator_ position, Modifier mod)
    {
        return derived().base().modify(

            derived().template functor<iterator_to_base_>()(position),

            ::hboost::bimaps::detail::relation_modifier_adaptor
            <
                Modifier,
                HBOOST_DEDUCED_TYPENAME BimapType::relation,
                HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
                data_extractor
                <
                    Tag, HBOOST_DEDUCED_TYPENAME BimapType::relation

                >::type,
                HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
                data_extractor
                <
                    HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
                        opossite_tag<Tag,BimapType>::type,
                    HBOOST_DEDUCED_TYPENAME BimapType::relation

                >::type

            >(mod)
        );
    }
    */

    template< class Modifier >
    bool modify_key(iterator_ position, Modifier mod)
    {
        return derived().base().modify_key(
            derived().template functor<iterator_to_base_>()(position), mod
        );
    }

    template< class Modifier >
    bool modify_data(iterator_ position, Modifier mod)
    {
        typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
        data_extractor
        <
            HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
                        opossite_tag<Tag,BimapType>::type,
            HBOOST_DEDUCED_TYPENAME BimapType::relation

        >::type data_extractor_;

        return derived().base().modify(

            derived().template functor<iterator_to_base_>()(position),

            // this may be replaced later by
            // ::hboost::bind( mod, ::hboost::bind(data_extractor_(),_1) )

            ::hboost::bimaps::detail::unary_modifier_adaptor
            <
                Modifier,
                HBOOST_DEDUCED_TYPENAME BimapType::relation,
                data_extractor_

            >(mod)
        );
    }

    protected:

    typedef map_view_base map_view_base_;

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




template< class Derived, class Tag, class BimapType>
class mutable_data_unique_map_view_access
{
    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::support::
                          data_type_by<Tag,BimapType>::type      data_type_;

    public:

    template< class CompatibleKey >
    data_type_ & at(const CompatibleKey& k)
    {
        typedef ::hboost::bimaps::detail::
            map_view_iterator<Tag,BimapType> iterator;

        iterator iter = derived().find(k);
        if( iter == derived().end() )
        {
            ::hboost::throw_exception(
                std::out_of_range("bimap<>: invalid key")
            );
        }
        return iter->second;
    }

    template< class CompatibleKey >
    const data_type_ & at(const CompatibleKey& k) const
    {
        typedef ::hboost::bimaps::detail::
                const_map_view_iterator<Tag,BimapType> const_iterator;

        const_iterator iter = derived().find(k);
        if( iter == derived().end() )
        {
            ::hboost::throw_exception(
                std::out_of_range("bimap<>: invalid key")
            );
        }
        return iter->second;
    }

    template< class CompatibleKey >
    data_type_ & operator[](const CompatibleKey& k)
    {
        typedef ::hboost::bimaps::detail::
                      map_view_iterator<Tag,BimapType>          iterator;

        typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::support::
                         value_type_by<Tag,BimapType>::type     value_type;

        iterator iter = derived().find(k);
        if( iter == derived().end() )
        {
            iter = derived().insert( value_type(k,data_type_()) ).first;
        }
        return iter->second;
    }

    protected:

    typedef mutable_data_unique_map_view_access
                mutable_data_unique_map_view_access_;

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


template< class Derived, class Tag, class BimapType>
class non_mutable_data_unique_map_view_access
{
    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::support::
                          data_type_by<Tag,BimapType>::type      data_type_;

    public:

    template< class CompatibleKey >
    const data_type_ & at(const CompatibleKey& k) const
    {
        typedef ::hboost::bimaps::detail::
                const_map_view_iterator<Tag,BimapType> const_iterator;

        const_iterator iter = derived().find(k);
        if( iter == derived().end() )
        {
            ::hboost::throw_exception(
                std::out_of_range("bimap<>: invalid key")
            );
        }
        return iter->second;
    }

    template< class CompatibleKey >
    data_type_ & operator[](const CompatibleKey&)
    {
        HBOOST_BIMAP_STATIC_ERROR( OPERATOR_BRACKET_IS_NOT_SUPPORTED, (Derived));
    }

    protected:

    typedef non_mutable_data_unique_map_view_access
                non_mutable_data_unique_map_view_access_;

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


template< class Derived, class Tag, class BimapType>
struct unique_map_view_access
{
    private:
    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::support::
        value_type_by<Tag,BimapType>::type value_type;

    public:
    typedef HBOOST_DEDUCED_TYPENAME ::hboost::mpl::if_
    <
        typename ::hboost::is_const<
            HBOOST_DEDUCED_TYPENAME value_type::second_type >::type,

        non_mutable_data_unique_map_view_access<Derived,Tag,BimapType>,
        mutable_data_unique_map_view_access<Derived,Tag,BimapType>

    >::type type;
};

// Map views specialize the following structs to provide to the bimap class
// the extra side typedefs (i.e. left_local_iterator for unordered_maps, 
// right_range_type for maps)

template< class MapView >
struct  left_map_view_extra_typedefs {};

template< class MapView >
struct right_map_view_extra_typedefs {};

} // namespace detail

// This function is already part of Boost.Lambda.
// They may be moved to Boost.Utility.

template <class T> inline const T&  make_const(const T& t) { return t; }

} // namespace bimaps
} // namespace hboost


// The following macros avoids code duplication in map views
// Maybe this can be changed in the future using a scheme similar to
// the one used with map_view_base.

/*===========================================================================*/
#define HBOOST_BIMAP_MAP_VIEW_RANGE_IMPLEMENTATION(BASE)                       \
                                                                              \
typedef std::pair<                                                            \
    HBOOST_DEDUCED_TYPENAME base_::iterator,                                   \
    HBOOST_DEDUCED_TYPENAME base_::iterator> range_type;                       \
                                                                              \
typedef std::pair<                                                            \
    HBOOST_DEDUCED_TYPENAME base_::const_iterator,                             \
    HBOOST_DEDUCED_TYPENAME base_::const_iterator> const_range_type;           \
                                                                              \
                                                                              \
template< class LowerBounder, class UpperBounder>                             \
range_type range(LowerBounder lower,UpperBounder upper)                       \
{                                                                             \
    std::pair<                                                                \
                                                                              \
        HBOOST_DEDUCED_TYPENAME BASE::base_type::iterator,                     \
        HBOOST_DEDUCED_TYPENAME BASE::base_type::iterator                      \
                                                                              \
    > r( this->base().range(lower,upper) );                                   \
                                                                              \
    return range_type(                                                        \
        this->template functor<                                               \
            HBOOST_DEDUCED_TYPENAME BASE::iterator_from_base                   \
        >()                                         ( r.first ),              \
        this->template functor<                                               \
            HBOOST_DEDUCED_TYPENAME BASE::iterator_from_base                   \
        >()                                         ( r.second )              \
    );                                                                        \
}                                                                             \
                                                                              \
template< class LowerBounder, class UpperBounder>                             \
const_range_type range(LowerBounder lower,UpperBounder upper) const           \
{                                                                             \
    std::pair<                                                                \
                                                                              \
        HBOOST_DEDUCED_TYPENAME BASE::base_type::const_iterator,               \
        HBOOST_DEDUCED_TYPENAME BASE::base_type::const_iterator                \
                                                                              \
    > r( this->base().range(lower,upper) );                                   \
                                                                              \
    return const_range_type(                                                  \
        this->template functor<                                               \
            HBOOST_DEDUCED_TYPENAME BASE::iterator_from_base                   \
        >()                                         ( r.first ),              \
        this->template functor<                                               \
            HBOOST_DEDUCED_TYPENAME BASE::iterator_from_base                   \
        >()                                         ( r.second )              \
    );                                                                        \
}
/*===========================================================================*/


/*===========================================================================*/
#define HBOOST_BIMAP_VIEW_ASSIGN_IMPLEMENTATION(BASE)                          \
                                                                              \
template< class InputIterator >                                               \
void assign(InputIterator first,InputIterator last)                           \
{                                                                             \
    this->clear();                                                            \
    this->insert(this->end(),first,last);                                     \
}                                                                             \
                                                                              \
void assign(HBOOST_DEDUCED_TYPENAME BASE::size_type n,                         \
            const HBOOST_DEDUCED_TYPENAME BASE::value_type& v)                 \
{                                                                             \
    this->clear();                                                            \
    for(HBOOST_DEDUCED_TYPENAME BASE::size_type i = 0 ; i < n ; ++i)           \
    {                                                                         \
        this->push_back(v);                                                   \
    }                                                                         \
}
/*===========================================================================*/


/*===========================================================================*/
#define HBOOST_BIMAP_VIEW_FRONT_BACK_IMPLEMENTATION(BASE)                      \
                                                                              \
HBOOST_DEDUCED_TYPENAME BASE::reference front()                                \
{                                                                             \
    return this->template functor<                                            \
        HBOOST_DEDUCED_TYPENAME base_::value_from_base>()                      \
    (                                                                         \
        const_cast                                                            \
        <                                                                     \
            HBOOST_DEDUCED_TYPENAME BASE::base_type::value_type &              \
                                                                              \
        > ( this->base().front() )                                            \
    );                                                                        \
}                                                                             \
                                                                              \
HBOOST_DEDUCED_TYPENAME BASE::reference back()                                 \
{                                                                             \
    return this->template functor<                                            \
        HBOOST_DEDUCED_TYPENAME base_::value_from_base>()                      \
    (                                                                         \
        const_cast                                                            \
        <                                                                     \
            HBOOST_DEDUCED_TYPENAME BASE::base_type::value_type &              \
                                                                              \
        >( this->base().back() )                                              \
    );                                                                        \
}                                                                             \
                                                                              \
HBOOST_DEDUCED_TYPENAME BASE::const_reference front() const                    \
{                                                                             \
    return this->template functor<                                            \
        HBOOST_DEDUCED_TYPENAME BASE::value_from_base>()                       \
    (                                                                         \
        this->base().front()                                                  \
    );                                                                        \
}                                                                             \
                                                                              \
HBOOST_DEDUCED_TYPENAME BASE::const_reference back() const                     \
{                                                                             \
    return this->template functor<                                            \
        HBOOST_DEDUCED_TYPENAME BASE::value_from_base>()                       \
    (                                                                         \
        this->base().back()                                                   \
    );                                                                        \
}
/*===========================================================================*/


#endif // HBOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP
