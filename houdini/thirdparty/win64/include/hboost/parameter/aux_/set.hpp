// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PARAMETER_SET_060912_HPP
# define HBOOST_PARAMETER_SET_060912_HPP

# include <hboost/detail/workaround.hpp>

# if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
#  include <hboost/mpl/insert.hpp>
#  include <hboost/mpl/set/set0.hpp>
#  include <hboost/mpl/has_key.hpp>

namespace hboost { namespace parameter { namespace aux {

typedef mpl::set0<> set0;

template <class Set, class K>
struct insert_
{
    typedef typename mpl::insert<Set, K>::type type;
};

template <class Set, class K>
struct has_key_
{
    typedef typename mpl::has_key<Set, K>::type type;
};

}}} // namespace hboost::parameter::aux

# else

#  include <hboost/mpl/list.hpp>
#  include <hboost/mpl/end.hpp>
#  include <hboost/mpl/find.hpp>
#  include <hboost/mpl/not.hpp>
#  include <hboost/mpl/push_front.hpp>

namespace hboost { namespace parameter { namespace aux {

typedef mpl::list0<> set0;

template <class Set, class K>
struct insert_
{
    typedef typename mpl::push_front<Set, K>::type type;
};

template <class Set, class K>
struct has_key_
{
    typedef typename mpl::find<Set, K>::type iter;
    typedef mpl::not_<
        is_same<iter, typename mpl::end<Set>::type> 
    > type;
};

}}} // namespace hboost::parameter::aux

# endif


#endif // HBOOST_PARAMETER_SET_060912_HPP

