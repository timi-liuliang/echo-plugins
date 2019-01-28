/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_EQUAL_TO_IMPL_20060128_1423)
#define HBOOST_FUSION_EQUAL_TO_IMPL_20060128_1423

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/mpl.hpp>

#include <hboost/mpl/lambda.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/transform_view.hpp>
#include <hboost/mpl/zip_view.hpp>
#include <hboost/mpl/vector.hpp>
#include <hboost/mpl/unpack_args.hpp>
#include <hboost/mpl/find_if.hpp>
#include <hboost/mpl/end.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/equal_to.hpp>

#include <hboost/type_traits/is_same.hpp>

#include <hboost/fusion/iterator/equal_to.hpp>

namespace hboost { namespace fusion {

    struct zip_view_iterator_tag;

    namespace detail
    {
        template<typename It1, typename It2>
        struct zip_iterators_equal
        {
            typedef mpl::zip_view<mpl::vector2<typename It1::iterators, typename It2::iterators> > zipped;
            typedef mpl::transform_view<zipped, mpl::unpack_args<result_of::equal_to<mpl::_,mpl::_> > > transformed;

            typedef typename mpl::find_if<transformed, mpl::equal_to<mpl::_, mpl::false_> >::type found;

            typedef typename is_same<typename mpl::end<transformed>::type, found>::type type;
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct equal_to_impl;

        template<>
        struct equal_to_impl<zip_view_iterator_tag>
        {
            template<typename It1, typename It2>
            struct apply
                : detail::zip_iterators_equal<It1, It2>::type
            {};
        };
    }
}}

#endif
