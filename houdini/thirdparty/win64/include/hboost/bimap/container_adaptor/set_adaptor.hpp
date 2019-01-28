// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/set_adaptor.hpp
/// \brief Container adaptor to easily build a std::set signature compatible container.

#ifndef HBOOST_BIMAP_CONTAINER_ADAPTOR_SET_ADAPTOR_HPP
#define HBOOST_BIMAP_CONTAINER_ADAPTOR_SET_ADAPTOR_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/container_adaptor/ordered_associative_container_adaptor.hpp>
#include <hboost/mpl/vector.hpp>
#include <hboost/mpl/aux_/na.hpp>

namespace hboost {
namespace bimaps {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::set signature compatible container.

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
class set_adaptor :

    public ::hboost::bimaps::container_adaptor::
                ordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        HBOOST_DEDUCED_TYPENAME Iterator::value_type,
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
        HBOOST_DEDUCED_TYPENAME Iterator::value_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // Access -----------------------------------------------------------------

    public:

    explicit set_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef set_adaptor set_adaptor_;

};


} // namespace container_adaptor
} // namespace bimaps
} // namespace hboost


#endif // HBOOST_BIMAP_CONTAINER_ADAPTOR_SET_ADAPTOR_HPP


