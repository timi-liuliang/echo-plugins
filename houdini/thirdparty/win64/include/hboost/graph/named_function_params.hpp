//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef HBOOST_GRAPH_NAMED_FUNCTION_PARAMS_HPP
#define HBOOST_GRAPH_NAMED_FUNCTION_PARAMS_HPP

#include <functional>
#include <vector>
#include <hboost/limits.hpp>
#include <hboost/ref.hpp>
#include <hboost/utility/result_of.hpp>
#include <hboost/preprocessor.hpp>
#include <hboost/parameter/name.hpp>
#include <hboost/parameter/binding.hpp>
#include <hboost/type_traits.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/graph/properties.hpp>
#include <hboost/graph/detail/d_ary_heap.hpp>
#include <hboost/property_map/property_map.hpp>
#include <hboost/property_map/shared_array_property_map.hpp>

namespace hboost {

  struct parity_map_t { };
  struct vertex_assignment_map_t { };
  struct distance_compare_t { };
  struct distance_combine_t { };
  struct distance_inf_t { };
  struct distance_zero_t { };
  struct buffer_param_t { };
  struct edge_copy_t { };
  struct vertex_copy_t { };
  struct vertex_isomorphism_t { };
  struct vertex_invariant_t { };
  struct vertex_invariant1_t { };
  struct vertex_invariant2_t { };
  struct edge_compare_t { };
  struct vertex_max_invariant_t { };
  struct orig_to_copy_t { };
  struct root_vertex_t { };
  struct polling_t { };
  struct lookahead_t { };
  struct in_parallel_t { };
  struct attractive_force_t { };
  struct repulsive_force_t { };
  struct force_pairs_t { };
  struct cooling_t { };
  struct vertex_displacement_t { };
  struct iterations_t { };
  struct diameter_range_t { };
  struct learning_constant_range_t { };
  struct vertices_equivalent_t { };
  struct edges_equivalent_t { };
  struct index_in_heap_map_t { };
  struct max_priority_queue_t { };

#define HBOOST_BGL_DECLARE_NAMED_PARAMS \
    HBOOST_BGL_ONE_PARAM_CREF(weight_map, edge_weight) \
    HBOOST_BGL_ONE_PARAM_CREF(weight_map2, edge_weight2) \
    HBOOST_BGL_ONE_PARAM_CREF(distance_map, vertex_distance) \
    HBOOST_BGL_ONE_PARAM_CREF(distance_map2, vertex_distance2) \
    HBOOST_BGL_ONE_PARAM_CREF(predecessor_map, vertex_predecessor) \
    HBOOST_BGL_ONE_PARAM_CREF(rank_map, vertex_rank) \
    HBOOST_BGL_ONE_PARAM_CREF(root_map, vertex_root) \
    HBOOST_BGL_ONE_PARAM_CREF(root_vertex, root_vertex) \
    HBOOST_BGL_ONE_PARAM_CREF(edge_centrality_map, edge_centrality) \
    HBOOST_BGL_ONE_PARAM_CREF(centrality_map, vertex_centrality) \
    HBOOST_BGL_ONE_PARAM_CREF(parity_map, parity_map) \
    HBOOST_BGL_ONE_PARAM_CREF(color_map, vertex_color) \
    HBOOST_BGL_ONE_PARAM_CREF(edge_color_map, edge_color) \
    HBOOST_BGL_ONE_PARAM_CREF(capacity_map, edge_capacity) \
    HBOOST_BGL_ONE_PARAM_CREF(residual_capacity_map, edge_residual_capacity) \
    HBOOST_BGL_ONE_PARAM_CREF(reverse_edge_map, edge_reverse) \
    HBOOST_BGL_ONE_PARAM_CREF(discover_time_map, vertex_discover_time) \
    HBOOST_BGL_ONE_PARAM_CREF(lowpoint_map, vertex_lowpoint) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_index_map, vertex_index) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_index1_map, vertex_index1) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_index2_map, vertex_index2) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_assignment_map, vertex_assignment_map) \
    HBOOST_BGL_ONE_PARAM_CREF(visitor, graph_visitor) \
    HBOOST_BGL_ONE_PARAM_CREF(distance_compare, distance_compare) \
    HBOOST_BGL_ONE_PARAM_CREF(distance_combine, distance_combine) \
    HBOOST_BGL_ONE_PARAM_CREF(distance_inf, distance_inf) \
    HBOOST_BGL_ONE_PARAM_CREF(distance_zero, distance_zero) \
    HBOOST_BGL_ONE_PARAM_CREF(edge_copy, edge_copy) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_copy, vertex_copy) \
    HBOOST_BGL_ONE_PARAM_REF(buffer, buffer_param) \
    HBOOST_BGL_ONE_PARAM_CREF(orig_to_copy, orig_to_copy) \
    HBOOST_BGL_ONE_PARAM_CREF(isomorphism_map, vertex_isomorphism) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_invariant, vertex_invariant) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_invariant1, vertex_invariant1) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_invariant2, vertex_invariant2) \
    HBOOST_BGL_ONE_PARAM_CREF(vertex_max_invariant, vertex_max_invariant) \
    HBOOST_BGL_ONE_PARAM_CREF(polling, polling) \
    HBOOST_BGL_ONE_PARAM_CREF(lookahead, lookahead) \
    HBOOST_BGL_ONE_PARAM_CREF(in_parallel, in_parallel) \
    HBOOST_BGL_ONE_PARAM_CREF(displacement_map, vertex_displacement) \
    HBOOST_BGL_ONE_PARAM_CREF(attractive_force, attractive_force) \
    HBOOST_BGL_ONE_PARAM_CREF(repulsive_force, repulsive_force) \
    HBOOST_BGL_ONE_PARAM_CREF(force_pairs, force_pairs) \
    HBOOST_BGL_ONE_PARAM_CREF(cooling, cooling) \
    HBOOST_BGL_ONE_PARAM_CREF(iterations, iterations) \
    HBOOST_BGL_ONE_PARAM_CREF(diameter_range, diameter_range) \
    HBOOST_BGL_ONE_PARAM_CREF(learning_constant_range, learning_constant_range) \
    HBOOST_BGL_ONE_PARAM_CREF(vertices_equivalent, vertices_equivalent) \
    HBOOST_BGL_ONE_PARAM_CREF(edges_equivalent, edges_equivalent) \
    HBOOST_BGL_ONE_PARAM_CREF(index_in_heap_map, index_in_heap_map) \
    HBOOST_BGL_ONE_PARAM_REF(max_priority_queue, max_priority_queue)

  template <typename T, typename Tag, typename Base = no_property>
  struct bgl_named_params
  {
    typedef bgl_named_params self;
    typedef Base next_type;
    typedef Tag tag_type;
    typedef T value_type;
    bgl_named_params(T v = T()) : m_value(v) { }
    bgl_named_params(T v, const Base& b) : m_value(v), m_base(b) { }
    T m_value;
    Base m_base;

#define HBOOST_BGL_ONE_PARAM_REF(name, key) \
    template <typename PType> \
    bgl_named_params<hboost::reference_wrapper<PType>, HBOOST_PP_CAT(key, _t), self> \
    name(PType& p) const { \
      typedef bgl_named_params<hboost::reference_wrapper<PType>, HBOOST_PP_CAT(key, _t), self> Params; \
      return Params(hboost::ref(p), *this); \
    } \

#define HBOOST_BGL_ONE_PARAM_CREF(name, key) \
    template <typename PType> \
    bgl_named_params<PType, HBOOST_PP_CAT(key, _t), self> \
    name(const PType& p) const { \
      typedef bgl_named_params<PType, HBOOST_PP_CAT(key, _t), self> Params; \
      return Params(p, *this); \
    } \

HBOOST_BGL_DECLARE_NAMED_PARAMS

#undef HBOOST_BGL_ONE_PARAM_REF
#undef HBOOST_BGL_ONE_PARAM_CREF

    // Duplicate
    template <typename PType>
    bgl_named_params<PType, vertex_color_t, self>
    vertex_color_map(const PType& p) const {return this->color_map(p);}
  };

