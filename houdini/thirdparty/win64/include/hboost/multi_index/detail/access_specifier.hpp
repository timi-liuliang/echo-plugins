/* Copyright 2003-2013 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_DETAIL_ACCESS_SPECIFIER_HPP
#define HBOOST_MULTI_INDEX_DETAIL_ACCESS_SPECIFIER_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>

/* In those compilers that do not accept the member template friend syntax,
 * some protected and private sections might need to be specified as
 * public.
 */

#if defined(HBOOST_NO_MEMBER_TEMPLATE_FRIENDS)
#define HBOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS public
#define HBOOST_MULTI_INDEX_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS public
#else
#define HBOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS protected
#define HBOOST_MULTI_INDEX_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS private
#endif

/* GCC does not correctly support in-class using declarations for template
 * functions. See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=9810
 * MSVC 7.1/8.0 seem to have a similar problem, though the conditions in
 * which the error happens are not that simple. I have yet to isolate this
 * into a snippet suitable for bug reporting.
 * Sun Studio also has this problem, which might be related, from the
 * information gathered at Sun forums, with a known issue notified at the
 * internal bug report 6421933. The bug is present up to Studio Express 2,
 * the latest preview version of the future Sun Studio 12. As of this writing
 * (October 2006) it is not known whether a fix will finally make it into the
 * official Sun Studio 12.
 */

#if HBOOST_WORKAROUND(__GNUC__,==3)&&(__GNUC_MINOR__<4)||\
    HBOOST_WORKAROUND(HBOOST_MSVC,==1310)||\
    HBOOST_WORKAROUND(HBOOST_MSVC,==1400)||\
    HBOOST_WORKAROUND(__SUNPRO_CC,HBOOST_TESTED_AT(0x590))
#define HBOOST_MULTI_INDEX_PRIVATE_IF_USING_DECL_FOR_TEMPL_FUNCTIONS public
#else
#define HBOOST_MULTI_INDEX_PRIVATE_IF_USING_DECL_FOR_TEMPL_FUNCTIONS private
#endif

#endif
