// Copyright 2005 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef HBOOST_GRAPH_ACCOUNTING_HPP
#define HBOOST_GRAPH_ACCOUNTING_HPP

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <hboost/mpi/config.hpp>

namespace hboost { namespace graph { namespace accounting {

typedef double time_type;

inline time_type get_time()
{
  return MPI_Wtime();
}

inline std::string print_time(time_type t)
{
  std::ostringstream out;
  out << std::setiosflags(std::ios::fixed) << std::setprecision(2) << t;
  return out.str();
}

} } } // end namespace hboost::graph::accounting

#endif // HBOOST_GRAPH_ACCOUNTING_HPP
