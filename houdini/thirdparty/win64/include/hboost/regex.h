/*
 *
 * Copyright (c) 1998-2000
 * Dr John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
 
 /*
  *   LOCATION:    see http://www.boost.org/libs/regex for documentation.
  *   FILE         regex.h
  *   VERSION      3.12
  *   DESCRIPTION: Declares POSIX API functions
  */

#ifndef HBOOST_RE_REGEX_H
#define HBOOST_RE_REGEX_H

#include <hboost/cregex.hpp>

/*
*  add using declarations to bring POSIX API functions into
* global scope, only if this is C++ (and not C).
*/
#ifdef __cplusplus

using hboost::regoff_t;
using hboost::regex_tA;
using hboost::regmatch_t;
using hboost::REG_BASIC;
using hboost::REG_EXTENDED;
using hboost::REG_ICASE;
using hboost::REG_NOSUB;
using hboost::REG_NEWLINE;
using hboost::REG_NOSPEC;
using hboost::REG_PEND;
using hboost::REG_DUMP;
using hboost::REG_NOCOLLATE;
using hboost::REG_ESCAPE_IN_LISTS;
using hboost::REG_NEWLINE_ALT;
using hboost::REG_PERL;
using hboost::REG_AWK;
using hboost::REG_GREP;
using hboost::REG_EGREP;
using hboost::REG_ASSERT;
using hboost::REG_INVARG;
using hboost::REG_ATOI;
using hboost::REG_ITOA;

using hboost::REG_NOTBOL;
using hboost::REG_NOTEOL;
using hboost::REG_STARTEND;

using hboost::reg_comp_flags;
using hboost::reg_exec_flags;
using hboost::regcompA;
using hboost::regerrorA;
using hboost::regexecA;
using hboost::regfreeA;

#ifndef HBOOST_NO_WREGEX
using hboost::regcompW;
using hboost::regerrorW;
using hboost::regexecW;
using hboost::regfreeW;
using hboost::regex_tW;
#endif

using hboost::REG_NOERROR;
using hboost::REG_NOMATCH;
using hboost::REG_BADPAT;
using hboost::REG_ECOLLATE;
using hboost::REG_ECTYPE;
using hboost::REG_EESCAPE;
using hboost::REG_ESUBREG;
using hboost::REG_EBRACK;
using hboost::REG_EPAREN;
using hboost::REG_EBRACE;
using hboost::REG_BADBR;
using hboost::REG_ERANGE;
using hboost::REG_ESPACE;
using hboost::REG_BADRPT;
using hboost::REG_EEND;
using hboost::REG_ESIZE;
using hboost::REG_ERPAREN;
using hboost::REG_EMPTY;
using hboost::REG_E_MEMORY;
using hboost::REG_E_UNKNOWN;
using hboost::reg_errcode_t;

#endif /* __cplusplus */

#endif /* HBOOST_RE_REGEX_H */




