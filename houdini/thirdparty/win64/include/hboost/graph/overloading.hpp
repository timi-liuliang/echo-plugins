// Copyright 2004 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine

//
// This file contains helps that enable concept-based overloading
// within the Boost Graph Library.
//
#ifndef HBOOST_GRAPH_OVERLOADING_HPP
#define HBOOST_GRAPH_OVERLOADING_HPP

#include <hboost/type_traits/is_base_and_derived.hpp>
#include <hboost/utility/enable_if.hpp>

namespace hboost {  namespace graph { namespace detail {

struct no_parameter {};

} } } // end namespace hboost::graph::detail

#ifndef HBOOST_NO_SFINAE

#define HBOOST_GRAPH_ENABLE_IF_MODELS(Graph, Tag, Type)               \
  typename enable_if_c<(is_base_and_derived<                         \
                          Tag,                                       \
                          typename graph_traits<Graph>::traversal_category>::value), \
                       Type>::type

#define HBOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph, Tag)                   \
  , HBOOST_GRAPH_ENABLE_IF_MODELS(Graph, Tag,                            \
                                 ::hboost::graph::detail::no_parameter)  \
    = ::hboost::graph::detail::no_parameter()

#else

#define HBOOST_GRAPH_ENABLE_IF_MODELS(Graph, Tag, Type) Type
#define HBOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph, Tag)

#endif // no SFINAE support

#endif // HBOOST_GRAPH_OVERLOADING_HPP
