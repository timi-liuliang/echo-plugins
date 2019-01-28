//  hboost/filesystem/convenience.hpp  ----------------------------------------//

//  Copyright Beman Dawes, 2002-2005
//  Copyright Vladimir Prus, 2002
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/filesystem

//----------------------------------------------------------------------------// 

#ifndef HBOOST_FILESYSTEM3_CONVENIENCE_HPP
#define HBOOST_FILESYSTEM3_CONVENIENCE_HPP

#include <hboost/config.hpp>

# if defined( HBOOST_NO_STD_WSTRING )
#   error Configuration not supported: Boost.Filesystem V3 and later requires std::wstring support
# endif

#include <hboost/filesystem/operations.hpp>
#include <hboost/system/error_code.hpp>

#include <hboost/config/abi_prefix.hpp> // must be the last #include

namespace hboost
{
  namespace filesystem
  {

# ifndef HBOOST_FILESYSTEM_NO_DEPRECATED

    inline std::string extension(const path & p)
    {
      return p.extension().string();
    }

    inline std::string basename(const path & p)
    {
      return p.stem().string();
    }

    inline path change_extension( const path & p, const path & new_extension )
    { 
      path new_p( p );
      new_p.replace_extension( new_extension );
      return new_p;
    }

# endif


  } // namespace filesystem
} // namespace hboost

#include <hboost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas
#endif // HBOOST_FILESYSTEM3_CONVENIENCE_HPP
