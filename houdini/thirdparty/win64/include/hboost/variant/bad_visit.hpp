//-----------------------------------------------------------------------------
// boost variant/bad_visit.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_BAD_VISIT_HPP
#define HBOOST_VARIANT_BAD_VISIT_HPP

#include <exception>

namespace hboost {

//////////////////////////////////////////////////////////////////////////
// class bad_visit
//
// Exception thrown when a visitation attempt via apply_visitor fails due
// to invalid visited subtype or contents.
//
struct bad_visit
    : std::exception
{
public: // std::exception interface

    virtual const char * what() const HBOOST_NOEXCEPT_OR_NOTHROW
    {
        return "hboost::bad_visit: "
               "failed visitation using hboost::apply_visitor";
    }

};

} // namespace hboost

#endif // HBOOST_VARIANT_BAD_VISIT_HPP
