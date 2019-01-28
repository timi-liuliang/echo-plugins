//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_FWD_HPP
#define HBOOST_INTERPROCESS_FWD_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#ifndef HBOOST_CSTDINT_HPP
#  include <hboost/cstdint.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/std_fwd.hpp>

//! \file
//! This header file forward declares the basic interprocess types:
//!   - hboost::interprocess::offset_ptr;
//!   - hboost::interprocess::permissions;
//!   - hboost::interprocess::mapped_region;
//!   - hboost::interprocess::file_mapping;
//!   - hboost::interprocess::shared_memory_object;
//!   - hboost::interprocess::windows_shared_memory;
//!   - hboost::interprocess::xsi_shared_memory;
//!
//! The following synchronization mechanisms and locks:
//!   - hboost::interprocess::null_mutex;
//!   - hboost::interprocess::interprocess_mutex;
//!   - hboost::interprocess::interprocess_recursive_mutex;
//!   - hboost::interprocess::interprocess_semaphore;
//!   - hboost::interprocess::named_mutex;
//!   - hboost::interprocess::named_recursive_mutex;
//!   - hboost::interprocess::named_semaphore;
//!   - hboost::interprocess::interprocess_sharable_mutex;
//!   - hboost::interprocess::interprocess_condition;
//!   - hboost::interprocess::scoped_lock;
//!   - hboost::interprocess::sharable_lock;
//!   - hboost::interprocess::upgradable_lock;
//!
//! The following mutex families:
//!   - hboost::interprocess::mutex_family;
//!   - hboost::interprocess::null_mutex_family;
//!
//! The following allocators:
//!   - hboost::interprocess::allocator;
//!   - hboost::interprocess::node_allocator;
//!   - hboost::interprocess::private_node_allocator;
//!   - hboost::interprocess::cached_node_allocator;
//!   - hboost::interprocess::adaptive_pool;
//!   - hboost::interprocess::private_adaptive_pool;
//!   - hboost::interprocess::cached_adaptive_pool;
//!
//! The following allocation algorithms:
//!   - hboost::interprocess::simple_seq_fit;
//!   - hboost::interprocess::rbtree_best_fit;
//!
//! The following index types:
//!   - hboost::interprocess::flat_map_index;
//!   - hboost::interprocess::iset_index;
//!   - hboost::interprocess::iunordered_set_index;
//!   - hboost::interprocess::map_index;
//!   - hboost::interprocess::null_index;
//!   - hboost::interprocess::unordered_map_index;
//!
//! The following managed memory types:
//!   - hboost::interprocess::segment_manager;
//!   - hboost::interprocess::basic_managed_external_buffer
//!   - hboost::interprocess::managed_external_buffer
//!   - hboost::interprocess::wmanaged_external_buffer
//!   - hboost::interprocess::basic_managed_shared_memory
//!   - hboost::interprocess::managed_shared_memory
//!   - hboost::interprocess::wmanaged_shared_memory
//!   - hboost::interprocess::basic_managed_windows_shared_memory
//!   - hboost::interprocess::managed_windows_shared_memory
//!   - hboost::interprocess::wmanaged_windows_shared_memory
//!   - hboost::interprocess::basic_managed_xsi_shared_memory
//!   - hboost::interprocess::managed_xsi_shared_memory
//!   - hboost::interprocess::wmanaged_xsi_shared_memory
//!   - hboost::interprocess::fixed_managed_shared_memory
//!   - hboost::interprocess::wfixed_managed_shared_memory
//!   - hboost::interprocess::basic_managed_heap_memory
//!   - hboost::interprocess::managed_heap_memory
//!   - hboost::interprocess::wmanaged_heap_memory
//!   - hboost::interprocess::basic_managed_mapped_file
//!   - hboost::interprocess::managed_mapped_file
//!   - hboost::interprocess::wmanaged_mapped_file
//!
//! The following exception types:
//!   - hboost::interprocess::interprocess_exception
//!   - hboost::interprocess::lock_exception
//!   - hboost::interprocess::bad_alloc
//!
//! The following stream types:
//!   - hboost::interprocess::basic_bufferbuf
//!   - hboost::interprocess::basic_ibufferstream
//!   - hboost::interprocess::basic_obufferstream
//!   - hboost::interprocess::basic_bufferstream
//!   - hboost::interprocess::basic_vectorbuf
//!   - hboost::interprocess::basic_ivectorstream
//!   - hboost::interprocess::basic_ovectorstream
//!   - hboost::interprocess::basic_vectorstream
//!
//! The following smart pointer types:
//!   - hboost::interprocess::scoped_ptr
//!   - hboost::interprocess::intrusive_ptr
//!   - hboost::interprocess::shared_ptr
//!   - hboost::interprocess::weak_ptr
//!
//! The following interprocess communication types:
//!   - hboost::interprocess::message_queue_t;
//!   - hboost::interprocess::message_queue;

