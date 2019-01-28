//
//=======================================================================
// Copyright 2002 Marc Wintermantel (wintermantel@even-ag.ch)
// ETH Zurich, Center of Structure Technologies (www.imes.ethz.ch/st)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef HBOOST_GRAPH_PROFILE_HPP
#define HBOOST_GRAPH_PROFILE_HPP

#include <hboost/graph/graph_traits.hpp>
#include <hboost/detail/numeric_traits.hpp>
#include <hboost/graph/bandwidth.hpp>

namespace hboost {

  template <typename Graph, typename VertexIndexMap>
  typename graph_traits<Graph>::vertices_size_type
  profile(const Graph& g, VertexIndexMap index)
  {
    typename graph_traits<Graph>::vertices_size_type b = 0;
    typename graph_traits<Graph>::vertex_iterator i, end;
    for (hboost::tie(i, end) = vertices(g); i != end; ++i){
      b += ith_bandwidth(*i, g, index) + 1;
    }
    
    return b;
  }

  template <typename Graph>
  typename graph_traits<Graph>::vertices_size_type
  profile(const Graph& g)
  {
    return profile(g, get(vertex_index, g));
  }
 
  
} // namespace hboost

#endif // HBOOST_GRAPH_PROFILE_HPP
