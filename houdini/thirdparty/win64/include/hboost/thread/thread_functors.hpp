// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2009-2012 Anthony Williams
// (C) Copyright 2012 Vicente J. Botet Escriba

// Based on the Anthony's idea of scoped_thread in CCiA

#ifndef HBOOST_THREAD_THREAD_FUNCTORS_HPP
#define HBOOST_THREAD_THREAD_FUNCTORS_HPP

#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/detail/delete.hpp>
#include <hboost/thread/detail/move.hpp>
#include <hboost/thread/thread_only.hpp>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{

  struct detach
  {
    void operator()(thread& t)
    {
      t.detach();
    }
  };

  struct join_if_joinable
  {
    void operator()(thread& t)
    {
      if (t.joinable())
      {
        t.join();
      }
    }
  };

#if defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS
  struct interrupt_and_join_if_joinable
  {
    void operator()(thread& t)
    {
      t.interrupt();
      if (t.joinable())
      {
        t.join();
      }
    }
  };
#endif
}
#include <hboost/config/abi_suffix.hpp>

#endif
