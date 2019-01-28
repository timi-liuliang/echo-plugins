// Copyright (c) 2001 Ronald Garcia, Indiana University (garcia@osl.iu.edu)
// Andrew Lumsdaine, Indiana University (lums@osl.iu.edu).
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ARCHIVE_DETAIL_UTF8_CODECVT_FACET_HPP
#define HBOOST_ARCHIVE_DETAIL_UTF8_CODECVT_FACET_HPP

#include <hboost/config.hpp>

// std::codecvt_utf8 doesn't seem to work for msvc
// versions prior to MSVC 14.0

#if defined(_MSC_VER) && _MSC_VER < 1900 \
||  defined( HBOOST_NO_CXX11_HDR_CODECVT )
    #include <hboost/archive/detail/decl.hpp>
    #define HBOOST_UTF8_BEGIN_NAMESPACE \
         namespace hboost { namespace archive { namespace detail {
    #define HBOOST_UTF8_DECL HBOOST_ARCHIVE_DECL
    #define HBOOST_UTF8_END_NAMESPACE }}}

    #include <hboost/detail/utf8_codecvt_facet.hpp>

    #undef HBOOST_UTF8_END_NAMESPACE
    #undef HBOOST_UTF8_DECL
    #undef HBOOST_UTF8_BEGIN_NAMESPACE
#else
    #include <codecvt>
    namespace hboost { namespace archive { namespace detail {
        typedef std::codecvt_utf8<wchar_t> utf8_codecvt_facet;
    } } }
#endif // HBOOST_NO_CXX11_HDR_CODECVT
#endif // HBOOST_ARCHIVE_DETAIL_UTF8_CODECVT_FACET_HPP
