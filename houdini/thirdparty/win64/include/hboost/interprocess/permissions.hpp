//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_PERMISSIONS_HPP
#define HBOOST_INTERPROCESS_PERMISSIONS_HPP

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/interprocess/detail/workaround.hpp>
#include <hboost/interprocess/interprocess_fwd.hpp>

#if defined(HBOOST_INTERPROCESS_WINDOWS)

#include <hboost/interprocess/detail/win32_api.hpp>

#endif

#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

//!\file
//!Describes permissions class

namespace hboost {
namespace interprocess {

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

#if defined(HBOOST_INTERPROCESS_WINDOWS)

namespace ipcdetail {

template <int Dummy>
struct unrestricted_permissions_holder
{
   static winapi::interprocess_all_access_security unrestricted;
};

template<int Dummy>
winapi::interprocess_all_access_security unrestricted_permissions_holder<Dummy>::unrestricted;

}  //namespace ipcdetail {

#endif   //defined HBOOST_INTERPROCESS_WINDOWS

#endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

//!The permissions class represents permissions to be set to shared memory or
//!files, that can be constructed form usual permission representations:
//!a SECURITY_ATTRIBUTES pointer in windows or ORed rwx chmod integer in UNIX.
class permissions
{
   #if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

   #if defined(HBOOST_INTERPROCESS_WINDOWS)
   typedef void*  os_permissions_type;
   #else
   typedef int    os_permissions_type;
   #endif
   os_permissions_type  m_perm;

   #endif   //#ifndef HBOOST_INTERPROCESS_DOXYGEN_INVOKED

   public:
   //!Constructs a permissions object from a user provided os-dependent
   //!permissions.
   permissions(os_permissions_type type)
      : m_perm(type)
   {}

   //!Constructs a default permissions object:
   //!A null security attributes pointer for windows or 0644
   //!for UNIX.
   permissions()
   {  set_default(); }

   //!Sets permissions to default values:
   //!A null security attributes pointer for windows or 0644
   //!for UNIX.
   void set_default()
   {
      #if defined (HBOOST_INTERPROCESS_WINDOWS)
      m_perm = 0;
      #else
      m_perm = 0644;
      #endif
   }

   //!Sets permissions to unrestricted access:
   //!A null DACL for windows or 0666 for UNIX.
   void set_unrestricted()
   {
      #if defined (HBOOST_INTERPROCESS_WINDOWS)
      m_perm = &ipcdetail::unrestricted_permissions_holder<0>::unrestricted;
      #else
      m_perm = 0666;
      #endif
   }

   //!Sets permissions from a user provided os-dependent
   //!permissions.
   void set_permissions(os_permissions_type perm)
   {  m_perm = perm; }

   //!Returns stored os-dependent
   //!permissions
   os_permissions_type get_permissions() const
   {  return m_perm; }
};

}  //namespace interprocess {
}  //namespace hboost {

#include <hboost/interprocess/detail/config_end.hpp>

#endif   //HBOOST_INTERPROCESS_PERMISSIONS_HPP

