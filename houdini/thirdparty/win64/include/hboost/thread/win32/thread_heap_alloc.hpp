// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams
#ifndef HBOOST_THREAD_HEAP_ALLOC_WIN32_HPP
#define HBOOST_THREAD_HEAP_ALLOC_WIN32_HPP
#include <new>
#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/win32/thread_primitives.hpp>
#include <stdexcept>
#include <hboost/assert.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/core/no_exceptions_support.hpp>

#if defined( HBOOST_USE_WINDOWS_H )
# include <windows.h>

namespace hboost
{
    namespace detail
    {
        namespace win32
        {
            using ::GetProcessHeap;
            using ::HeapAlloc;
            using ::HeapFree;
        }
    }
}

#else

# ifdef HeapAlloc
# undef HeapAlloc
# endif

namespace hboost
{
    namespace detail
    {
        namespace win32
        {
            extern "C"
            {
                __declspec(dllimport) handle __stdcall GetProcessHeap();
                __declspec(dllimport) void* __stdcall HeapAlloc(handle,unsigned long,ulong_ptr);
                __declspec(dllimport) int __stdcall HeapFree(handle,unsigned long,void*);
            }
        }
    }
}

#endif

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
    namespace detail
    {
        inline void* allocate_raw_heap_memory(unsigned size)
        {
            void* const heap_memory=detail::win32::HeapAlloc(detail::win32::GetProcessHeap(),0,size);
            if(!heap_memory)
            {
                hboost::throw_exception(std::bad_alloc());
            }
            return heap_memory;
        }

        inline void free_raw_heap_memory(void* heap_memory)
        {
            HBOOST_VERIFY(detail::win32::HeapFree(detail::win32::GetProcessHeap(),0,heap_memory)!=0);
        }

        template<typename T>
        inline T* heap_new()
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T();
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
        template<typename T,typename A1>
        inline T* heap_new(A1&& a1)
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T(static_cast<A1&&>(a1));
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }
        template<typename T,typename A1,typename A2>
        inline T* heap_new(A1&& a1,A2&& a2)
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T(static_cast<A1&&>(a1),static_cast<A2&&>(a2));
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }
        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1&& a1,A2&& a2,A3&& a3)
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T(static_cast<A1&&>(a1),static_cast<A2&&>(a2),
                                                  static_cast<A3&&>(a3));
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1&& a1,A2&& a2,A3&& a3,A4&& a4)
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T(static_cast<A1&&>(a1),static_cast<A2&&>(a2),
                                                  static_cast<A3&&>(a3),static_cast<A4&&>(a4));
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }
#else
        template<typename T,typename A1>
        inline T* heap_new_impl(A1 a1)
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T(a1);
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }

        template<typename T,typename A1,typename A2>
        inline T* heap_new_impl(A1 a1,A2 a2)
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T(a1,a2);
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }

        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new_impl(A1 a1,A2 a2,A3 a3)
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T(a1,a2,a3);
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }

        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new_impl(A1 a1,A2 a2,A3 a3,A4 a4)
        {
            void* const heap_memory=allocate_raw_heap_memory(sizeof(T));
            HBOOST_TRY
            {
                T* const data=new (heap_memory) T(a1,a2,a3,a4);
                return data;
            }
            HBOOST_CATCH(...)
            {
                free_raw_heap_memory(heap_memory);
                HBOOST_RETHROW
            }
            HBOOST_CATCH_END
        }


        template<typename T,typename A1>
        inline T* heap_new(A1 const& a1)
        {
            return heap_new_impl<T,A1 const&>(a1);
        }
        template<typename T,typename A1>
        inline T* heap_new(A1& a1)
        {
            return heap_new_impl<T,A1&>(a1);
        }

        template<typename T,typename A1,typename A2>
        inline T* heap_new(A1 const& a1,A2 const& a2)
        {
            return heap_new_impl<T,A1 const&,A2 const&>(a1,a2);
        }
        template<typename T,typename A1,typename A2>
        inline T* heap_new(A1& a1,A2 const& a2)
        {
            return heap_new_impl<T,A1&,A2 const&>(a1,a2);
        }
        template<typename T,typename A1,typename A2>
        inline T* heap_new(A1 const& a1,A2& a2)
        {
            return heap_new_impl<T,A1 const&,A2&>(a1,a2);
        }
        template<typename T,typename A1,typename A2>
        inline T* heap_new(A1& a1,A2& a2)
        {
            return heap_new_impl<T,A1&,A2&>(a1,a2);
        }

        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1 const& a1,A2 const& a2,A3 const& a3)
        {
            return heap_new_impl<T,A1 const&,A2 const&,A3 const&>(a1,a2,a3);
        }
        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1& a1,A2 const& a2,A3 const& a3)
        {
            return heap_new_impl<T,A1&,A2 const&,A3 const&>(a1,a2,a3);
        }
        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1 const& a1,A2& a2,A3 const& a3)
        {
            return heap_new_impl<T,A1 const&,A2&,A3 const&>(a1,a2,a3);
        }
        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1& a1,A2& a2,A3 const& a3)
        {
            return heap_new_impl<T,A1&,A2&,A3 const&>(a1,a2,a3);
        }

        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1 const& a1,A2 const& a2,A3& a3)
        {
            return heap_new_impl<T,A1 const&,A2 const&,A3&>(a1,a2,a3);
        }
        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1& a1,A2 const& a2,A3& a3)
        {
            return heap_new_impl<T,A1&,A2 const&,A3&>(a1,a2,a3);
        }
        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1 const& a1,A2& a2,A3& a3)
        {
            return heap_new_impl<T,A1 const&,A2&,A3&>(a1,a2,a3);
        }
        template<typename T,typename A1,typename A2,typename A3>
        inline T* heap_new(A1& a1,A2& a2,A3& a3)
        {
            return heap_new_impl<T,A1&,A2&,A3&>(a1,a2,a3);
        }

        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1 const& a1,A2 const& a2,A3 const& a3,A4 const& a4)
        {
            return heap_new_impl<T,A1 const&,A2 const&,A3 const&,A4 const&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1& a1,A2 const& a2,A3 const& a3,A4 const& a4)
        {
            return heap_new_impl<T,A1&,A2 const&,A3 const&,A4 const&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1 const& a1,A2& a2,A3 const& a3,A4 const& a4)
        {
            return heap_new_impl<T,A1 const&,A2&,A3 const&,A4 const&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1& a1,A2& a2,A3 const& a3,A4 const& a4)
        {
            return heap_new_impl<T,A1&,A2&,A3 const&,A4 const&>(a1,a2,a3,a4);
        }

        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1 const& a1,A2 const& a2,A3& a3,A4 const& a4)
        {
            return heap_new_impl<T,A1 const&,A2 const&,A3&,A4 const&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1& a1,A2 const& a2,A3& a3,A4 const& a4)
        {
            return heap_new_impl<T,A1&,A2 const&,A3&,A4 const&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1 const& a1,A2& a2,A3& a3,A4 const& a4)
        {
            return heap_new_impl<T,A1 const&,A2&,A3&,A4 const&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1& a1,A2& a2,A3& a3,A4 const& a4)
        {
            return heap_new_impl<T,A1&,A2&,A3&,A4 const&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1 const& a1,A2 const& a2,A3 const& a3,A4& a4)
        {
            return heap_new_impl<T,A1 const&,A2 const&,A3 const&,A4&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1& a1,A2 const& a2,A3 const& a3,A4& a4)
        {
            return heap_new_impl<T,A1&,A2 const&,A3 const&,A4&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1 const& a1,A2& a2,A3 const& a3,A4& a4)
        {
            return heap_new_impl<T,A1 const&,A2&,A3 const&,A4&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1& a1,A2& a2,A3 const& a3,A4& a4)
        {
            return heap_new_impl<T,A1&,A2&,A3 const&,A4&>(a1,a2,a3,a4);
        }

        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1 const& a1,A2 const& a2,A3& a3,A4& a4)
        {
            return heap_new_impl<T,A1 const&,A2 const&,A3&,A4&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1& a1,A2 const& a2,A3& a3,A4& a4)
        {
            return heap_new_impl<T,A1&,A2 const&,A3&,A4&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1 const& a1,A2& a2,A3& a3,A4& a4)
        {
            return heap_new_impl<T,A1 const&,A2&,A3&,A4&>(a1,a2,a3,a4);
        }
        template<typename T,typename A1,typename A2,typename A3,typename A4>
        inline T* heap_new(A1& a1,A2& a2,A3& a3,A4& a4)
        {
            return heap_new_impl<T,A1&,A2&,A3&,A4&>(a1,a2,a3,a4);
        }

#endif
        template<typename T>
        inline void heap_delete(T* data)
        {
            data->~T();
            free_raw_heap_memory(data);
        }

        template<typename T>
        struct do_heap_delete
        {
            void operator()(T* data) const
            {
                detail::heap_delete(data);
            }
        };
    }
}

#include <hboost/config/abi_suffix.hpp>


#endif
