// Boost.Signals library

// Copyright Douglas Gregor 2001-2003. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef HBOOST_SIGNALS_SIGNAL3_HEADER
#define HBOOST_SIGNALS_SIGNAL3_HEADER

#define HBOOST_SIGNALS_NUM_ARGS 3
#define HBOOST_SIGNALS_TEMPLATE_PARMS typename T1, typename T2, typename T3
#define HBOOST_SIGNALS_TEMPLATE_ARGS T1, T2, T3
#define HBOOST_SIGNALS_PARMS T1 a1, T2 a2, T3 a3
#define HBOOST_SIGNALS_ARGS a1, a2, a3
#define HBOOST_SIGNALS_BOUND_ARGS args->a1, args->a2, args->a3
#define HBOOST_SIGNALS_ARGS_AS_MEMBERS T1 a1;T2 a2;T3 a3;
#define HBOOST_SIGNALS_COPY_PARMS T1 ia1, T2 ia2, T3 ia3
#define HBOOST_SIGNALS_INIT_ARGS :a1(ia1), a2(ia2), a3(ia3)
#define HBOOST_SIGNALS_ARG_TYPES typedef T1 arg1_type; typedef T2 arg2_type; typedef T3 arg3_type;

#include <hboost/signals/signal_template.hpp>

#undef HBOOST_SIGNALS_ARG_TYPES
#undef HBOOST_SIGNALS_INIT_ARGS
#undef HBOOST_SIGNALS_COPY_PARMS
#undef HBOOST_SIGNALS_ARGS_AS_MEMBERS
#undef HBOOST_SIGNALS_BOUND_ARGS
#undef HBOOST_SIGNALS_ARGS
#undef HBOOST_SIGNALS_PARMS
#undef HBOOST_SIGNALS_TEMPLATE_ARGS
#undef HBOOST_SIGNALS_TEMPLATE_PARMS
#undef HBOOST_SIGNALS_NUM_ARGS

#endif // HBOOST_SIGNALS_SIGNAL3_HEADER
