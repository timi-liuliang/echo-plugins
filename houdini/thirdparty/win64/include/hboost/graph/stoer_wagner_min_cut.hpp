//            Copyright Daniel Trebbien 2010.
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE_1_0.txt or the copy at
//         http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_GRAPH_STOER_WAGNER_MIN_CUT_HPP
#define HBOOST_GRAPH_STOER_WAGNER_MIN_CUT_HPP 1

#include <hboost/assert.hpp>
#include <set>
#include <vector>
#include <hboost/concept_check.hpp>
#include <hboost/concept/assert.hpp>
#include <hboost/graph/adjacency_list.hpp>
#include <hboost/graph/buffer_concepts.hpp>
#include <hboost/graph/exception.hpp>
#include <hboost/graph/graph_traits.hpp>
#include <hboost/graph/maximum_adjacency_search.hpp>
#include <hboost/graph/named_function_params.hpp>
#include <hboost/graph/one_bit_color_map.hpp>
#include <hboost/graph/detail/d_ary_heap.hpp>
#include <hboost/property_map/property_map.hpp>
#include <hboost/tuple/tuple.hpp>
#include <hboost/utility/result_of.hpp>
#include <hboost/graph/iteration_macros.hpp>

namespace hboost {

  namespace detail {
    template < typename ParityMap, typename WeightMap, typename IndexMap >
    class mas_min_cut_visitor : public hboost::default_mas_visitor {
      typedef one_bit_color_map <IndexMap> InternalParityMap;
      typedef typename hboost::property_traits<WeightMap>::value_type weight_type;
    public:
      template < typename Graph >
      mas_min_cut_visitor(const Graph& g,
                          ParityMap parity,
                          weight_type& cutweight,
                          const WeightMap& weight_map, 
                          IndexMap index_map)
        : m_bestParity(parity),
          m_parity(make_one_bit_color_map(num_vertices(g), index_map)),
          m_bestWeight(cutweight),
          m_cutweight(0),
          m_visited(0),
          m_weightMap(weight_map)
      {
        // set here since the init list sets the reference
        m_bestWeight = (std::numeric_limits<weight_type>::max)();
      }

      template < typename Vertex, typename Graph >
      void initialize_vertex(Vertex u, const Graph & g)
      {
        typedef typename hboost::property_traits<ParityMap>::value_type parity_type;
        typedef typename hboost::property_traits<InternalParityMap>::value_type internal_parity_type;

        put(m_parity, u, internal_parity_type(0));
        put(m_bestParity, u, parity_type(0));
      }

      template < typename Edge, typename Graph >
      void examine_edge(Edge e, const Graph & g)
      {
        weight_type w = get(m_weightMap, e);

        // if the target of e is already marked then decrease cutweight
        // otherwise, increase it
        if (get(m_parity, hboost::target(e, g))) {
          m_cutweight -= w;
        } else {
          m_cutweight += w;
        }
      }

      template < typename Vertex, typename Graph >
      void finish_vertex(Vertex u, const Graph & g)
      {
        typedef typename hboost::property_traits<InternalParityMap>::value_type internal_parity_type;

        ++m_visited;
        put(m_parity, u, internal_parity_type(1));

        if (m_cutweight < m_bestWeight && m_visited < num_vertices(g)) {
          m_bestWeight = m_cutweight;
          BGL_FORALL_VERTICES_T(i, g, Graph) {
            put(m_bestParity,i, get(m_parity,i));
          }
        }
      }

      inline void clear() {
        m_bestWeight = (std::numeric_limits<weight_type>::max)();
        m_visited = 0;
        m_cutweight = 0;
      }

    private:
      ParityMap m_bestParity;
      InternalParityMap m_parity;
      weight_type& m_bestWeight;
      weight_type m_cutweight;
      unsigned m_visited;
      const WeightMap& m_weightMap;
    };

