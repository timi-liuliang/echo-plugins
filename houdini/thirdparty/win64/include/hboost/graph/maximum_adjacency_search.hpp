//
//=======================================================================
// Copyright 2012 Fernando Vilas
//           2010 Daniel Trebbien
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

// The maximum adjacency search algorithm was originally part of the
// Stoer-Wagner min cut implementation by Daniel Trebbien. It has been
// broken out into its own file to be a public search algorithm, with
// visitor concepts.
#ifndef HBOOST_GRAPH_MAXIMUM_ADJACENCY_SEARCH_H
#define HBOOST_GRAPH_MAXIMUM_ADJACENCY_SEARCH_H

/**
 * This is an implementation of the maximum adjacency search on an
 * undirected graph. It allows a visitor object to perform some
 * operation on each vertex as that vertex is visited.
 *
 * The algorithm runs as follows:
 *
 * Initialize all nodes to be unvisited (reach count = 0)
 *   and call vis.initialize_vertex
 * For i = number of nodes in graph downto 1
 *   Select the unvisited node with the highest reach count
 *     The user provides the starting node to break the first tie,
 *     but future ties are broken arbitrarily
 *   Visit the node by calling vis.start_vertex
 *   Increment the reach count for all unvisited neighbors
 *     and call vis.examine_edge for each of these edges
 *   Mark the node as visited and call vis.finish_vertex
 *
 */

#include <hboost/concept_check.hpp>
#include <hboost/concept/assert.hpp>
#include <hboost/graph/buffer_concepts.hpp>
#include <hboost/graph/exception.hpp>
#include <hboost/graph/graph_concepts.hpp>
#include <hboost/graph/iteration_macros.hpp>
#include <hboost/graph/named_function_params.hpp>
#include <hboost/graph/visitors.hpp>
#include <hboost/tuple/tuple.hpp>

#include <set>

namespace hboost {
  template <class Visitor, class Graph>
  struct MASVisitorConcept {
    void constraints() {
      hboost::function_requires< hboost::CopyConstructibleConcept<Visitor> >();
      vis.initialize_vertex(u, g);
      vis.start_vertex(u, g);
      vis.examine_edge(e, g);
      vis.finish_vertex(u, g);
    }
    Visitor vis;
    Graph g;
    typename hboost::graph_traits<Graph>::vertex_descriptor u;
    typename hboost::graph_traits<Graph>::edge_descriptor e;
  };

  template <class Visitors = null_visitor>
  class mas_visitor {
  public:
    mas_visitor() { }
    mas_visitor(Visitors vis) : m_vis(vis) { }

    template <class Vertex, class Graph>
    void
    initialize_vertex(Vertex u, Graph& g)
    {
      invoke_visitors(m_vis, u, g, ::hboost::on_initialize_vertex());
    }

    template <class Vertex, class Graph>
    void
    start_vertex(Vertex u, Graph& g)
    {
      invoke_visitors(m_vis, u, g, ::hboost::on_start_vertex());
    }

    template <class Edge, class Graph>
    void
    examine_edge(Edge e, Graph& g)
    {
      invoke_visitors(m_vis, e, g, ::hboost::on_examine_edge());
    }

    template <class Vertex, class Graph>
    void
    finish_vertex(Vertex u, Graph& g)
    {
      invoke_visitors(m_vis, u, g, ::hboost::on_finish_vertex());
    }

    HBOOST_GRAPH_EVENT_STUB(on_initialize_vertex,mas)
    HBOOST_GRAPH_EVENT_STUB(on_start_vertex,mas)
    HBOOST_GRAPH_EVENT_STUB(on_examine_edge,mas)
    HBOOST_GRAPH_EVENT_STUB(on_finish_vertex,mas)

  protected:
    Visitors m_vis;
  };
  template <class Visitors>
  mas_visitor<Visitors>
  make_mas_visitor(Visitors vis) {
    return mas_visitor<Visitors>(vis);
  }
  typedef mas_visitor<> default_mas_visitor;

