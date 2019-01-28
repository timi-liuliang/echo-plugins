// Copyright (C) 2004-2008 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Nick Edmonds
//           Douglas Gregor
//           Andrew Lumsdaine
#ifndef HBOOST_DISTRIBUTED_FILTERED_GRAPH_HPP
#define HBOOST_DISTRIBUTED_FILTERED_GRAPH_HPP

#ifndef HBOOST_GRAPH_USE_MPI
#error "Parallel BGL files should not be included unless <hboost/graph/use_mpi.hpp> has been included"
#endif

#include <hboost/graph/parallel/process_group.hpp>
#include <hboost/graph/filtered_graph.hpp>

namespace hboost {
  namespace graph {
          namespace parallel {
      /// Retrieve the process group from a filtered graph
      template<typename Graph, typename EdgePredicate, typename VertexPredicate>
      struct process_group_type<filtered_graph<Graph, EdgePredicate, VertexPredicate> >
        : process_group_type<Graph> { };

      template<typename Graph, typename EdgePredicate, typename VertexPredicate>
      struct process_group_type<const filtered_graph<Graph, EdgePredicate, VertexPredicate> >
        : process_group_type<Graph> { };
    }

  }

  /// Retrieve the process group from a filtered graph
  template<typename Graph, typename EdgePredicate, typename VertexPredicate>
  inline typename graph::parallel::process_group_type<Graph>::type
  process_group(filtered_graph<Graph, EdgePredicate, VertexPredicate> const& g) {
    return process_group(g.m_g);
  }

  /// Forward vertex() to vertex() of the base graph 
  template <typename Graph, typename EdgePredicate, typename VertexPredicate>
  typename graph_traits<Graph>::vertex_descriptor
  vertex(typename graph_traits<Graph>::vertices_size_type i, 
         filtered_graph<Graph, EdgePredicate, VertexPredicate> const& g)
  { return vertex(i, g.m_g); }

}

#endif // HBOOST_DISTRIBUTED_FILTERED_GRAPH_HPP
