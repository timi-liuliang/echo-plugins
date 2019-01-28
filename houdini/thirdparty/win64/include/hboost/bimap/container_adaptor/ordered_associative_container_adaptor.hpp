// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/ordered_associative_container_adaptor.hpp
/// \brief Container adaptor to build a type that is compliant to the concept of an ordered associative container.

#ifndef HBOOST_BIMAP_CONTAINER_ADAPTOR_ORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
#define HBOOST_BIMAP_CONTAINER_ADAPTOR_ORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/container_adaptor/associative_container_adaptor.hpp>
#include <hboost/bimap/container_adaptor/detail/comparison_adaptor.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/vector.hpp>
#include <hboost/mpl/push_front.hpp>
#include <hboost/mpl/aux_/na.hpp>
#include <hboost/operators.hpp>
#include <hboost/call_traits.hpp>

namespace hboost {
namespace bimaps {
namespace container_adaptor {

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template
<
    class Base, class Iterator, class ConstIterator,
    class ReverseIterator, class ConstReverseIterator, class KeyType,
    class IteratorToBaseConverter, class IteratorFromBaseConverter,
    class ReverseIteratorFromBaseConverter,
    class ValueToBaseConverter, class ValueFromBaseConverter, 
    class KeyToBaseConverter,
    class FunctorsFromDerivedClasses
>
struct ordered_associative_container_adaptor_base
{
    typedef associative_container_adaptor<
        Base, Iterator, ConstIterator, KeyType,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter, KeyToBaseConverter,

        HBOOST_DEDUCED_TYPENAME mpl::push_front<

            FunctorsFromDerivedClasses,

            HBOOST_DEDUCED_TYPENAME mpl::if_<
                ::hboost::mpl::is_na<ReverseIteratorFromBaseConverter>,
            // {
                    detail::iterator_from_base_identity
                    <
                        HBOOST_DEDUCED_TYPENAME Base::reverse_iterator,
                        ReverseIterator,
                        HBOOST_DEDUCED_TYPENAME Base::const_reverse_iterator,
                        ConstReverseIterator
                    >,
            // }
            // else
            // {
                    ReverseIteratorFromBaseConverter
            // }

            >::type

        >::type

    > type;
};

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief Container adaptor to build a type that is compliant to the concept of an ordered associative container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,
    class ReverseIterator,
    class ConstReverseIterator,

    class KeyType,

    class IteratorToBaseConverter          = ::hboost::mpl::na,
    class IteratorFromBaseConverter        = ::hboost::mpl::na,
    class ReverseIteratorFromBaseConverter = ::hboost::mpl::na,
    class ValueToBaseConverter             = ::hboost::mpl::na,
    class ValueFromBaseConverter           = ::hboost::mpl::na,
    class KeyToBaseConverter               = ::hboost::mpl::na,

    class FunctorsFromDerivedClasses = mpl::vector<>
>
class ordered_associative_container_adaptor :

    public ordered_associative_container_adaptor_base
    <
        Base, Iterator, ConstIterator,
        ReverseIterator, ConstReverseIterator, KeyType,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter, KeyToBaseConverter,
        FunctorsFromDerivedClasses

    >::type,

