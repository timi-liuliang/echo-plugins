// Boost.Signals library

// Copyright Douglas Gregor 2001-2006. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/signals

#ifndef HBOOST_SIGNAL_HPP
#define HBOOST_SIGNAL_HPP

#ifndef HBOOST_SIGNALS_NO_DEPRECATION_WARNING
# if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#  pragma message ("Warning: Boost.Signals is no longer being maintained and is now deprecated. Please switch to Boost.Signals2. To disable this warning message, define HBOOST_SIGNALS_NO_DEPRECATION_WARNING.")
# elif defined(__GNUC__) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
#  warning                  "Boost.Signals is no longer being maintained and is now deprecated. Please switch to Boost.Signals2. To disable this warning message, define HBOOST_SIGNALS_NO_DEPRECATION_WARNING."
# endif
#endif

#ifndef HBOOST_SIGNALS_MAX_ARGS
#  define HBOOST_SIGNALS_MAX_ARGS 10
#endif

#include <hboost/config.hpp>
#include <hboost/type_traits/function_traits.hpp>
#include <hboost/signals/signal0.hpp>
#include <hboost/signals/signal1.hpp>
#include <hboost/signals/signal2.hpp>
#include <hboost/signals/signal3.hpp>
#include <hboost/signals/signal4.hpp>
#include <hboost/signals/signal5.hpp>
#include <hboost/signals/signal6.hpp>
#include <hboost/signals/signal7.hpp>
#include <hboost/signals/signal8.hpp>
#include <hboost/signals/signal9.hpp>
#include <hboost/signals/signal10.hpp>
#include <hboost/function.hpp>

#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_PREFIX
#endif

namespace hboost {
#ifndef HBOOST_FUNCTION_NO_FUNCTION_TYPE_SYNTAX
  namespace HBOOST_SIGNALS_NAMESPACE {
    namespace detail {
      template<int Arity,
               typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl;

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<0, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal0<typename traits::result_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<1, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal1<typename traits::result_type,
                        typename traits::arg1_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<2, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal2<typename traits::result_type,
                        typename traits::arg1_type,
                        typename traits::arg2_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<3, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal3<typename traits::result_type,
                        typename traits::arg1_type,
                        typename traits::arg2_type,
                        typename traits::arg3_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<4, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal4<typename traits::result_type,
                        typename traits::arg1_type,
                        typename traits::arg2_type,
                        typename traits::arg3_type,
                        typename traits::arg4_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<5, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal5<typename traits::result_type,
                        typename traits::arg1_type,
                        typename traits::arg2_type,
                        typename traits::arg3_type,
                        typename traits::arg4_type,
                        typename traits::arg5_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<6, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal6<typename traits::result_type,
                        typename traits::arg1_type,
                        typename traits::arg2_type,
                        typename traits::arg3_type,
                        typename traits::arg4_type,
                        typename traits::arg5_type,
                        typename traits::arg6_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<7, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal7<typename traits::result_type,
                        typename traits::arg1_type,
                        typename traits::arg2_type,
                        typename traits::arg3_type,
                        typename traits::arg4_type,
                        typename traits::arg5_type,
                        typename traits::arg6_type,
                        typename traits::arg7_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<8, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal8<typename traits::result_type,
                        typename traits::arg1_type,
                        typename traits::arg2_type,
                        typename traits::arg3_type,
                        typename traits::arg4_type,
                        typename traits::arg5_type,
                        typename traits::arg6_type,
                        typename traits::arg7_type,
                        typename traits::arg8_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<9, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal9<typename traits::result_type,
                        typename traits::arg1_type,
                        typename traits::arg2_type,
                        typename traits::arg3_type,
                        typename traits::arg4_type,
                        typename traits::arg5_type,
                        typename traits::arg6_type,
                        typename traits::arg7_type,
                        typename traits::arg8_type,
                        typename traits::arg9_type,
                        Combiner,
                        Group,
                        GroupCompare,
                        SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      class real_get_signal_impl<10, Signature, Combiner, Group, GroupCompare,
                                 SlotFunction>
      {
        typedef function_traits<Signature> traits;

      public:
        typedef signal10<typename traits::result_type,
                         typename traits::arg1_type,
                         typename traits::arg2_type,
                         typename traits::arg3_type,
                         typename traits::arg4_type,
                         typename traits::arg5_type,
                         typename traits::arg6_type,
                         typename traits::arg7_type,
                         typename traits::arg8_type,
                         typename traits::arg9_type,
                         typename traits::arg10_type,
                         Combiner,
                         Group,
                         GroupCompare,
                         SlotFunction> type;
      };

      template<typename Signature,
               typename Combiner,
               typename Group,
               typename GroupCompare,
               typename SlotFunction>
      struct get_signal_impl :
        public real_get_signal_impl<(function_traits<Signature>::arity),
                                    Signature,
                                    Combiner,
                                    Group,
                                    GroupCompare,
                                    SlotFunction>
      {
      };

    } // end namespace detail
  } // end namespace HBOOST_SIGNALS_NAMESPACE

  // Very lightweight wrapper around the signalN classes that allows signals to
  // be created where the number of arguments does not need to be part of the
  // class name.
  template<
    typename Signature, // function type R (T1, T2, ..., TN)
    typename Combiner = last_value<typename function_traits<Signature>::result_type>,
    typename Group = int,
    typename GroupCompare = std::less<Group>,
    typename SlotFunction = function<Signature>
  >
  class signal :
    public HBOOST_SIGNALS_NAMESPACE::detail::get_signal_impl<Signature,
                                                            Combiner,
                                                            Group,
                                                            GroupCompare,
                                                            SlotFunction>::type
  {
    typedef typename HBOOST_SIGNALS_NAMESPACE::detail::get_signal_impl<
                       Signature,
                       Combiner,
                       Group,
                       GroupCompare,
                       SlotFunction>::type base_type;

  public:
    explicit signal(const Combiner& combiner = Combiner(),
                    const GroupCompare& group_compare = GroupCompare()) :
      base_type(combiner, group_compare)
    {
    }
  };
#endif // ndef HBOOST_FUNCTION_NO_FUNCTION_TYPE_SYNTAX

} // end namespace hboost

#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_SUFFIX
#endif

#endif // HBOOST_SIGNAL_HPP
