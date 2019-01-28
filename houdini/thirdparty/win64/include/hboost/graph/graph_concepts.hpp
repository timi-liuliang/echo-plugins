//
//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Copyright 2009, Andrew Sutton
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//
#ifndef HBOOST_GRAPH_CONCEPTS_HPP
#define HBOOST_GRAPH_CONCEPTS_HPP

#include <hboost/config.hpp>
#include <hboost/property_map/property_map.hpp>
#include <hboost/graph/graph_traits.hpp>
#include <hboost/graph/properties.hpp>
#include <hboost/graph/numeric_values.hpp>
#include <hboost/graph/buffer_concepts.hpp>
#include <hboost/concept_check.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/concept/assert.hpp>

#include <hboost/concept/detail/concept_def.hpp>
namespace hboost
{
// dwa 2003/7/11 -- This clearly shouldn't be necessary, but if
// you want to use vector_as_graph, it is!  I'm sure the graph
// library leaves these out all over the place.  Probably a
// redesign involving specializing a template with a static
// member function is in order :(
//
// It is needed in order to allow us to write using hboost::vertices as
// needed for ADL when using vector_as_graph below.
#if !defined(HBOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)            \
 && !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
# define HBOOST_VECTOR_AS_GRAPH_GRAPH_ADL_HACK
#endif

#ifdef HBOOST_VECTOR_AS_GRAPH_GRAPH_ADL_HACK
template <class T>
typename T::ThereReallyIsNoMemberByThisNameInT vertices(T const&);
#endif

    namespace concepts {
    HBOOST_concept(MultiPassInputIterator,(T)) {
        HBOOST_CONCEPT_USAGE(MultiPassInputIterator) {
            HBOOST_CONCEPT_ASSERT((InputIterator<T>));
        }
    };

    HBOOST_concept(Graph,(G))
    {
        typedef typename graph_traits<G>::vertex_descriptor vertex_descriptor;
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
        typedef typename graph_traits<G>::directed_category directed_category;
        typedef typename graph_traits<G>::edge_parallel_category edge_parallel_category; 
        typedef typename graph_traits<G>::traversal_category traversal_category;

        HBOOST_CONCEPT_USAGE(Graph)
        {
            HBOOST_CONCEPT_ASSERT((DefaultConstructible<vertex_descriptor>));
            HBOOST_CONCEPT_ASSERT((EqualityComparable<vertex_descriptor>));
            HBOOST_CONCEPT_ASSERT((Assignable<vertex_descriptor>));
        }
        G g;
    };

    HBOOST_concept(IncidenceGraph,(G))
        : Graph<G>
    {
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
        typedef typename graph_traits<G>::out_edge_iterator out_edge_iterator;
        typedef typename graph_traits<G>::degree_size_type degree_size_type;
        typedef typename graph_traits<G>::traversal_category traversal_category;

        HBOOST_STATIC_ASSERT((hboost::mpl::not_<hboost::is_same<out_edge_iterator, void> >::value));
        HBOOST_STATIC_ASSERT((hboost::mpl::not_<hboost::is_same<degree_size_type, void> >::value));

        HBOOST_CONCEPT_USAGE(IncidenceGraph) {
            HBOOST_CONCEPT_ASSERT((MultiPassInputIterator<out_edge_iterator>));
            HBOOST_CONCEPT_ASSERT((DefaultConstructible<edge_descriptor>));
            HBOOST_CONCEPT_ASSERT((EqualityComparable<edge_descriptor>));
            HBOOST_CONCEPT_ASSERT((Assignable<edge_descriptor>));
            HBOOST_CONCEPT_ASSERT((Convertible<traversal_category,
                                    incidence_graph_tag>));

            p = out_edges(u, g);
            n = out_degree(u, g);
            e = *p.first;
            u = source(e, g);
            v = target(e, g);
            const_constraints(g);
        }
        void const_constraints(const G& cg) {
            p = out_edges(u, cg);
            n = out_degree(u, cg);
            e = *p.first;
            u = source(e, cg);
            v = target(e, cg);
        }
        std::pair<out_edge_iterator, out_edge_iterator> p;
        typename graph_traits<G>::vertex_descriptor u, v;
        typename graph_traits<G>::edge_descriptor e;
        typename graph_traits<G>::degree_size_type n;
        G g;
    };

