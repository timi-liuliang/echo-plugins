// Copyright (c) 2001 Ronald Garcia, Indiana University (garcia@osl.iu.edu)
// Andrew Lumsdaine, Indiana University (lums@osl.iu.edu). Permission to copy, 
// use, modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided "as is"
// without express or implied warranty, and with no claim as to its suitability
// for any purpose.

#ifndef HBOOST_PROGRAM_OPTIONS_UTF8_CODECVT_FACET_HPP
#define HBOOST_PROGRAM_OPTIONS_UTF8_CODECVT_FACET_HPP

#include <hboost/program_options/config.hpp>

#define HBOOST_UTF8_BEGIN_NAMESPACE \
     namespace hboost { namespace program_options { namespace detail {

#define HBOOST_UTF8_END_NAMESPACE }}}
#define HBOOST_UTF8_DECL HBOOST_PROGRAM_OPTIONS_DECL

#include <hboost/detail/utf8_codecvt_facet.hpp>

#undef HBOOST_UTF8_BEGIN_NAMESPACE
#undef HBOOST_UTF8_END_NAMESPACE
#undef HBOOST_UTF8_DECL

#endif
