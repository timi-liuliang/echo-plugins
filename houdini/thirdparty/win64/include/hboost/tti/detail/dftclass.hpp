
//  (C) Copyright Edward Diener 2012,2013
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_TTI_DETAIL_FTCLASS_HPP)
#define HBOOST_TTI_DETAIL_FTCLASS_HPP

#include <hboost/function_types/parameter_types.hpp>
#include <hboost/mpl/at.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/quote.hpp>

namespace hboost
  {
  namespace tti
    {
    namespace detail
      {
      template<class HBOOST_TTI_DETAIL_TP_F>
      struct class_type :
          hboost::mpl::at
            <
            typename
            hboost::function_types::parameter_types
              <
              HBOOST_TTI_DETAIL_TP_F,
              hboost::mpl::quote1
                <
                hboost::mpl::identity
                >
              >::type,
              hboost::mpl::int_<0>
            >
        {
        };
      }
    }
  }
  
#endif // HBOOST_TTI_DETAIL_FTCLASS_HPP
