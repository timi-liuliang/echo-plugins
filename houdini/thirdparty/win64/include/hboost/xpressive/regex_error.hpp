///////////////////////////////////////////////////////////////////////////////
/// \file regex_error.hpp
/// Contains the definition of the regex_error exception class.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_REGEX_ERROR_HPP_EAN_10_04_2005
#define HBOOST_XPRESSIVE_REGEX_ERROR_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

#include <string>
#include <stdexcept>
#include <hboost/throw_exception.hpp>
#include <hboost/current_function.hpp>
#include <hboost/exception/exception.hpp>
#include <hboost/exception/info.hpp>
#include <hboost/xpressive/regex_constants.hpp>

//{{AFX_DOC_COMMENT
///////////////////////////////////////////////////////////////////////////////
// This is a hack to get Doxygen to show the inheritance relation between
// regex_error and std::runtime_error.
#ifdef HBOOST_XPRESSIVE_DOXYGEN_INVOKED
/// INTERNAL ONLY
namespace std
{
    /// INTERNAL ONLY
    struct runtime_error {};
}
#endif
//}}AFX_DOC_COMMENT

namespace hboost { namespace xpressive
{

////////////////////////////////////////////////////////////////////////////////
//  regex_error
//
/// \brief The class regex_error defines the type of objects thrown as
/// exceptions to report errors during the conversion from a string representing
/// a regular expression to a finite state machine.
struct regex_error
  : std::runtime_error
  , hboost::exception
{
    /// Constructs an object of class regex_error.
    /// \param code The error_type this regex_error represents.
    /// \param str The message string of this regex_error.
    /// \post code() == code
    explicit regex_error(regex_constants::error_type code, char const *str = "")
      : std::runtime_error(str)
      , hboost::exception()
      , code_(code)
    {
    }

    /// Accessor for the error_type value
    /// \return the error_type code passed to the constructor
    /// \throw nothrow
    regex_constants::error_type code() const
    {
        return this->code_;
    }

    /// Destructor for class regex_error
    /// \throw nothrow
    virtual ~regex_error() throw()
    {}

private:

    regex_constants::error_type code_;
};

namespace detail
{
    inline bool ensure_(
        bool cond
      , regex_constants::error_type code
      , char const *msg
      , char const *fun
      , char const *file
      , unsigned long line
    )
    {
        if(!cond)
        {
            #ifndef HBOOST_EXCEPTION_DISABLE
            hboost::throw_exception(
                hboost::xpressive::regex_error(code, msg)
                    << hboost::throw_function(fun)
                    << hboost::throw_file(file)
                    << hboost::throw_line((int)line)
            );
            #else
            hboost::throw_exception(hboost::xpressive::regex_error(code, msg));
            #endif
        }
        return true;
    }
}

#define HBOOST_XPR_ENSURE_(pred, code, msg)                                                          \
    hboost::xpressive::detail::ensure_(!!(pred), code, msg, HBOOST_CURRENT_FUNCTION, __FILE__, __LINE__)  \
    /**/

}} // namespace hboost::xpressive

#endif
