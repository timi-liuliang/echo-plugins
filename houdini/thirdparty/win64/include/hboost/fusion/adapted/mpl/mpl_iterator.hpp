/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_MPL_ITERATOR_05052005_0731)
#define HBOOST_FUSION_MPL_ITERATOR_05052005_0731

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/detail/mpl_iterator_category.hpp>
#include <hboost/fusion/iterator/iterator_facade.hpp>
#include <hboost/type_traits/remove_const.hpp>
#include <hboost/mpl/deref.hpp>
#include <hboost/mpl/next.hpp>
#include <hboost/mpl/prior.hpp>
#include <hboost/mpl/advance.hpp>
#include <hboost/mpl/distance.hpp>

namespace hboost { namespace fusion
{
    template <typename Iterator_>
    struct mpl_iterator
        : iterator_facade<
            mpl_iterator<Iterator_>
          , typename detail::mpl_iterator_category<typename Iterator_::category>::type
        >
    {
        typedef typename remove_const<Iterator_>::type iterator_type;

        template <typename Iterator>
        struct value_of : mpl::deref<typename Iterator::iterator_type> {};

        template <typename Iterator>
        struct deref
        {
            typedef typename mpl::deref<
                typename Iterator::iterator_type>::type
            type;

            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            static type
            call(Iterator)
            {
                return type();
            }
        };

        template <typename Iterator>
        struct next
        {
            typedef mpl_iterator<
                typename mpl::next<typename Iterator::iterator_type>::type> 
            type;

            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            static type
            call(Iterator)
            {
                return type();
            }
        };

        template <typename Iterator>
        struct prior
        {
            typedef mpl_iterator<
                typename mpl::prior<typename Iterator::iterator_type>::type> 
            type;

            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            static type
            call(Iterator)
            {
                return type();
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef mpl_iterator<
                typename mpl::advance<typename Iterator::iterator_type, N>::type>
            type;

            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            static type
            call(Iterator const& /*i*/)
            {
                return type();
            }
        };

        template <typename I1, typename I2>
        struct distance : 
            mpl::distance<
                typename I1::iterator_type
              , typename I2::iterator_type>
        {
            typedef typename 
                mpl::distance<
                    typename I1::iterator_type
                  , typename I2::iterator_type
                >::type
            type;
            
            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
            static type
            call(I1 const&, I2 const&)
            {
                return type();
            }
        };
    };
}}

#ifdef HBOOST_FUSION_WORKAROUND_FOR_LWG_2408
namespace std
{
    template <typename Iterator>
    struct iterator_traits< ::hboost::fusion::mpl_iterator<Iterator> >
    { };
}
#endif

#endif


