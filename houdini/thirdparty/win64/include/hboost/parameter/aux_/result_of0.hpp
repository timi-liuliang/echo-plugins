// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PARAMETER_AUX_RESULT_OF0_DWA2005511_HPP
# define HBOOST_PARAMETER_AUX_RESULT_OF0_DWA2005511_HPP

# include <hboost/utility/result_of.hpp>

// A metafunction returning the result of invoking a nullary function
// object of the given type.

#ifndef HBOOST_NO_RESULT_OF

# include <hboost/utility/result_of.hpp>
namespace hboost { namespace parameter { namespace aux { 
template <class F>
struct result_of0 : result_of<F()>
{};

}}} // namespace hboost::parameter::aux_

#else

namespace hboost { namespace parameter { namespace aux { 
template <class F>
struct result_of0
{
    typedef typename F::result_type type;
};

}}} // namespace hboost::parameter::aux_

#endif 


#endif // HBOOST_PARAMETER_AUX_RESULT_OF0_DWA2005511_HPP
