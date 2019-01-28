//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_NAMED_PROXY_HPP
#define HBOOST_INTERPROCESS_NAMED_PROXY_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/interprocess/detail/workaround.hpp>

// interprocess/detail
#include <hboost/interprocess/detail/in_place_interface.hpp>
#include <hboost/interprocess/detail/mpl.hpp>
#include <hboost/move/utility_core.hpp>
#ifndef HBOOST_INTERPROCESS_PERFECT_FORWARDING
#include <hboost/move/detail/fwd_macros.hpp>
#else
#include <hboost/move/utility_core.hpp>
#include <hboost/interprocess/detail/variadic_templates_tools.hpp>
#endif   //#ifdef HBOOST_INTERPROCESS_PERFECT_FORWARDING
#include <hboost/container/detail/placement_new.hpp>

//!\file
//!Describes a proxy class that implements named allocation syntax.

namespace hboost {
namespace interprocess {
namespace ipcdetail {

#ifdef HBOOST_INTERPROCESS_PERFECT_FORWARDING

template<class T, bool is_iterator, class ...Args>
struct CtorArgN : public placement_destroy<T>
{
   typedef bool_<is_iterator> IsIterator;
   typedef CtorArgN<T, is_iterator, Args...> self_t;
   typedef typename build_number_seq<sizeof...(Args)>::type index_tuple_t;

   self_t& operator++()
   {
      this->do_increment(IsIterator(), index_tuple_t());
      return *this;
   }

   self_t  operator++(int) {  return ++*this;   *this;  }

   CtorArgN(Args && ...args)
      :  args_(args...)
   {}

   virtual void construct_n(void *mem
                     , std::size_t num
                     , std::size_t &constructed)
   {
      T* memory      = static_cast<T*>(mem);
      for(constructed = 0; constructed < num; ++constructed){
         this->construct(memory++, IsIterator(), index_tuple_t());
         this->do_increment(IsIterator(), index_tuple_t());
      }
   }

   private:
   template<int ...IdxPack>
   void construct(void *mem, true_, const index_tuple<IdxPack...>&)
   {  ::new((void*)mem, hboost_container_new_t())T(*hboost::forward<Args>(get<IdxPack>(args_))...); }

   template<int ...IdxPack>
   void construct(void *mem, false_, const index_tuple<IdxPack...>&)
   {  ::new((void*)mem, hboost_container_new_t())T(hboost::forward<Args>(get<IdxPack>(args_))...); }

   template<int ...IdxPack>
   void do_increment(true_, const index_tuple<IdxPack...>&)
   {
      this->expansion_helper(++get<IdxPack>(args_)...);
   }

   template<class ...ExpansionArgs>
   void expansion_helper(ExpansionArgs &&...)
   {}

   template<int ...IdxPack>
   void do_increment(false_, const index_tuple<IdxPack...>&)
   {}

   tuple<Args&...> args_;
};

//!Describes a proxy class that implements named
//!allocation syntax.
template
   < class SegmentManager  //segment manager to construct the object
   , class T               //type of object to build
   , bool is_iterator      //passing parameters are normal object or iterators?
   >
class named_proxy
{
   typedef typename SegmentManager::char_type char_type;
   const char_type *    mp_name;
   SegmentManager *     mp_mngr;
   mutable std::size_t  m_num;
   const bool           m_find;
   const bool           m_dothrow;

   public:
   named_proxy(SegmentManager *mngr, const char_type *name, bool find, bool dothrow)
      :  mp_name(name), mp_mngr(mngr), m_num(1)
      ,  m_find(find),  m_dothrow(dothrow)
   {}

   template<class ...Args>
   T *operator()(Args &&...args) const
   {
      CtorArgN<T, is_iterator, Args...> &&ctor_obj = CtorArgN<T, is_iterator, Args...>
         (hboost::forward<Args>(args)...);
      return mp_mngr->template
         generic_construct<T>(mp_name, m_num, m_find, m_dothrow, ctor_obj);
   }

