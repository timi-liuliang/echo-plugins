// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/map_adaptor.hpp
/// \brief Container adaptor to easily build a std::map signature compatible container.

#ifndef HBOOST_BIMAP_CONTAINER_ADAPTOR_MAP_ADAPTOR_HPP
#define HBOOST_BIMAP_CONTAINER_ADAPTOR_MAP_ADAPTOR_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/container_adaptor/ordered_associative_container_adaptor.hpp>
#include <hboost/mpl/vector.hpp>
#include <hboost/mpl/aux_/na.hpp>
#include <hboost/call_traits.hpp>

namespace hboost {
namespace bimaps {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::map signature compatible container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,
    class ReverseIterator,
    class ConstReverseIterator,

    class IteratorToBaseConverter          = ::hboost::mpl::na,
    class IteratorFromBaseConverter        = ::hboost::mpl::na,
    class ReverseIteratorFromBaseConverter = ::hboost::mpl::na,
    class ValueToBaseConverter             = ::hboost::mpl::na,
    class ValueFromBaseConverter           = ::hboost::mpl::na,
    class KeyToBaseConverter               = ::hboost::mpl::na,

    class FunctorsFromDerivedClasses = mpl::vector<>
>
class map_adaptor :

    public ::hboost::bimaps::container_adaptor::
                ordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        HBOOST_DEDUCED_TYPENAME Iterator::value_type::first_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses
    >
{

    typedef ::hboost::bimaps::container_adaptor::
                ordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        HBOOST_DEDUCED_TYPENAME Iterator::value_type::first_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // MetaData -------------------------------------------------------------

    public:

    typedef HBOOST_DEDUCED_TYPENAME Iterator::value_type::second_type data_type;
    typedef data_type mapped_type;

    // Access -----------------------------------------------------------------

    public:

    explicit map_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef map_adaptor map_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    template< class CompatibleKey >
    data_type& operator[](const CompatibleKey & k)
    {
        return this->base()
            [this->template functor<HBOOST_DEDUCED_TYPENAME base_::key_to_base>()(k)];
    }

    template< class CompatibleKey >
    data_type& at(const CompatibleKey & k)
    {
        return this->base().
            at(this->template functor<HBOOST_DEDUCED_TYPENAME base_::key_to_base>()(k));
    }

    template< class CompatibleKey >
    const data_type& at(const CompatibleKey & k) const
    {
        return this->base().
            at(this->template functor<HBOOST_DEDUCED_TYPENAME base_::key_to_base>()(k));
    }

};


} // namespace container_adaptor
} // namespace bimaps
} // namespace hboost


#endif // HBOOST_BIMAP_CONTAINER_ADAPTOR_MAP_ADAPTOR_HPP

