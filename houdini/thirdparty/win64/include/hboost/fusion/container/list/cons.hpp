/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CONS_07172005_0843)
#define HBOOST_FUSION_CONS_07172005_0843

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/void.hpp>
#include <hboost/fusion/support/detail/enabler.hpp>
#include <hboost/fusion/container/list/cons_fwd.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/iterator/next.hpp>
#include <hboost/fusion/iterator/deref.hpp>
#include <hboost/fusion/container/list/nil.hpp>
#include <hboost/fusion/container/list/cons_iterator.hpp>
#include <hboost/fusion/container/list/detail/begin_impl.hpp>
#include <hboost/fusion/container/list/detail/end_impl.hpp>
#include <hboost/fusion/container/list/detail/at_impl.hpp>
#include <hboost/fusion/container/list/detail/value_at_impl.hpp>
#include <hboost/fusion/container/list/detail/empty_impl.hpp>
#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/type_traits/is_base_of.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/fusion/support/sequence_base.hpp>
#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/not.hpp>

namespace hboost { namespace fusion
{
    struct cons_tag;
    struct forward_traversal_tag;
    struct fusion_sequence_tag;

    template <typename Car, typename Cdr /*= nil_*/>
    struct cons : sequence_base<cons<Car, Cdr> >
    {
        typedef mpl::int_<Cdr::size::value+1> size;
        typedef cons_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef forward_traversal_tag category;
        typedef Car car_type;
        typedef Cdr cdr_type;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        cons()
            : car(), cdr() {}

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        explicit cons(typename detail::call_param<Car>::type in_car)
            : car(in_car), cdr() {}

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        cons(
            typename detail::call_param<Car>::type in_car
          , typename detail::call_param<Cdr>::type in_cdr)
            : car(in_car), cdr(in_cdr) {}
        
        template <typename Car2, typename Cdr2>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        cons(cons<Car2, Cdr2> const& rhs)
            : car(rhs.car), cdr(rhs.cdr) {}

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        cons(cons const& rhs)
            : car(rhs.car), cdr(rhs.cdr) {}

        template <typename Sequence>
        HBOOST_FUSION_GPU_ENABLED
        cons(
            Sequence const& seq
          , typename hboost::enable_if<
                mpl::and_<
                    traits::is_sequence<Sequence>
                  , mpl::not_<is_base_of<cons, Sequence> >
                  , mpl::not_<is_convertible<Sequence, Car> > > // use copy to car instead
              , detail::enabler_
            >::type = detail::enabler
        )
            : car(*fusion::begin(seq))
            , cdr(fusion::next(fusion::begin(seq)), mpl::true_()) {}

        template <typename Iterator>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        cons(Iterator const& iter, mpl::true_ /*this_is_an_iterator*/)
            : car(*iter)
            , cdr(fusion::next(iter), mpl::true_()) {}

        template <typename Car2, typename Cdr2>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        cons& operator=(cons<Car2, Cdr2> const& rhs)
        {
            car = rhs.car;
            cdr = rhs.cdr;
            return *this;
        }

        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        cons& operator=(cons const& rhs)
        {
            car = rhs.car;
            cdr = rhs.cdr;
            return *this;
        }

        template <typename Sequence>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        typename hboost::enable_if<
            mpl::and_<
                traits::is_sequence<Sequence>
              , mpl::not_<is_convertible<Sequence, Car> > >
          , cons&>::type
        operator=(Sequence const& seq)
        {
            typedef typename result_of::begin<Sequence const>::type Iterator;
            Iterator iter = fusion::begin(seq);
            this->assign_from_iter(iter);
            return *this;
        }

        template <typename Iterator>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        void assign_from_iter(Iterator const& iter)
        {
            car = *iter;
            cdr.assign_from_iter(fusion::next(iter));
        }

        car_type car;
        cdr_type cdr;
    };
}}

#endif

