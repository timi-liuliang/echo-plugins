//
//=======================================================================
// Author: Philipp Moeller
//
// Copyright 2012, Philipp Moeller
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//

#ifndef HBOOST_PROPERTY_MAP_FUNCTION_PROPERTY_MAP_HPP
#define HBOOST_PROPERTY_MAP_FUNCTION_PROPERTY_MAP_HPP

#include <hboost/config.hpp>
#include <hboost/property_map/property_map.hpp>
#include <hboost/type_traits.hpp>
#include <hboost/utility/result_of.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/not.hpp>
#include <utility>

namespace hboost {

template<typename Func, typename Key, typename Ret = typename hboost::result_of<const Func(const Key&)>::type>
class function_property_map: public put_get_helper<Ret, function_property_map<Func, Key, Ret> > {
  public:
  typedef Key key_type;
  typedef Ret reference;
  typedef typename hboost::remove_cv<typename hboost::remove_reference<Ret>::type>::type value_type;

  typedef typename hboost::mpl::if_<
                     hboost::mpl::and_<
                       hboost::is_reference<Ret>,
                       hboost::mpl::not_<hboost::is_const<Ret> >
                     >,
                     hboost::lvalue_property_map_tag,
                     hboost::readable_property_map_tag>::type
    category;

  function_property_map(Func f = Func()) : f(f) {}

  reference operator[](const Key& k) const {
    return f(k);
  }

  private:
  Func f;
};

template<typename Key, typename Func>
function_property_map<Func, Key>
make_function_property_map(const Func& f) {
  return function_property_map<Func, Key>(f);
}

template<typename Key, typename Ret, typename Func>
function_property_map<Func, Key, Ret>
make_function_property_map(const Func& f) {
  return function_property_map<Func, Key, Ret>(f);
}

} // boost

#endif /* HBOOST_PROPERTY_MAP_FUNCTION_PROPERTY_MAP_HPP */
