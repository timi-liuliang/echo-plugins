/*=============================================================================
    Copyright (c) 2003 Joel de Guzman
    Copyright (c) 2003 Vaclav Vesely
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef HBOOST_SPIRIT_LAZY_HPP
#define HBOOST_SPIRIT_LAZY_HPP

////////////////////////////////////////////////////////////////////////////////
#include <hboost/spirit/home/classic/namespace.hpp>
#include <hboost/spirit/home/classic/core/parser.hpp>
#include <hboost/spirit/home/classic/phoenix/actor.hpp>

////////////////////////////////////////////////////////////////////////////////

namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

    ////////////////////////////////////////////////////////////////////////////
    //
    // lazy_parser, holds phoenix actor which returns a spirit parser.
    //
    ////////////////////////////////////////////////////////////////////////////

    template<class ActorT>
    struct lazy_parser : parser<lazy_parser<ActorT> >
    {
        typedef lazy_parser<ActorT> self_t;
        typedef typename ::hboostphoenix::actor_result<
            ActorT, ::hboostphoenix::tuple<> >::plain_type actor_result_t;

        template<typename ScannerT>
        struct result
        {
            typedef typename
                parser_result<actor_result_t, ScannerT>::type
            type;
        };

        lazy_parser(ActorT const& actor_)
        : actor(actor_) {}

        template<typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        { return actor().parse(scan); }

        ActorT actor;
    };

    //////////////////////////////
    // lazy_p, returns lazy_parser
    // Usage: lazy_p(actor)
    template<class ActorT>
    lazy_parser<ActorT> lazy_p(ActorT const& actor)
    { return lazy_parser<ActorT>(actor); }

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}} // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif // HBOOST_SPIRIT_LAZY_HPP