    HBOOST_concept(BidirectionalGraph,(G))
        : IncidenceGraph<G>
    {
        typedef typename graph_traits<G>::in_edge_iterator
        in_edge_iterator;
        typedef typename graph_traits<G>::traversal_category
        traversal_category;

        HBOOST_CONCEPT_USAGE(BidirectionalGraph) {
        HBOOST_CONCEPT_ASSERT((MultiPassInputIterator<in_edge_iterator>));
        HBOOST_CONCEPT_ASSERT((Convertible<traversal_category,
            bidirectional_graph_tag>));

        HBOOST_STATIC_ASSERT((hboost::mpl::not_<hboost::is_same<in_edge_iterator, void> >::value));

        p = in_edges(v, g);
        n = in_degree(v, g);
        e = *p.first;
        const_constraints(g);
        }
        void const_constraints(const G& cg) {
        p = in_edges(v, cg);
        n = in_degree(v, cg);
        e = *p.first;
        }
        std::pair<in_edge_iterator, in_edge_iterator> p;
        typename graph_traits<G>::vertex_descriptor v;
        typename graph_traits<G>::edge_descriptor e;
        typename graph_traits<G>::degree_size_type n;
        G g;
    };

    HBOOST_concept(AdjacencyGraph,(G))
        : Graph<G>
    {
        typedef typename graph_traits<G>::adjacency_iterator
        adjacency_iterator;
        typedef typename graph_traits<G>::traversal_category
        traversal_category;

        HBOOST_CONCEPT_USAGE(AdjacencyGraph) {
        HBOOST_CONCEPT_ASSERT((MultiPassInputIterator<adjacency_iterator>));
        HBOOST_CONCEPT_ASSERT((Convertible<traversal_category,
            adjacency_graph_tag>));

        HBOOST_STATIC_ASSERT((hboost::mpl::not_<hboost::is_same<adjacency_iterator, void> >::value));

        p = adjacent_vertices(v, g);
        v = *p.first;
        const_constraints(g);
        }
        void const_constraints(const G& cg) {
        p = adjacent_vertices(v, cg);
        }
        std::pair<adjacency_iterator,adjacency_iterator> p;
        typename graph_traits<G>::vertex_descriptor v;
        G g;
    };

    HBOOST_concept(VertexListGraph,(G))
        : Graph<G>
    {
        typedef typename graph_traits<G>::vertex_iterator vertex_iterator;
        typedef typename graph_traits<G>::vertices_size_type vertices_size_type;
        typedef typename graph_traits<G>::traversal_category
        traversal_category;

        HBOOST_CONCEPT_USAGE(VertexListGraph) {
        HBOOST_CONCEPT_ASSERT((MultiPassInputIterator<vertex_iterator>));
        HBOOST_CONCEPT_ASSERT((Convertible<traversal_category,
            vertex_list_graph_tag>));

        HBOOST_STATIC_ASSERT((hboost::mpl::not_<hboost::is_same<vertex_iterator, void> >::value));
        HBOOST_STATIC_ASSERT((hboost::mpl::not_<hboost::is_same<vertices_size_type, void> >::value));

#ifdef HBOOST_VECTOR_AS_GRAPH_GRAPH_ADL_HACK
        // dwa 2003/7/11 -- This clearly shouldn't be necessary, but if
        // you want to use vector_as_graph, it is!  I'm sure the graph
        // library leaves these out all over the place.  Probably a
        // redesign involving specializing a template with a static
        // member function is in order :(
        using hboost::vertices;
#endif
        p = vertices(g);
        v = *p.first;
        const_constraints(g);
        }
        void const_constraints(const G& cg) {
#ifdef HBOOST_VECTOR_AS_GRAPH_GRAPH_ADL_HACK
        // dwa 2003/7/11 -- This clearly shouldn't be necessary, but if
        // you want to use vector_as_graph, it is!  I'm sure the graph
        // library leaves these out all over the place.  Probably a
        // redesign involving specializing a template with a static
        // member function is in order :(
        using hboost::vertices;
#endif

        p = vertices(cg);
        v = *p.first;
        V = num_vertices(cg);
        }
        std::pair<vertex_iterator,vertex_iterator> p;
        typename graph_traits<G>::vertex_descriptor v;
        G g;
        vertices_size_type V;
    };

