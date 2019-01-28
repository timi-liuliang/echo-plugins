// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/map_view_iterator.hpp
/// \brief Iterator adaptors from multi-index to bimap.

#ifndef HBOOST_BIMAP_DETAIL_MAP_VIEW_ITERATOR_HPP
#define HBOOST_BIMAP_DETAIL_MAP_VIEW_ITERATOR_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

// Boost

#ifndef HBOOST_BIMAP_DISABLE_SERIALIZATION
  #include <hboost/serialization/nvp.hpp>
#endif // HBOOST_BIMAP_DISABLE_SERIALIZATION

#include <hboost/iterator/detail/enable_if.hpp>
#include <hboost/iterator/iterator_adaptor.hpp>
#include <hboost/bimap/relation/support/pair_by.hpp>

// check
#include <hboost/bimap/relation/detail/metadata_access_builder.hpp>
#include <hboost/bimap/relation/detail/static_access_builder.hpp>

namespace hboost {
namespace bimaps {
namespace detail {

/** \brief Map View Iterator adaptors from multi index to bimap.

These classes are based on transform iterators from Boost.Iterator.
                                                                         **/

template< class Tag, class BimapCore > struct       map_view_iterator ;
template< class Tag, class BimapCore > struct const_map_view_iterator ;

template< class Tag, class BimapCore > struct       reverse_map_view_iterator ;
template< class Tag, class BimapCore > struct const_reverse_map_view_iterator ;

template< class Tag, class BimapCore > struct       local_map_view_iterator ;
template< class Tag, class BimapCore > struct const_local_map_view_iterator ;


#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#ifndef HBOOST_BIMAP_DISABLE_SERIALIZATION
/*===========================================================================*/
#define HBOOST_BIMAP_MAP_VIEW_ITERATOR_SERIALIZATION_SUPPORT                   \
    HBOOST_SERIALIZATION_SPLIT_MEMBER()                                        \
                                                                              \
    friend class ::hboost::serialization::access;                              \
                                                                              \
    template< class Archive >                                                 \
    void save(Archive & ar, const unsigned int) const                         \
    {                                                                         \
        ar << ::hboost::serialization::make_nvp("mi_iterator",this->base());   \
    }                                                                         \
                                                                              \
    template< class Archive >                                                 \
    void load(Archive & ar, const unsigned int)                               \
    {                                                                         \
        HBOOST_DEDUCED_TYPENAME base_::base_type iter;                         \
        ar >> ::hboost::serialization::make_nvp("mi_iterator",iter);           \
        this->base_reference() = iter;                                        \
    }
/*===========================================================================*/
#else
#define HBOOST_BIMAP_MAP_VIEW_ITERATOR_SERIALIZATION_SUPPORT // None
#endif // HBOOST_BIMAP_DISABLE_SERIALIZATION

/*===========================================================================*/
#define HBOOST_BIMAP_CORE_ITERATOR_TYPE_BY_BUILDER( METANAME, ITERATOR )       \
HBOOST_BIMAP_SYMMETRIC_STATIC_ACCESS_BUILDER( METANAME, BimapCore,             \
    typedef HBOOST_DEDUCED_TYPENAME BimapCore::core_type::HBOOST_NESTED_TEMPLATE\
        index<HBOOST_DEDUCED_TYPENAME BimapCore::left_tag>                     \
            ::type::ITERATOR type,                                            \
    typedef HBOOST_DEDUCED_TYPENAME BimapCore::core_type::HBOOST_NESTED_TEMPLATE\
        index<HBOOST_DEDUCED_TYPENAME BimapCore::right_tag>                    \
            ::type::ITERATOR type                                             \
)
/*===========================================================================*/


HBOOST_BIMAP_CORE_ITERATOR_TYPE_BY_BUILDER( core_iterator_type_by
                                         , iterator )

HBOOST_BIMAP_CORE_ITERATOR_TYPE_BY_BUILDER( reverse_core_iterator_type_by
                                         , reverse_iterator )

HBOOST_BIMAP_CORE_ITERATOR_TYPE_BY_BUILDER( local_core_iterator_type_by
                                         , local_iterator )


// map_view_iterator

template< class Tag, class BimapCore >
struct map_view_iterator_adaptor {
    typedef iterator_adaptor<
        map_view_iterator<Tag,BimapCore>,
        HBOOST_DEDUCED_TYPENAME core_iterator_type_by<Tag,BimapCore>::type,
        HBOOST_DEDUCED_TYPENAME 
            ::hboost::bimaps::support::value_type_by<Tag,BimapCore>::type
    > type;
};
template< class Tag, class BimapCore >
struct map_view_iterator : 
    public map_view_iterator_adaptor<Tag,BimapCore>::type
{
    typedef HBOOST_DEDUCED_TYPENAME 
        map_view_iterator_adaptor<Tag,BimapCore>::type base_;
    public:

    map_view_iterator() {}
    map_view_iterator(HBOOST_DEDUCED_TYPENAME base_::base_type const& iter)
      : base_(iter) {}
    map_view_iterator(map_view_iterator const & iter)
      : base_(iter.base()) {}

    HBOOST_DEDUCED_TYPENAME base_::reference dereference() const
    {
        return ::hboost::bimaps::relation::support::pair_by<Tag>(
            *const_cast<HBOOST_DEDUCED_TYPENAME base_::base_type::value_type*>(
                &(*this->base())
            )
        );
    }
    private:
    friend class iterator_core_access;
    HBOOST_BIMAP_MAP_VIEW_ITERATOR_SERIALIZATION_SUPPORT
};


template< class Tag, class BimapCore >
struct const_map_view_iterator_adaptor {
    typedef iterator_adaptor<
        const_map_view_iterator<Tag,BimapCore>,
        HBOOST_DEDUCED_TYPENAME core_iterator_type_by<Tag,BimapCore>::type,
        const HBOOST_DEDUCED_TYPENAME 
            ::hboost::bimaps::support::value_type_by<Tag,BimapCore>::type
    > type;
};
template< class Tag, class BimapCore >
struct const_map_view_iterator : 
    public const_map_view_iterator_adaptor<Tag,BimapCore>::type
{
    typedef HBOOST_DEDUCED_TYPENAME 
        const_map_view_iterator_adaptor<Tag,BimapCore>::type base_;
    public:

    const_map_view_iterator() {}
    const_map_view_iterator(
            HBOOST_DEDUCED_TYPENAME base_::base_type const& iter)
      : base_(iter) {}
    const_map_view_iterator(const_map_view_iterator const & iter)
      : base_(iter.base()) {}
	const_map_view_iterator(map_view_iterator<Tag,BimapCore> i)
      : base_(i.base()) {}

    HBOOST_DEDUCED_TYPENAME base_::reference dereference() const
    {
        return ::hboost::bimaps::relation::support::pair_by<Tag>(*this->base());
    }
    private:
    friend class iterator_core_access;
    HBOOST_BIMAP_MAP_VIEW_ITERATOR_SERIALIZATION_SUPPORT
};


// reverse_map_view_iterator

template< class Tag, class BimapCore >
struct reverse_map_view_iterator_adaptor {
    typedef iterator_adaptor<
        reverse_map_view_iterator<Tag,BimapCore>,
        HBOOST_DEDUCED_TYPENAME 
            reverse_core_iterator_type_by<Tag,BimapCore>::type,
        HBOOST_DEDUCED_TYPENAME 
            ::hboost::bimaps::support::value_type_by<Tag,BimapCore>::type
    > type;
};
template< class Tag, class BimapCore >
struct reverse_map_view_iterator :
    public reverse_map_view_iterator_adaptor<Tag,BimapCore>::type
{
    typedef HBOOST_DEDUCED_TYPENAME 
        reverse_map_view_iterator_adaptor<Tag,BimapCore>::type base_;
    public:

    reverse_map_view_iterator() {}
    reverse_map_view_iterator(
            HBOOST_DEDUCED_TYPENAME base_::base_type const& iter)
      : base_(iter) {}
    reverse_map_view_iterator(reverse_map_view_iterator const & iter)
      : base_(iter.base()) {}

    HBOOST_DEDUCED_TYPENAME base_::reference dereference() const
    {
        return ::hboost::bimaps::relation::support::pair_by<Tag>(
            *const_cast<HBOOST_DEDUCED_TYPENAME base_::base_type::value_type*>(
                &(*this->base())
            )
        );
    }
    private:
    friend class iterator_core_access;
    HBOOST_BIMAP_MAP_VIEW_ITERATOR_SERIALIZATION_SUPPORT
};

template< class Tag, class BimapCore >
struct const_reverse_map_view_iterator_adaptor {
    typedef iterator_adaptor<
        const_reverse_map_view_iterator<Tag,BimapCore>,
        HBOOST_DEDUCED_TYPENAME 
            reverse_core_iterator_type_by<Tag,BimapCore>::type,
        const HBOOST_DEDUCED_TYPENAME 
            ::hboost::bimaps::support::value_type_by<Tag,BimapCore>::type
    > type;
};
template< class Tag, class BimapCore >
struct const_reverse_map_view_iterator :
    public const_reverse_map_view_iterator_adaptor<Tag,BimapCore>::type
{
    typedef HBOOST_DEDUCED_TYPENAME 
        const_reverse_map_view_iterator_adaptor<Tag,BimapCore>::type base_;
    
    public:

    const_reverse_map_view_iterator() {}
    const_reverse_map_view_iterator(
            HBOOST_DEDUCED_TYPENAME base_::base_type const& iter)
      : base_(iter) {}
    const_reverse_map_view_iterator(const_reverse_map_view_iterator const & iter)
      : base_(iter.base()) {}
	const_reverse_map_view_iterator(reverse_map_view_iterator<Tag,BimapCore> i)
      : base_(i.base()) {}

    HBOOST_DEDUCED_TYPENAME base_::reference dereference() const
    {
        return ::hboost::bimaps::relation::support::pair_by<Tag>(*this->base());
    }
    private:
    friend class iterator_core_access;
    HBOOST_BIMAP_MAP_VIEW_ITERATOR_SERIALIZATION_SUPPORT
};


// local_map_view_iterator

template< class Tag, class BimapCore >
struct local_map_view_iterator_adaptor {
    typedef iterator_adaptor<
        local_map_view_iterator<Tag,BimapCore>,
        HBOOST_DEDUCED_TYPENAME 
            local_core_iterator_type_by<Tag,BimapCore>::type,
        HBOOST_DEDUCED_TYPENAME 
            ::hboost::bimaps::support::value_type_by<Tag,BimapCore>::type
    > type;
};
template< class Tag, class BimapCore >
struct local_map_view_iterator :
    public local_map_view_iterator_adaptor<Tag,BimapCore>::type
{
    typedef HBOOST_DEDUCED_TYPENAME 
        local_map_view_iterator_adaptor<Tag,BimapCore>::type base_;
    public:

    local_map_view_iterator() {}
    local_map_view_iterator(
            HBOOST_DEDUCED_TYPENAME base_::base_type const& iter)
      : base_(iter) {}
    local_map_view_iterator(local_map_view_iterator const & iter)
      : base_(iter.base()) {}

    HBOOST_DEDUCED_TYPENAME base_::reference dereference() const
    {
        return ::hboost::bimaps::relation::support::pair_by<Tag>(
            *const_cast<HBOOST_DEDUCED_TYPENAME base_::base_type::value_type*>(
                &(*this->base())
            )
        );
    }
    private:
    friend class iterator_core_access;
    HBOOST_BIMAP_MAP_VIEW_ITERATOR_SERIALIZATION_SUPPORT
};

template< class Tag, class BimapCore >
struct const_local_map_view_iterator_adaptor {
    typedef iterator_adaptor<
        const_local_map_view_iterator<Tag,BimapCore>,
        HBOOST_DEDUCED_TYPENAME 
            local_core_iterator_type_by<Tag,BimapCore>::type,
        const HBOOST_DEDUCED_TYPENAME 
            ::hboost::bimaps::support::value_type_by<Tag,BimapCore>::type
    > type;
};
template< class Tag, class BimapCore >
struct const_local_map_view_iterator :
    public const_local_map_view_iterator_adaptor<Tag,BimapCore>::type
{
    typedef HBOOST_DEDUCED_TYPENAME 
        const_local_map_view_iterator_adaptor<Tag,BimapCore>::type base_;
    public:

    const_local_map_view_iterator() {}
    const_local_map_view_iterator(
            HBOOST_DEDUCED_TYPENAME base_::base_type const& iter)
      : base_(iter) {}
    const_local_map_view_iterator(const_local_map_view_iterator const & iter)
      : base_(iter.base()) {}
	const_local_map_view_iterator(local_map_view_iterator<Tag,BimapCore> i)
      : base_(i.base()) {}

    HBOOST_DEDUCED_TYPENAME base_::reference dereference() const
    {
        return ::hboost::bimaps::relation::support::pair_by<Tag>(*this->base());
    }
    private:
    friend class iterator_core_access;
    HBOOST_BIMAP_MAP_VIEW_ITERATOR_SERIALIZATION_SUPPORT
};

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

} // namespace detail
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_DETAIL_MAP_VIEW_ITERATOR_HPP


