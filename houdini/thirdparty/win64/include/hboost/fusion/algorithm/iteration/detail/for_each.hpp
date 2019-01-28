/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_FOR_EACH_05052005_1028)
#define HBOOST_FUSION_FOR_EACH_05052005_1028

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/iterator/equal_to.hpp>
#include <hboost/fusion/iterator/next.hpp>
#include <hboost/fusion/iterator/deref.hpp>
#include <hboost/fusion/iterator/distance.hpp>
#include <hboost/fusion/support/category_of.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost { namespace fusion {
namespace detail
{
    template <typename First, typename Last, typename F>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline void
    for_each_linear(First const&, Last const&, F const&, mpl::true_)
    {
    }

    template <typename First, typename Last, typename F>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline void
    for_each_linear(First const& first, Last const& last, F const& f, mpl::false_)
    {
        f(*first);
        detail::for_each_linear(fusion::next(first), last, f, 
                                result_of::equal_to<typename result_of::next<First>::type, Last>());
    }


    template <typename Sequence, typename F, typename Tag>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline void
    for_each_dispatch(Sequence& seq, F const& f, Tag)
    {
        detail::for_each_linear(
                                fusion::begin(seq)
                                , fusion::end(seq)
                                , f
                                , result_of::equal_to<
                                typename result_of::begin<Sequence>::type
                                , typename result_of::end<Sequence>::type>());
    }

    template<int N>
    struct for_each_unrolled
    {
        template<typename I0, typename F>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static void call(I0 const& i0, F const& f)
        {
            f(*i0);
            typedef typename result_of::next<I0>::type I1;
            I1 i1(fusion::next(i0));
            f(*i1);
            typedef typename result_of::next<I1>::type I2;
            I2 i2(fusion::next(i1));
            f(*i2);
            typedef typename result_of::next<I2>::type I3;
            I3 i3(fusion::next(i2));
            f(*i3);
            for_each_unrolled<N-4>::call(fusion::next(i3), f);
        }
    };

    template<>
    struct for_each_unrolled<3>
    {
        template<typename I0, typename F>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static void call(I0 const& i0, F const& f)
        {
            f(*i0);
            typedef typename result_of::next<I0>::type I1;
            I1 i1(fusion::next(i0));
            f(*i1);
            typedef typename result_of::next<I1>::type I2;
            I2 i2(fusion::next(i1));
            f(*i2);
        }
    };

    template<>
    struct for_each_unrolled<2>
    {
        template<typename I0, typename F>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static void call(I0 const& i0, F const& f)
        {
            f(*i0);
            typedef typename result_of::next<I0>::type I1;
            I1 i1(fusion::next(i0));
            f(*i1);
        }
    };

    template<>
    struct for_each_unrolled<1>
    {
        template<typename I0, typename F>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static void call(I0 const& i0, F const& f)
        {
            f(*i0);
        }
    };

    template<>
    struct for_each_unrolled<0>
    {
        template<typename It, typename F>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static void call(It const&, F const&)
        {
        }
    };

    template <typename Sequence, typename F>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline void
    for_each_dispatch(Sequence& seq, F const& f, random_access_traversal_tag)
    {
        typedef typename result_of::begin<Sequence>::type begin;
        typedef typename result_of::end<Sequence>::type end;
        for_each_unrolled<result_of::distance<begin, end>::type::value>::call(fusion::begin(seq), f);
    }

    template <typename Sequence, typename F>
    HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline void
    for_each(Sequence& seq, F const& f, mpl::false_) // unsegmented implementation
    {
        detail::for_each_dispatch(seq, f, typename traits::category_of<Sequence>::type());
    }
}}}


#endif