    HBOOST_concept(EdgeListGraph,(G))
        : Graph<G>
    {
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
        typedef typename graph_traits<G>::edge_iterator edge_iterator;
        typedef typename graph_traits<G>::edges_size_type edges_size_type;
        typedef typename graph_traits<G>::traversal_category
        traversal_category;

        HBOOST_CONCEPT_USAGE(EdgeListGraph) {
        HBOOST_CONCEPT_ASSERT((MultiPassInputIterator<edge_iterator>));
        HBOOST_CONCEPT_ASSERT((DefaultConstructible<edge_descriptor>));
        HBOOST_CONCEPT_ASSERT((EqualityComparable<edge_descriptor>));
        HBOOST_CONCEPT_ASSERT((Assignable<edge_descriptor>));
        HBOOST_CONCEPT_ASSERT((Convertible<traversal_category,
            edge_list_graph_tag>));

        HBOOST_STATIC_ASSERT((hboost::mpl::not_<hboost::is_same<edge_iterator, void> >::value));
        HBOOST_STATIC_ASSERT((hboost::mpl::not_<hboost::is_same<edges_size_type, void> >::value));

        p = edges(g);
        e = *p.first;
        u = source(e, g);
        v = target(e, g);
        const_constraints(g);
        }
        void const_constraints(const G& cg) {
        p = edges(cg);
        E = num_edges(cg);
        e = *p.first;
        u = source(e, cg);
        v = target(e, cg);
        }
        std::pair<edge_iterator,edge_iterator> p;
        typename graph_traits<G>::vertex_descriptor u, v;
        typename graph_traits<G>::edge_descriptor e;
        edges_size_type E;
        G g;
    };

    HBOOST_concept(VertexAndEdgeListGraph,(G))
        : VertexListGraph<G>
        , EdgeListGraph<G>
    {
    };

    // Where to put the requirement for this constructor?
    //      G g(n_vertices);
    // Not in mutable graph, then LEDA graph's can't be models of
    // MutableGraph.
    HBOOST_concept(EdgeMutableGraph,(G))
    {
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;

        HBOOST_CONCEPT_USAGE(EdgeMutableGraph) {
        p = add_edge(u, v, g);
        remove_edge(u, v, g);
        remove_edge(e, g);
        clear_vertex(v, g);
        }
        G g;
        edge_descriptor e;
        std::pair<edge_descriptor, bool> p;
        typename graph_traits<G>::vertex_descriptor u, v;
    };

    HBOOST_concept(VertexMutableGraph,(G))
    {

        HBOOST_CONCEPT_USAGE(VertexMutableGraph) {
        v = add_vertex(g);
        remove_vertex(v, g);
        }
        G g;
        typename graph_traits<G>::vertex_descriptor u, v;
    };

    HBOOST_concept(MutableGraph,(G))
        : EdgeMutableGraph<G>
        , VertexMutableGraph<G>
    {
    };

    template <class edge_descriptor>
    struct dummy_edge_predicate {
        bool operator()(const edge_descriptor&) const {
        return false;
        }
    };

    HBOOST_concept(MutableIncidenceGraph,(G))
        : MutableGraph<G>
    {
        HBOOST_CONCEPT_USAGE(MutableIncidenceGraph) {
        remove_edge(iter, g);
        remove_out_edge_if(u, p, g);
        }
        G g;
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
        dummy_edge_predicate<edge_descriptor> p;
        typename hboost::graph_traits<G>::vertex_descriptor u;
        typename hboost::graph_traits<G>::out_edge_iterator iter;
    };

    HBOOST_concept(MutableBidirectionalGraph,(G))
        : MutableIncidenceGraph<G>
    {
        HBOOST_CONCEPT_USAGE(MutableBidirectionalGraph)
        {
            remove_in_edge_if(u, p, g);
        }
        G g;
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
        dummy_edge_predicate<edge_descriptor> p;
        typename hboost::graph_traits<G>::vertex_descriptor u;
    };

    HBOOST_concept(MutableEdgeListGraph,(G))
        : EdgeMutableGraph<G>
    {
        HBOOST_CONCEPT_USAGE(MutableEdgeListGraph) {
        remove_edge_if(p, g);
        }
        G g;
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;
        dummy_edge_predicate<edge_descriptor> p;
    };

    HBOOST_concept(VertexMutablePropertyGraph,(G))
        : VertexMutableGraph<G>
    {
        HBOOST_CONCEPT_USAGE(VertexMutablePropertyGraph) {
        v = add_vertex(vp, g);
        }
        G g;
        typename graph_traits<G>::vertex_descriptor v;
        typename vertex_property_type<G>::type vp;
    };

