//
//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//
#ifndef HBOOST_GRAPH_TRANSPOSE_HPP
#define HBOOST_GRAPH_TRANSPOSE_HPP

#include <hboost/config.hpp>
#include <hboost/graph/graph_traits.hpp>
#include <hboost/graph/reverse_graph.hpp>
#include <hboost/graph/copy.hpp>


namespace hboost {

  template <class VertexListGraph, class MutableGraph> 
  void transpose_graph(const VertexListGraph& G, MutableGraph& G_T)
  {
    reverse_graph<VertexListGraph> R(G);
    copy_graph(R, G_T);
  }
  
  template <class VertexListGraph, class MutableGraph, 
    class P, class T, class R> 
  void transpose_graph(const VertexListGraph& G, MutableGraph& G_T,
                       const bgl_named_params<P, T, R>& params)
  {
    reverse_graph<VertexListGraph> Rev(G);
    copy_graph(Rev, G_T, params);
  }

} // namespace hboost

#endif // HBOOST_GRAPH_TRANSPOSE_HPP
