//=======================================================================
// Copyright 2002 Indiana University.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef HBOOST_GRAPH_SELECTORS_HPP
#define HBOOST_GRAPH_SELECTORS_HPP

#include <hboost/mpl/bool.hpp>

namespace hboost {

  //===========================================================================
  // Selectors for the Directed template parameter of adjacency_list
  // and adjacency_matrix.

  struct directedS { enum { is_directed = true, is_bidir = false }; 
    typedef mpl::true_ is_directed_t; 
    typedef mpl::false_ is_bidir_t;
  };
  struct undirectedS { 
    enum { is_directed = false, is_bidir = false }; 
    typedef mpl::false_ is_directed_t;
    typedef mpl::false_ is_bidir_t;
  };
  struct bidirectionalS { 
    enum { is_directed = true, is_bidir = true }; 
    typedef mpl::true_ is_directed_t;
    typedef mpl::true_ is_bidir_t;
  };

} // namespace hboost

#endif // HBOOST_GRAPH_SELECTORS_HPP
