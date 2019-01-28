// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_TAG_DWA2002720_HPP
# define HBOOST_TAG_DWA2002720_HPP

# include <hboost/python/detail/prefix.hpp>

namespace hboost { namespace python { 

// used only to prevent argument-dependent lookup from finding the
// wrong function in some cases. Cheaper than qualification.
enum tag_t { tag };

}} // namespace hboost::python

#endif // HBOOST_TAG_DWA2002720_HPP
