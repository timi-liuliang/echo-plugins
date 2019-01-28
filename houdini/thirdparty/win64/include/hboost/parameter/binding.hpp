// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PARAMETER_BINDING_DWA200558_HPP
# define HBOOST_PARAMETER_BINDING_DWA200558_HPP

# include <hboost/mpl/apply.hpp>
# include <hboost/mpl/assert.hpp>
# include <hboost/mpl/and.hpp>
# include <hboost/parameter/aux_/result_of0.hpp>
# include <hboost/parameter/aux_/void.hpp>
# include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace parameter { 

// A metafunction that, given an argument pack, returns the type of
// the parameter identified by the given keyword.  If no such
// parameter has been specified, returns Default

# if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
template <class Parameters, class Keyword, class Default>
struct binding0
{
    typedef typename mpl::apply_wrap3<
        typename Parameters::binding,Keyword,Default,mpl::true_
    >::type type;

    HBOOST_MPL_ASSERT_NOT((
        mpl::and_<
            is_same<Default, void_>
          , is_same<type, void_>
        >
    ));
};
# endif

template <class Parameters, class Keyword, class Default = void_>
struct binding
{
# if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
    typedef typename mpl::eval_if<
        mpl::is_placeholder<Parameters>
      , mpl::identity<int>
      , binding0<Parameters,Keyword,Default>
    >::type type;
# else
    typedef typename mpl::apply_wrap3<
        typename Parameters::binding,Keyword,Default,mpl::true_
    >::type type;

    HBOOST_MPL_ASSERT_NOT((
        mpl::and_<
            is_same<Default, void_>
          , is_same<type, void_>
        >
    ));
# endif

    HBOOST_MPL_AUX_LAMBDA_SUPPORT(3,binding,(Parameters,Keyword,Default))
};

// A metafunction that, given an argument pack, returns the type of
// the parameter identified by the given keyword.  If no such
// parameter has been specified, returns the type returned by invoking
// DefaultFn
template <class Parameters, class Keyword, class DefaultFn>
struct lazy_binding
{
  typedef typename mpl::apply_wrap3<
      typename Parameters::binding
    , Keyword
    , typename aux::result_of0<DefaultFn>::type
    , mpl::true_
  >::type type;
};


}} // namespace hboost::parameter

#endif // HBOOST_PARAMETER_BINDING_DWA200558_HPP