    /**
     * \brief Computes a min-cut of the input graph
     *
     * Computes a min-cut of the input graph using the Stoer-Wagner algorithm.
     *
     * \pre \p g is a connected, undirected graph
     * \pre <code>pq.empty()</code>
     * \param[in] g the input graph
     * \param[in] weights a readable property map from each edge to its weight (a non-negative value)
     * \param[out] parities a writable property map from each vertex to a bool type object for
     *     distinguishing the two vertex sets of the min-cut
     * \param[out] assignments a read/write property map from each vertex to a \c vertex_descriptor object. This
     *     map serves as work space, and no particular meaning should be derived from property values
     *     after completion of the algorithm.
     * \param[out] pq a keyed, updatable max-priority queue
     * \returns the cut weight of the min-cut
     * \see http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.114.6687&rep=rep1&type=pdf
     * \see http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.31.614&rep=rep1&type=pdf
     *
     * \author Daniel Trebbien
     * \date 2010-09-11
     */
    template <class UndirectedGraph, class WeightMap, class ParityMap, class VertexAssignmentMap, class KeyedUpdatablePriorityQueue, class IndexMap>
    typename hboost::property_traits<WeightMap>::value_type
    stoer_wagner_min_cut(const UndirectedGraph& g, WeightMap weights, ParityMap parities, VertexAssignmentMap assignments, KeyedUpdatablePriorityQueue& pq, IndexMap index_map) {
      typedef typename hboost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_descriptor;
      typedef typename hboost::property_traits<WeightMap>::value_type weight_type;

      typename graph_traits<UndirectedGraph>::vertex_iterator u_iter, u_end;

      weight_type bestW = (std::numeric_limits<weight_type>::max)();
      weight_type bestThisTime = (std::numeric_limits<weight_type>::max)();
      vertex_descriptor bestStart = hboost::graph_traits<UndirectedGraph>::null_vertex();

      detail::mas_min_cut_visitor<ParityMap, WeightMap, IndexMap>
        vis(g, parities, bestThisTime, weights, index_map);

      // for each node in the graph,
      for (hboost::tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter) {
        // run the MAS and find the min cut
        vis.clear();
        hboost::maximum_adjacency_search(g,
            hboost::weight_map(weights).
            visitor(vis).
            root_vertex(*u_iter).
            vertex_assignment_map(assignments).
            max_priority_queue(pq));
        if (bestThisTime < bestW) {
          bestW = bestThisTime;
          bestStart = *u_iter;
        }
      }

      // Run one more time, starting from the best start location, to
      // ensure the visitor has the best values.
      vis.clear();
      hboost::maximum_adjacency_search(g,
        hboost::vertex_assignment_map(assignments).
        weight_map(weights).
        visitor(vis).
        root_vertex(bestStart).
        max_priority_queue(pq));

      return bestW;
    }
  } // end `namespace detail` within `namespace hboost`

