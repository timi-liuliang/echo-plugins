/*
  A meta function which supports using named template type parameters
  via Boost.Parameter to specify the template type parameters for
  the hboost::signals2::signal class.

  Author: Frank Mori Hess <fmhess@users.sourceforge.net>
  Begin: 2009-01-22
*/
// Copyright Frank Mori Hess 2009
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef HBOOST_SIGNALS2_SIGNAL_TYPE_HPP
#define HBOOST_SIGNALS2_SIGNAL_TYPE_HPP

// support for function types is currently broken in Boost.Parameter
// #define HBOOST_SIGNALS2_NAMED_SIGNATURE_PARAMETER

#include <hboost/signals2/signal.hpp>

#if !defined(HBOOST_PARAMETER_MAX_ARITY)
#define HBOOST_PARAMETER_MAX_ARITY 7
#else
#if HBOOST_PARAMETER_MAX_ARITY < 7
#error This header requires HBOOST_PARAMETER_MAX_ARITY to be defined as 7 or greater prior to including Boost.Parameter headers
#endif // HBOOST_PARAMETER_MAX_ARITY < 7
#endif // !defined(HBOOST_PARAMETER_MAX_ARITY)
#include <hboost/parameter.hpp>

#include <hboost/type_traits/is_function.hpp>

namespace hboost
{
  namespace signals2
  {
    namespace keywords
    {
#ifdef HBOOST_SIGNALS2_NAMED_SIGNATURE_PARAMETER
      HBOOST_PARAMETER_TEMPLATE_KEYWORD(signature_type)
#endif
      HBOOST_PARAMETER_TEMPLATE_KEYWORD(combiner_type)
      HBOOST_PARAMETER_TEMPLATE_KEYWORD(group_type)
      HBOOST_PARAMETER_TEMPLATE_KEYWORD(group_compare_type)
      HBOOST_PARAMETER_TEMPLATE_KEYWORD(slot_function_type)
      HBOOST_PARAMETER_TEMPLATE_KEYWORD(extended_slot_function_type)
      HBOOST_PARAMETER_TEMPLATE_KEYWORD(mutex_type)
    } // namespace keywords

    template <
#ifdef HBOOST_SIGNALS2_NAMED_SIGNATURE_PARAMETER
        typename A0,
#else
        typename Signature,
#endif
        typename A1 = parameter::void_,
        typename A2 = parameter::void_,
        typename A3 = parameter::void_,
        typename A4 = parameter::void_,
        typename A5 = parameter::void_,
        typename A6 = parameter::void_
      >
    class signal_type
    {
      typedef parameter::parameters<
#ifdef HBOOST_SIGNALS2_NAMED_SIGNATURE_PARAMETER
          parameter::required<keywords::tag::signature_type, is_function<hboost::mpl::_> >,
#endif
          parameter::optional<keywords::tag::combiner_type>,
          parameter::optional<keywords::tag::group_type>,
          parameter::optional<keywords::tag::group_compare_type>,
          parameter::optional<keywords::tag::slot_function_type>,
          parameter::optional<keywords::tag::extended_slot_function_type>,
          parameter::optional<keywords::tag::mutex_type>
        > parameter_spec;

    public:
      // ArgumentPack
      typedef typename
        parameter_spec::bind<
#ifdef HBOOST_SIGNALS2_NAMED_SIGNATURE_PARAMETER
        A0,
#endif
        A1, A2, A3, A4, A5, A6>::type
        args;

#ifdef HBOOST_SIGNALS2_NAMED_SIGNATURE_PARAMETER
      typedef typename parameter::value_type<args, keywords::tag::signature_type>::type
        signature_type;
#else
      typedef Signature signature_type;
#endif

      typedef typename parameter::value_type
        <
          args,
          keywords::tag::combiner_type,
          optional_last_value
            <
              typename hboost::function_traits<signature_type>::result_type
            >
        >::type combiner_type;

      typedef typename
        parameter::value_type<args, keywords::tag::group_type, int>::type group_type;

      typedef typename
        parameter::value_type<args, keywords::tag::group_compare_type, std::less<group_type> >::type
        group_compare_type;

      typedef typename
        parameter::value_type<args, keywords::tag::slot_function_type, function<signature_type> >::type
        slot_function_type;

      typedef typename
        parameter::value_type
          <
            args,
            keywords::tag::extended_slot_function_type,
            typename detail::extended_signature<function_traits<signature_type>::arity, signature_type>::function_type
          >::type
          extended_slot_function_type;

      typedef typename
        parameter::value_type<args, keywords::tag::mutex_type, mutex>::type mutex_type;

      typedef signal
        <
          signature_type,
          combiner_type,
          group_type,
          group_compare_type,
          slot_function_type,
          extended_slot_function_type,
          mutex_type
        > type;
    };
  } // namespace signals2
} // namespace hboost

#endif // HBOOST_SIGNALS2_SIGNAL_TYPE_HPP
