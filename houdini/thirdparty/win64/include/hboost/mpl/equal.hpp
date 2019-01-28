
#ifndef HBOOST_MPL_EQUAL_HPP_INCLUDED
#define HBOOST_MPL_EQUAL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/iter_fold_if_impl.hpp>
#include <hboost/mpl/aux_/iter_apply.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/mpl/begin_end.hpp>
#include <hboost/mpl/next.hpp>
#include <hboost/mpl/always.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/lambda.hpp>
#include <hboost/mpl/bind.hpp>
#include <hboost/mpl/apply.hpp>
#include <hboost/mpl/void.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>
#include <hboost/mpl/aux_/msvc_eti_base.hpp>

#include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace mpl {

namespace aux {

template<
      typename Predicate
    , typename LastIterator1
    , typename LastIterator2
    >
struct equal_pred
{
    template<
          typename Iterator2
        , typename Iterator1
        >
    struct apply
    {
        typedef typename and_< 
              not_< is_same<Iterator1,LastIterator1> >
            , not_< is_same<Iterator2,LastIterator2> >
            , aux::iter_apply2<Predicate,Iterator1,Iterator2>
            >::type type;
    };
};

template<
      typename Sequence1
    , typename Sequence2
    , typename Predicate
    >
struct equal_impl
{
    typedef typename begin<Sequence1>::type first1_;
    typedef typename begin<Sequence2>::type first2_;
    typedef typename end<Sequence1>::type last1_;
    typedef typename end<Sequence2>::type last2_;

    typedef aux::iter_fold_if_impl<
          first1_
        , first2_
        , next<>
        , protect< aux::equal_pred<Predicate,last1_,last2_> >
        , void_
        , always<false_>
        > fold_;

    typedef typename fold_::iterator iter1_;
    typedef typename fold_::state iter2_;
    typedef and_<
          is_same<iter1_,last1_>
        , is_same<iter2_,last2_>
        > result_;

    typedef typename result_::type type;
};


} // namespace aux


template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence1)
    , typename HBOOST_MPL_AUX_NA_PARAM(Sequence2)
    , typename Predicate = is_same<_,_>
    >
struct equal
    : aux::msvc_eti_base< 
          typename aux::equal_impl<Sequence1,Sequence2,Predicate>::type
        >::type
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,equal,(Sequence1,Sequence2))
};

HBOOST_MPL_AUX_NA_SPEC(2, equal)

}}

#endif // HBOOST_MPL_EQUAL_HPP_INCLUDED
