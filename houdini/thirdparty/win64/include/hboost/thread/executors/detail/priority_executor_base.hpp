// Copyright (C) 2014 Ian Forbed
// Copyright (C) 2014 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_THREAD_EXECUTORS_DETAIL_PRIORITY_EXECUTOR_BASE_HPP
#define HBOOST_THREAD_EXECUTORS_DETAIL_PRIORITY_EXECUTOR_BASE_HPP

#include <hboost/atomic.hpp>
#include <hboost/function.hpp>
#include <hboost/thread/thread.hpp>
#include <hboost/thread/concurrent_queues/sync_timed_queue.hpp>
#include <hboost/thread/executors/work.hpp>

namespace hboost
{
namespace executors
{
namespace detail
{
  template <class Queue>
  class priority_executor_base
  {
  public:
    //typedef hboost::function<void()> work;
    typedef executors::work_pq work;
  protected:
    typedef Queue queue_type;
    queue_type _workq;

    priority_executor_base() {}
  public:

    ~priority_executor_base()
    {
      if(!closed())
      {
        this->close();
      }
    }

    void close()
    {
      _workq.close();
    }

    bool closed()
    {
      return _workq.closed();
    }

    void loop()
    {
      try
      {
        for(;;)
        {
          work task;
          queue_op_status st = _workq.wait_pull(task);
          if (st == queue_op_status::closed) return;
          task();
        }
      }
      catch (...)
      {
        std::terminate();
        return;
      }
    }
  }; //end class

} //end detail namespace
} //end executors namespace
} //end boost namespace
#endif
