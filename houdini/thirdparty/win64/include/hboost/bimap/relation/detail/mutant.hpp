// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/detail/mutant.hpp
/// \brief Mutate functions to extract views of mutant classes.

#ifndef HBOOST_BIMAP_RELATION_DETAIL_MUTANT_HPP
#define HBOOST_BIMAP_RELATION_DETAIL_MUTANT_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/detail/debug/static_error.hpp>
#include <hboost/mpl/contains.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/utility/addressof.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/utility/enable_if.hpp>

namespace hboost {
namespace bimaps {
namespace relation {

/// \brief Relation details, mutant idiom and symmetrical metafunctions builders.

namespace detail {

//@{
/// \brief Converts a mutant class to a view with zero overhead.
/**

This function is a safe wrapper around reinterpret_cast. It checks at
compile time that the desired view is supported by the mutant class.
See also mutant, can_mutate_in.
\ingroup mutant_group
                                                                            **/


template< class View, class Type >
HBOOST_DEDUCED_TYPENAME enable_if< mpl::not_< is_const< Type > >,

View&

>::type mutate( Type & m )
{
    HBOOST_MPL_ASSERT((
        ::hboost::mpl::contains<HBOOST_DEDUCED_TYPENAME Type::mutant_views,View>
    ));
    return *reinterpret_cast< View* >(hboost::addressof(m));
}

template< class View, class Type >
HBOOST_DEDUCED_TYPENAME enable_if< is_const< Type >,

const View&

>::type mutate( Type & m )
{
    HBOOST_MPL_ASSERT((
        ::hboost::mpl::contains<HBOOST_DEDUCED_TYPENAME Type::mutant_views,View> 
    ));
    return *reinterpret_cast< const View* >(hboost::addressof(m));
}

//@}

} // namespace detail
} // namespace relation
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_RELATION_DETAIL_MUTANT_HPP

