// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/set_view_iterator.hpp
/// \brief Iterator adaptors from multi-index to bimap.

#ifndef HBOOST_BIMAP_DETAIL_SET_VIEW_ITERATOR_HPP
#define HBOOST_BIMAP_DETAIL_SET_VIEW_ITERATOR_HPP

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
#include <hboost/bimap/relation/support/get_pair_functor.hpp>

namespace hboost {
namespace bimaps {
namespace detail {


/** \brief Set View Iterator adaptor from multi index to bimap.

This is class is based on transform iterator from Boost.Iterator that is
modified to allow serialization. It has been specialized for this
library, and EBO optimization was applied to the functor.

                                                                      **/

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class CoreIterator > struct set_view_iterator;

template< class CoreIterator >
struct set_view_iterator_base
{
    typedef iterator_adaptor
    <
        set_view_iterator< CoreIterator >,
        CoreIterator,
        HBOOST_DEDUCED_TYPENAME CoreIterator::value_type::above_view

    > type;
};

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class CoreIterator >
struct set_view_iterator : public set_view_iterator_base<CoreIterator>::type
{
    typedef HBOOST_DEDUCED_TYPENAME set_view_iterator_base<CoreIterator>::type base_;

    public:

    set_view_iterator() {}

    set_view_iterator(CoreIterator const& iter)
      : base_(iter) {}

    set_view_iterator(set_view_iterator const & iter)
      : base_(iter.base()) {}

    typename base_::reference dereference() const
    {
        return const_cast<
            HBOOST_DEDUCED_TYPENAME base_::base_type::value_type*>(
                &(*this->base())
            )->get_view();
    }

    private:

    friend class iterator_core_access;

    #ifndef HBOOST_BIMAP_DISABLE_SERIALIZATION

    // Serialization support

    HBOOST_SERIALIZATION_SPLIT_MEMBER()

    friend class ::hboost::serialization::access;

    template< class Archive >
    void save(Archive & ar, const unsigned int) const
    {
        ar << ::hboost::serialization::make_nvp("mi_iterator",this->base());
    }

    template< class Archive >
    void load(Archive & ar, const unsigned int)
    {
        CoreIterator iter;
        ar >> ::hboost::serialization::make_nvp("mi_iterator",iter);
        this->base_reference() = iter;
    }

    #endif // HBOOST_BIMAP_DISABLE_SERIALIZATION
};

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class CoreIterator > struct const_set_view_iterator;

template< class CoreIterator >
struct const_set_view_iterator_base
{
    typedef iterator_adaptor
    <
        const_set_view_iterator< CoreIterator >,
        CoreIterator,
        const HBOOST_DEDUCED_TYPENAME CoreIterator::value_type::above_view

    > type;
};

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES


/** \brief Const Set View Iterator adaptor from multi index to bimap.

See also set_view_iterator.
                                                                      **/

template< class CoreIterator >
struct const_set_view_iterator : public const_set_view_iterator_base<CoreIterator>::type
{
    typedef HBOOST_DEDUCED_TYPENAME const_set_view_iterator_base<CoreIterator>::type base_;

    public:

    const_set_view_iterator() {}

    const_set_view_iterator(CoreIterator const& iter)
      : base_(iter) {}

    const_set_view_iterator(const_set_view_iterator const & iter)
      : base_(iter.base()) {}

    const_set_view_iterator(set_view_iterator<CoreIterator> i)
      : base_(i.base()) {}

    HBOOST_DEDUCED_TYPENAME base_::reference dereference() const
    {
        return this->base()->get_view();
    }

    private:

    friend class iterator_core_access;

    #ifndef HBOOST_BIMAP_DISABLE_SERIALIZATION

    // Serialization support

    HBOOST_SERIALIZATION_SPLIT_MEMBER()

    friend class ::hboost::serialization::access;

    template< class Archive >
    void save(Archive & ar, const unsigned int) const
    {
        ar << ::hboost::serialization::make_nvp("mi_iterator",this->base());
    }

    template< class Archive >
    void load(Archive & ar, const unsigned int)
    {
        CoreIterator iter;
        ar >> ::hboost::serialization::make_nvp("mi_iterator",iter);
        this->base_reference() = iter;
    }

    #endif // HBOOST_BIMAP_DISABLE_SERIALIZATION
};


} // namespace detail
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_DETAIL_MAP_VIEW_ITERATOR_HPP


