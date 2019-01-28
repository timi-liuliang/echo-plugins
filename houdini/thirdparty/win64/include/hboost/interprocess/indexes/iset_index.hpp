//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_ISET_INDEX_HPP
#define HBOOST_INTERPROCESS_ISET_INDEX_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/interprocess/detail/workaround.hpp>

#include <hboost/intrusive/detail/minimal_pair_header.hpp>
#include <hboost/interprocess/detail/utilities.hpp>
#include <hboost/intrusive/detail/minimal_pair_header.hpp>         //std::pair
#include <hboost/intrusive/detail/minimal_less_equal_header.hpp>   //std::less
#include <hboost/container/detail/minimal_char_traits_header.hpp>  //std::char_traits
#include <hboost/intrusive/set.hpp>

//!\file
//!Describes index adaptor of hboost::intrusive::set container, to use it
//!as name/shared memory index

namespace hboost {
namespace interprocess {

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

//!Helper class to define typedefs from IndexTraits
template <class MapConfig>
struct iset_index_aux
{
   typedef typename
      MapConfig::segment_manager_base                          segment_manager_base;

   typedef typename
      segment_manager_base::void_pointer                       void_pointer;
   typedef typename bi::make_set_base_hook
      < bi::void_pointer<void_pointer>
      , bi::optimize_size<true>
      >::type                                                  derivation_hook;

   typedef typename MapConfig::template
      intrusive_value_type<derivation_hook>::type              value_type;
   typedef std::less<value_type>                               value_compare;
   typedef typename bi::make_set
      < value_type
      , bi::base_hook<derivation_hook>
      >::type                                                  index_t;
};
#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

//!Index type based in hboost::intrusive::set.
//!Just derives from hboost::intrusive::set
//!and defines the interface needed by managed memory segments*/
template <class MapConfig>
class iset_index
   //Derive class from map specialization
   :  public iset_index_aux<MapConfig>::index_t
{
   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   typedef iset_index_aux<MapConfig>                     index_aux;
   typedef typename index_aux::index_t                   index_type;
   typedef typename MapConfig::
      intrusive_compare_key_type                         intrusive_compare_key_type;
   typedef typename MapConfig::char_type                 char_type;
   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

   public:
   typedef typename index_type::iterator                 iterator;
   typedef typename index_type::const_iterator           const_iterator;
   typedef typename index_type::insert_commit_data       insert_commit_data;
   typedef typename index_type::value_type               value_type;

   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
   private:

   struct intrusive_key_value_less
   {
      bool operator()(const intrusive_compare_key_type &i, const value_type &b) const
      {
         std::size_t blen = b.name_length();
         return (i.m_len < blen) ||
                  (i.m_len == blen &&
                  std::char_traits<char_type>::compare
                     (i.mp_str, b.name(), i.m_len) < 0);
      }

      bool operator()(const value_type &b, const intrusive_compare_key_type &i) const
      {
         std::size_t blen = b.name_length();
         return (blen < i.m_len) ||
                  (blen == i.m_len &&
                  std::char_traits<char_type>::compare
                     (b.name(), i.mp_str, i.m_len) < 0);
      }
   };

   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

   public:

   //!Constructor. Takes a pointer to the
   //!segment manager. Can throw
   iset_index(typename MapConfig::segment_manager_base *)
      : index_type(/*typename index_aux::value_compare()*/)
   {}

   //!This reserves memory to optimize the insertion of n
   //!elements in the index
   void reserve(typename MapConfig::segment_manager_base::size_type)
   {  /*Does nothing, map has not reserve or rehash*/  }

   //!This frees all unnecessary memory
   void shrink_to_fit()
   {  /*Does nothing, this intrusive index does not allocate memory;*/   }

   iterator find(const intrusive_compare_key_type &key)
   {  return index_type::find(key, intrusive_key_value_less());  }

   const_iterator find(const intrusive_compare_key_type &key) const
   {  return index_type::find(key, intrusive_key_value_less());  }

   std::pair<iterator, bool>insert_check
      (const intrusive_compare_key_type &key, insert_commit_data &commit_data)
   {  return index_type::insert_check(key, intrusive_key_value_less(), commit_data); }
};

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

//!Trait class to detect if an index is an intrusive
//!index.
template<class MapConfig>
struct is_intrusive_index
   <hboost::interprocess::iset_index<MapConfig> >
{
   static const bool value = true;
};
#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

}  //namespace interprocess {
}  //namespace hboost

#include <hboost/interprocess/detail/config_end.hpp>

#endif   //#ifndef HBOOST_INTERPROCESS_ISET_INDEX_HPP