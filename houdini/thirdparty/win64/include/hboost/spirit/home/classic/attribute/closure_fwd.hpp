/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(HBOOST_SPIRIT_CLOSURE_FWD_HPP)
#define HBOOST_SPIRIT_CLOSURE_FWD_HPP

#include <hboost/spirit/home/classic/namespace.hpp>
#include <hboost/spirit/home/classic/phoenix/tuples.hpp>

#if !defined(HBOOST_SPIRIT_CLOSURE_LIMIT)
#   define HBOOST_SPIRIT_CLOSURE_LIMIT PHOENIX_LIMIT
#endif

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    template<typename ClosureT>
    class closure_context;

    template <typename ClosureT>
    class init_closure_context;

    template <typename ParserT, typename ActorTupleT>
    struct init_closure_parser;

    template <
            typename DerivedT
        ,   typename T0 = ::hboostphoenix::nil_t
        ,   typename T1 = ::hboostphoenix::nil_t
        ,   typename T2 = ::hboostphoenix::nil_t

    #if HBOOST_SPIRIT_CLOSURE_LIMIT > 3
        ,   typename T3 = ::hboostphoenix::nil_t
        ,   typename T4 = ::hboostphoenix::nil_t
        ,   typename T5 = ::hboostphoenix::nil_t

    #if HBOOST_SPIRIT_CLOSURE_LIMIT > 6
        ,   typename T6 = ::hboostphoenix::nil_t
        ,   typename T7 = ::hboostphoenix::nil_t
        ,   typename T8 = ::hboostphoenix::nil_t

    #if HBOOST_SPIRIT_CLOSURE_LIMIT > 9
        ,   typename T9 = ::hboostphoenix::nil_t
        ,   typename T10 = ::hboostphoenix::nil_t
        ,   typename T11 = ::hboostphoenix::nil_t

    #if HBOOST_SPIRIT_CLOSURE_LIMIT > 12
        ,   typename T12 = ::hboostphoenix::nil_t
        ,   typename T13 = ::hboostphoenix::nil_t
        ,   typename T14 = ::hboostphoenix::nil_t

    #endif
    #endif
    #endif
    #endif
    >
    struct closure;

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif

