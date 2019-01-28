//  (C) Copyright Jeremy Siek 2006
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROPERTY_SERIALIZE_HPP
#define HBOOST_PROPERTY_SERIALIZE_HPP

#include <hboost/pending/property.hpp>
#ifdef HBOOST_GRAPH_USE_MPI
#include <hboost/mpi/datatype.hpp>
#include <hboost/serialization/is_bitwise_serializable.hpp>
#endif // HBOOST_GRAPH_USE_MPI

#include <hboost/serialization/base_object.hpp>
#include <hboost/serialization/nvp.hpp>

namespace hboost {
  template<class Archive>
  inline void serialize(Archive&, no_property&, const unsigned int) { }

  template<class Archive, class Tag, class T, class Base>
  void 
  serialize(Archive& ar, property<Tag, T, Base>& prop, 
            const unsigned int /*version*/) 
  {
    ar & serialization::make_nvp( "property_value" , prop.m_value );
    ar & serialization::make_nvp( "property_base" , prop.m_base );
  }

#ifdef HBOOST_GRAPH_USE_MPI
  namespace mpi {
    template<typename Tag, typename T, typename Base>
    struct is_mpi_datatype<property<Tag, T, Base> >
      : mpl::and_<is_mpi_datatype<T>,
                  is_mpi_datatype<Base> > { };
  }

  namespace serialization {
    template<typename Tag, typename T, typename Base>
    struct is_bitwise_serializable<property<Tag, T, Base> >
      : mpl::and_<is_bitwise_serializable<T>,
                  is_bitwise_serializable<Base> > { };

  template<typename Tag, typename T, typename Base>
  struct implementation_level<property<Tag, T, Base>  >
   : mpl::int_<object_serializable> {} ;

  template<typename Tag, typename T, typename Base>
  struct tracking_level<property<Tag, T, Base>  >
   : mpl::int_<track_never> {} ;

  }
#endif // HBOOST_GRAPH_USE_MPI
  
} // end namespace hboost

#ifdef HBOOST_GRAPH_USE_MPI
namespace hboost { namespace mpi {
    template<>
    struct is_mpi_datatype<hboost::no_property> : mpl::true_ { };

} } // end namespace hboost::mpi

HBOOST_IS_BITWISE_SERIALIZABLE(hboost::no_property)
HBOOST_CLASS_IMPLEMENTATION(hboost::no_property,object_serializable)
HBOOST_CLASS_TRACKING(hboost::no_property,track_never)
#endif // HBOOST_GRAPH_USE_MPI

#endif // HBOOST_PROPERTY_SERIALIZE_HPP
