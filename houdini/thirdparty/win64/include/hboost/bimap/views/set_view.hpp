// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/set_view.hpp
/// \brief View of a bimap that is signature compatible with std::set.

#ifndef HBOOST_BIMAP_VIEWS_SET_VIEW_HPP
#define HBOOST_BIMAP_VIEWS_SET_VIEW_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/container_adaptor/set_adaptor.hpp>
#include <hboost/bimap/detail/set_view_base.hpp>

namespace hboost {
namespace bimaps {
namespace views {

/// \brief View of a bimap that is signature compatible with std::set.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::set.

See also const_set_view.
                                                                                    **/

template< class CoreIndex >
class set_view
:
    public HBOOST_BIMAP_SET_VIEW_CONTAINER_ADAPTOR(
        set_adaptor,
        CoreIndex,
        reverse_iterator, const_reverse_iterator
    ),

    public ::hboost::bimaps::detail::
                set_view_base< set_view< CoreIndex >, CoreIndex >
{
    typedef HBOOST_BIMAP_SET_VIEW_CONTAINER_ADAPTOR(
        set_adaptor,
        CoreIndex,
        reverse_iterator, const_reverse_iterator

    ) base_;

    HBOOST_BIMAP_SET_VIEW_BASE_FRIEND(set_view,CoreIndex)

    public:

    set_view(HBOOST_DEDUCED_TYPENAME base_::base_type & c) : base_(c) {}

    /*
    template< class LowerBounder, class UpperBounder >
    std::pair<HBOOST_DEDUCED_TYPENAME base_::const_iterator,
              HBOOST_DEDUCED_TYPENAME base_::const_iterator>
        range(LowerBounder lower,UpperBounder upper) const
    {
        return this->base().range(

            ::hboost::bimaps::container_adaptor::detail::unary_check_adaptor
            <
                LowerBounder,
                HBOOST_DEDUCED_TYPENAME base_::base_type::value_type,
                HBOOST_DEDUCED_TYPENAME base_::value_from_base

            >( lower, this->template functor<
                            HBOOST_DEDUCED_TYPENAME base_::value_from_base>() ),

            ::hboost::bimaps::container_adaptor::detail::unary_check_adaptor
            <
                UpperBounder,
                HBOOST_DEDUCED_TYPENAME base_::base_type::value_type,
                HBOOST_DEDUCED_TYPENAME base_::value_from_base

            >( upper, this->template functor<
                            HBOOST_DEDUCED_TYPENAME base_::value_from_base>() )

        );
    }
    */

    set_view & operator=(const set_view & v) 
    {
        this->base() = v.base();
        return *this;
    }
};


} // namespace views
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_VIEWS_SET_VIEW_HPP