    HBOOST_concept(EdgeMutablePropertyGraph,(G))
        : EdgeMutableGraph<G>
    {
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;

        HBOOST_CONCEPT_USAGE(EdgeMutablePropertyGraph) {
        p = add_edge(u, v, ep, g);
        }
        G g;
        std::pair<edge_descriptor, bool> p;
        typename graph_traits<G>::vertex_descriptor u, v;
        typename edge_property_type<G>::type ep;
    };

    HBOOST_concept(AdjacencyMatrix,(G))
        : Graph<G>
    {
        typedef typename graph_traits<G>::edge_descriptor edge_descriptor;

        HBOOST_CONCEPT_USAGE(AdjacencyMatrix) {
        p = edge(u, v, g);
        const_constraints(g);
        }
        void const_constraints(const G& cg) {
        p = edge(u, v, cg);
        }
        typename graph_traits<G>::vertex_descriptor u, v;
        std::pair<edge_descriptor, bool> p;
        G g;
    };

    HBOOST_concept(ReadablePropertyGraph,(G)(X)(Property))
        : Graph<G>
    {
        typedef typename property_map<G, Property>::const_type const_Map;

        HBOOST_CONCEPT_USAGE(ReadablePropertyGraph)
        {
        HBOOST_CONCEPT_ASSERT((ReadablePropertyMapConcept<const_Map, X>));

        const_constraints(g);
        }
        void const_constraints(const G& cg) {
        const_Map pmap = get(Property(), cg);
        pval = get(Property(), cg, x);
        ignore_unused_variable_warning(pmap);
        }
        G g;
        X x;
        typename property_traits<const_Map>::value_type pval;
    };

    HBOOST_concept(PropertyGraph,(G)(X)(Property))
        : ReadablePropertyGraph<G, X, Property>
    {
        typedef typename property_map<G, Property>::type Map;
        HBOOST_CONCEPT_USAGE(PropertyGraph) {
        HBOOST_CONCEPT_ASSERT((ReadWritePropertyMapConcept<Map, X>));

        Map pmap = get(Property(), g);
        pval = get(Property(), g, x);
        put(Property(), g, x, pval);
        ignore_unused_variable_warning(pmap);
        }
        G g;
        X x;
        typename property_traits<Map>::value_type pval;
    };

    HBOOST_concept(LvaluePropertyGraph,(G)(X)(Property))
        : ReadablePropertyGraph<G, X, Property>
    {
        typedef typename property_map<G, Property>::type Map;
        typedef typename property_map<G, Property>::const_type const_Map;

        HBOOST_CONCEPT_USAGE(LvaluePropertyGraph) {
        HBOOST_CONCEPT_ASSERT((LvaluePropertyMapConcept<const_Map, X>));

        pval = get(Property(), g, x);
        put(Property(), g, x, pval);
        }
        G g;
        X x;
        typename property_traits<Map>::value_type pval;
    };

    // The *IndexGraph concepts are "semantic" graph concpepts. These can be
    // applied to describe any graph that has an index map that can be accessed
    // using the get(*_index, g) method. For example, adjacency lists with
    // VertexSet == vecS are implicitly models of this concept.
    //
    // NOTE: We could require an associated type vertex_index_type, but that
    // would mean propagating that type name into graph_traits and all of the
    // other graph implementations. Much easier to simply call it unsigned.

    HBOOST_concept(VertexIndexGraph,(Graph))
    {
        HBOOST_CONCEPT_USAGE(VertexIndexGraph)
        {
            typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
            typedef typename property_map<Graph, vertex_index_t>::type Map;
            typedef unsigned Index; // This could be Graph::vertex_index_type
            Map m = get(vertex_index, g);
            Index x = get(vertex_index, g, Vertex());
            ignore_unused_variable_warning(m);
            ignore_unused_variable_warning(x);

            // This is relaxed
            renumber_vertex_indices(g);

            const_constraints(g);
        }
        void const_constraints(const Graph& g_)
        {
            typedef typename property_map<Graph, vertex_index_t>::const_type Map;
            Map m = get(vertex_index, g_);
            ignore_unused_variable_warning(m);
        }
    private:
        Graph g;
    };

