// Copyright (C) 2007 Matthias Troyer
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//
// This file contains helper data structures for use in transmitting
// properties. The basic idea is to optimize away any storage for the
// properties when no properties are specified.
#ifndef HBOOST_PARALLEL_DETAIL_UNTRACKED_PAIR_HPP
#define HBOOST_PARALLEL_DETAIL_UNTRACKED_PAIR_HPP

#include <hboost/mpi/datatype.hpp>
#include <utility> // for std::pair
#include <hboost/serialization/utility.hpp>

namespace hboost { namespace parallel { namespace detail {

/**
 * This structure is like std::pair, with the only difference
 * that tracking in the serialization library is turned off.
 */
 
template<typename T, typename U>
struct untracked_pair : public std::pair<T,U>
{
  untracked_pair() {}

  untracked_pair(const T& t, const U& u)
  : std::pair<T,U>(t,u) {}

  template<class T1, class U1>
  untracked_pair(std::pair<T1,U1> const& p)
  : std::pair<T,U>(p) {}  
};

template<typename T, typename U>
inline untracked_pair<T, U>
make_untracked_pair(const T& t, const U& u)
{
  return untracked_pair<T,U>(t,u);
}

} } } // end namespace hboost::parallel::detail

namespace hboost { namespace mpi {

template<typename T, typename U>
struct is_mpi_datatype<hboost::parallel::detail::untracked_pair<T, U> >
  : is_mpi_datatype<std::pair<T,U> > {};

} } // end namespace hboost::mpi

namespace hboost { namespace serialization {

// pair
template<class Archive, class F, class S>
inline void serialize(
    Archive & ar,
    hboost::parallel::detail::untracked_pair<F, S> & p,
    const unsigned int /* file_version */
){
    ar & hboost::serialization::make_nvp("first", p.first);
    ar & hboost::serialization::make_nvp("second", p.second);
}

template<typename T, typename U>
struct is_bitwise_serializable<
        hboost::parallel::detail::untracked_pair<T, U> >
  : is_bitwise_serializable<std::pair<T, U> > {};

template<typename T, typename U>
struct implementation_level<hboost::parallel::detail::untracked_pair<T, U> >
 : mpl::int_<object_serializable> {} ;

template<typename T, typename U>
struct tracking_level<hboost::parallel::detail::untracked_pair<T, U> >
 : mpl::int_<track_never> {} ;

} } // end namespace hboost::serialization

#endif // HBOOST_PARALLEL_DETAIL_UNTRACKED_PAIR_HPP
