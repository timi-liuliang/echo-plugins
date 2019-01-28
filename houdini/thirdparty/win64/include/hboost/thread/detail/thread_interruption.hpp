#ifndef HBOOST_THREAD_DETAIL_THREAD_INTERRUPTION_HPP
#define HBOOST_THREAD_DETAIL_THREAD_INTERRUPTION_HPP
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007-9 Anthony Williams
// (C) Copyright 2012 Vicente J. Botet Escriba

#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/detail/delete.hpp>

#if defined HBOOST_THREAD_PROVIDES_INTERRUPTIONS

namespace hboost
{
    namespace this_thread
    {
        class HBOOST_THREAD_DECL disable_interruption
        {
          bool interruption_was_enabled;
          friend class restore_interruption;
        public:
            HBOOST_THREAD_NO_COPYABLE(disable_interruption)
            disable_interruption() HBOOST_NOEXCEPT;
            ~disable_interruption() HBOOST_NOEXCEPT;
        };

        class HBOOST_THREAD_DECL restore_interruption
        {
        public:
            HBOOST_THREAD_NO_COPYABLE(restore_interruption)
            explicit restore_interruption(disable_interruption& d) HBOOST_NOEXCEPT;
            ~restore_interruption() HBOOST_NOEXCEPT;
        };
    }
}

#endif // HBOOST_THREAD_PROVIDES_INTERRUPTIONS
#endif // header
