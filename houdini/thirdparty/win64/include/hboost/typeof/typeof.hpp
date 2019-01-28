// Copyright (C) 2004 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_TYPEOF_HPP_INCLUDED
#define HBOOST_TYPEOF_TYPEOF_HPP_INCLUDED

#if defined(HBOOST_TYPEOF_COMPLIANT)
#   define HBOOST_TYPEOF_EMULATION
#endif

#if defined(HBOOST_TYPEOF_EMULATION) && defined(HBOOST_TYPEOF_NATIVE)
#   error both typeof emulation and native mode requested
#endif

#if defined(__COMO__)
#   ifdef __GNUG__
#       ifndef HBOOST_TYPEOF_EMULATION
#           ifndef HBOOST_TYPEOF_NATIVE
#               define HBOOST_TYPEOF_NATIVE
#           endif
#           define HBOOST_TYPEOF_KEYWORD typeof
#       endif
#   else
#       ifndef HBOOST_TYPEOF_NATIVE
#           ifndef HBOOST_TYPEOF_EMULATION
#               define HBOOST_TYPEOF_EMULATION
#           endif
#       else
#           error native typeof is not supported
#       endif
#   endif

#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#   ifdef __GNUC__
#       ifndef HBOOST_TYPEOF_EMULATION
#           ifndef HBOOST_TYPEOF_NATIVE
#               define HBOOST_TYPEOF_NATIVE
#           endif
#           define HBOOST_TYPEOF_KEYWORD __typeof__
#       endif
#   else
#       ifndef HBOOST_TYPEOF_NATIVE
#           ifndef HBOOST_TYPEOF_EMULATION
#               define HBOOST_TYPEOF_EMULATION
#           endif
#       else
#           error native typeof is not supported
#       endif
#   endif

#elif defined(__GNUC__)
#   ifndef HBOOST_TYPEOF_EMULATION
#       ifndef HBOOST_TYPEOF_NATIVE
#           define HBOOST_TYPEOF_NATIVE
#       endif
#       define HBOOST_TYPEOF_KEYWORD __typeof__
#   endif

#elif defined(__MWERKS__)
#   if(__MWERKS__ <= 0x3003)  // 8.x
#       ifndef HBOOST_TYPEOF_EMULATION
#           ifndef HBOOST_TYPEOF_NATIVE
#               define HBOOST_TYPEOF_NATIVE
#           endif
#           define HBOOST_TYPEOF_KEYWORD __typeof__
#       else
#           define HBOOST_TYPEOF_EMULATION_UNSUPPORTED
#       endif
#   else // 9.x
#       ifndef HBOOST_TYPEOF_EMULATION
#           ifndef HBOOST_TYPEOF_NATIVE
#               define HBOOST_TYPEOF_NATIVE
#           endif
#           define HBOOST_TYPEOF_KEYWORD __typeof__
#       endif
#   endif
#elif defined __CODEGEARC__
#   ifndef HBOOST_TYPEOF_EMULATION
#       ifndef HBOOST_TYPEOF_NATIVE
#           define HBOOST_TYPEOF_EMULATION_UNSUPPORTED
#       endif
#   else
#       define HBOOST_TYPEOF_EMULATION_UNSUPPORTED
#   endif
#elif defined __BORLANDC__
#   ifndef HBOOST_TYPEOF_EMULATION
#       ifndef HBOOST_TYPEOF_NATIVE
#           define HBOOST_TYPEOF_EMULATION_UNSUPPORTED
#       endif
#   else
#       define HBOOST_TYPEOF_EMULATION_UNSUPPORTED
#   endif
#elif defined __DMC__
#   ifndef HBOOST_TYPEOF_EMULATION
#       ifndef HBOOST_TYPEOF_NATIVE
#           define HBOOST_TYPEOF_NATIVE
#       endif
#       include <hboost/typeof/dmc/typeof_impl.hpp>
#       define MSVC_TYPEOF_HACK
#   endif
#elif defined(_MSC_VER)
#   if (_MSC_VER <= 1300)  // 6.5, 7.0
#       ifndef HBOOST_TYPEOF_EMULATION
#           ifndef HBOOST_TYPEOF_NATIVE
#               define HBOOST_TYPEOF_NATIVE
#           endif
#           include <hboost/typeof/msvc/typeof_impl.hpp>
#           define MSVC_TYPEOF_HACK
#       else
#           error typeof emulation is not supported
#       endif
#   elif (_MSC_VER >= 1310)  // 7.1 ->
#       ifndef HBOOST_TYPEOF_EMULATION
#           ifndef HBOOST_TYPEOF_NATIVE
#               ifndef _MSC_EXTENSIONS
#                   define HBOOST_TYPEOF_EMULATION
#               else
#                   define HBOOST_TYPEOF_NATIVE
#               endif
#           endif
#       endif
#       ifdef HBOOST_TYPEOF_NATIVE
#           include <hboost/typeof/msvc/typeof_impl.hpp>
#           define MSVC_TYPEOF_HACK
#       endif
#   endif
#elif defined(__HP_aCC)
#   ifndef HBOOST_TYPEOF_NATIVE
#       ifndef HBOOST_TYPEOF_EMULATION
#           define HBOOST_TYPEOF_EMULATION
#       endif
#   else
#       error native typeof is not supported
#   endif

