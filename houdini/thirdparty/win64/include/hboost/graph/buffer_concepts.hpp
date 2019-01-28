//            Copyright Daniel Trebbien 2010.
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE_1_0.txt or the copy at
//         http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_GRAPH_BUFFER_CONCEPTS_HPP
#define HBOOST_GRAPH_BUFFER_CONCEPTS_HPP 1
#include <hboost/concept_check.hpp>
#include <hboost/concept/detail/concept_def.hpp>
#include <hboost/property_map/property_map.hpp>
#include <hboost/typeof/typeof.hpp>
#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/add_reference.hpp>
#include <hboost/type_traits/remove_reference.hpp>

namespace hboost {

  HBOOST_concept(Buffer, (B))
  {
    typedef typename B::value_type value_type;
    typedef typename B::size_type size_type;
    
    HBOOST_CONCEPT_USAGE(Buffer) {
      typedef typename hboost::add_reference<value_type>::type reference;
      
      HBOOST_CONCEPT_ASSERT((Assignable<value_type>));
      
      buf.push(g_ct);
      buf.pop();
      reference t = buf.top();
      hboost::ignore_unused_variable_warning(t);
    }
    
    void const_constraints(const B& cbuf) {
      typedef typename hboost::add_const<typename hboost::remove_reference<value_type>::type>::type& const_reference;
      
      const_reference ct = cbuf.top();
      s = cbuf.size();
      if (cbuf.empty())
        dummy = __LINE__;
    }
    
    int dummy;
    
    static const value_type g_ct;
    size_type s;
    B buf;
  };
  
  HBOOST_concept(UpdatableQueue, (Q))
    : Buffer<Q>
  {
    HBOOST_CONCEPT_USAGE(UpdatableQueue) {
      q.update(g_ct);
    }
    
    void const_constraints(const Q& cq) {
      if (cq.contains(g_ct))
        dummy = __LINE__;
    }
    
    int dummy;
    
    static const typename Buffer<Q>::value_type g_ct;
    Q q;
  };
  
  HBOOST_concept(KeyedUpdatableQueue, (Q))
    : UpdatableQueue<Q>
  {
    typedef typename Q::key_type key_type;
    typedef typename Q::key_map key_map;
    
    HBOOST_CONCEPT_USAGE(KeyedUpdatableQueue) {
      HBOOST_CONCEPT_ASSERT((hboost::ReadWritePropertyMapConcept<key_map, typename Buffer<Q>::value_type>));
    }
    
    void const_constraints(const Q& cq) {
      km = cq.keys();
      k = get(km, g_ct);
    }
    
    static const typename Buffer<Q>::value_type g_ct;
    key_type k;
    key_map km;
    Q q;
  };

} // end `namespace hboost`

#endif // !HBOOST_GRAPH_BUFFER_CONCEPTS_HPP