#define HBOOST_BGL_ONE_PARAM_REF(name, key) \
    template <typename PType> \
    bgl_named_params<hboost::reference_wrapper<PType>, HBOOST_PP_CAT(key, _t)> \
    name(PType& p) { \
      typedef bgl_named_params<hboost::reference_wrapper<PType>, HBOOST_PP_CAT(key, _t)> Params; \
      return Params(hboost::ref(p)); \
    } \

#define HBOOST_BGL_ONE_PARAM_CREF(name, key) \
    template <typename PType> \
    bgl_named_params<PType, HBOOST_PP_CAT(key, _t)> \
    name(const PType& p) { \
      typedef bgl_named_params<PType, HBOOST_PP_CAT(key, _t)> Params; \
      return Params(p); \
    } \

HBOOST_BGL_DECLARE_NAMED_PARAMS

#undef HBOOST_BGL_ONE_PARAM_REF
#undef HBOOST_BGL_ONE_PARAM_CREF

  // Duplicate
  template <typename PType>
  bgl_named_params<PType, vertex_color_t>
  vertex_color_map(const PType& p) {return color_map(p);}

  namespace detail {
    struct unused_tag_type {};
  }
  typedef bgl_named_params<char, detail::unused_tag_type> no_named_parameters;

  //===========================================================================
  // Functions for extracting parameters from bgl_named_params

  template <typename Tag, typename Args>
  struct lookup_named_param {};

  template <typename T, typename Tag, typename Base>
  struct lookup_named_param<Tag, bgl_named_params<T, Tag, Base> > {
    typedef T type;
    static const T& get(const bgl_named_params<T, Tag, Base>& p) {
      return p.m_value;
    }
  };

  template <typename Tag1, typename T, typename Tag, typename Base>
  struct lookup_named_param<Tag1, bgl_named_params<T, Tag, Base> > {
    typedef typename lookup_named_param<Tag1, Base>::type type;
    static const type& get(const bgl_named_params<T, Tag, Base>& p) {
      return lookup_named_param<Tag1, Base>::get(p.m_base);
    }
  };

  template <typename Tag, typename Args, typename Def>
  struct lookup_named_param_def {
    typedef Def type;
    static const Def& get(const Args&, const Def& def) {return def;}
  };

  template <typename T, typename Tag, typename Base, typename Def>
  struct lookup_named_param_def<Tag, bgl_named_params<T, Tag, Base>, Def> {
    typedef T type;
    static const type& get(const bgl_named_params<T, Tag, Base>& p, const Def&) {
      return p.m_value;
    }
  };

  template <typename Tag1, typename T, typename Tag, typename Base, typename Def>
  struct lookup_named_param_def<Tag1, bgl_named_params<T, Tag, Base>, Def> {
    typedef typename lookup_named_param_def<Tag1, Base, Def>::type type;
    static const type& get(const bgl_named_params<T, Tag, Base>& p, const Def& def) {
      return lookup_named_param_def<Tag1, Base, Def>::get(p.m_base, def);
    }
  };

  struct param_not_found {};

  template <typename Tag, typename Args>
  struct get_param_type: 
    lookup_named_param_def<Tag, Args, param_not_found> {};

  template <class Tag, typename Args>
  inline
  const typename lookup_named_param_def<Tag, Args, param_not_found>::type&
  get_param(const Args& p, Tag) {
    return lookup_named_param_def<Tag, Args, param_not_found>::get(p, param_not_found());
  }

  template <class P, class Default> 
  const P& choose_param(const P& param, const Default&) { 
    return param;
  }

  template <class Default>
  Default choose_param(const param_not_found&, const Default& d) {
    return d;
  }

  template <typename T>
  inline bool is_default_param(const T&) { return false; }

  inline bool is_default_param(const param_not_found&)
    { return true; }

  namespace detail {
    template <typename T>
    struct const_type_as_type {typedef typename T::const_type type;};
  } // namespace detail
  

  // Use this function instead of choose_param() when you want
  // to avoid requiring get(tag, g) when it is not used. 
  namespace detail {
    template <typename GraphIsConst, typename Graph, typename Param, typename Tag>
    struct choose_impl_result:
      hboost::mpl::eval_if<
        hboost::is_same<Param, param_not_found>, 
        hboost::mpl::eval_if<
          GraphIsConst,
          detail::const_type_as_type<property_map<Graph, Tag> >,
          property_map<Graph, Tag> >,
        hboost::mpl::identity<Param> > {};

    // Parameters of f are (GraphIsConst, Graph, Param, Tag)
    template <bool Found> struct choose_impl_helper;

    template <> struct choose_impl_helper<false> {
      template <typename Param, typename Graph, typename PropertyTag>
      static typename property_map<typename hboost::remove_const<Graph>::type, PropertyTag>::const_type
      f(hboost::mpl::true_, const Graph& g, const Param&, PropertyTag tag) {
        return get(tag, g);
      }

      template <typename Param, typename Graph, typename PropertyTag>
      static typename property_map<typename hboost::remove_const<Graph>::type, PropertyTag>::type
      f(hboost::mpl::false_, Graph& g, const Param&, PropertyTag tag) {
        return get(tag, g);
      }
    };

    template <> struct choose_impl_helper<true> {
      template <typename GraphIsConst, typename Param, typename Graph, typename PropertyTag>
      static Param f(GraphIsConst, const Graph&, const Param& p, PropertyTag) {
        return p;
      }
    };
  }

  template <typename Param, typename Graph, typename PropertyTag>
  typename detail::choose_impl_result<hboost::mpl::true_, Graph, Param, PropertyTag>::type
  choose_const_pmap(const Param& p, const Graph& g, PropertyTag tag)
  { 
    return detail::choose_impl_helper<!hboost::is_same<Param, param_not_found>::value>
             ::f(hboost::mpl::true_(), g, p, tag);
  }

  template <typename Param, typename Graph, typename PropertyTag>
  typename detail::choose_impl_result<hboost::mpl::false_, Graph, Param, PropertyTag>::type
  choose_pmap(const Param& p, Graph& g, PropertyTag tag)
  { 
    return detail::choose_impl_helper<!hboost::is_same<Param, param_not_found>::value>
             ::f(hboost::mpl::false_(), g, p, tag);
  }

  namespace detail {

    // used in the max-flow algorithms
    template <class Graph, class P, class T, class R>
    struct edge_capacity_value
    {
      typedef bgl_named_params<P, T, R> Params;
      typedef typename detail::choose_impl_result<hboost::mpl::true_, Graph, typename get_param_type<Params, edge_capacity_t>::type, edge_capacity_t>::type CapacityEdgeMap;
      typedef typename property_traits<CapacityEdgeMap>::value_type type;
    };

  }

  // Declare all new tags
  namespace graph {
    namespace keywords {
#define HBOOST_BGL_ONE_PARAM_REF(name, key) HBOOST_PARAMETER_NAME(name)
#define HBOOST_BGL_ONE_PARAM_CREF(name, key) HBOOST_PARAMETER_NAME(name)
      HBOOST_BGL_DECLARE_NAMED_PARAMS
#undef HBOOST_BGL_ONE_PARAM_REF
#undef HBOOST_BGL_ONE_PARAM_CREF
    }
  }

  namespace detail {
    template <typename Tag> struct convert_one_keyword {};
#define HBOOST_BGL_ONE_PARAM_REF(name, key) \
    template <> \
    struct convert_one_keyword<HBOOST_PP_CAT(key, _t)> { \
      typedef hboost::graph::keywords::tag::name type; \
    };
#define HBOOST_BGL_ONE_PARAM_CREF(name, key) HBOOST_BGL_ONE_PARAM_REF(name, key)
    HBOOST_BGL_DECLARE_NAMED_PARAMS
#undef HBOOST_BGL_ONE_PARAM_REF
#undef HBOOST_BGL_ONE_PARAM_CREF

    template <typename T>
    struct convert_bgl_params_to_hboost_parameter {
      typedef typename convert_one_keyword<typename T::tag_type>::type new_kw;
      typedef hboost::parameter::aux::tagged_argument<new_kw, const typename T::value_type> tagged_arg_type;
      typedef convert_bgl_params_to_hboost_parameter<typename T::next_type> rest_conv;
      typedef hboost::parameter::aux::arg_list<tagged_arg_type, typename rest_conv::type> type;
      static type conv(const T& x) {
        return type(tagged_arg_type(x.m_value), rest_conv::conv(x.m_base));
      }
    };

    template <typename P, typename R>
    struct convert_bgl_params_to_hboost_parameter<bgl_named_params<P, int, R> > {
      typedef convert_bgl_params_to_hboost_parameter<R> rest_conv;
      typedef typename rest_conv::type type;
      static type conv(const bgl_named_params<P, int, R>& x) {
        return rest_conv::conv(x.m_base);
      }
    };

    template <>
    struct convert_bgl_params_to_hboost_parameter<hboost::no_property> {
      typedef hboost::parameter::aux::empty_arg_list type;
      static type conv(const hboost::no_property&) {return type();}
    };

    template <>
    struct convert_bgl_params_to_hboost_parameter<hboost::no_named_parameters> {
      typedef hboost::parameter::aux::empty_arg_list type;
      static type conv(const hboost::no_named_parameters&) {return type();}
    };

    struct bgl_parameter_not_found_type {};

    template <typename ArgPack, typename KeywordType>
    struct parameter_exists : hboost::mpl::not_<hboost::is_same<typename hboost::parameter::binding<ArgPack, KeywordType, bgl_parameter_not_found_type>::type, bgl_parameter_not_found_type> > {};
  }

