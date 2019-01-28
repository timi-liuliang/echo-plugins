// Copyright (C) 2005-2006 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Nick Edmonds
//           Andrew Lumsdaine
#ifndef HBOOST_GRAPH_DISTRIBUTED_REVERSE_GRAPH_HPP
#define HBOOST_GRAPH_DISTRIBUTED_REVERSE_GRAPH_HPP

#ifndef HBOOST_GRAPH_USE_MPI
#error "Parallel BGL files should not be included unless <hboost/graph/use_mpi.hpp> has been included"
#endif

#include <hboost/graph/reverse_graph.hpp>
#include <hboost/graph/parallel/container_traits.hpp>

namespace hboost {
  namespace graph {
    namespace parallel {
      /// Retrieve the process group from a reverse graph
      template<typename Graph, typename GraphRef>
      struct process_group_type<reverse_graph<Graph, GraphRef> >
        : process_group_type<Graph> { };
    }

  }

  /// Retrieve the process group from a reverse graph
  template<typename Graph, typename GraphRef>
  inline typename graph::parallel::process_group_type<Graph>::type
  process_group(reverse_graph<Graph, GraphRef> const& g) {
    return process_group(g.m_g);
  }
} // namespace hboost

#endif
