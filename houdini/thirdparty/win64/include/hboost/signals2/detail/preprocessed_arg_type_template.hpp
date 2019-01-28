// Copyright Frank Mori Hess 2009
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

// This file is included iteratively, and should not be protected from multiple inclusion

#define HBOOST_SIGNALS2_NUM_ARGS HBOOST_PP_ITERATION()

namespace hboost
{
  namespace signals2
  {
    namespace detail
    {
      template<unsigned n HBOOST_PP_COMMA_IF(HBOOST_SIGNALS2_NUM_ARGS)
        HBOOST_SIGNALS2_ARGS_TEMPLATE_DECL(HBOOST_SIGNALS2_NUM_ARGS)>
        class HBOOST_SIGNALS2_PREPROCESSED_ARG_N_TYPE_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS);

// template<typename T1, typename T2, ... , typename TN> class preprocessed_arg_typeN<n, T1, T2, ..., TN>{...} ...
#define HBOOST_SIGNALS2_PREPROCESSED_ARG_TYPE_CLASS_TEMPLATE_SPECIALIZATION(z, n, data) \
  template<HBOOST_SIGNALS2_ARGS_TEMPLATE_DECL(HBOOST_SIGNALS2_NUM_ARGS)> \
  class HBOOST_SIGNALS2_PREPROCESSED_ARG_N_TYPE_CLASS_NAME(HBOOST_SIGNALS2_NUM_ARGS)<n, \
    HBOOST_SIGNALS2_ARGS_TEMPLATE_INSTANTIATION(HBOOST_SIGNALS2_NUM_ARGS)> \
  { \
  public: \
    typedef HBOOST_PP_CAT(T, HBOOST_PP_INC(n)) type; \
  };
      HBOOST_PP_REPEAT(HBOOST_SIGNALS2_NUM_ARGS, HBOOST_SIGNALS2_PREPROCESSED_ARG_TYPE_CLASS_TEMPLATE_SPECIALIZATION, ~)

    } // namespace detail
  } // namespace signals2
} // namespace hboost

#undef HBOOST_SIGNALS2_NUM_ARGS
