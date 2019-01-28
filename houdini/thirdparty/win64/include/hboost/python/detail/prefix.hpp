// Copyright David Abrahams 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PREFIX_DWA2003531_HPP
# define HBOOST_PREFIX_DWA2003531_HPP

// The rule is that <Python.h> must be included before any system
// headers (so it can get control over some awful macros).
// Unfortunately, Boost.Python needs to #include <limits.h> first, at
// least... but this gets us as close as possible.

# include <hboost/python/detail/wrap_python.hpp>
# include <hboost/python/detail/config.hpp>

#endif // HBOOST_PREFIX_DWA2003531_HPP
