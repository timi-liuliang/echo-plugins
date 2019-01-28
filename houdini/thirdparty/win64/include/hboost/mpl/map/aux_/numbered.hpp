
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

#if !defined(HBOOST_PP_IS_ITERATING)

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

#else

#include <hboost/mpl/aux_/config/typeof.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>
#include <hboost/preprocessor/enum_params.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/cat.hpp>

#define i_ HBOOST_PP_FRAME_ITERATION(1)

#   define AUX778076_MAP_TAIL(map, i_, P) \
    HBOOST_PP_CAT(map,i_)< \
          HBOOST_PP_ENUM_PARAMS(i_, P) \
        > \
    /**/


#if defined(HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

template<
      HBOOST_PP_ENUM_PARAMS(i_, typename P)
    >
struct HBOOST_PP_CAT(map,i_)
    : m_item<
          typename HBOOST_PP_CAT(P,HBOOST_PP_DEC(i_))::first
        , typename HBOOST_PP_CAT(P,HBOOST_PP_DEC(i_))::second
        , AUX778076_MAP_TAIL(map,HBOOST_PP_DEC(i_),P)
        >
{
    typedef HBOOST_PP_CAT(map,i_) type;
};

#else // "brute force" implementation

#   if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< typename Map>
struct m_at<Map,HBOOST_PP_DEC(i_)>
{
    typedef typename Map::HBOOST_PP_CAT(item,HBOOST_PP_DEC(i_)) type;
};

template< typename Key, typename T, typename Base >
struct m_item<i_,Key,T,Base>
    : m_item_<Key,T,Base>
{
    typedef pair<Key,T> HBOOST_PP_CAT(item,HBOOST_PP_DEC(i_));
};

#   else

template<>
struct m_at_impl<HBOOST_PP_DEC(i_)>
{
    template< typename Map > struct result_
    {
        typedef typename Map::HBOOST_PP_CAT(item,HBOOST_PP_DEC(i_)) type;
    };
};

template<>
struct m_item_impl<i_>
{
    template< typename Key, typename T, typename Base > struct result_
        : m_item_<Key,T,Base>
    {
        typedef pair<Key,T> HBOOST_PP_CAT(item,HBOOST_PP_DEC(i_));
    };
};

#   endif

template<
      HBOOST_PP_ENUM_PARAMS(i_, typename P)
    >
struct HBOOST_PP_CAT(map,i_)
    : m_item<
          i_
        , typename HBOOST_PP_CAT(P,HBOOST_PP_DEC(i_))::first
        , typename HBOOST_PP_CAT(P,HBOOST_PP_DEC(i_))::second
        , AUX778076_MAP_TAIL(map,HBOOST_PP_DEC(i_),P)
        >
{
    typedef HBOOST_PP_CAT(map,i_) type;
};

#endif // HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

#   undef AUX778076_MAP_TAIL

#undef i_

#endif // HBOOST_PP_IS_ITERATING
