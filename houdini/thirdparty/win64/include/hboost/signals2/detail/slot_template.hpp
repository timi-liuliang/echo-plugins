// Boost.Signals2 library

// Copyright Frank Mori Hess 2007-2008.
// Copyright Timmo Stange 2007.
// Copyright Douglas Gregor 2001-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

// This file is included iteratively, and should not be protected from multiple inclusion

#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
#define HBOOST_SIGNALS2_NUM_ARGS HBOOST_PP_ITERATION()
#else
#define HBOOST_SIGNALS2_NUM_ARGS 1
#endif


namespace hboost
{
  namespace signals2
  {
#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
    template<typename Signature, typename SlotFunction> class slot;
#else
    template<typename Signature, typename SlotFunction = hboost::function<Signature> >
      class slot;

#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1900)
    template<typename Signature, typename SlotFunction> class slot{};
#endif
#endif // HBOOST_NO_CXX11_VARIADIC_TEMPLATES

    template<HBOOST_SIGNALS2_SLOT_TEMPLATE_SPECIALIZATION_DECL(HBOOST_SIGNALS2_NUM_ARGS)>
      class HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS) HBOOST_SIGNALS2_SLOT_TEMPLATE_SPECIALIZATION
      : public slot_base, public detail::HBOOST_SIGNALS2_STD_FUNCTIONAL_BASE(R)

    {
    public:
      template<HBOOST_SIGNALS2_PREFIXED_SIGNATURE_TEMPLATE_DECL(HBOOST_SIGNALS2_NUM_ARGS, Other), typename OtherSlotFunction>
      friend class HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS);

      typedef SlotFunction slot_function_type;
      typedef R result_type;
      typedef typename mpl::identity<HBOOST_SIGNALS2_SIGNATURE_FUNCTION_TYPE(HBOOST_SIGNALS2_NUM_ARGS)>::type signature_type;

#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES

// typedef Tn argn_type;
#define HBOOST_SIGNALS2_MISC_STATEMENT(z, n, data) \
    typedef HBOOST_PP_CAT(T, HBOOST_PP_INC(n)) HBOOST_PP_CAT(HBOOST_PP_CAT(arg, HBOOST_PP_INC(n)), _type);
          HBOOST_PP_REPEAT(HBOOST_SIGNALS2_NUM_ARGS, HBOOST_SIGNALS2_MISC_STATEMENT, ~)
#undef HBOOST_SIGNALS2_MISC_STATEMENT
#if HBOOST_SIGNALS2_NUM_ARGS == 1
      typedef arg1_type argument_type;
#elif HBOOST_SIGNALS2_NUM_ARGS == 2
      typedef arg1_type first_argument_type;
      typedef arg2_type second_argument_type;
#endif

      template<unsigned n> class arg : public
        detail::HBOOST_SIGNALS2_PREPROCESSED_ARG_N_TYPE_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)
        <n HBOOST_SIGNALS2_PP_COMMA_IF(HBOOST_SIGNALS2_NUM_ARGS)
        HBOOST_SIGNALS2_ARGS_TEMPLATE_INSTANTIATION(HBOOST_SIGNALS2_NUM_ARGS)>
      {};

      HBOOST_STATIC_CONSTANT(int, arity = HBOOST_SIGNALS2_NUM_ARGS);

#else // HBOOST_NO_CXX11_VARIADIC_TEMPLATES

      template<unsigned n> class arg
      {
      public:
        typedef typename detail::variadic_arg_type<n, Args...>::type type;
      };
      HBOOST_STATIC_CONSTANT(int, arity = sizeof...(Args));

#endif // HBOOST_NO_CXX11_VARIADIC_TEMPLATES

      template<typename F>
      HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)(const F& f)
      {
        init_slot_function(f);
      }
      // copy constructors
