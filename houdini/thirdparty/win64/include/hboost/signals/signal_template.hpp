// Boost.Signals library

// Copyright Douglas Gregor 2001-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

// This file intentionally does not have include guards, because it is meant
// to be included multiple times (one for each signalN class). The
// HBOOST_SIGNALS_SIGNAL_TEMPLATE_HEADER_INCLUDED macro merely serves to
// suppress reinclusion of the files that this header depends on.

#ifndef HBOOST_SIGNALS_SIGNAL_TEMPLATE_HEADER_INCLUDED
#define HBOOST_SIGNALS_SIGNAL_TEMPLATE_HEADER_INCLUDED
#  include <hboost/config.hpp>
#  include <hboost/signals/connection.hpp>
#  include <hboost/ref.hpp>
#  include <hboost/signals/slot.hpp>
#  include <hboost/last_value.hpp>
#  include <hboost/signals/detail/signal_base.hpp>
#  include <hboost/signals/detail/slot_call_iterator.hpp>
#  include <hboost/mpl/bool.hpp>
#  include <hboost/type_traits/is_convertible.hpp>
#  include <cassert>
#  include <functional>
#  include <memory>
#endif // !HBOOST_SIGNALS_SIGNAL_TEMPLATE_HEADER_INCLUDED

#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_PREFIX
#endif

// Include the appropriate functionN header
#define HBOOST_SIGNAL_FUNCTION_N_HEADER HBOOST_JOIN(<hboost/function/function,HBOOST_SIGNALS_NUM_ARGS.hpp>)
#include HBOOST_SIGNAL_FUNCTION_N_HEADER

// Determine if a comma should follow a listing of the arguments/parameters
#if HBOOST_SIGNALS_NUM_ARGS == 0
#  define HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
#else
#  define HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS ,
#endif // HBOOST_SIGNALS_NUM_ARGS > 0

// Define class names used
#define HBOOST_SIGNALS_SIGNAL HBOOST_JOIN(signal,HBOOST_SIGNALS_NUM_ARGS)
#define HBOOST_SIGNALS_FUNCTION HBOOST_JOIN(function,HBOOST_SIGNALS_NUM_ARGS)
#define HBOOST_SIGNALS_ARGS_STRUCT HBOOST_JOIN(args,HBOOST_SIGNALS_NUM_ARGS)
#define HBOOST_SIGNALS_CALL_BOUND HBOOST_JOIN(call_bound,HBOOST_SIGNALS_NUM_ARGS)

// Define commonly-used instantiations
#define HBOOST_SIGNALS_ARGS_STRUCT_INST \
  HBOOST_SIGNALS_NAMESPACE::detail::HBOOST_SIGNALS_ARGS_STRUCT<HBOOST_SIGNALS_TEMPLATE_ARGS>

namespace hboost {
  namespace HBOOST_SIGNALS_NAMESPACE {
    namespace detail {
      // Holds the arguments for a bound slot call in a single place
      template<HBOOST_SIGNALS_TEMPLATE_PARMS
               HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
               typename Dummy = int>
      struct HBOOST_SIGNALS_ARGS_STRUCT {
        HBOOST_SIGNALS_ARGS_STRUCT(HBOOST_SIGNALS_COPY_PARMS)
          HBOOST_SIGNALS_INIT_ARGS
        {
        }

        HBOOST_SIGNALS_ARGS_AS_MEMBERS
      };

      // Function object that calls the function object given to it, passing
      // the bound arguments along to that underlying function object
      template<typename R>
      struct HBOOST_SIGNALS_CALL_BOUND {
        template<HBOOST_SIGNALS_TEMPLATE_PARMS
                 HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
                 typename F>
        struct caller {
          typedef HBOOST_SIGNALS_ARGS_STRUCT<HBOOST_SIGNALS_TEMPLATE_ARGS>*
            args_type;

          args_type args;

          typedef R result_type;

          caller() {}
          caller(args_type a) : args(a) {}

          template<typename Pair>
          R operator()(const Pair& slot) const
          {
            F* target = const_cast<F*>(unsafe_any_cast<F>(&slot.second));
            return (*target)(HBOOST_SIGNALS_BOUND_ARGS);
          }
        };
      };

      template<>
      struct HBOOST_SIGNALS_CALL_BOUND<void> {
        template<HBOOST_SIGNALS_TEMPLATE_PARMS
                 HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
                 typename F>
        struct caller {
          typedef HBOOST_SIGNALS_ARGS_STRUCT<HBOOST_SIGNALS_TEMPLATE_ARGS>*
            args_type;

          args_type args;

          typedef unusable result_type;

          caller(args_type a) : args(a) {}

          template<typename Pair>
          unusable operator()(const Pair& slot) const
          {
            F* target = const_cast<F*>(unsafe_any_cast<F>(&slot.second));
            (*target)(HBOOST_SIGNALS_BOUND_ARGS);
            return unusable();
          }
        };
      };
    } // namespace detail
  } // namespace HBOOST_SIGNALS_NAMESPACE

