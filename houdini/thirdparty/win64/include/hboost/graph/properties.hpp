//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef HBOOST_GRAPH_PROPERTIES_HPP
#define HBOOST_GRAPH_PROPERTIES_HPP

#include <hboost/config.hpp>
#include <hboost/assert.hpp>
#include <hboost/pending/property.hpp>
#include <hboost/detail/workaround.hpp>

// Include the property map library and extensions in the BGL.
#include <hboost/property_map/property_map.hpp>
#include <hboost/graph/property_maps/constant_property_map.hpp>
#include <hboost/graph/property_maps/null_property_map.hpp>

#include <hboost/graph/graph_traits.hpp>
#include <hboost/type_traits.hpp>
#include <hboost/limits.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/mpl/if.hpp>

namespace hboost {

  enum default_color_type { white_color, gray_color, green_color, red_color, black_color };

  template <class ColorValue>
  struct color_traits {
    static default_color_type white() { return white_color; }
    static default_color_type gray() { return gray_color; }
    static default_color_type green() { return green_color; }
    static default_color_type red() { return red_color; }
    static default_color_type black() { return black_color; }
  };

  // These functions are now obsolete, replaced by color_traits.
  inline default_color_type white(default_color_type) { return white_color; }
  inline default_color_type gray(default_color_type) { return gray_color; }
  inline default_color_type green(default_color_type) { return green_color; }
  inline default_color_type red(default_color_type) { return red_color; }
  inline default_color_type black(default_color_type) { return black_color; }


  struct graph_property_tag { };
  struct vertex_property_tag { };
  struct edge_property_tag { };

  // See examples/edge_property.cpp for how to use this.
#define HBOOST_INSTALL_PROPERTY(KIND, NAME) \
  template <> struct property_kind<KIND##_##NAME##_t> { \
    typedef KIND##_property_tag type; \
  }

#define HBOOST_DEF_PROPERTY(KIND, NAME) \
  enum KIND##_##NAME##_t { KIND##_##NAME }; \
  HBOOST_INSTALL_PROPERTY(KIND, NAME)

  // These three are defined in hboost/pending/property.hpp
  HBOOST_INSTALL_PROPERTY(vertex, all);
  HBOOST_INSTALL_PROPERTY(edge, all);
  HBOOST_INSTALL_PROPERTY(graph, all);
  HBOOST_DEF_PROPERTY(vertex, index);
  HBOOST_DEF_PROPERTY(vertex, index1);
  HBOOST_DEF_PROPERTY(vertex, index2);
  HBOOST_DEF_PROPERTY(vertex, root);
  HBOOST_DEF_PROPERTY(edge, index);
  HBOOST_DEF_PROPERTY(edge, name);
  HBOOST_DEF_PROPERTY(edge, weight);
  HBOOST_DEF_PROPERTY(edge, weight2);
  HBOOST_DEF_PROPERTY(edge, color);
  HBOOST_DEF_PROPERTY(vertex, name);
  HBOOST_DEF_PROPERTY(graph, name);
  HBOOST_DEF_PROPERTY(vertex, distance);
  HBOOST_DEF_PROPERTY(vertex, distance2);
  HBOOST_DEF_PROPERTY(vertex, color);
  HBOOST_DEF_PROPERTY(vertex, degree);
  HBOOST_DEF_PROPERTY(vertex, in_degree);
  HBOOST_DEF_PROPERTY(vertex, out_degree);
  HBOOST_DEF_PROPERTY(vertex, current_degree);
  HBOOST_DEF_PROPERTY(vertex, priority);
  HBOOST_DEF_PROPERTY(vertex, discover_time);
  HBOOST_DEF_PROPERTY(vertex, finish_time);
  HBOOST_DEF_PROPERTY(vertex, predecessor);
  HBOOST_DEF_PROPERTY(vertex, rank);
  HBOOST_DEF_PROPERTY(vertex, centrality);
  HBOOST_DEF_PROPERTY(vertex, lowpoint);
  HBOOST_DEF_PROPERTY(vertex, potential);
  HBOOST_DEF_PROPERTY(vertex, update);
  HBOOST_DEF_PROPERTY(vertex, underlying);
  HBOOST_DEF_PROPERTY(edge, reverse);
  HBOOST_DEF_PROPERTY(edge, capacity);
  HBOOST_DEF_PROPERTY(edge, flow);
  HBOOST_DEF_PROPERTY(edge, residual_capacity);
  HBOOST_DEF_PROPERTY(edge, centrality);
  HBOOST_DEF_PROPERTY(edge, discover_time);
  HBOOST_DEF_PROPERTY(edge, update);
  HBOOST_DEF_PROPERTY(edge, finished);
  HBOOST_DEF_PROPERTY(edge, underlying);
  HBOOST_DEF_PROPERTY(graph, visitor);

  // These tags are used for property bundles
  // These three are defined in hboost/pending/property.hpp
  HBOOST_INSTALL_PROPERTY(graph, bundle);
  HBOOST_INSTALL_PROPERTY(vertex, bundle);
  HBOOST_INSTALL_PROPERTY(edge, bundle);

  // These tags are used to denote the owners and local descriptors
  // for the vertices and edges of a distributed graph.
  HBOOST_DEF_PROPERTY(vertex, global);
  HBOOST_DEF_PROPERTY(vertex, owner);
  HBOOST_DEF_PROPERTY(vertex, local);
  HBOOST_DEF_PROPERTY(edge, global);
  HBOOST_DEF_PROPERTY(edge, owner);
  HBOOST_DEF_PROPERTY(edge, local);
  HBOOST_DEF_PROPERTY(vertex, local_index);
  HBOOST_DEF_PROPERTY(edge, local_index);

