//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTERPROCESS_DETAIL_PTIME_TO_TIMESPEC_HPP
#define HBOOST_INTERPROCESS_DETAIL_PTIME_TO_TIMESPEC_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif
#
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/interprocess/detail/posix_time_types_wrk.hpp>

namespace hboost {

namespace interprocess {

namespace ipcdetail {

inline timespec ptime_to_timespec (const hboost::posix_time::ptime &tm)
{
   const hboost::posix_time::ptime epoch(hboost::gregorian::date(1970,1,1));
   //Avoid negative absolute times
   hboost::posix_time::time_duration duration  = (tm <= epoch) ? hboost::posix_time::time_duration(epoch - epoch)
                                                              : hboost::posix_time::time_duration(tm - epoch);
   timespec ts;
   ts.tv_sec  = duration.total_seconds();
   ts.tv_nsec = duration.total_nanoseconds() % 1000000000;
   return ts;
}

}  //namespace ipcdetail {

}  //namespace interprocess {

}  //namespace hboost {

#endif   //ifndef HBOOST_INTERPROCESS_DETAIL_PTIME_TO_TIMESPEC_HPP