#define HBOOST_GRAPH_DECLARE_CONVERTED_PARAMETERS(old_type, old_var) \
  typedef typename hboost::detail::convert_bgl_params_to_hboost_parameter<old_type>::type arg_pack_type; \
  arg_pack_type arg_pack = hboost::detail::convert_bgl_params_to_hboost_parameter<old_type>::conv(old_var);

  namespace detail {

    template <typename ArgType, typename Prop, typename Graph, bool Exists>
    struct override_const_property_t {
      typedef typename hboost::remove_const<ArgType>::type result_type;
      result_type operator()(const Graph&, const ArgType& a) const {return a;}
    };

    template <typename ArgType, typename Prop, typename Graph>
    struct override_const_property_t<ArgType, Prop, Graph, false> {
      typedef typename hboost::property_map<Graph, Prop>::const_type result_type;
      result_type operator()(const Graph& g, const ArgType&) const {return get(Prop(), g);}
    };

    template <typename ArgPack, typename Tag, typename Prop, typename Graph>
    struct override_const_property_result {
      typedef 
        typename override_const_property_t<
                   typename hboost::parameter::value_type<ArgPack, Tag, int>::type,
                   Prop,
                   Graph,
                   hboost::detail::parameter_exists<ArgPack, Tag>::value
                 >::result_type
        type;
    };

    template <typename ArgPack, typename Tag, typename Prop, typename Graph>
    typename override_const_property_result<ArgPack, Tag, Prop, Graph>::type
    override_const_property(const ArgPack& ap, const hboost::parameter::keyword<Tag>& t, const Graph& g, Prop) {
    return override_const_property_t<
             typename hboost::parameter::value_type<ArgPack, Tag, int>::type,
             Prop,
             Graph,
             hboost::detail::parameter_exists<ArgPack, Tag>::value
           >()(g, ap[t | 0]);
    }

    template <typename ArgType, typename Prop, typename Graph, bool Exists>
    struct override_property_t {
      typedef ArgType result_type;
      result_type operator()(const Graph&, const typename hboost::add_reference<ArgType>::type a) const {return a;}
    };

    template <typename ArgType, typename Prop, typename Graph>
    struct override_property_t<ArgType, Prop, Graph, false> {
      typedef typename hboost::property_map<Graph, Prop>::type result_type;
      result_type operator()(const Graph& g, const ArgType&) const {return get(Prop(), g);}
    };

    template <typename ArgPack, typename Tag, typename Prop, typename Graph>
    struct override_property_result {
      typedef 
        typename override_property_t<
                   typename hboost::parameter::value_type<ArgPack, Tag, int>::type,
                   Prop,
                   Graph,
                   hboost::detail::parameter_exists<ArgPack, Tag>::value
                 >::result_type
        type;
    };

    template <typename ArgPack, typename Tag, typename Prop, typename Graph>
    typename override_property_result<ArgPack, Tag, Prop, Graph>::type
    override_property(const ArgPack& ap, const hboost::parameter::keyword<Tag>& t, const Graph& g, Prop) {
    return override_property_t<
             typename hboost::parameter::value_type<ArgPack, Tag, int>::type,
             Prop,
             Graph,
             hboost::detail::parameter_exists<ArgPack, Tag>::value
           >()(g, ap[t | 0]);
    }

    template <typename F> struct make_arg_pack_type;
    template <> struct make_arg_pack_type<void()> {typedef hboost::parameter::aux::empty_arg_list type;};
    template <typename K, typename A>
    struct make_arg_pack_type<void(K, A)> {
      typedef hboost::parameter::aux::tagged_argument<K, A> type;
    };

#define HBOOST_GRAPH_OPENING_PART_OF_PAIR(z, i, n) hboost::parameter::aux::arg_list<hboost::parameter::aux::tagged_argument<HBOOST_PP_CAT(Keyword, HBOOST_PP_SUB(n, i)),  HBOOST_PP_CAT(Arg, HBOOST_PP_SUB(n, i))>,
#define HBOOST_GRAPH_MAKE_PAIR_PARAM(z, i, _) const hboost::parameter::aux::tagged_argument<HBOOST_PP_CAT(Keyword, i), HBOOST_PP_CAT(Arg, i)>& HBOOST_PP_CAT(kw, i)

#define HBOOST_GRAPH_MAKE_AP_TYPE_SPECIALIZATION(z, i, _) \
    template <HBOOST_PP_ENUM_PARAMS(i, typename Keyword), HBOOST_PP_ENUM_PARAMS(i, typename Arg)> \
    struct make_arg_pack_type<void(HBOOST_PP_ENUM_PARAMS(i, Keyword), HBOOST_PP_ENUM_PARAMS(i, Arg))> { \
      typedef \
        HBOOST_PP_REPEAT(i, HBOOST_GRAPH_OPENING_PART_OF_PAIR, HBOOST_PP_DEC(i)) hboost::parameter::aux::empty_arg_list HBOOST_PP_REPEAT(i, > HBOOST_PP_TUPLE_EAT(3), ~) \
        type; \
    };
    HBOOST_PP_REPEAT_FROM_TO(2, 11, HBOOST_GRAPH_MAKE_AP_TYPE_SPECIALIZATION, ~)
#undef HBOOST_GRAPH_MAKE_AP_TYPE_SPECIALIZATION

#define HBOOST_GRAPH_MAKE_FORWARDING_FUNCTION(name, nfixed, nnamed_max) \
  /* Entry point for conversion from BGL-style named parameters */ \
  template <HBOOST_PP_ENUM_PARAMS(nfixed, typename Param) HBOOST_PP_COMMA_IF(nfixed) typename ArgPack> \
  typename hboost::result_of< \
             detail::HBOOST_PP_CAT(name, _impl)<HBOOST_PP_ENUM_PARAMS(nfixed, Param)>(HBOOST_PP_ENUM_PARAMS(nfixed, Param) HBOOST_PP_COMMA_IF(nfixed) const ArgPack&) \
           >::type \
  HBOOST_PP_CAT(name, _with_named_params)(HBOOST_PP_ENUM_BINARY_PARAMS(nfixed, const Param, & param) HBOOST_PP_COMMA_IF(nfixed) const ArgPack& arg_pack) { \
    return detail::HBOOST_PP_CAT(name, _impl)<HBOOST_PP_ENUM_PARAMS(nfixed, Param)>()(HBOOST_PP_ENUM_PARAMS(nfixed, param) HBOOST_PP_COMMA_IF(nfixed) arg_pack); \
  } \
  /* Individual functions taking Boost.Parameter-style keyword arguments */ \
  HBOOST_PP_REPEAT(HBOOST_PP_INC(nnamed_max), HBOOST_GRAPH_MAKE_FORWARDING_FUNCTION_ONE, (name)(nfixed))

#define HBOOST_GRAPH_MAKE_FORWARDING_FUNCTION_ONE(z, nnamed, seq) \
  HBOOST_GRAPH_MAKE_FORWARDING_FUNCTION_ONEX(z, nnamed, HBOOST_PP_SEQ_ELEM(0, seq), HBOOST_PP_SEQ_ELEM(1, seq))

#define HBOOST_GRAPH_MAKE_FORWARDING_FUNCTION_ONEX(z, nnamed, name, nfixed) \
  template <HBOOST_PP_ENUM_PARAMS(nfixed, typename Param) HBOOST_PP_ENUM_TRAILING_PARAMS(nnamed, typename Keyword) HBOOST_PP_ENUM_TRAILING_PARAMS(nnamed, typename Arg)> \
  typename hboost::result_of< \
             detail::HBOOST_PP_CAT(name, _impl)<HBOOST_PP_ENUM_PARAMS(nfixed, Param)> \
               (HBOOST_PP_ENUM_PARAMS(nfixed, Param) HBOOST_PP_COMMA_IF(nfixed) \
                const typename hboost::detail::make_arg_pack_type<void(HBOOST_PP_ENUM_PARAMS(nnamed, Keyword) HBOOST_PP_COMMA_IF(nnamed) HBOOST_PP_ENUM_PARAMS(nnamed, Arg))>::type&) \
           >::type \
  name(HBOOST_PP_ENUM_BINARY_PARAMS(nfixed, const Param, & param) \
       HBOOST_PP_ENUM_TRAILING(nnamed, HBOOST_GRAPH_MAKE_PAIR_PARAM, ~)) { \
    return detail::HBOOST_PP_CAT(name, _impl)<HBOOST_PP_ENUM_PARAMS(nfixed, Param)>() \
             (HBOOST_PP_ENUM_PARAMS(nfixed, param), \
              (hboost::parameter::aux::empty_arg_list() HBOOST_PP_ENUM_TRAILING_PARAMS(nnamed, kw))); \
  }

#define HBOOST_GRAPH_MAKE_OLD_STYLE_PARAMETER_FUNCTION(name, nfixed) \
  template <HBOOST_PP_ENUM_PARAMS(nfixed, typename Param) HBOOST_PP_COMMA_IF(nfixed) class P, class T, class R> \
  typename hboost::result_of< \
    ::hboost::graph::detail::HBOOST_PP_CAT(name, _impl) HBOOST_PP_EXPR_IF(nfixed, <) HBOOST_PP_ENUM_PARAMS(nfixed, Param) HBOOST_PP_EXPR_IF(nfixed, >) \
      (HBOOST_PP_ENUM_PARAMS(nfixed, Param) HBOOST_PP_COMMA_IF(nfixed) \
       const typename hboost::detail::convert_bgl_params_to_hboost_parameter<hboost::bgl_named_params<P, T, R> >::type &) \
    >::type \
  name(HBOOST_PP_ENUM_BINARY_PARAMS(nfixed, const Param, & param) HBOOST_PP_COMMA_IF(nfixed) const hboost::bgl_named_params<P, T, R>& old_style_params) { \
    typedef hboost::bgl_named_params<P, T, R> old_style_params_type; \
    HBOOST_GRAPH_DECLARE_CONVERTED_PARAMETERS(old_style_params_type, old_style_params) \
    return ::hboost::graph::HBOOST_PP_CAT(name, _with_named_params)(HBOOST_PP_ENUM_PARAMS(nfixed, param) HBOOST_PP_COMMA_IF(nfixed) arg_pack); \
  } \
  \
  HBOOST_PP_EXPR_IF(nfixed, template <) HBOOST_PP_ENUM_PARAMS(nfixed, typename Param) HBOOST_PP_EXPR_IF(nfixed, >) \
  HBOOST_PP_EXPR_IF(nfixed, typename) hboost::result_of< \
    ::hboost::graph::detail::HBOOST_PP_CAT(name, _impl) HBOOST_PP_EXPR_IF(nfixed, <) HBOOST_PP_ENUM_PARAMS(nfixed, Param) HBOOST_PP_EXPR_IF(nfixed, >) \
      (HBOOST_PP_ENUM_PARAMS(nfixed, Param) HBOOST_PP_COMMA_IF(nfixed) const hboost::parameter::aux::empty_arg_list &) \
    >::type \
  name(HBOOST_PP_ENUM_BINARY_PARAMS(nfixed, const Param, & param)) { \
    HBOOST_GRAPH_DECLARE_CONVERTED_PARAMETERS(hboost::no_named_parameters, hboost::no_named_parameters()) \
    return ::hboost::graph::HBOOST_PP_CAT(name, _with_named_params)(HBOOST_PP_ENUM_PARAMS(nfixed, param) HBOOST_PP_COMMA_IF(nfixed) arg_pack); \
  }

  }

  namespace detail {

    template <bool Exists, typename Graph, typename ArgPack, typename Value, typename PM>
    struct map_maker_helper {
      typedef PM map_type;
      static PM make_map(const Graph&, Value, const PM& pm, const ArgPack&) {
        return pm;
      }
    };

    template <typename Graph, typename ArgPack, typename Value, typename PM>
    struct map_maker_helper<false, Graph, ArgPack, Value, PM> {
      typedef typename hboost::remove_const<
        typename override_const_property_t<
          typename hboost::parameter::value_type<
            ArgPack, hboost::graph::keywords::tag::vertex_index_map, int>::type,
          hboost::vertex_index_t,
          Graph,
          hboost::detail::parameter_exists<
            ArgPack, hboost::graph::keywords::tag::vertex_index_map>::value
        >::result_type>::type vi_map_type;
      typedef
        hboost::shared_array_property_map<Value, vi_map_type>
        map_type;
      static map_type make_map(const Graph& g,
                               Value v,
                               const PM&,
                               const ArgPack& ap) {
        return make_shared_array_property_map(
                 num_vertices(g), 
                 v,
                 override_const_property(
                   ap,
                   hboost::graph::keywords::_vertex_index_map,
                   g, vertex_index));
      }
    };

    template <typename Graph, typename ArgPack, typename MapTag, typename ValueType>
    struct map_maker {
      HBOOST_STATIC_CONSTANT(
        bool,
        has_map =
          (parameter_exists<ArgPack, MapTag>
           ::value));
      typedef map_maker_helper<has_map, Graph, ArgPack, ValueType,
                               typename hboost::remove_const<
                                 typename hboost::parameter::value_type<
                                            ArgPack,
                                            MapTag,
                                            int
                                          >::type
                                        >::type> helper;
      typedef typename helper::map_type map_type;
      static map_type make_map(const Graph& g, const ArgPack& ap, ValueType default_value) {
        return helper::make_map(g, default_value, ap[::hboost::parameter::keyword<MapTag>::instance | 0], ap);
      }
    };

    template <typename MapTag, typename ValueType = void>
    class make_property_map_from_arg_pack_gen {
      ValueType default_value;

      public:
      make_property_map_from_arg_pack_gen(ValueType default_value)
        : default_value(default_value) {}

      template <typename Graph, typename ArgPack>
      typename map_maker<Graph, ArgPack, MapTag, ValueType>::map_type
      operator()(const Graph& g, const ArgPack& ap) const {
        return map_maker<Graph, ArgPack, MapTag, ValueType>::make_map(g, ap, default_value);
      }
    };

    template <typename MapTag>
    class make_property_map_from_arg_pack_gen<MapTag, void> {
      public:
      template <typename ValueType, typename Graph, typename ArgPack>
      typename map_maker<Graph, ArgPack, MapTag, ValueType>::map_type
      operator()(const Graph& g, const ArgPack& ap, ValueType default_value) const {
        return map_maker<Graph, ArgPack, MapTag, ValueType>::make_map(g, ap, default_value);
      }
    };

    static const
      make_property_map_from_arg_pack_gen<
        hboost::graph::keywords::tag::color_map,
        default_color_type>
      make_color_map_from_arg_pack(white_color);

    template <bool Exists, class Graph, class ArgPack, class KeyT, class ValueT, class KeyMapTag, class IndexInHeapMapTag, class Compare, class Q>
    struct priority_queue_maker_helper {
      typedef Q priority_queue_type;

      static priority_queue_type
      make_queue(const Graph&, const ArgPack&, KeyT, const Q& q) {
        return q;
      }
    };

    template <class Graph, class ArgPack, class KeyT, class ValueT, class KeyMapTag, class IndexInHeapMapTag, class Compare, class Q>
    struct priority_queue_maker_helper<false, Graph, ArgPack, KeyT, ValueT, KeyMapTag, IndexInHeapMapTag, Compare, Q> {
      typedef typename std::vector<ValueT>::size_type default_index_in_heap_type;
      typedef typename map_maker<Graph, ArgPack, IndexInHeapMapTag, default_index_in_heap_type>::helper::map_type index_in_heap_map;
      typedef hboost::d_ary_heap_indirect<ValueT, 4, index_in_heap_map, typename map_maker<Graph, ArgPack, KeyMapTag, KeyT>::helper::map_type, Compare> priority_queue_type;

      static priority_queue_type
      make_queue(const Graph& g, const ArgPack& ap, KeyT defaultKey, const Q&) {
        return priority_queue_type(
            map_maker<Graph, ArgPack, KeyMapTag, KeyT>::make_map(g, ap, defaultKey),
            map_maker<Graph, ArgPack, IndexInHeapMapTag, default_index_in_heap_type>::make_map(g, ap, typename hboost::property_traits<index_in_heap_map>::value_type(-1))
          );
      }
    };

    template <class Graph, class ArgPack, class KeyT, class ValueT, class PriorityQueueTag, class KeyMapTag, class IndexInHeapMapTag, class Compare>
    struct priority_queue_maker {
      HBOOST_STATIC_CONSTANT(
        bool,
        g_hasQ =
          (parameter_exists<ArgPack, PriorityQueueTag>
           ::value));
      typedef hboost::reference_wrapper<int> int_refw;
      typedef typename hboost::parameter::value_type<
                         ArgPack,
                         PriorityQueueTag,
                         int_refw
                       >::type
        param_value_type_wrapper;
      typedef typename param_value_type_wrapper::type
        param_value_type;
      typedef typename hboost::remove_const<param_value_type>::type param_value_type_no_const;
      typedef priority_queue_maker_helper<g_hasQ, Graph, ArgPack, KeyT, ValueT, KeyMapTag, IndexInHeapMapTag, Compare,
                                          param_value_type_no_const> helper;
      typedef typename helper::priority_queue_type priority_queue_type;

      static priority_queue_type make_queue(const Graph& g, const ArgPack& ap, KeyT defaultKey) {
        return helper::make_queue(g, ap, defaultKey, ap[::hboost::parameter::keyword<PriorityQueueTag>::instance | 0]);
      }
    };

    template <class PriorityQueueTag, class KeyT, class ValueT, class Compare = std::less<KeyT>, class KeyMapTag = hboost::graph::keywords::tag::distance_map, class IndexInHeapMapTag = hboost::graph::keywords::tag::index_in_heap_map>
    struct make_priority_queue_from_arg_pack_gen {
      KeyT defaultKey;

      make_priority_queue_from_arg_pack_gen(KeyT defaultKey_) : defaultKey(defaultKey_) { }

      template <class F>
      struct result {
        typedef typename remove_const<typename remove_reference<typename function_traits<F>::arg1_type>::type>::type graph_type;
        typedef typename remove_const<typename remove_reference<typename function_traits<F>::arg2_type>::type>::type arg_pack_type;
        typedef typename priority_queue_maker<graph_type, arg_pack_type, KeyT, ValueT, PriorityQueueTag, KeyMapTag, IndexInHeapMapTag, Compare>::priority_queue_type type;
      };

      template <class Graph, class ArgPack>
      typename priority_queue_maker<Graph, ArgPack, KeyT, ValueT, PriorityQueueTag, KeyMapTag, IndexInHeapMapTag, Compare>::priority_queue_type
      operator()(const Graph& g, const ArgPack& ap) const {
        return priority_queue_maker<Graph, ArgPack, KeyT, ValueT, PriorityQueueTag, KeyMapTag, IndexInHeapMapTag, Compare>::make_queue(g, ap, defaultKey);
      }
    };

    template <typename G>
    typename hboost::graph_traits<G>::vertex_descriptor
    get_null_vertex(const G&) {return hboost::graph_traits<G>::null_vertex();}

    template <typename G>
    typename hboost::graph_traits<G>::vertex_descriptor
    get_default_starting_vertex(const G& g) {
      std::pair<typename hboost::graph_traits<G>::vertex_iterator, typename hboost::graph_traits<G>::vertex_iterator> iters = vertices(g);
      return (iters.first == iters.second) ? hboost::graph_traits<G>::null_vertex() : *iters.first;
    }

    template <typename G>
    struct get_default_starting_vertex_t {
      typedef typename hboost::graph_traits<G>::vertex_descriptor result_type;
      const G& g;
      get_default_starting_vertex_t(const G& g): g(g) {}
      result_type operator()() const {return get_default_starting_vertex(g);}
    };

    // Wrapper to avoid instantiating numeric_limits when users provide distance_inf value manually
    template <typename T>
    struct get_max {
      T operator()() const {
        return (std::numeric_limits<T>::max)();
      }
      typedef T result_type;
    };

  } // namespace detail

} // namespace hboost

#undef HBOOST_BGL_DECLARE_NAMED_PARAMS

#endif // HBOOST_GRAPH_NAMED_FUNCTION_PARAMS_HPP