#elif defined(__DECCXX)
#   ifndef HBOOST_TYPEOF_NATIVE
#       ifndef HBOOST_TYPEOF_EMULATION
#           define HBOOST_TYPEOF_EMULATION
#       endif
#   else
#       error native typeof is not supported
#   endif

#elif defined(__BORLANDC__)
#   if (__BORLANDC__ < 0x590)
#       define HBOOST_TYPEOF_NO_FUNCTION_TYPES
#       define HBOOST_TYPEOF_NO_MEMBER_FUNCTION_TYPES
#   endif
#   ifndef HBOOST_TYPEOF_NATIVE
#       ifndef HBOOST_TYPEOF_EMULATION
#           define HBOOST_TYPEOF_EMULATION
#       endif
#   else
#       error native typeof is not supported
#   endif
#elif defined(__SUNPRO_CC)
#   if (__SUNPRO_CC < 0x590 )
#     ifdef HBOOST_TYPEOF_NATIVE
#         error native typeof is not supported
#     endif
#     ifndef HBOOST_TYPEOF_EMULATION
#         define HBOOST_TYPEOF_EMULATION
#     endif
#   else
#     ifndef HBOOST_TYPEOF_EMULATION
#         ifndef HBOOST_TYPEOF_NATIVE
#             define HBOOST_TYPEOF_NATIVE
#         endif
#         define HBOOST_TYPEOF_KEYWORD __typeof__
#     endif
#   endif
#else //unknown compiler
#   ifndef HBOOST_TYPEOF_NATIVE
#       ifndef HBOOST_TYPEOF_EMULATION
#           define HBOOST_TYPEOF_EMULATION
#       endif
#   else
#       ifndef HBOOST_TYPEOF_KEYWORD
#           define HBOOST_TYPEOF_KEYWORD typeof
#       endif
#   endif

#endif

#define HBOOST_TYPEOF_UNIQUE_ID()\
     HBOOST_TYPEOF_REGISTRATION_GROUP * 0x10000 + __LINE__

#define HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()\
     <hboost/typeof/incr_registration_group.hpp>

#ifdef HBOOST_TYPEOF_EMULATION_UNSUPPORTED
#   include <hboost/typeof/unsupported.hpp>
#elif defined HBOOST_TYPEOF_EMULATION
#   define HBOOST_TYPEOF_TEXT "using typeof emulation"
#   include <hboost/typeof/message.hpp>
#   include <hboost/typeof/typeof_impl.hpp>
#   include <hboost/typeof/type_encoding.hpp>
#   include <hboost/typeof/template_encoding.hpp>
#   include <hboost/typeof/modifiers.hpp>
#   include <hboost/typeof/pointers_data_members.hpp>
#   include <hboost/typeof/register_functions.hpp>
#   include <hboost/typeof/register_fundamental.hpp>

#elif defined(HBOOST_TYPEOF_NATIVE)
#   define HBOOST_TYPEOF_TEXT "using native typeof"
#   include <hboost/typeof/message.hpp>
#   include <hboost/typeof/native.hpp>
#else
#   error typeof configuration error
#endif

// auto
#define HBOOST_AUTO(Var, Expr) HBOOST_TYPEOF(Expr) Var = Expr
#define HBOOST_AUTO_TPL(Var, Expr) HBOOST_TYPEOF_TPL(Expr) Var = Expr

#endif//HBOOST_TYPEOF_TYPEOF_HPP_INCLUDED
