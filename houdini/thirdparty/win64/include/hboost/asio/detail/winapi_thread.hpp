//
// detail/winapi_thread.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_WINAPI_THREAD_HPP
#define HBOOST_ASIO_DETAIL_WINAPI_THREAD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_WINDOWS)
#if defined(HBOOST_ASIO_WINDOWS_APP) || defined(UNDER_CE)

#include <memory>
#include <hboost/asio/detail/noncopyable.hpp>
#include <hboost/asio/detail/socket_types.hpp>
#include <hboost/asio/detail/throw_error.hpp>
#include <hboost/asio/error.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

DWORD WINAPI winapi_thread_function(LPVOID arg);

class winapi_thread
  : private noncopyable
{
public:
  // Constructor.
  template <typename Function>
  winapi_thread(Function f, unsigned int = 0)
  {
    std::auto_ptr<func_base> arg(new func<Function>(f));
    DWORD thread_id = 0;
    thread_ = ::CreateThread(0, 0, winapi_thread_function,
        arg.get(), 0, &thread_id);
    if (!thread_)
    {
      DWORD last_error = ::GetLastError();
      hboost::system::error_code ec(last_error,
          hboost::asio::error::get_system_category());
      hboost::asio::detail::throw_error(ec, "thread");
    }
    arg.release();
  }

  // Destructor.
  ~winapi_thread()
  {
    ::CloseHandle(thread_);
  }

  // Wait for the thread to exit.
  void join()
  {
#if defined(HBOOST_ASIO_WINDOWS_APP)
    ::WaitForSingleObjectEx(thread_, INFINITE, false);
#else // defined(HBOOST_ASIO_WINDOWS_APP)
    ::WaitForSingleObject(thread_, INFINITE);
#endif // defined(HBOOST_ASIO_WINDOWS_APP)
  }

private:
  friend DWORD WINAPI winapi_thread_function(LPVOID arg);

  class func_base
  {
  public:
    virtual ~func_base() {}
    virtual void run() = 0;
  };

  template <typename Function>
  class func
    : public func_base
  {
  public:
    func(Function f)
      : f_(f)
    {
    }

    virtual void run()
    {
      f_();
    }

  private:
    Function f_;
  };

  ::HANDLE thread_;
};

inline DWORD WINAPI winapi_thread_function(LPVOID arg)
{
  std::auto_ptr<winapi_thread::func_base> func(
      static_cast<winapi_thread::func_base*>(arg));
  func->run();
  return 0;
}

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_WINDOWS_APP) || defined(UNDER_CE)
#endif // defined(HBOOST_ASIO_WINDOWS)

#endif // HBOOST_ASIO_DETAIL_WINAPI_THREAD_HPP
