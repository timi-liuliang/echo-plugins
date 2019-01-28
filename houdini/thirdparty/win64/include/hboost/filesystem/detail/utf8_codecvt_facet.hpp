// Copyright (c) 2001 Ronald Garcia, Indiana University (garcia@osl.iu.edu)
// Andrew Lumsdaine, Indiana University (lums@osl.iu.edu).

// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_FILESYSTEM_UTF8_CODECVT_FACET_HPP
#define HBOOST_FILESYSTEM_UTF8_CODECVT_FACET_HPP

#include <hboost/filesystem/config.hpp>

#define HBOOST_UTF8_BEGIN_NAMESPACE \
     namespace hboost { namespace filesystem { namespace detail {

#define HBOOST_UTF8_END_NAMESPACE }}}
#define HBOOST_UTF8_DECL HBOOST_FILESYSTEM_DECL

#include <hboost/detail/utf8_codecvt_facet.hpp>

#undef HBOOST_UTF8_BEGIN_NAMESPACE
#undef HBOOST_UTF8_END_NAMESPACE
#undef HBOOST_UTF8_DECL

#endif