   //This operator allows --> named_new("Name")[3]; <-- syntax
   const named_proxy &operator[](std::size_t num) const
   {  m_num *= num; return *this;  }
};

#else //#ifdef HBOOST_INTERPROCESS_PERFECT_FORWARDING

////////////////////////////////////////////////////////////////
//    What the macro should generate (n == 2):
//
//    template<class T, bool is_iterator, class P1, class P2>
//    struct Ctor2Arg
//      :  public placement_destroy<T>
//    {
//       typedef bool_<is_iterator> IsIterator;
//       typedef Ctor2Arg self_t;
//
//       void do_increment(false_)
//       { ++m_p1; ++m_p2;  }
//
//       void do_increment(true_){}
//
//       self_t& operator++()
//       {
//          this->do_increment(IsIterator());
//          return *this;
//       }
//
//       self_t  operator++(int) {  return ++*this;   *this;  }
//
//       Ctor2Arg(const P1 &p1, const P2 &p2)
//          : p1((P1 &)p_1), p2((P2 &)p_2) {}
//
//       void construct(void *mem)
//       {  new((void*)object)T(m_p1, m_p2); }
//
//       virtual void construct_n(void *mem
//                                , std::size_t num
//                                , std::size_t &constructed)
//       {
//          T* memory      = static_cast<T*>(mem);
//          for(constructed = 0; constructed < num; ++constructed){
//             this->construct(memory++, IsIterator());
//             this->do_increment(IsIterator());
//          }
//       }
//
//       private:
//       void construct(void *mem, true_)
//       {  new((void*)mem)T(*m_p1, *m_p2); }
//
//       void construct(void *mem, false_)
//       {  new((void*)mem)T(m_p1, m_p2); }
//
//       P1 &m_p1; P2 &m_p2;
//    };
////////////////////////////////////////////////////////////////

#define HBOOST_INTERPROCESS_NAMED_PROXY_CTORARGN(N)\
\
template<class T HBOOST_MOVE_I##N HBOOST_MOVE_CLASS##N >  \
struct CtorArg##N : placement_destroy<T>\
{\
   typedef CtorArg##N self_t;\
   \
   CtorArg##N ( HBOOST_MOVE_UREF##N  )\
      HBOOST_MOVE_COLON##N HBOOST_MOVE_FWD_INIT##N{}\
   \
   virtual void construct_n(void *mem, std::size_t num, std::size_t &constructed)\
   {\
      T* memory      = static_cast<T*>(mem);\
      for(constructed = 0; constructed < num; ++constructed){\
         ::new((void*)memory++) T ( HBOOST_MOVE_MFWD##N );\
      }\
   }\
   \
   private:\
   HBOOST_MOVE_MREF##N\
};\
//!
HBOOST_MOVE_ITERATE_0TO9(HBOOST_INTERPROCESS_NAMED_PROXY_CTORARGN)
#undef HBOOST_INTERPROCESS_NAMED_PROXY_CTORARGN

#define HBOOST_INTERPROCESS_NAMED_PROXY_CTORITN(N)\
\
template<class T HBOOST_MOVE_I##N HBOOST_MOVE_CLASS##N > \
struct CtorIt##N : public placement_destroy<T>\
{\
   typedef CtorIt##N self_t;\
   \
   self_t& operator++()\
   {  HBOOST_MOVE_MINC##N;  return *this;  }\
   \
   self_t  operator++(int) {  return ++*this; *this;  }\
   \
   CtorIt##N ( HBOOST_MOVE_VAL##N  )\
      HBOOST_MOVE_COLON##N HBOOST_MOVE_VAL_INIT##N{}\
   \
   virtual void construct_n(void *mem, std::size_t num, std::size_t &constructed)\
   {\
      T* memory      = static_cast<T*>(mem);\
      for(constructed = 0; constructed < num; ++constructed){\
         ::new((void*)memory++) T( HBOOST_MOVE_MITFWD##N );\
         ++(*this);\
      }\
   }\
   \
   private:\
   HBOOST_MOVE_MEMB##N\
};\
//!
HBOOST_MOVE_ITERATE_0TO9(HBOOST_INTERPROCESS_NAMED_PROXY_CTORITN)
#undef HBOOST_INTERPROCESS_NAMED_PROXY_CTORITN

//!Describes a proxy class that implements named
//!allocation syntax.
template
   < class SegmentManager  //segment manager to construct the object
   , class T               //type of object to build
   , bool is_iterator      //passing parameters are normal object or iterators?
   >
class named_proxy
{
   typedef typename SegmentManager::char_type char_type;
   const char_type *    mp_name;
   SegmentManager *     mp_mngr;
   mutable std::size_t  m_num;
   const bool           m_find;
   const bool           m_dothrow;

   public:
   named_proxy(SegmentManager *mngr, const char_type *name, bool find, bool dothrow)
      :  mp_name(name), mp_mngr(mngr), m_num(1)
      ,  m_find(find),  m_dothrow(dothrow)
   {}

   #define HBOOST_INTERPROCESS_NAMED_PROXY_CALL_OPERATOR(N)\
   \
   HBOOST_MOVE_TMPL_LT##N HBOOST_MOVE_CLASS##N HBOOST_MOVE_GT##N \
   T *operator()( HBOOST_MOVE_UREF##N ) const\
   {\
      typedef typename if_c<is_iterator \
         , CtorIt##N<T HBOOST_MOVE_I##N HBOOST_MOVE_TARG##N> \
         , CtorArg##N<T HBOOST_MOVE_I##N HBOOST_MOVE_TARG##N> \
         >::type ctor_obj_t;\
      ctor_obj_t ctor_obj = ctor_obj_t( HBOOST_MOVE_FWD##N );\
      return mp_mngr->template generic_construct<T>(mp_name, m_num, m_find, m_dothrow, ctor_obj);\
   }\
   //
   HBOOST_MOVE_ITERATE_0TO9(HBOOST_INTERPROCESS_NAMED_PROXY_CALL_OPERATOR)
   #undef HBOOST_INTERPROCESS_NAMED_PROXY_CALL_OPERATOR

   ////////////////////////////////////////////////////////////////////////
   //             What the macro should generate (n == 2)
   ////////////////////////////////////////////////////////////////////////
   //
   // template <class P1, class P2>
   // T *operator()(P1 &p1, P2 &p2) const
   // {
   //    typedef CtorArg2
   //       <T, is_iterator, P1, P2>
   //       ctor_obj_t;
   //    ctor_obj_t ctor_obj(p1, p2);
   //
   //    return mp_mngr->template generic_construct<T>
   //       (mp_name, m_num, m_find, m_dothrow, ctor_obj);
   // }
   //
   //////////////////////////////////////////////////////////////////////////

   //This operator allows --> named_new("Name")[3]; <-- syntax
   const named_proxy &operator[](std::size_t num) const
      {  m_num *= num; return *this;  }
};

#endif   //#ifdef HBOOST_INTERPROCESS_PERFECT_FORWARDING

}}}   //namespace hboost { namespace interprocess { namespace ipcdetail {

#include <hboost/interprocess/detail/config_end.hpp>

#endif //#ifndef HBOOST_INTERPROCESS_NAMED_PROXY_HPP
