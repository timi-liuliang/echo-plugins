///////////////////////////////////////////////////////////////////////////////
/// \file algorithm.hpp
///   Includes the range-based versions of the algorithms in the
///   C++ standard header file <algorithm>
//
/////////////////////////////////////////////////////////////////////////////

// Copyright 2009 Neil Groves.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Acknowledgements:
// This code uses combinations of ideas, techniques and code snippets
// from: Thorsten Ottosen, Eric Niebler, Jeremy Siek,
// and Vladimir Prus'
//
// The original mutating algorithms that served as the first version
// were originally written by Vladimir Prus'
// <ghost@cs.msu.su> code from Boost Wiki

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef HBOOST_RANGE_ALGORITHM_HPP_INCLUDED_01012009
#define HBOOST_RANGE_ALGORITHM_HPP_INCLUDED_01012009

#include <hboost/range/concepts.hpp>
#include <hboost/range/iterator_range.hpp>
#include <hboost/range/difference_type.hpp>
#include <hboost/range/detail/range_return.hpp>
#include <hboost/iterator/iterator_traits.hpp>
#include <hboost/next_prior.hpp>
#include <algorithm>

// Non-mutating algorithms
#include <hboost/range/algorithm/adjacent_find.hpp>
#include <hboost/range/algorithm/count.hpp>
#include <hboost/range/algorithm/count_if.hpp>
#include <hboost/range/algorithm/equal.hpp>
#include <hboost/range/algorithm/for_each.hpp>
#include <hboost/range/algorithm/find.hpp>
#include <hboost/range/algorithm/find_end.hpp>
#include <hboost/range/algorithm/find_first_of.hpp>
#include <hboost/range/algorithm/find_if.hpp>
#include <hboost/range/algorithm/lexicographical_compare.hpp>
#include <hboost/range/algorithm/mismatch.hpp>
#include <hboost/range/algorithm/search.hpp>
#include <hboost/range/algorithm/search_n.hpp>

// Mutating algorithms
#include <hboost/range/algorithm/copy.hpp>
#include <hboost/range/algorithm/copy_backward.hpp>
#include <hboost/range/algorithm/fill.hpp>
#include <hboost/range/algorithm/fill_n.hpp>
#include <hboost/range/algorithm/generate.hpp>
#include <hboost/range/algorithm/inplace_merge.hpp>
#include <hboost/range/algorithm/merge.hpp>
#include <hboost/range/algorithm/nth_element.hpp>
#include <hboost/range/algorithm/partial_sort.hpp>
#include <hboost/range/algorithm/partial_sort_copy.hpp>
#include <hboost/range/algorithm/partition.hpp>
#include <hboost/range/algorithm/random_shuffle.hpp>
#include <hboost/range/algorithm/remove.hpp>
#include <hboost/range/algorithm/remove_copy.hpp>
#include <hboost/range/algorithm/remove_copy_if.hpp>
#include <hboost/range/algorithm/remove_if.hpp>
#include <hboost/range/algorithm/replace.hpp>
#include <hboost/range/algorithm/replace_copy.hpp>
#include <hboost/range/algorithm/replace_copy_if.hpp>
#include <hboost/range/algorithm/replace_if.hpp>
#include <hboost/range/algorithm/reverse.hpp>
#include <hboost/range/algorithm/reverse_copy.hpp>
#include <hboost/range/algorithm/rotate.hpp>
#include <hboost/range/algorithm/rotate_copy.hpp>
#include <hboost/range/algorithm/sort.hpp>
#include <hboost/range/algorithm/stable_partition.hpp>
#include <hboost/range/algorithm/stable_sort.hpp>
#include <hboost/range/algorithm/transform.hpp>
#include <hboost/range/algorithm/unique.hpp>
#include <hboost/range/algorithm/unique_copy.hpp>

// Binary search
#include <hboost/range/algorithm/binary_search.hpp>
#include <hboost/range/algorithm/equal_range.hpp>
#include <hboost/range/algorithm/lower_bound.hpp>
#include <hboost/range/algorithm/upper_bound.hpp>

// Set operations of sorted ranges
#include <hboost/range/algorithm/set_algorithm.hpp>

// Heap operations
#include <hboost/range/algorithm/heap_algorithm.hpp>

// Minimum and Maximum
#include <hboost/range/algorithm/max_element.hpp>
#include <hboost/range/algorithm/min_element.hpp>

// Permutations
#include <hboost/range/algorithm/permutation.hpp>

#endif // include guard

