// ----------------------------------------------------------------------------
// unset_macros.hpp
// ----------------------------------------------------------------------------

//  Copyright Samuel Krempp 2003. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/format for library home page

// ----------------------------------------------------------------------------

// *** Undefine 'local' macros :
#ifdef HBOOST_NO_OVERLOAD_FOR_NON_CONST
#undef HBOOST_NO_OVERLOAD_FOR_NON_CONST
#endif
#ifdef HBOOST_NO_LOCALE_ISDIGIT
#undef HBOOST_NO_LOCALE_ISDIGIT
#endif
#ifdef HBOOST_IO_STD
#undef HBOOST_IO_STD
#endif
#ifdef HBOOST_IO_NEEDS_USING_DECLARATION
#undef HBOOST_IO_NEEDS_USING_DECLARATION
#endif
#ifdef HBOOST_NO_TEMPLATE_STD_STREAM
#undef HBOOST_NO_TEMPLATE_STD_STREAM
#endif
#ifdef HBOOST_FORMAT_STREAMBUF_DEFINED
#undef HBOOST_FORMAT_STREAMBUF_DEFINED
#endif
#ifdef HBOOST_FORMAT_OSTREAM_DEFINED
#undef HBOOST_FORMAT_OSTREAM_DEFINED
#endif
