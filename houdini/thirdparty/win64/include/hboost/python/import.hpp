// Copyright Stefan Seefeld 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef IMPORT_SS20050624_HPP
# define IMPORT_SS20050624_HPP

# include <hboost/python/object.hpp>
# include <hboost/python/str.hpp>

namespace hboost 
{ 
namespace python 
{

// Import the named module and return a reference to it.
object HBOOST_PYTHON_DECL import(str name);

}
}

#endif
