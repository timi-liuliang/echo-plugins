
//  (C) Copyright Edward Diener 2011,2012,2013
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_TTI_DETAIL_PTMF_HPP)
#define HBOOST_TTI_DETAIL_PTMF_HPP

#include <hboost/config.hpp>
#include <hboost/mpl/push_front.hpp>
#include <hboost/function_types/member_function_pointer.hpp>

namespace hboost
  {
  namespace tti
    {
    namespace detail
      {
      template
        <
        class HBOOST_TTI_DETAIL_TP_T,
        class HBOOST_TTI_DETAIL_TP_R,
        class HBOOST_TTI_DETAIL_TP_FS,
        class HBOOST_TTI_DETAIL_TP_TAG
        >
      struct ptmf_seq
        {
        typedef typename
        hboost::function_types::member_function_pointer
          <
          typename
          hboost::mpl::push_front
            <
            typename
            hboost::mpl::push_front<HBOOST_TTI_DETAIL_TP_FS,HBOOST_TTI_DETAIL_TP_T>::type,
            HBOOST_TTI_DETAIL_TP_R
            >::type,
          HBOOST_TTI_DETAIL_TP_TAG
          >::type type;
        };
      }
    }
  }
  
#endif // HBOOST_TTI_DETAIL_PTMF_HPP