    ::hboost::totally_ordered
    <
        ordered_associative_container_adaptor
        <
            Base, Iterator, ConstIterator,
            ReverseIterator, ConstReverseIterator,
            KeyType, IteratorToBaseConverter, IteratorFromBaseConverter,
            ReverseIteratorFromBaseConverter,
            ValueToBaseConverter, ValueFromBaseConverter,
            KeyToBaseConverter, FunctorsFromDerivedClasses
        >
    >
{
    // MetaData -------------------------------------------------------------

    typedef HBOOST_DEDUCED_TYPENAME ordered_associative_container_adaptor_base
    <
        Base, Iterator, ConstIterator,
        ReverseIterator, ConstReverseIterator, KeyType,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter, KeyToBaseConverter,
        FunctorsFromDerivedClasses

    >::type base_;

    public:

    typedef detail::compatible_comparison_adaptor
    <
        HBOOST_DEDUCED_TYPENAME Base::key_compare,
        HBOOST_DEDUCED_TYPENAME base_::key_type,
        HBOOST_DEDUCED_TYPENAME base_::key_to_base

    > key_compare;

    typedef detail::comparison_adaptor
    <
        HBOOST_DEDUCED_TYPENAME Base::value_compare,
        HBOOST_DEDUCED_TYPENAME base_::value_type,
        HBOOST_DEDUCED_TYPENAME base_::value_to_base

    > value_compare;

    typedef ReverseIterator      reverse_iterator;
    typedef ConstReverseIterator const_reverse_iterator;

    protected:

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_<
        ::hboost::mpl::is_na<ReverseIteratorFromBaseConverter>,
        // {
                detail::iterator_from_base_identity
                <
                    HBOOST_DEDUCED_TYPENAME Base::reverse_iterator,
                    reverse_iterator,
                    HBOOST_DEDUCED_TYPENAME Base::const_reverse_iterator,
                    const_reverse_iterator
                >,
        // }
        // else
        // {
                ReverseIteratorFromBaseConverter
        // }

        >::type reverse_iterator_from_base;

    // Access -----------------------------------------------------------------

    public:

    explicit ordered_associative_container_adaptor(Base & c)
        : base_(c) {}

    protected:

    typedef ordered_associative_container_adaptor
                ordered_associative_container_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    reverse_iterator rbegin()
    {
        return this->template functor<
            reverse_iterator_from_base
        >()                            ( this->base().rbegin() );

    }

    reverse_iterator rend()
    {
        return this->template functor<
            reverse_iterator_from_base
        >()                            ( this->base().rend() );
    }

    const_reverse_iterator rbegin() const
    {
        return this->template functor<
            reverse_iterator_from_base
        >()                            ( this->base().rbegin() );
    }

    const_reverse_iterator rend() const
    {
        return this->template functor<
            reverse_iterator_from_base
        >()                            ( this->base().rend() );
    }

    key_compare key_comp() const
    {
        typedef HBOOST_DEDUCED_TYPENAME base_::key_to_base key_to_base_;

        return key_compare(
            this->base().key_comp(),
            this->template functor<key_to_base_>()
        );
    }

    value_compare value_comp() const
    {
        typedef HBOOST_DEDUCED_TYPENAME base_::value_to_base value_to_base_;

        return value_compare(
            this->base().value_comp(),
            this->template functor<value_to_base_>()
        );
    }

    template< class CompatibleKey >
    HBOOST_DEDUCED_TYPENAME base_::iterator lower_bound(const CompatibleKey & k)
    {
       return this->template functor<
            HBOOST_DEDUCED_TYPENAME base_::iterator_from_base>()(
                this->base().lower_bound(
                    this->template functor<HBOOST_DEDUCED_TYPENAME base_::key_to_base>()(k)
                )
            );
    }

    template< class CompatibleKey >
    HBOOST_DEDUCED_TYPENAME base_::const_iterator lower_bound(const CompatibleKey & k) const
    {
       return this->template functor<
            HBOOST_DEDUCED_TYPENAME base_::iterator_from_base>()(
                this->base().lower_bound(
                    this->template functor<HBOOST_DEDUCED_TYPENAME base_::key_to_base>()(k)
                )
            );
    }

    template< class CompatibleKey >
    HBOOST_DEDUCED_TYPENAME base_::iterator upper_bound(const CompatibleKey & k)
    {
       return this->template functor<
            HBOOST_DEDUCED_TYPENAME base_::iterator_from_base>()(
                this->base().upper_bound(
                    this->template functor<HBOOST_DEDUCED_TYPENAME base_::key_to_base>()(k)
                )
            );
    }

    template< class CompatibleKey >
    HBOOST_DEDUCED_TYPENAME base_::const_iterator upper_bound(const CompatibleKey & k) const
    {
        return this->template functor<
            HBOOST_DEDUCED_TYPENAME base_::iterator_from_base>()(
                this->base().upper_bound(
                    this->template functor<HBOOST_DEDUCED_TYPENAME base_::key_to_base>()(k)
                )
            );
    }

    // Totally ordered implementation

    bool operator==(const ordered_associative_container_adaptor & c) const
    {
        return ( this->base() == c.base() );
    }

    bool operator<(const ordered_associative_container_adaptor & c) const
    {
        return ( this->base() < c.base() );
    }
};


} // namespace container_adaptor
} // namespace bimaps
} // namespace hboost


#endif // HBOOST_BIMAP_CONTAINER_ADAPTOR_ORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
