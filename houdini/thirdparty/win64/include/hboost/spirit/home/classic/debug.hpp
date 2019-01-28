/*=============================================================================
    Copyright (c) 2001-2003 Joel de Guzman
    Copyright (c) 2002-2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_DEBUG_MAIN_HPP)
#define HBOOST_SPIRIT_DEBUG_MAIN_HPP

///////////////////////////////////////////////////////////////////////////
#if defined(HBOOST_SPIRIT_DEBUG)

#include <hboost/spirit/home/classic/version.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit.Debug includes and defines
//
///////////////////////////////////////////////////////////////////////////////

    #include <iostream>

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The HBOOST_SPIRIT_DEBUG_OUT defines the stream object, which should be used
    //  for debug diagnostics. This defaults to std::cout.
    //
    ///////////////////////////////////////////////////////////////////////////
    #if !defined(HBOOST_SPIRIT_DEBUG_OUT)
    #define HBOOST_SPIRIT_DEBUG_OUT std::cout
    #endif

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The HBOOST_SPIRIT_DEBUG_PRINT_SOME constant defines the number of characters
    //  from the stream to be printed for diagnosis. This defaults to the first
    //  20 characters.
    //
    ///////////////////////////////////////////////////////////////////////////
    #if !defined(HBOOST_SPIRIT_DEBUG_PRINT_SOME)
    #define HBOOST_SPIRIT_DEBUG_PRINT_SOME 20
    #endif

    ///////////////////////////////////////////////////////////////////////////
    //
    //  Additional HBOOST_SPIRIT_DEBUG_FLAGS control the level of diagnostics printed
    //  Basic constants are defined in debug/minimal.hpp.
    //
    ///////////////////////////////////////////////////////////////////////////
    #define HBOOST_SPIRIT_DEBUG_FLAGS_NODES        0x0001  // node diagnostics
    #define HBOOST_SPIRIT_DEBUG_FLAGS_ESCAPE_CHAR  0x0002  // escape_char_parse diagnostics
    #define HBOOST_SPIRIT_DEBUG_FLAGS_TREES        0x0004  // parse tree/ast diagnostics
    #define HBOOST_SPIRIT_DEBUG_FLAGS_CLOSURES     0x0008  // closure diagnostics
    #define HBOOST_SPIRIT_DEBUG_FLAGS_SLEX         0x8000  // slex diagnostics

    #define HBOOST_SPIRIT_DEBUG_FLAGS_MAX          0xFFFF  // print maximal diagnostics

    #if !defined(HBOOST_SPIRIT_DEBUG_FLAGS)
    #define HBOOST_SPIRIT_DEBUG_FLAGS HBOOST_SPIRIT_DEBUG_FLAGS_MAX
    #endif

    ///////////////////////////////////////////////////////////////////////////
    //
    //  By default all nodes are traced (even those, not registered with
    //  HBOOST_SPIRIT_DEBUG_RULE et.al. - see below). The following constant may be
    //  used to redefine this default.
    //
    ///////////////////////////////////////////////////////////////////////////
    #if !defined(HBOOST_SPIRIT_DEBUG_TRACENODE)
    #define HBOOST_SPIRIT_DEBUG_TRACENODE          (true)
    #endif // !defined(HBOOST_SPIRIT_DEBUG_TRACENODE)

    ///////////////////////////////////////////////////////////////////////////
    //
    //  Helper macros for giving rules and subrules a name accessible through
    //  parser_name() functions (see parser_names.hpp).
    //
    //  Additionally, the macros HBOOST_SPIRIT_DEBUG_RULE, SPIRIT_DEBUG_NODE and
    //  HBOOST_SPIRIT_DEBUG_GRAMMAR enable/disable the tracing of the 
    //  correspondingnode accordingly to the PP constant 
    //  HBOOST_SPIRIT_DEBUG_TRACENODE.
    //
    //  The macros HBOOST_SPIRIT_DEBUG_TRACE_RULE, HBOOST_SPIRIT_DEBUG_TRACE_NODE 
    //  and HBOOST_SPIRIT_DEBUG_TRACE_GRAMMAR allow to specify a flag to define, 
    //  whether the corresponding node is to be traced or not.
    //
    ///////////////////////////////////////////////////////////////////////////
    #if !defined(HBOOST_SPIRIT_DEBUG_RULE)
    #define HBOOST_SPIRIT_DEBUG_RULE(r)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, #r, HBOOST_SPIRIT_DEBUG_TRACENODE)
    #endif // !defined(HBOOST_SPIRIT_DEBUG_RULE)

    #if !defined(HBOOST_SPIRIT_DEBUG_NODE)
    #define HBOOST_SPIRIT_DEBUG_NODE(r)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, #r, HBOOST_SPIRIT_DEBUG_TRACENODE)
    #endif // !defined(HBOOST_SPIRIT_DEBUG_NODE)

    #if !defined(HBOOST_SPIRIT_DEBUG_GRAMMAR)
    #define HBOOST_SPIRIT_DEBUG_GRAMMAR(r)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, #r, HBOOST_SPIRIT_DEBUG_TRACENODE)
    #endif // !defined(HBOOST_SPIRIT_DEBUG_GRAMMAR)

    #if !defined(HBOOST_SPIRIT_DEBUG_TRACE_RULE)
    #define HBOOST_SPIRIT_DEBUG_TRACE_RULE(r, t)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, #r, (t))
    #endif // !defined(HBOOST_SPIRIT_TRACE_RULE)

    #if !defined(HBOOST_SPIRIT_DEBUG_TRACE_NODE)
    #define HBOOST_SPIRIT_DEBUG_TRACE_NODE(r, t)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, #r, (t))
    #endif // !defined(HBOOST_SPIRIT_DEBUG_TRACE_NODE)

    #if !defined(HBOOST_SPIRIT_DEBUG_TRACE_GRAMMAR)
    #define HBOOST_SPIRIT_DEBUG_TRACE_GRAMMAR(r, t)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, #r, (t))
    #endif // !defined(HBOOST_SPIRIT_DEBUG_TRACE_GRAMMAR)

    #if !defined(HBOOST_SPIRIT_DEBUG_TRACE_RULE_NAME)
    #define HBOOST_SPIRIT_DEBUG_TRACE_RULE_NAME(r, n, t)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, (n), (t))
    #endif // !defined(HBOOST_SPIRIT_DEBUG_TRACE_RULE_NAME)

    #if !defined(HBOOST_SPIRIT_DEBUG_TRACE_NODE_NAME)
    #define HBOOST_SPIRIT_DEBUG_TRACE_NODE_NAME(r, n, t)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, (n), (t))
    #endif // !defined(HBOOST_SPIRIT_DEBUG_TRACE_NODE_NAME)

    #if !defined(HBOOST_SPIRIT_DEBUG_TRACE_GRAMMAR_NAME)
    #define HBOOST_SPIRIT_DEBUG_TRACE_GRAMMAR_NAME(r, n, t)    \
        ::HBOOST_SPIRIT_CLASSIC_NS::impl::get_node_registry(). \
            register_node(&r, (n), (t))
    #endif // !defined(HBOOST_SPIRIT_DEBUG_TRACE_GRAMMAR_NAME)

    //////////////////////////////////
    #include <hboost/spirit/home/classic/debug/debug_node.hpp>

#else
    //////////////////////////////////
    #include <hboost/spirit/home/classic/debug/minimal.hpp>

#endif // HBOOST_SPIRIT_DEBUG

#endif

