
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

#ifndef HBOOST_MPL_PLACEHOLDERS_HPP_INCLUDED
#define HBOOST_MPL_PLACEHOLDERS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
// Copyright Peter Dimov 2001-2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$


#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/arg.hpp>
#   include <hboost/mpl/aux_/adl_barrier.hpp>

#   if !defined(HBOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE)
#       define HBOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(type) \
        using ::HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::type; \
        /**/
#   else
#       define HBOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(type) /**/
#   endif

#endif

#include <hboost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
 && !defined(HBOOST_MPL_PREPROCESSING_MODE)

#   define HBOOST_MPL_PREPROCESSED_HEADER placeholders.hpp
#   include <hboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <hboost/mpl/aux_/nttp_decl.hpp>
#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/cat.hpp>

// watch out for GNU gettext users, who #define _(x)
#if !defined(_) || defined(HBOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT)
HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
typedef arg<-1> _;
HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

namespace hboost { namespace mpl { 

HBOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(_)

namespace placeholders {
using HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::_;
}

}}
#endif

/// agurt, 17/mar/02: one more placeholder for the last 'apply#' 
/// specialization
#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY + 1, <hboost/mpl/placeholders.hpp>))
#include HBOOST_PP_ITERATE()

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // HBOOST_MPL_PLACEHOLDERS_HPP_INCLUDED

///// iteration

#else
#define i_ HBOOST_PP_FRAME_ITERATION(1)

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

typedef arg<i_> HBOOST_PP_CAT(_,i_);

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

namespace hboost { namespace mpl { 

HBOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(HBOOST_PP_CAT(_,i_))

namespace placeholders {
using HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::HBOOST_PP_CAT(_,i_);
}

}}

#undef i_
#endif // HBOOST_PP_IS_ITERATING
