// ----------------------------------------------------------------------------
// format.hpp :  primary header
// ----------------------------------------------------------------------------

//  Copyright Samuel Krempp 2003. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/format for library home page


// ----------------------------------------------------------------------------

#ifndef HBOOST_FORMAT_HPP
#define HBOOST_FORMAT_HPP

#include <vector>
#include <string>
#include <hboost/detail/workaround.hpp>
#include <hboost/config.hpp>

#ifndef HBOOST_NO_STD_LOCALE
#include <locale>
#endif

// ***   Compatibility framework
#include <hboost/format/detail/compat_workarounds.hpp>

#ifdef HBOOST_NO_LOCALE_ISIDIGIT
#include <cctype>  // we'll use the non-locale  <cctype>'s std::isdigit(int)
#endif

// ****  Forward declarations ----------------------------------
#include <hboost/format/format_fwd.hpp>     // basic_format<Ch,Tr>, and other frontends
#include <hboost/format/internals_fwd.hpp>  // misc forward declarations for internal use

// ****  Auxiliary structs (stream_format_state<Ch,Tr> , and format_item<Ch,Tr> )
#include <hboost/format/internals.hpp>    

// ****  Format  class  interface --------------------------------
#include <hboost/format/format_class.hpp>

// **** Exceptions -----------------------------------------------
#include <hboost/format/exceptions.hpp>

// **** Implementation -------------------------------------------
#include <hboost/format/format_implementation.hpp>   // member functions
#include <hboost/format/group.hpp>                   // class for grouping arguments
#include <hboost/format/feed_args.hpp>               // argument-feeding functions
#include <hboost/format/parsing.hpp>                 // format-string parsing (member-)functions

// **** Implementation of the free functions ----------------------
#include <hboost/format/free_funcs.hpp>


// *** Undefine 'local' macros :
#include <hboost/format/detail/unset_macros.hpp>

#endif // HBOOST_FORMAT_HPP
