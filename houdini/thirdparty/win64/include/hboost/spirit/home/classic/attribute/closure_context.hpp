/*=============================================================================
    Copyright (c) 2002-2003 Joel de Guzman
    Copyright (c) 2002-2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_CLOSURE_CONTEXT_HPP)
#define HBOOST_SPIRIT_CLOSURE_CONTEXT_HPP

#include <hboost/spirit/home/classic/namespace.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

#if !defined(HBOOST_SPIRIT_CLOSURE_CONTEXT_LINKER_DEFINED)
#define HBOOST_SPIRIT_CLOSURE_CONTEXT_LINKER_DEFINED

///////////////////////////////////////////////////////////////////////////////
//
//  closure_context_linker
//  { helper template for the closure extendability }
//
//      This classes can be 'overloaded' (defined elsewhere), to plug
//      in additional functionality into the closure parsing process.
//
///////////////////////////////////////////////////////////////////////////////

template<typename ContextT>
struct closure_context_linker : public ContextT
{
    template <typename ParserT>
    closure_context_linker(ParserT const& p)
    : ContextT(p) {}

    template <typename ParserT, typename ScannerT>
    void pre_parse(ParserT const& p, ScannerT const& scan)
    { ContextT::pre_parse(p, scan); }

    template <typename ResultT, typename ParserT, typename ScannerT>
    ResultT&
    post_parse(ResultT& hit, ParserT const& p, ScannerT const& scan)
    { return ContextT::post_parse(hit, p, scan); }
};

#endif // !defined(HBOOST_SPIRIT_CLOSURE_CONTEXT_LINKER_DEFINED)

///////////////////////////////////////////////////////////////////////////////
HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif // HBOOST_SPIRIT_CLOSURE_CONTEXT_HPP
