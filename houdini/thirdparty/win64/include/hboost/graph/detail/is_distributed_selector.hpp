// Copyright 2012 The Trustees of Indiana University.

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Jeremiah Willcock
//           Andrew Lumsdaine

// Selector to determine whether a selector is distributedS (can only be true
// if <hboost/graph/distributed/selector.hpp> has been included) so that we can
// disable various sequential-graph-only traits specializations for distributed
// graphs.

#ifndef HBOOST_GRAPH_DETAIL_IS_DISTRIBUTED_SELECTOR_HPP
#define HBOOST_GRAPH_DETAIL_IS_DISTRIBUTED_SELECTOR_HPP

#include <hboost/mpl/bool.hpp>

namespace hboost {
  namespace detail {
    template <typename> struct is_distributed_selector: hboost::mpl::false_ {};
  }
}

#endif // HBOOST_GRAPH_DETAIL_IS_DISTRIBUTED_SELECTOR_HPP
