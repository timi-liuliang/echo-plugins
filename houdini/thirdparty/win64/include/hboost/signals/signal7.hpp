// Boost.Signals library

// Copyright Douglas Gregor 2001-2003. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef HBOOST_SIGNALS_SIGNAL7_HEADER
#define HBOOST_SIGNALS_SIGNAL7_HEADER

#define HBOOST_SIGNALS_NUM_ARGS 7
#define HBOOST_SIGNALS_TEMPLATE_PARMS typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7
#define HBOOST_SIGNALS_TEMPLATE_ARGS T1, T2, T3, T4, T5, T6, T7
#define HBOOST_SIGNALS_PARMS T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7
#define HBOOST_SIGNALS_ARGS a1, a2, a3, a4, a5, a6, a7
#define HBOOST_SIGNALS_BOUND_ARGS args->a1, args->a2, args->a3, args->a4, args->a5, args->a6, args->a7
#define HBOOST_SIGNALS_ARGS_AS_MEMBERS T1 a1;T2 a2;T3 a3;T4 a4;T5 a5;T6 a6;T7 a7;
#define HBOOST_SIGNALS_COPY_PARMS T1 ia1, T2 ia2, T3 ia3, T4 ia4, T5 ia5, T6 ia6, T7 ia7
#define HBOOST_SIGNALS_INIT_ARGS :a1(ia1), a2(ia2), a3(ia3), a4(ia4), a5(ia5), a6(ia6), a7(ia7)
#define HBOOST_SIGNALS_ARG_TYPES typedef T1 arg1_type; typedef T2 arg2_type; typedef T3 arg3_type; typedef T4 arg4_type; typedef T5 arg5_type; typedef T6 arg6_type; typedef T7 arg7_type;

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

#endif // HBOOST_SIGNALS_SIGNAL7_HEADER
