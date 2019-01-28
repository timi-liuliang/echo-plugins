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

#ifndef HBOOST_PROPERTY_MAP_TRANSFORM_VALUE_PROPERTY_MAP_HPP
#define HBOOST_PROPERTY_MAP_TRANSFORM_VALUE_PROPERTY_MAP_HPP

#include <hboost/config.hpp>
#include <hboost/property_map/property_map.hpp>
#include <hboost/type_traits.hpp>
#include <hboost/utility/result_of.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/not.hpp>
#include <utility>

namespace hboost {

template<typename Func, typename PM, typename Ret = typename hboost::result_of<const Func(typename property_traits<PM>::reference)>::type>
class transform_value_property_map: public put_get_helper<Ret, transform_value_property_map<Func, PM, Ret> > {
  public:
  typedef typename property_traits<PM>::key_type key_type;
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

  transform_value_property_map(Func f, PM pm) : f(f), pm(pm) {}

  reference operator[](const key_type& k) const {
    return f(get(pm, k));
  }

  private:
  Func f;
  PM pm;
};

template<typename PM, typename Func>
transform_value_property_map<Func, PM>
make_transform_value_property_map(const Func& f, const PM& pm) {
  return transform_value_property_map<Func, PM>(f, pm);
}

template<typename Ret, typename PM, typename Func>
transform_value_property_map<Func, PM, Ret>
make_transform_value_property_map(const Func& f, const PM& pm) {
  return transform_value_property_map<Func, PM, Ret>(f, pm);
}

} // boost

#endif /* HBOOST_PROPERTY_MAP_TRANSFORM_VALUE_PROPERTY_MAP_HPP */