#undef HBOOST_DEF_PROPERTY

  namespace detail {

    template <typename G, typename Tag>
    struct property_kind_from_graph: property_kind<Tag> {};

#ifndef HBOOST_GRAPH_NO_BUNDLED_PROPERTIES
    template <typename G, typename R, typename T>
    struct property_kind_from_graph<G, R T::*> {
      typedef typename hboost::mpl::if_<
                hboost::is_base_of<T, typename vertex_bundle_type<G>::type>,
                vertex_property_tag,
                typename hboost::mpl::if_<
                  hboost::is_base_of<T, typename edge_bundle_type<G>::type>,
                  edge_property_tag,
                  typename hboost::mpl::if_<
                    hboost::is_base_of<T, typename graph_bundle_type<G>::type>,
                    graph_property_tag,
                    void>::type>::type>::type type;
    };
#endif

    struct dummy_edge_property_selector {
      template <class Graph, class Property, class Tag>
      struct bind_ {
        typedef identity_property_map type;
        typedef identity_property_map const_type;
      };
    };
    struct dummy_vertex_property_selector {
      template <class Graph, class Property, class Tag>
      struct bind_ {
        typedef identity_property_map type;
        typedef identity_property_map const_type;
      };
    };

  } // namespace detail

  // Graph classes can either partially specialize property_map
  // or they can specialize these two selector classes.
  template <class GraphTag>
  struct edge_property_selector {
    typedef detail::dummy_edge_property_selector type;
  };

  template <class GraphTag>
  struct vertex_property_selector {
    typedef detail::dummy_vertex_property_selector type;
  };

  namespace detail {

    template <typename A> struct return_void {typedef void type;};

    template <typename Graph, typename Enable = void>
    struct graph_tag_or_void {
      typedef void type;
    };

    template <typename Graph>
    struct graph_tag_or_void<Graph, typename return_void<typename Graph::graph_tag>::type> {
      typedef typename Graph::graph_tag type;
    };

    template <class Graph, class PropertyTag>
    struct edge_property_map
      : edge_property_selector<
          typename graph_tag_or_void<Graph>::type
        >::type::template bind_<
                            Graph,
                            typename edge_property_type<Graph>::type,
                            PropertyTag>
      {};
    template <class Graph, class PropertyTag>
    struct vertex_property_map
      : vertex_property_selector<
          typename graph_tag_or_void<Graph>::type
        >::type::template bind_<
                            Graph,
                            typename vertex_property_type<Graph>::type,
                            PropertyTag>
      {};
  } // namespace detail

  template <class Graph, class Property, class Enable = void>
  struct property_map:
    mpl::if_<
      is_same<typename detail::property_kind_from_graph<Graph, Property>::type, edge_property_tag>,
      detail::edge_property_map<Graph, Property>,
      detail::vertex_property_map<Graph, Property> >::type
  {};

  // shortcut for accessing the value type of the property map
  template <class Graph, class Property>
  class property_map_value {
    typedef typename property_map<Graph, Property>::const_type PMap;
  public:
    typedef typename property_traits<PMap>::value_type type;
  };

  template <class Graph, class Property>
  class graph_property {
  public:
    typedef typename property_value<
      typename hboost::graph_property_type<Graph>::type, Property
    >::type type;
  };

  template <typename Graph> struct vertex_property: vertex_property_type<Graph> {};
  template <typename Graph> struct edge_property: edge_property_type<Graph> {};

  template <typename Graph>
  class degree_property_map
    : public put_get_helper<typename graph_traits<Graph>::degree_size_type,
                            degree_property_map<Graph> >
  {
  public:
    typedef typename graph_traits<Graph>::vertex_descriptor key_type;
    typedef typename graph_traits<Graph>::degree_size_type value_type;
    typedef value_type reference;
    typedef readable_property_map_tag category;
    degree_property_map(const Graph& g) : m_g(g) { }
    value_type operator[](const key_type& v) const {
      return degree(v, m_g);
    }
  private:
    const Graph& m_g;
  };
  template <typename Graph>
  inline degree_property_map<Graph>
  make_degree_map(const Graph& g) {
    return degree_property_map<Graph>(g);
  }

  //========================================================================
  // Iterator Property Map Generating Functions contributed by
  // Kevin Vanhorn. (see also the property map generating functions
  // in hboost/property_map/property_map.hpp)