    HBOOST_concept(EdgeIndexGraph,(Graph))
    {
        HBOOST_CONCEPT_USAGE(EdgeIndexGraph)
        {
            typedef typename graph_traits<Graph>::edge_descriptor Edge;
            typedef typename property_map<Graph, edge_index_t>::type Map;
            typedef unsigned Index; // This could be Graph::vertex_index_type
            Map m = get(edge_index, g);
            Index x = get(edge_index, g, Edge());
            ignore_unused_variable_warning(m);
            ignore_unused_variable_warning(x);

            // This is relaxed
            renumber_edge_indices(g);

            const_constraints(g);
        }
        void const_constraints(const Graph& g_)
        {
            typedef typename property_map<Graph, edge_index_t>::const_type Map;
            Map m = get(edge_index, g_);
            ignore_unused_variable_warning(m);
        }
    private:
        Graph g;
    };

    HBOOST_concept(ColorValue,(C))
        : EqualityComparable<C>
        , DefaultConstructible<C>
    {
        HBOOST_CONCEPT_USAGE(ColorValue) {
        c = color_traits<C>::white();
        c = color_traits<C>::gray();
        c = color_traits<C>::black();
        }
        C c;
    };

    HBOOST_concept(BasicMatrix,(M)(I)(V))
    {
        HBOOST_CONCEPT_USAGE(BasicMatrix) {
        V& elt = A[i][j];
        const_constraints(A);
        ignore_unused_variable_warning(elt);
        }
        void const_constraints(const M& cA) {
        const V& elt = cA[i][j];
        ignore_unused_variable_warning(elt);
        }
        M A;
        I i, j;
    };

    // The following concepts describe aspects of numberic values and measure
    // functions. We're extending the notion of numeric values to include
    // emulation for zero and infinity.

    HBOOST_concept(NumericValue,(Numeric))
    {
        HBOOST_CONCEPT_USAGE(NumericValue)
        {
            HBOOST_CONCEPT_ASSERT(( DefaultConstructible<Numeric> ));
            HBOOST_CONCEPT_ASSERT(( CopyConstructible<Numeric> ));
            numeric_values<Numeric>::zero();
            numeric_values<Numeric>::infinity();
        }
    };

    HBOOST_concept(DegreeMeasure,(Measure)(Graph))
    {
        HBOOST_CONCEPT_USAGE(DegreeMeasure)
        {
            typedef typename Measure::degree_type Degree;
            typedef typename Measure::vertex_type Vertex;

            Degree d = m(Vertex(), g);
            ignore_unused_variable_warning(d);
        }
    private:
        Measure m;
        Graph g;
    };

    HBOOST_concept(DistanceMeasure,(Measure)(Graph))
    {
        HBOOST_CONCEPT_USAGE(DistanceMeasure)
        {
            typedef typename Measure::distance_type Distance;
            typedef typename Measure::result_type Result;
            Result r = m(Distance(), g);
            ignore_unused_variable_warning(r);
        }
    private:
        Measure m;
        Graph g;
    };

} /* namespace concepts */

using hboost::concepts::MultiPassInputIteratorConcept;

// Graph concepts
using hboost::concepts::GraphConcept;
using hboost::concepts::IncidenceGraphConcept;
using hboost::concepts::BidirectionalGraphConcept;
using hboost::concepts::AdjacencyGraphConcept;
using hboost::concepts::VertexListGraphConcept;
using hboost::concepts::EdgeListGraphConcept;
using hboost::concepts::VertexAndEdgeListGraphConcept;
using hboost::concepts::EdgeMutableGraphConcept;
using hboost::concepts::VertexMutableGraphConcept;
using hboost::concepts::MutableGraphConcept;
using hboost::concepts::MutableIncidenceGraphConcept;
using hboost::concepts::MutableBidirectionalGraphConcept;
using hboost::concepts::MutableEdgeListGraphConcept;
using hboost::concepts::VertexMutablePropertyGraphConcept;
using hboost::concepts::EdgeMutablePropertyGraphConcept;
using hboost::concepts::AdjacencyMatrixConcept;
using hboost::concepts::ReadablePropertyGraphConcept;
using hboost::concepts::PropertyGraphConcept;
using hboost::concepts::LvaluePropertyGraphConcept;
using hboost::concepts::VertexIndexGraphConcept;
using hboost::concepts::EdgeIndexGraphConcept;

// Utility concepts
using hboost::concepts::ColorValueConcept;
using hboost::concepts::BasicMatrixConcept;
using hboost::concepts::NumericValueConcept;
using hboost::concepts::DistanceMeasureConcept;
using hboost::concepts::DegreeMeasureConcept;


} /* namespace hboost */
#include <hboost/concept/detail/concept_undef.hpp>

#endif /* HBOOST_GRAPH_CONCEPTS_H */
