// Boost.Signals2 library

// Copyright Frank Mori Hess 2007-2009.
// Copyright Timmo Stange 2007.
// Copyright Douglas Gregor 2001-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef HBOOST_SIGNALS2_PREPROCESSED_SLOT_HPP
#define HBOOST_SIGNALS2_PREPROCESSED_SLOT_HPP

#include <hboost/preprocessor/repetition.hpp>
#include <hboost/signals2/detail/preprocessed_arg_type.hpp>
#include <hboost/type_traits/function_traits.hpp>

#ifndef HBOOST_SIGNALS2_SLOT_MAX_BINDING_ARGS
#define HBOOST_SIGNALS2_SLOT_MAX_BINDING_ARGS 10
#endif


// template<typename Func, typename BindArgT0, typename BindArgT1, ..., typename BindArgTN-1> slotN(...
#define HBOOST_SIGNALS2_SLOT_N_BINDING_CONSTRUCTOR(z, n, data) \
  template<typename Func, HBOOST_SIGNALS2_PREFIXED_ARGS_TEMPLATE_DECL(n, BindArg)> \
  HBOOST_SIGNALS2_SLOT_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)( \
    const Func &func, HBOOST_SIGNALS2_PREFIXED_FULL_REF_ARGS(n, const BindArg)) \
  { \
    init_slot_function(hboost::bind(func, HBOOST_SIGNALS2_SIGNATURE_ARG_NAMES(n))); \
  }
#define HBOOST_SIGNALS2_SLOT_N_BINDING_CONSTRUCTORS \
  HBOOST_PP_REPEAT_FROM_TO(1, HBOOST_SIGNALS2_SLOT_MAX_BINDING_ARGS, HBOOST_SIGNALS2_SLOT_N_BINDING_CONSTRUCTOR, ~)


#define HBOOST_PP_ITERATION_LIMITS (0, HBOOST_PP_INC(HBOOST_SIGNALS2_MAX_ARGS))
#define HBOOST_PP_FILENAME_1 <hboost/signals2/detail/slot_template.hpp>
#include HBOOST_PP_ITERATE()

#undef HBOOST_SIGNALS2_SLOT_N_BINDING_CONSTRUCTOR
#undef HBOOST_SIGNALS2_SLOT_N_BINDING_CONSTRUCTORS

namespace hboost
{
  namespace signals2
  {
    template<typename Signature,
      typename SlotFunction = hboost::function<Signature> >
    class slot: public detail::slotN<function_traits<Signature>::arity,
      Signature, SlotFunction>::type
    {
    private:
      typedef typename detail::slotN<hboost::function_traits<Signature>::arity,
        Signature, SlotFunction>::type base_type;
    public:
      template<typename F>
      slot(const F& f): base_type(f)
      {}
      // bind syntactic sugar
// template<typename F, typename BindArgT0, typename BindArgT1, ..., typename BindArgTn-1> slot(...
#define HBOOST_SIGNALS2_SLOT_BINDING_CONSTRUCTOR(z, n, data) \
  template<typename Func, HBOOST_SIGNALS2_PREFIXED_ARGS_TEMPLATE_DECL(n, BindArg)> \
    slot(const Func &func, HBOOST_SIGNALS2_PREFIXED_FULL_REF_ARGS(n, const BindArg)): \
    base_type(func, HBOOST_SIGNALS2_SIGNATURE_ARG_NAMES(n)) \
  {}
      HBOOST_PP_REPEAT_FROM_TO(1, HBOOST_SIGNALS2_SLOT_MAX_BINDING_ARGS, HBOOST_SIGNALS2_SLOT_BINDING_CONSTRUCTOR, ~)
#undef HBOOST_SIGNALS2_SLOT_BINDING_CONSTRUCTOR
    };
  } // namespace signals2
}

#endif // HBOOST_SIGNALS2_PREPROCESSED_SLOT_HPP
