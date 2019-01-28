// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PARAMETER_AUX_TAG_DWA2005610_HPP
# define HBOOST_PARAMETER_AUX_TAG_DWA2005610_HPP

# include <hboost/parameter/aux_/unwrap_cv_reference.hpp>
# include <hboost/parameter/aux_/tagged_argument.hpp>

namespace hboost { namespace parameter { namespace aux { 

template <class Keyword, class ActualArg
#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
        , class = typename is_cv_reference_wrapper<ActualArg>::type
#endif 
          >
struct tag
{
    typedef tagged_argument<
        Keyword
      , typename unwrap_cv_reference<ActualArg>::type
    > type;
};

#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
template <class Keyword, class ActualArg>
struct tag<Keyword,ActualArg,mpl::false_>
{
    typedef tagged_argument<
        Keyword
      , ActualArg
    > type;
};
#endif 

}}} // namespace hboost::parameter::aux_

#endif // HBOOST_PARAMETER_AUX_TAG_DWA2005610_HPP
