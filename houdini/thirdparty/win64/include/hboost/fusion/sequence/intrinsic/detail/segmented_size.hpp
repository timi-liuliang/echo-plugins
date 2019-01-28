/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SEGMENTED_SIZE_08112006_1141)
#define HBOOST_FUSION_SEGMENTED_SIZE_08112006_1141

#include <hboost/fusion/support/config.hpp>
#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/mpl/fold.hpp>
#include <hboost/mpl/plus.hpp>
#include <hboost/mpl/size_t.hpp>
#include <hboost/mpl/placeholders.hpp>
#include <hboost/fusion/sequence/intrinsic_fwd.hpp>
#include <hboost/fusion/mpl/begin.hpp>
#include <hboost/fusion/mpl/end.hpp>
#include <hboost/fusion/support/is_segmented.hpp>

namespace hboost { namespace fusion { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////
    // calculates the size of any segmented data structure.
    template<typename Sequence>
    struct segmented_size;

    ///////////////////////////////////////////////////////////////////////////
    template<typename Sequence, bool IsSegmented = traits::is_segmented<Sequence>::value>
    struct segmented_size_impl
      : mpl::fold<
            typename remove_reference<
                typename add_const<
                    typename result_of::segments<Sequence>::type
                >::type
            >::type
          , mpl::size_t<0>
          , mpl::plus<mpl::_1, segmented_size<remove_reference<mpl::_2> > >
        >::type
    {};

    template<typename Sequence>
    struct segmented_size_impl<Sequence, false>
      : result_of::size<Sequence>::type
    {};

    template<typename Sequence>
    struct segmented_size
      : segmented_size_impl<Sequence>
    {};

}}}

#endif