#include <hboost/interprocess/detail/config_begin.hpp>
#include <hboost/interprocess/detail/workaround.hpp>

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

#include <cstddef>

//////////////////////////////////////////////////////////////////////////////
//                        Standard predeclarations
//////////////////////////////////////////////////////////////////////////////

namespace hboost{  namespace intrusive{ }  }
namespace hboost{  namespace interprocess{ namespace bi = hboost::intrusive; }  }

namespace hboost { namespace interprocess {

//////////////////////////////////////////////////////////////////////////////
//                            permissions
//////////////////////////////////////////////////////////////////////////////

class permissions;

//////////////////////////////////////////////////////////////////////////////
//                            shared_memory
//////////////////////////////////////////////////////////////////////////////

class shared_memory_object;

#if defined (HBOOST_INTERPROCESS_WINDOWS)
class windows_shared_memory;
#endif   //#if defined (HBOOST_INTERPROCESS_WINDOWS)

#if defined(HBOOST_INTERPROCESS_XSI_SHARED_MEMORY_OBJECTS)
class xsi_shared_memory;
#endif   //#if defined (HBOOST_INTERPROCESS_WINDOWS)

//////////////////////////////////////////////////////////////////////////////
//              file mapping / mapped region
//////////////////////////////////////////////////////////////////////////////

class file_mapping;
class mapped_region;

//////////////////////////////////////////////////////////////////////////////
//                               Mutexes
//////////////////////////////////////////////////////////////////////////////

class null_mutex;

class interprocess_mutex;
class interprocess_recursive_mutex;

class named_mutex;
class named_recursive_mutex;

class interprocess_semaphore;
class named_semaphore;

//////////////////////////////////////////////////////////////////////////////
//                         Mutex families
//////////////////////////////////////////////////////////////////////////////

struct mutex_family;
struct null_mutex_family;

//////////////////////////////////////////////////////////////////////////////
//                   Other synchronization classes
//////////////////////////////////////////////////////////////////////////////

class interprocess_sharable_mutex;
class interprocess_condition;

//////////////////////////////////////////////////////////////////////////////
//                              Locks
//////////////////////////////////////////////////////////////////////////////

template <class Mutex>
class scoped_lock;

template <class SharableMutex>
class sharable_lock;

template <class UpgradableMutex>
class upgradable_lock;

//////////////////////////////////////////////////////////////////////////////
//                      STL compatible allocators
//////////////////////////////////////////////////////////////////////////////

template<class T, class SegmentManager>
class allocator;

template<class T, class SegmentManager, std::size_t NodesPerBlock = 64>
class node_allocator;

template<class T, class SegmentManager, std::size_t NodesPerBlock = 64>
class private_node_allocator;

template<class T, class SegmentManager, std::size_t NodesPerBlock = 64>
class cached_node_allocator;

template< class T, class SegmentManager, std::size_t NodesPerBlock = 64
        , std::size_t MaxFreeBlocks = 2, unsigned char OverheadPercent = 5 >
class adaptive_pool;

template< class T, class SegmentManager, std::size_t NodesPerBlock = 64
        , std::size_t MaxFreeBlocks = 2, unsigned char OverheadPercent = 5 >
class private_adaptive_pool;

template< class T, class SegmentManager, std::size_t NodesPerBlock = 64
        , std::size_t MaxFreeBlocks = 2, unsigned char OverheadPercent = 5 >
class cached_adaptive_pool;


//////////////////////////////////////////////////////////////////////////////
//                            offset_ptr
//////////////////////////////////////////////////////////////////////////////

static const std::size_t offset_type_alignment = 0;

#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)
#  ifdef HBOOST_HAS_INTPTR_T
      using ::hboost::uintptr_t;
#  else
      typedef std::size_t uintptr_t;
#  endif
#endif

template < class T, class DifferenceType = std::ptrdiff_t
         , class OffsetType = uintptr_t, std::size_t Alignment = offset_type_alignment>
class offset_ptr;

//////////////////////////////////////////////////////////////////////////////
//                    Memory allocation algorithms
//////////////////////////////////////////////////////////////////////////////

//Single segment memory allocation algorithms
template<class MutexFamily, class VoidMutex = offset_ptr<void> >
class simple_seq_fit;

template<class MutexFamily, class VoidMutex = offset_ptr<void>, std::size_t MemAlignment = 0>
class rbtree_best_fit;

//////////////////////////////////////////////////////////////////////////////
//                         Index Types
//////////////////////////////////////////////////////////////////////////////

template<class IndexConfig> class flat_map_index;
template<class IndexConfig> class iset_index;
template<class IndexConfig> class iunordered_set_index;
template<class IndexConfig> class map_index;
template<class IndexConfig> class null_index;
template<class IndexConfig> class unordered_map_index;

//////////////////////////////////////////////////////////////////////////////
//                         Segment manager
//////////////////////////////////////////////////////////////////////////////

template <class CharType
         ,class MemoryAlgorithm
         ,template<class IndexConfig> class IndexType>
class segment_manager;

//////////////////////////////////////////////////////////////////////////////
//                  External buffer managed memory classes
//////////////////////////////////////////////////////////////////////////////

template <class CharType
         ,class MemoryAlgorithm
         ,template<class IndexConfig> class IndexType>
class basic_managed_external_buffer;

typedef basic_managed_external_buffer
   <char
   ,rbtree_best_fit<null_mutex_family>
   ,iset_index>
managed_external_buffer;

typedef basic_managed_external_buffer
   <wchar_t
   ,rbtree_best_fit<null_mutex_family>
   ,iset_index>
wmanaged_external_buffer;

//////////////////////////////////////////////////////////////////////////////
//                      managed memory classes
//////////////////////////////////////////////////////////////////////////////

template <class CharType
         ,class MemoryAlgorithm
         ,template<class IndexConfig> class IndexType>
class basic_managed_shared_memory;

typedef basic_managed_shared_memory
   <char
   ,rbtree_best_fit<mutex_family>
   ,iset_index>
managed_shared_memory;

typedef basic_managed_shared_memory
   <wchar_t
   ,rbtree_best_fit<mutex_family>
   ,iset_index>
wmanaged_shared_memory;


//////////////////////////////////////////////////////////////////////////////
//                      Windows shared memory managed memory classes
//////////////////////////////////////////////////////////////////////////////

#if defined (HBOOST_INTERPROCESS_WINDOWS)

template <class CharType
         ,class MemoryAlgorithm
         ,template<class IndexConfig> class IndexType>
class basic_managed_windows_shared_memory;

typedef basic_managed_windows_shared_memory
   <char
   ,rbtree_best_fit<mutex_family>
   ,iset_index>
managed_windows_shared_memory;

typedef basic_managed_windows_shared_memory
   <wchar_t
   ,rbtree_best_fit<mutex_family>
   ,iset_index>
wmanaged_windows_shared_memory;

#endif   //#if defined (HBOOST_INTERPROCESS_WINDOWS)

#if defined(HBOOST_INTERPROCESS_XSI_SHARED_MEMORY_OBJECTS)

template <class CharType
         ,class MemoryAlgorithm
         ,template<class IndexConfig> class IndexType>
class basic_managed_xsi_shared_memory;

typedef basic_managed_xsi_shared_memory
   <char
   ,rbtree_best_fit<mutex_family>
   ,iset_index>
managed_xsi_shared_memory;

typedef basic_managed_xsi_shared_memory
   <wchar_t
   ,rbtree_best_fit<mutex_family>
   ,iset_index>
wmanaged_xsi_shared_memory;

#endif //#if defined(HBOOST_INTERPROCESS_XSI_SHARED_MEMORY_OBJECTS)

//////////////////////////////////////////////////////////////////////////////
//                      Fixed address shared memory
//////////////////////////////////////////////////////////////////////////////

typedef basic_managed_shared_memory
   <char
   ,rbtree_best_fit<mutex_family, void*>
   ,iset_index>
fixed_managed_shared_memory;

typedef basic_managed_shared_memory
   <wchar_t
   ,rbtree_best_fit<mutex_family, void*>
   ,iset_index>
wfixed_managed_shared_memory;

//////////////////////////////////////////////////////////////////////////////
//                      Heap memory managed memory classes
//////////////////////////////////////////////////////////////////////////////

template
   <class CharType
   ,class MemoryAlgorithm
   ,template<class IndexConfig> class IndexType>
class basic_managed_heap_memory;

typedef basic_managed_heap_memory
   <char
   ,rbtree_best_fit<null_mutex_family>
   ,iset_index>
managed_heap_memory;

typedef basic_managed_heap_memory
   <wchar_t
   ,rbtree_best_fit<null_mutex_family>
   ,iset_index>
wmanaged_heap_memory;

//////////////////////////////////////////////////////////////////////////////
//                         Mapped file managed memory classes
//////////////////////////////////////////////////////////////////////////////

template
   <class CharType
   ,class MemoryAlgorithm
   ,template<class IndexConfig> class IndexType>
class basic_managed_mapped_file;

typedef basic_managed_mapped_file
   <char
   ,rbtree_best_fit<mutex_family>
   ,iset_index>
managed_mapped_file;

typedef basic_managed_mapped_file
   <wchar_t
   ,rbtree_best_fit<mutex_family>
   ,iset_index>
wmanaged_mapped_file;

//////////////////////////////////////////////////////////////////////////////
//                            Exceptions
//////////////////////////////////////////////////////////////////////////////

class interprocess_exception;
class lock_exception;
class bad_alloc;

//////////////////////////////////////////////////////////////////////////////
//                            Bufferstream
//////////////////////////////////////////////////////////////////////////////

//bufferstream
template <class CharT
         ,class CharTraits = std::char_traits<CharT> >
class basic_bufferbuf;

template <class CharT
         ,class CharTraits = std::char_traits<CharT> >
class basic_ibufferstream;

template <class CharT
         ,class CharTraits = std::char_traits<CharT> >
class basic_obufferstream;

template <class CharT
         ,class CharTraits = std::char_traits<CharT> >
class basic_bufferstream;

//////////////////////////////////////////////////////////////////////////////
//                            Vectorstream
//////////////////////////////////////////////////////////////////////////////

template <class CharVector
         ,class CharTraits = std::char_traits<typename CharVector::value_type> >
class basic_vectorbuf;

template <class CharVector
         ,class CharTraits = std::char_traits<typename CharVector::value_type> >
class basic_ivectorstream;

template <class CharVector
         ,class CharTraits = std::char_traits<typename CharVector::value_type> >
class basic_ovectorstream;

template <class CharVector
         ,class CharTraits = std::char_traits<typename CharVector::value_type> >
class basic_vectorstream;

//////////////////////////////////////////////////////////////////////////////
//                             Smart pointers
//////////////////////////////////////////////////////////////////////////////

template<class T, class Deleter>
class scoped_ptr;

template<class T, class VoidPointer>
class intrusive_ptr;

template<class T, class VoidAllocator, class Deleter>
class shared_ptr;

template<class T, class VoidAllocator, class Deleter>
class weak_ptr;

//////////////////////////////////////////////////////////////////////////////
//                                  IPC
//////////////////////////////////////////////////////////////////////////////

template<class VoidPointer>
class message_queue_t;

typedef message_queue_t<offset_ptr<void> > message_queue;

}}  //namespace hboost { namespace interprocess {

#endif   //#if !defined(HBOOST_INTERPROCESS_DOXYGEN_INVOKED)

#include <hboost/interprocess/detail/config_end.hpp>

#endif //#ifndef HBOOST_INTERPROCESS_FWD_HPP
