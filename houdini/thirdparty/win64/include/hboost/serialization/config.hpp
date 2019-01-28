#ifndef HBOOST_SERIALIZATION_CONFIG_HPP
#define HBOOST_SERIALIZATION_CONFIG_HPP

//  config.hpp  ---------------------------------------------//

//  (c) Copyright Robert Ramey 2004
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/serialization

//----------------------------------------------------------------------------// 

// This header implements separate compilation features as described in
// http://www.boost.org/more/separate_compilation.html

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>

// note: this version incorporates the related code into the the 
// the same library as HBOOST_ARCHIVE.  This could change some day in the
// future

// if HBOOST_SERIALIZATION_DECL is defined undefine it now:
#ifdef HBOOST_SERIALIZATION_DECL
    #undef HBOOST_SERIALIZATION_DECL
#endif

// we need to import/export our code only if the user has specifically
// asked for it by defining either HBOOST_ALL_DYN_LINK if they want all boost
// libraries to be dynamically linked, or HBOOST_SERIALIZATION_DYN_LINK
// if they want just this one to be dynamically liked:
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_SERIALIZATION_DYN_LINK)
    #if !defined(HBOOST_DYN_LINK)
        #define HBOOST_DYN_LINK
    #endif
    // export if this is our own source, otherwise import:
    #if defined(HBOOST_SERIALIZATION_SOURCE)
        #define HBOOST_SERIALIZATION_DECL HBOOST_SYMBOL_EXPORT
    #else
        #define HBOOST_SERIALIZATION_DECL HBOOST_SYMBOL_IMPORT
    #endif // defined(HBOOST_SERIALIZATION_SOURCE)
#endif // defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_SERIALIZATION_DYN_LINK)

// if HBOOST_SERIALIZATION_DECL isn't defined yet define it now:
#ifndef HBOOST_SERIALIZATION_DECL
    #define HBOOST_SERIALIZATION_DECL
#endif

//  enable automatic library variant selection  ------------------------------// 

#if !defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_SERIALIZATION_NO_LIB) \
&&  !defined(HBOOST_ARCHIVE_SOURCE) && !defined(HBOOST_WARCHIVE_SOURCE)  \
&&  !defined(HBOOST_SERIALIZATION_SOURCE)
    //
    // Set the name of our library, this will get undef'ed by auto_link.hpp
    // once it's done with it:
    //
    #define HBOOST_LIB_NAME hboost_serialization
    //
    // If we're importing code from a dll, then tell auto_link.hpp about it:
    //
    #if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_SERIALIZATION_DYN_LINK)
    #  define HBOOST_DYN_LINK
    #endif
    //
    // And include the header that does the work:
    //
    #include <hboost/config/auto_link.hpp>

#endif  

#endif // HBOOST_SERIALIZATION_CONFIG_HPP
