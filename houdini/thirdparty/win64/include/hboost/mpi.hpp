// Copyright (C) 2006 Douglas Gregor <doug.gregor -at- gmail.com>.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Message Passing Interface

//  See www.boost.org/libs/mpi for documentation.

/** @file mpi.hpp
 *
 *  This file is a top-level convenience header that includes all of
 *  the Boost.MPI library headers. Users concerned about compile time
 *  may wish to include only specific headers from the Boost.MPI
 *  library.
 *
 */
#ifndef HBOOST_MPI_HPP
#define HBOOST_MPI_HPP

#include <hboost/mpi/allocator.hpp>
#include <hboost/mpi/collectives.hpp>
#include <hboost/mpi/communicator.hpp>
#include <hboost/mpi/datatype.hpp>
#include <hboost/mpi/environment.hpp>
#include <hboost/mpi/graph_communicator.hpp>
#include <hboost/mpi/group.hpp>
#include <hboost/mpi/intercommunicator.hpp>
#include <hboost/mpi/nonblocking.hpp>
#include <hboost/mpi/operations.hpp>
#include <hboost/mpi/skeleton_and_content.hpp>
#include <hboost/mpi/timer.hpp>

#endif // HBOOST_MPI_HPP
