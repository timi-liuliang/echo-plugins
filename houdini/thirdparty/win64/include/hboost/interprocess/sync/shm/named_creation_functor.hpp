//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2007-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_SYNC_NAMED_CREATION_FUNCTOR_HPP
#define HBOOST_INTERPROCESS_SYNC_NAMED_CREATION_FUNCTOR_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/creation_tags.hpp>
#include <hboost/interprocess/detail/type_traits.hpp>
#include <hboost/interprocess/detail/mpl.hpp>
#include <hboost/container/detail/placement_new.hpp>

namespace hboost {
namespace interprocess {
namespace ipcdetail {

struct named_creation_functor_no_arg{};

template <class T, class Arg = named_creation_functor_no_arg>
class named_creation_functor
{
   typedef named_creation_functor_no_arg no_arg_t;
   public:
   named_creation_functor(create_enum_t type, Arg arg = Arg())
      :  m_creation_type(type), m_arg(arg){}

   template<class ArgType>
   void construct(void *address, typename enable_if_c<is_same<ArgType, no_arg_t>::value>::type * = 0) const
   {  ::new(address, hboost_container_new_t())T; }

   template<class ArgType>
   void construct(void *address, typename enable_if_c<!is_same<ArgType, no_arg_t>::value>::type * = 0) const
   {  ::new(address, hboost_container_new_t())T(m_arg); }

   bool operator()(void *address, std::size_t, bool created) const
   {
      switch(m_creation_type){
         case DoOpen:
            return true;
         break;
         case DoCreate:
         case DoOpenOrCreate:
            if(created){
               construct<Arg>(address);
            }
            return true;
         break;

         default:
            return false;
         break;
      }
   }

   std::size_t get_min_size() const
   {  return sizeof(T);  }

   private:
   create_enum_t m_creation_type;
   Arg m_arg;
};

}  //namespace ipcdetail {
}  //namespace interprocess {
}  //namespace hboost {

#endif   //HBOOST_INTERPROCESS_SYNC_NAMED_CREATION_FUNCTOR_HPP
