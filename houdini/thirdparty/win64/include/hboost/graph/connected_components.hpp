//
//=======================================================================
// Copyright 1997-2001 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//
#ifndef HBOOST_GRAPH_CONNECTED_COMPONENTS_HPP
#define HBOOST_GRAPH_CONNECTED_COMPONENTS_HPP

#include <hboost/config.hpp>
#include <hboost/graph/depth_first_search.hpp>
#include <hboost/graph/properties.hpp>
#include <hboost/graph/graph_concepts.hpp>
#include <hboost/graph/overloading.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/concept/assert.hpp>

namespace hboost {

  namespace detail {

    // This visitor is used both in the connected_components algorithm
    // and in the kosaraju strong components algorithm during the
    // second DFS traversal.
    template <class ComponentsMap>
    class components_recorder : public dfs_visitor<>
    {
      typedef typename property_traits<ComponentsMap>::value_type comp_type;
    public:
      components_recorder(ComponentsMap c, 
                          comp_type& c_count)
        : m_component(c), m_count(c_count) {}

      template <class Vertex, class Graph>
      void start_vertex(Vertex, Graph&) {
        if (m_count == (std::numeric_limits<comp_type>::max)())
          m_count = 0; // start counting components at zero
        else
          ++m_count;
      }
      template <class Vertex, class Graph>
      void discover_vertex(Vertex u, Graph&) {
        put(m_component, u, m_count);
      }
    protected:
      ComponentsMap m_component;
      comp_type& m_count;
    };

  } // namespace detail

  // This function computes the connected components of an undirected
  // graph using a single application of depth first search.

  template <class Graph, class ComponentMap, class P, class T, class R>
  inline typename property_traits<ComponentMap>::value_type
  connected_components(const Graph& g, ComponentMap c, 
                       const bgl_named_params<P, T, R>& params
                       HBOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph, vertex_list_graph_tag))
  {
    if (num_vertices(g) == 0) return 0;

    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    HBOOST_CONCEPT_ASSERT(( WritablePropertyMapConcept<ComponentMap, Vertex> ));
    typedef typename hboost::graph_traits<Graph>::directed_category directed;
    HBOOST_STATIC_ASSERT((hboost::is_same<directed, undirected_tag>::value));

    typedef typename property_traits<ComponentMap>::value_type comp_type;
    // c_count initialized to "nil" (with nil represented by (max)())
    comp_type c_count((std::numeric_limits<comp_type>::max)());
    detail::components_recorder<ComponentMap> vis(c, c_count);
    depth_first_search(g, params.visitor(vis));
    return c_count + 1;
  }

  template <class Graph, class ComponentMap>
  inline typename property_traits<ComponentMap>::value_type
  connected_components(const Graph& g, ComponentMap c
                       HBOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph, vertex_list_graph_tag))
  {
    if (num_vertices(g) == 0) return 0;

    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    HBOOST_CONCEPT_ASSERT(( WritablePropertyMapConcept<ComponentMap, Vertex> ));
    // typedef typename hboost::graph_traits<Graph>::directed_category directed;
    // HBOOST_STATIC_ASSERT((hboost::is_same<directed, undirected_tag>::value));

    typedef typename property_traits<ComponentMap>::value_type comp_type;
    // c_count initialized to "nil" (with nil represented by (max)())
    comp_type c_count((std::numeric_limits<comp_type>::max)());
    detail::components_recorder<ComponentMap> vis(c, c_count);
    depth_first_search(g, visitor(vis));
    return c_count + 1;
  }

  
} // namespace hboost

#ifdef HBOOST_GRAPH_USE_MPI
#  include <hboost/graph/distributed/connected_components.hpp>
#endif

#endif // HBOOST_GRAPH_CONNECTED_COMPONENTS_HPP