#if !defined(HBOOST_NO_STD_ITERATOR_TRAITS)
  // A helper function for creating a vertex property map out of a
  // random access iterator and the internal vertex index map from a
  // graph.
  template <class PropertyGraph, class RandomAccessIterator>
  inline
  iterator_property_map<
    RandomAccessIterator,
    typename property_map<PropertyGraph, vertex_index_t>::type,
    typename std::iterator_traits<RandomAccessIterator>::value_type,
    typename std::iterator_traits<RandomAccessIterator>::reference
  >
  make_iterator_vertex_map(RandomAccessIterator iter, const PropertyGraph& g)
  {
    return make_iterator_property_map(iter, get(vertex_index, g));
  }

  // Use this next function when vertex_descriptor is known to be an
  // integer type, with values ranging from 0 to num_vertices(g).
  //
  template <class RandomAccessIterator>
  inline
  iterator_property_map<
    RandomAccessIterator,
    identity_property_map,
    typename std::iterator_traits<RandomAccessIterator>::value_type,
    typename std::iterator_traits<RandomAccessIterator>::reference
  >
  make_iterator_vertex_map(RandomAccessIterator iter)
  {
    return make_iterator_property_map(iter, identity_property_map());
  }
#endif

  template <class PropertyGraph, class RandomAccessContainer>
  inline
  iterator_property_map<
    typename RandomAccessContainer::iterator,
    typename property_map<PropertyGraph, vertex_index_t>::type,
    typename RandomAccessContainer::value_type,
    typename RandomAccessContainer::reference
  >
  make_container_vertex_map(RandomAccessContainer& c, const PropertyGraph& g)
  {
    HBOOST_ASSERT(c.size() >= num_vertices(g));
    return make_iterator_vertex_map(c.begin(), g);
  }

  template <class RandomAccessContainer> inline
  iterator_property_map<
    typename RandomAccessContainer::iterator,
    identity_property_map,
    typename RandomAccessContainer::value_type,
    typename RandomAccessContainer::reference
  >
  make_container_vertex_map(RandomAccessContainer& c)
  {
    return make_iterator_vertex_map(c.begin());
  }


#if HBOOST_WORKAROUND(__SUNPRO_CC, HBOOST_TESTED_AT(0x590)) && !defined (HBOOST_GRAPH_NO_BUNDLED_PROPERTIES)
// This compiler cannot define a partial specialization based on a
// pointer-to-member type, as seen in hboost/graph/subgraph.hpp line 985 (as of
// trunk r53912)
#  define HBOOST_GRAPH_NO_BUNDLED_PROPERTIES
#endif

// NOTE: These functions are declared, but never defined since they need to
// be overloaded by graph implementations. However, we need them to be
// declared for the functions below.
template<typename Graph, typename Tag>
typename graph_property<Graph, graph_bundle_t>::type&
get_property(Graph& g, Tag);

template<typename Graph, typename Tag>
typename graph_property<Graph, graph_bundle_t>::type const&
get_property(Graph const& g, Tag);

#ifndef HBOOST_GRAPH_NO_BUNDLED_PROPERTIES
// NOTE: This operation is a simple adaptor over the overloaded get_property
// operations.
template<typename Graph>
inline typename graph_property<Graph, graph_bundle_t>::type&
get_property(Graph& g) {
  return get_property(g, graph_bundle);
}

template<typename Graph>
inline typename graph_property<Graph, graph_bundle_t>::type const&
get_property(const Graph& g) {
  return get_property(g, graph_bundle);
}
#endif

} // namespace hboost

#endif /* HBOOST_GRAPH_PROPERTIES_HPP */