  // The actual signalN class
  template<
    typename R,
    HBOOST_SIGNALS_TEMPLATE_PARMS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    typename Combiner = last_value<R>,
    typename Group = int,
    typename GroupCompare = std::less<Group>,
    typename SlotFunction = HBOOST_SIGNALS_FUNCTION<
                              R HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
                              HBOOST_SIGNALS_TEMPLATE_ARGS>
  >
  class HBOOST_SIGNALS_SIGNAL :
    public HBOOST_SIGNALS_NAMESPACE::detail::signal_base, // management of slot list
    public HBOOST_SIGNALS_NAMESPACE::trackable // signals are trackable
  {
  public:
    // The slot function type
    typedef SlotFunction slot_function_type;

    // Result type of a slot
    typedef typename HBOOST_SIGNALS_NAMESPACE::detail::slot_result_type<R>::type
      slot_result_type;

    // Argument types
    HBOOST_SIGNALS_ARG_TYPES

#if HBOOST_SIGNALS_NUM_ARGS == 1
    typedef T1 argument_type;
#elif HBOOST_SIGNALS_NUM_ARGS == 2
    typedef T1 first_argument_type;
    typedef T2 second_argument_type;
#endif

  private:
    // The real slot name comparison object type
    typedef HBOOST_SIGNALS_NAMESPACE::detail::group_bridge_compare<GroupCompare, Group>
      real_group_compare_type;

    // The function object passed to the slot call iterator that will call
    // the underlying slot function with its arguments bound
    typedef HBOOST_SIGNALS_NAMESPACE::detail::HBOOST_SIGNALS_CALL_BOUND<R>
      outer_bound_slot_caller;
    typedef typename outer_bound_slot_caller::template
              caller<HBOOST_SIGNALS_TEMPLATE_ARGS
                     HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
                     slot_function_type>
      call_bound_slot;

  public:
    // Combiner's result type
    typedef typename Combiner::result_type result_type;

    // Combiner type
    typedef Combiner combiner_type;

    // Slot type
    typedef slot<slot_function_type> slot_type;

    // Slot name type and comparison
    typedef Group group_type;
    typedef GroupCompare group_compare_type;

    typedef HBOOST_SIGNALS_NAMESPACE::detail::slot_call_iterator<
              call_bound_slot, iterator> slot_call_iterator;

    explicit
    HBOOST_SIGNALS_SIGNAL(const Combiner& c = Combiner(),
                         const GroupCompare& comp = GroupCompare()) :
      HBOOST_SIGNALS_NAMESPACE::detail::signal_base(real_group_compare_type(comp),
                                                   c)
    {
    }

    // Connect a slot to this signal
    HBOOST_SIGNALS_NAMESPACE::connection
    connect(const slot_type&,
            HBOOST_SIGNALS_NAMESPACE::connect_position at
              = HBOOST_SIGNALS_NAMESPACE::at_back);


    HBOOST_SIGNALS_NAMESPACE::connection
    connect(const group_type&, const slot_type&,
            HBOOST_SIGNALS_NAMESPACE::connect_position at
              = HBOOST_SIGNALS_NAMESPACE::at_back);

    template<typename T>
    void disconnect(const T& t)
    {
      typedef mpl::bool_<(is_convertible<T, group_type>::value)> is_group;
      this->do_disconnect(t, is_group());
    }

  private:
    // Disconnect a named slot
    void do_disconnect(const group_type& group, mpl::bool_<true>)
    {
      impl->disconnect(group);
    }

    template<typename Function>
    void do_disconnect(const Function& f, mpl::bool_<false>)
    {
      // Notify the slot handling code that we are iterating through the slots
      HBOOST_SIGNALS_NAMESPACE::detail::call_notification notification(this->impl);

      for (iterator i = impl->slots_.begin(); i != impl->slots_.end(); ++i) {
        slot_function_type& s = *unsafe_any_cast<slot_function_type>(&i->second);
        if (s == f) i->first.disconnect();
      }
    }

  public:

    // Emit the signal
    result_type operator()(HBOOST_SIGNALS_PARMS);
    result_type operator()(HBOOST_SIGNALS_PARMS) const;

    Combiner& combiner()
    { return *unsafe_any_cast<Combiner>(&impl->combiner_); }

    const Combiner& combiner() const
    { return *unsafe_any_cast<const Combiner>(&impl->combiner_); }
  };

  template<
    typename R,
    HBOOST_SIGNALS_TEMPLATE_PARMS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    typename Combiner,
    typename Group,
    typename GroupCompare,
    typename SlotFunction
  >
  HBOOST_SIGNALS_NAMESPACE::connection
  HBOOST_SIGNALS_SIGNAL<
    R, HBOOST_SIGNALS_TEMPLATE_ARGS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    Combiner, Group, GroupCompare, SlotFunction
  >::connect(const slot_type& in_slot,
             HBOOST_SIGNALS_NAMESPACE::connect_position at)
  {
    using hboost::HBOOST_SIGNALS_NAMESPACE::detail::stored_group;

    // If the slot has been disconnected, just return a disconnected
    // connection
    if (!in_slot.is_active()) {
      return HBOOST_SIGNALS_NAMESPACE::connection();
    }

    return impl->connect_slot(in_slot.get_slot_function(), stored_group(),
                              in_slot.get_data(), at);
  }