  namespace detail {
    template <class Graph, class WeightMap, class MASVisitor, class VertexAssignmentMap, class KeyedUpdatablePriorityQueue>
      void
      maximum_adjacency_search(const Graph& g, WeightMap weights, MASVisitor vis, const typename hboost::graph_traits<Graph>::vertex_descriptor start, VertexAssignmentMap assignments, KeyedUpdatablePriorityQueue pq) {
      typedef typename hboost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
      typedef typename hboost::property_traits<WeightMap>::value_type weight_type;

     std::set<vertex_descriptor> assignedVertices;

     // initialize `assignments` (all vertices are initially
     // assigned to themselves)
     BGL_FORALL_VERTICES_T(v, g, Graph) {
       put(assignments, v, v);
     }

      typename KeyedUpdatablePriorityQueue::key_map keys = pq.keys();

      // set number of visited neighbors for all vertices to 0
      BGL_FORALL_VERTICES_T(v, g, Graph) {
        if (v == get(assignments, v)) { // foreach u \in V do
          put(keys, v, weight_type(0));          vis.initialize_vertex(v, g);

          pq.push(v);
        }
      }
      HBOOST_ASSERT(pq.size() >= 2);

      // Give the starting vertex high priority
      put(keys, start, get(keys, start) + num_vertices(g) + 1);
      pq.update(start);

      // start traversing the graph
      //vertex_descriptor s, t;
      weight_type w;
      while (!pq.empty()) { // while PQ \neq {} do
        const vertex_descriptor u = pq.top(); // u = extractmax(PQ)
        w = get(keys, u);                        vis.start_vertex(u, g);
        pq.pop();                  //            vis.start_vertex(u, g);

        BGL_FORALL_OUTEDGES_T(u, e, g, Graph) { // foreach (u, v) \in E do
                                                 vis.examine_edge(e, g);

          const vertex_descriptor v = get(assignments, target(e, g));

          if (pq.contains(v)) { // if v \in PQ then
            put(keys, v, get(keys, v) + get(weights, e)); // increasekey(PQ, v, wA(v) + w(u, v))
            pq.update(v);
          }
        }

        typename std::set<vertex_descriptor>::const_iterator assignedVertexIt, assignedVertexEnd = assignedVertices.end();
        for (assignedVertexIt = assignedVertices.begin(); assignedVertexIt != assignedVertexEnd; ++assignedVertexIt) {
          const vertex_descriptor uPrime = *assignedVertexIt;

          if (get(assignments, uPrime) == u) {
            BGL_FORALL_OUTEDGES_T(uPrime, e, g, Graph) { // foreach (u, v) \in E do
                                                 vis.examine_edge(e, g);

              const vertex_descriptor v = get(assignments, target(e, g));

              if (pq.contains(v)) { // if v \in PQ then
                put(keys, v, get(keys, v) + get(weights, e)); // increasekey(PQ, v, wA(v) + w(u, v))
                pq.update(v);
              }
            }
          }
        }
                                                 vis.finish_vertex(u, g);
      }
    }
  } // end namespace detail

  template <class Graph, class WeightMap, class MASVisitor, class VertexAssignmentMap, class KeyedUpdatablePriorityQueue>
    void
maximum_adjacency_search(const Graph& g, WeightMap weights, MASVisitor vis, const typename hboost::graph_traits<Graph>::vertex_descriptor start, VertexAssignmentMap assignments, KeyedUpdatablePriorityQueue pq) {
    HBOOST_CONCEPT_ASSERT((hboost::IncidenceGraphConcept<Graph>));
    HBOOST_CONCEPT_ASSERT((hboost::VertexListGraphConcept<Graph>));
    typedef typename hboost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename hboost::graph_traits<Graph>::vertices_size_type vertices_size_type;
    typedef typename hboost::graph_traits<Graph>::edge_descriptor edge_descriptor;
    HBOOST_CONCEPT_ASSERT((hboost::Convertible<typename hboost::graph_traits<Graph>::directed_category, hboost::undirected_tag>));
    HBOOST_CONCEPT_ASSERT((hboost::ReadablePropertyMapConcept<WeightMap, edge_descriptor>));
    // typedef typename hboost::property_traits<WeightMap>::value_type weight_type;
    hboost::function_requires< MASVisitorConcept<MASVisitor, Graph> >();
    HBOOST_CONCEPT_ASSERT((hboost::ReadWritePropertyMapConcept<VertexAssignmentMap, vertex_descriptor>));
    HBOOST_CONCEPT_ASSERT((hboost::Convertible<vertex_descriptor, typename hboost::property_traits<VertexAssignmentMap>::value_type>));
    HBOOST_CONCEPT_ASSERT((hboost::KeyedUpdatableQueueConcept<KeyedUpdatablePriorityQueue>));

    vertices_size_type n = num_vertices(g);
    if (n < 2)
      throw hboost::bad_graph("the input graph must have at least two vertices.");
    else if (!pq.empty())
      throw std::invalid_argument("the max-priority queue must be empty initially.");

    detail::maximum_adjacency_search(g, weights,
                                            vis, start,
                                            assignments, pq);
  }