    template <class UndirectedGraph, class WeightMap, class ParityMap, class VertexAssignmentMap, class KeyedUpdatablePriorityQueue, class IndexMap>
    typename hboost::property_traits<WeightMap>::value_type
    stoer_wagner_min_cut(const UndirectedGraph& g, WeightMap weights, ParityMap parities, VertexAssignmentMap assignments, KeyedUpdatablePriorityQueue& pq, IndexMap index_map) {
      HBOOST_CONCEPT_ASSERT((hboost::IncidenceGraphConcept<UndirectedGraph>));
      HBOOST_CONCEPT_ASSERT((hboost::VertexListGraphConcept<UndirectedGraph>));
      typedef typename hboost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_descriptor;
      typedef typename hboost::graph_traits<UndirectedGraph>::vertices_size_type vertices_size_type;
      typedef typename hboost::graph_traits<UndirectedGraph>::edge_descriptor edge_descriptor;
      HBOOST_CONCEPT_ASSERT((hboost::Convertible<typename hboost::graph_traits<UndirectedGraph>::directed_category, hboost::undirected_tag>));
      HBOOST_CONCEPT_ASSERT((hboost::ReadablePropertyMapConcept<WeightMap, edge_descriptor>));
      // typedef typename hboost::property_traits<WeightMap>::value_type weight_type;
      HBOOST_CONCEPT_ASSERT((hboost::WritablePropertyMapConcept<ParityMap, vertex_descriptor>));
      // typedef typename hboost::property_traits<ParityMap>::value_type parity_type;
      HBOOST_CONCEPT_ASSERT((hboost::ReadWritePropertyMapConcept<VertexAssignmentMap, vertex_descriptor>));
      HBOOST_CONCEPT_ASSERT((hboost::Convertible<vertex_descriptor, typename hboost::property_traits<VertexAssignmentMap>::value_type>));
      HBOOST_CONCEPT_ASSERT((hboost::KeyedUpdatableQueueConcept<KeyedUpdatablePriorityQueue>));

      vertices_size_type n = num_vertices(g);
      if (n < 2)
        throw hboost::bad_graph("the input graph must have at least two vertices.");
      else if (!pq.empty())
        throw std::invalid_argument("the max-priority queue must be empty initially.");

      return detail::stoer_wagner_min_cut(g, weights,
                                          parities, assignments, pq, index_map);
    }

namespace graph {
  namespace detail {
    template <class UndirectedGraph, class WeightMap>
    struct stoer_wagner_min_cut_impl {
      typedef typename hboost::property_traits<WeightMap>::value_type result_type;
      template <typename ArgPack>
      result_type operator() (const UndirectedGraph& g, WeightMap weights, const ArgPack& arg_pack) const {
        using namespace hboost::graph::keywords;
        typedef typename hboost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_descriptor;
        typedef typename hboost::property_traits<WeightMap>::value_type weight_type;

        typedef hboost::detail::make_priority_queue_from_arg_pack_gen<hboost::graph::keywords::tag::max_priority_queue, weight_type, vertex_descriptor, std::greater<weight_type> > gen_type;

        gen_type gen(choose_param(get_param(arg_pack, hboost::distance_zero_t()), weight_type(0)));

        typename hboost::result_of<gen_type(const UndirectedGraph&, const ArgPack&)>::type pq = gen(g, arg_pack);

        return hboost::stoer_wagner_min_cut(g,
          weights,
          arg_pack [_parity_map | hboost::dummy_property_map()],
          hboost::detail::make_property_map_from_arg_pack_gen<tag::vertex_assignment_map, vertex_descriptor>(vertex_descriptor())(g, arg_pack),
          pq,
          hboost::detail::override_const_property(arg_pack, _vertex_index_map, g, vertex_index)
        );
      }
    };
  }
  HBOOST_GRAPH_MAKE_FORWARDING_FUNCTION(stoer_wagner_min_cut,2,4)
}

  // Named parameter interface
  HBOOST_GRAPH_MAKE_OLD_STYLE_PARAMETER_FUNCTION(stoer_wagner_min_cut, 2)
namespace graph {
    // version without IndexMap kept for backwards compatibility
    // (but requires vertex_index_t to be defined in the graph)
    // Place after the macro to avoid compilation errors
    template <class UndirectedGraph, class WeightMap, class ParityMap, class VertexAssignmentMap, class KeyedUpdatablePriorityQueue>
    typename hboost::property_traits<WeightMap>::value_type
    stoer_wagner_min_cut(const UndirectedGraph& g, WeightMap weights, ParityMap parities, VertexAssignmentMap assignments, KeyedUpdatablePriorityQueue& pq) {

      return stoer_wagner_min_cut(g, weights,
                                  parities, assignments, pq,
                                  get(vertex_index, g));
    }
} // end `namespace graph`
} // end `namespace hboost`

#include <hboost/graph/iteration_macros_undef.hpp>

#endif // !HBOOST_GRAPH_STOER_WAGNER_MIN_CUT_HPP