  template<
    typename R,
    HBOOST_SIGNALS_TEMPLATE_PARMS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    typename Combiner,
    typename Group,
    typename GroupCompare,
    typename SlotFunction
  >
  HBOOST_SIGNALS_NAMESPACE::connection
  HBOOST_SIGNALS_SIGNAL<
    R, HBOOST_SIGNALS_TEMPLATE_ARGS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    Combiner, Group, GroupCompare, SlotFunction
  >::connect(const group_type& group,
             const slot_type& in_slot,
             HBOOST_SIGNALS_NAMESPACE::connect_position at)
  {
    // If the slot has been disconnected, just return a disconnected
    // connection
    if (!in_slot.is_active()) {
      return HBOOST_SIGNALS_NAMESPACE::connection();
    }

    return impl->connect_slot(in_slot.get_slot_function(), group,
                              in_slot.get_data(), at);
  }

  template<
    typename R,
    HBOOST_SIGNALS_TEMPLATE_PARMS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    typename Combiner,
    typename Group,
    typename GroupCompare,
    typename SlotFunction
  >
  typename HBOOST_SIGNALS_SIGNAL<
             R, HBOOST_SIGNALS_TEMPLATE_ARGS
             HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
             Combiner, Group, GroupCompare, SlotFunction>::result_type
  HBOOST_SIGNALS_SIGNAL<
    R, HBOOST_SIGNALS_TEMPLATE_ARGS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    Combiner, Group, GroupCompare, SlotFunction
  >::operator()(HBOOST_SIGNALS_PARMS)
  {
    // Notify the slot handling code that we are making a call
    HBOOST_SIGNALS_NAMESPACE::detail::call_notification notification(this->impl);

    // Construct a function object that will call the underlying slots
    // with the given arguments.
#if HBOOST_SIGNALS_NUM_ARGS == 0
    HBOOST_SIGNALS_ARGS_STRUCT_INST args;
#else
    HBOOST_SIGNALS_ARGS_STRUCT_INST args(HBOOST_SIGNALS_ARGS);
#endif // HBOOST_SIGNALS_NUM_ARGS > 0
    call_bound_slot f(&args);

    typedef typename call_bound_slot::result_type call_result_type;
    optional<call_result_type> cache;
    // Let the combiner call the slots via a pair of input iterators
    return combiner()(slot_call_iterator(notification.impl->slots_.begin(),
                                         impl->slots_.end(), f, cache),
                      slot_call_iterator(notification.impl->slots_.end(),
                                         impl->slots_.end(), f, cache));
  }

  template<
    typename R,
    HBOOST_SIGNALS_TEMPLATE_PARMS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    typename Combiner,
    typename Group,
    typename GroupCompare,
    typename SlotFunction
  >
  typename HBOOST_SIGNALS_SIGNAL<
             R, HBOOST_SIGNALS_TEMPLATE_ARGS
             HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
             Combiner, Group, GroupCompare, SlotFunction>::result_type
  HBOOST_SIGNALS_SIGNAL<
    R, HBOOST_SIGNALS_TEMPLATE_ARGS
    HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS
    Combiner, Group, GroupCompare, SlotFunction
  >::operator()(HBOOST_SIGNALS_PARMS) const
  {
    // Notify the slot handling code that we are making a call
    HBOOST_SIGNALS_NAMESPACE::detail::call_notification notification(this->impl);

    // Construct a function object that will call the underlying slots
    // with the given arguments.
#if HBOOST_SIGNALS_NUM_ARGS == 0
    HBOOST_SIGNALS_ARGS_STRUCT_INST args;
#else
    HBOOST_SIGNALS_ARGS_STRUCT_INST args(HBOOST_SIGNALS_ARGS);
#endif // HBOOST_SIGNALS_NUM_ARGS > 0

    call_bound_slot f(&args);

    typedef typename call_bound_slot::result_type call_result_type;
    optional<call_result_type> cache;

    // Let the combiner call the slots via a pair of input iterators
    return combiner()(slot_call_iterator(notification.impl->slots_.begin(),
                                         impl->slots_.end(), f, cache),
                      slot_call_iterator(notification.impl->slots_.end(),
                                         impl->slots_.end(), f, cache));
  }
} // namespace hboost

#undef HBOOST_SIGNAL_FUNCTION_N_HEADER
#undef HBOOST_SIGNALS_ARGS_STRUCT_INST
#undef HBOOST_SIGNALS_CALL_BOUND
#undef HBOOST_SIGNALS_ARGS_STRUCT
#undef HBOOST_SIGNALS_FUNCTION
#undef HBOOST_SIGNALS_SIGNAL
#undef HBOOST_SIGNALS_COMMA_IF_NONZERO_ARGS

#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_SUFFIX
#endif