#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
      template<HBOOST_SIGNALS2_PREFIXED_SIGNATURE_TEMPLATE_DECL(HBOOST_SIGNALS2_NUM_ARGS, Other), typename OtherSlotFunction>
      HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)(const HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)
        <HBOOST_SIGNALS2_PREFIXED_SIGNATURE_TEMPLATE_INSTANTIATION(HBOOST_SIGNALS2_NUM_ARGS, Other), OtherSlotFunction> &other_slot):
        slot_base(other_slot), _slot_function(other_slot._slot_function)
      {
      }
#endif
      template<typename Signature, typename OtherSlotFunction>
      HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)(const slot<Signature, OtherSlotFunction> &other_slot):
        slot_base(other_slot), _slot_function(other_slot._slot_function)
      {
      }
      // bind syntactic sugar
      HBOOST_SIGNALS2_SLOT_N_BINDING_CONSTRUCTORS
      // invocation
      R operator()(HBOOST_SIGNALS2_SIGNATURE_FULL_ARGS(HBOOST_SIGNALS2_NUM_ARGS))
      {
        locked_container_type locked_objects = lock();
        return _slot_function(HBOOST_SIGNALS2_SIGNATURE_ARG_NAMES(HBOOST_SIGNALS2_NUM_ARGS));
      }
      R operator()(HBOOST_SIGNALS2_SIGNATURE_FULL_ARGS(HBOOST_SIGNALS2_NUM_ARGS)) const
      {
        locked_container_type locked_objects = lock();
        return _slot_function(HBOOST_SIGNALS2_SIGNATURE_ARG_NAMES(HBOOST_SIGNALS2_NUM_ARGS));
      }
      // tracking
      HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)& track(const weak_ptr<void> &tracked)      {
        _tracked_objects.push_back(tracked);
        return *this;
      }
      HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)& track(const signal_base &signal)
      {
        track_signal(signal);
        return *this;
      }
      HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)& track(const slot_base &slot)
      {
        tracked_container_type::const_iterator it;
        for(it = slot.tracked_objects().begin(); it != slot.tracked_objects().end(); ++it)
        {
          _tracked_objects.push_back(*it);
        }
        return *this;
      }
      template<typename ForeignWeakPtr>
      HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)& track_foreign(const ForeignWeakPtr &tracked,
        typename weak_ptr_traits<ForeignWeakPtr>::shared_type * /*SFINAE*/ = 0)
      {
        _tracked_objects.push_back(detail::foreign_void_weak_ptr(tracked));
        return *this;
      }
      template<typename ForeignSharedPtr>
      HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)& track_foreign(const ForeignSharedPtr &tracked,
        typename shared_ptr_traits<ForeignSharedPtr>::weak_type * /*SFINAE*/ = 0)
      {
        _tracked_objects.push_back
        (
          detail::foreign_void_weak_ptr
          (
            typename shared_ptr_traits<ForeignSharedPtr>::weak_type(tracked)
          )
        );
        return *this;
      }

      const slot_function_type& slot_function() const {return _slot_function;}
      slot_function_type& slot_function() {return _slot_function;}
    private:
      template<typename F>
      void init_slot_function(const F& f)
      {
        _slot_function = detail::get_invocable_slot(f, detail::tag_type(f));
        signals2::detail::tracked_objects_visitor visitor(this);
        hboost::visit_each(visitor, f);
      }

      SlotFunction _slot_function;
    };

#ifdef HBOOST_NO_CXX11_VARIADIC_TEMPLATES
    namespace detail
    {
      template<unsigned arity, typename Signature, typename SlotFunction>
      class slotN;
      // partial template specialization
      template<typename Signature, typename SlotFunction>
      class slotN<HBOOST_SIGNALS2_NUM_ARGS, Signature, SlotFunction>
      {
      public:
        typedef HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)<
          HBOOST_SIGNALS2_PORTABLE_SIGNATURE(HBOOST_SIGNALS2_NUM_ARGS, Signature),
          SlotFunction> type;
      };
    }
#endif
  } // end namespace signals2
} // end namespace hboost

#undef HBOOST_SIGNALS2_NUM_ARGS