  namespace graph {
    namespace detail {
      template <typename WeightMap>
      struct mas_dispatch {
        typedef void result_type;
        template <typename Graph, typename ArgPack>
        static result_type apply(const Graph& g, 
                          //const bgl_named_params<P,T,R>& params, 
                          const ArgPack& params, 
                          WeightMap w) {

          using namespace hboost::graph::keywords;
          typedef typename hboost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
          typedef typename WeightMap::value_type weight_type;

          typedef hboost::detail::make_priority_queue_from_arg_pack_gen<hboost::graph::keywords::tag::max_priority_queue, weight_type, vertex_descriptor, std::greater<weight_type> > default_pq_gen_type;

          default_pq_gen_type pq_gen(choose_param(get_param(params, hboost::distance_zero_t()), weight_type(0)));

          typename hboost::result_of<default_pq_gen_type(const Graph&, const ArgPack&)>::type pq = pq_gen(g, params);

          hboost::maximum_adjacency_search
               (g,
                w,
                params [ _visitor | make_mas_visitor(null_visitor())],
                params [ _root_vertex | *vertices(g).first],
                params [ _vertex_assignment_map | hboost::detail::make_property_map_from_arg_pack_gen<hboost::graph::keywords::tag::vertex_assignment_map, vertex_descriptor>(vertex_descriptor())(g, params)],
                pq
                );
        }
      };

      template <>
      struct mas_dispatch<hboost::param_not_found> {
        typedef void result_type;

        template <typename Graph, typename ArgPack>
        static result_type apply(const Graph& g, 
                          const ArgPack& params, 
                          param_not_found) {

          using namespace hboost::graph::keywords;
          typedef typename hboost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;

          // get edge_weight_t as the weight type
          typedef typename hboost::property_map<Graph, edge_weight_t> WeightMap;
          typedef typename WeightMap::value_type weight_type;

          typedef hboost::detail::make_priority_queue_from_arg_pack_gen<hboost::graph::keywords::tag::max_priority_queue, weight_type, vertex_descriptor, std::greater<weight_type> > default_pq_gen_type;

          default_pq_gen_type pq_gen(choose_param(get_param(params, hboost::distance_zero_t()), weight_type(0)));

          typename hboost::result_of<default_pq_gen_type(const Graph&, const ArgPack&)>::type pq = pq_gen(g, params);

          hboost::maximum_adjacency_search
               (g,
                get(edge_weight, g),
                params [ _visitor | make_mas_visitor(null_visitor())],
                params [ _root_vertex | *vertices(g).first],
                params [ _vertex_assignment_map | hboost::detail::make_property_map_from_arg_pack_gen<hboost::graph::keywords::tag::vertex_assignment_map, vertex_descriptor>(vertex_descriptor())(g, params)],
                pq
                );
        }
      };
    } // end namespace detail
  } // end namespace graph

  // Named parameter interface
  //HBOOST_GRAPH_MAKE_OLD_STYLE_PARAMETER_FUNCTION(maximum_adjacency_search, 1)
  template <typename Graph, typename P, typename T, typename R>
  void
  maximum_adjacency_search (const Graph& g,
      const bgl_named_params<P,T,R>& params) {

    typedef bgl_named_params<P, T, R> params_type;
    HBOOST_GRAPH_DECLARE_CONVERTED_PARAMETERS(params_type, params)

    // do the dispatch based on WeightMap
    typedef typename get_param_type<edge_weight_t, bgl_named_params<P,T,R> >::type W;
    graph::detail::mas_dispatch<W>::apply(g, arg_pack, get_param(params, edge_weight));
  }

  namespace graph {
    namespace detail {
      template <typename Graph>
      struct maximum_adjacency_search_impl {
        typedef void result_type;

        template <typename ArgPack>
        void
        operator() (const Graph& g, const ArgPack& arg_pack) const {
          // call the function that does the dispatching
          typedef typename get_param_type<edge_weight_t, ArgPack >::type W;
          graph::detail::mas_dispatch<W>::apply(g, arg_pack, get_param(arg_pack, edge_weight));
        }
      };
    } // end namespace detail
    HBOOST_GRAPH_MAKE_FORWARDING_FUNCTION(maximum_adjacency_search,1,5)
  } // end namespace graph

} // end namespace hboost

#include <hboost/graph/iteration_macros_undef.hpp>

#endif // HBOOST_GRAPH_MAXIMUM_ADJACENCY_SEARCH_H
