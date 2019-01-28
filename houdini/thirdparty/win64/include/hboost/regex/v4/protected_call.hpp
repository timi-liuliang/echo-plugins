/*
 *
 * Copyright (c) 2004
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         basic_regex_creator.cpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: Declares template class basic_regex_creator which fills in
  *                the data members of a regex_data object.
  */

#ifndef HBOOST_REGEX_V4_PROTECTED_CALL_HPP
#define HBOOST_REGEX_V4_PROTECTED_CALL_HPP

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable: 4103)
#endif
#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_PREFIX
#endif
#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

namespace hboost{
namespace HBOOST_REGEX_DETAIL_NS{

class HBOOST_REGEX_DECL abstract_protected_call
{
public:
   bool HBOOST_REGEX_CALL execute()const;
   // this stops gcc-4 from complaining:
   virtual ~abstract_protected_call(){}
private:
   virtual bool call()const = 0;
};

template <class T>
class concrete_protected_call
   : public abstract_protected_call
{
public:
   typedef bool (T::*proc_type)();
   concrete_protected_call(T* o, proc_type p)
      : obj(o), proc(p) {}
private:
   virtual bool call()const;
   T* obj;
   proc_type proc;
};

template <class T>
bool concrete_protected_call<T>::call()const
{
   return (obj->*proc)();
}

}
} // namespace hboost

#ifdef HBOOST_MSVC
#pragma warning(push)
#pragma warning(disable: 4103)
#endif
#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_SUFFIX
#endif
#ifdef HBOOST_MSVC
#pragma warning(pop)
#endif

#endif
