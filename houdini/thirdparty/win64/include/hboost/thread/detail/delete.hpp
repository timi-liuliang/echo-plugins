// Copyright (C) 2012 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_THREAD_DETAIL_DELETE_HPP
#define HBOOST_THREAD_DETAIL_DELETE_HPP

#include <hboost/config.hpp>

/**
 * HBOOST_THREAD_DELETE_COPY_CTOR deletes the copy constructor when the compiler supports it or
 * makes it private.
 *
 * HBOOST_THREAD_DELETE_COPY_ASSIGN deletes the copy assignment when the compiler supports it or
 * makes it private.
 */

#if ! defined HBOOST_NO_CXX11_DELETED_FUNCTIONS && ! defined HBOOST_NO_CXX11_RVALUE_REFERENCES
#define HBOOST_THREAD_DELETE_COPY_CTOR(CLASS) \
      CLASS(CLASS const&) = delete; \

#define HBOOST_THREAD_DELETE_COPY_ASSIGN(CLASS) \
      CLASS& operator=(CLASS const&) = delete;

#else // HBOOST_NO_CXX11_DELETED_FUNCTIONS
#if defined(HBOOST_MSVC) && _MSC_VER >= 1600
#define HBOOST_THREAD_DELETE_COPY_CTOR(CLASS) \
    private: \
      CLASS(CLASS const&); \
    public:

#define HBOOST_THREAD_DELETE_COPY_ASSIGN(CLASS) \
    private: \
      CLASS& operator=(CLASS const&); \
    public:
#else
#define HBOOST_THREAD_DELETE_COPY_CTOR(CLASS) \
    private: \
      CLASS(CLASS&); \
    public:

#define HBOOST_THREAD_DELETE_COPY_ASSIGN(CLASS) \
    private: \
      CLASS& operator=(CLASS&); \
    public:
#endif
#endif // HBOOST_NO_CXX11_DELETED_FUNCTIONS

/**
 * HBOOST_THREAD_NO_COPYABLE deletes the copy constructor and assignment when the compiler supports it or
 * makes them private.
 */
#define HBOOST_THREAD_NO_COPYABLE(CLASS) \
    HBOOST_THREAD_DELETE_COPY_CTOR(CLASS) \
    HBOOST_THREAD_DELETE_COPY_ASSIGN(CLASS)

#endif // HBOOST_THREAD_DETAIL_DELETE_HPP
