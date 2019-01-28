/*=============================================================================
    Copyright (c) 2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef HBOOST_SPIRIT_SELECT_HPP
#define HBOOST_SPIRIT_SELECT_HPP

#include <hboost/preprocessor/repeat.hpp>
#include <hboost/preprocessor/enum.hpp>
#include <hboost/preprocessor/enum_params.hpp>
#include <hboost/preprocessor/enum_params_with_defaults.hpp>
#include <hboost/preprocessor/inc.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp>

#include <hboost/spirit/home/classic/namespace.hpp>
#include <hboost/spirit/home/classic/core/parser.hpp>

#include <hboost/spirit/home/classic/phoenix/tuples.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit predefined maximum number of possible embedded select_p parsers.
//  It should NOT be greater than PHOENIX_LIMIT!
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_SPIRIT_SELECT_LIMIT)
#define HBOOST_SPIRIT_SELECT_LIMIT PHOENIX_LIMIT
#endif // !defined(HBOOST_SPIRIT_SELECT_LIMIT)

///////////////////////////////////////////////////////////////////////////////
//
// ensure   HBOOST_SPIRIT_SELECT_LIMIT <= PHOENIX_LIMIT and 
//          HBOOST_SPIRIT_SELECT_LIMIT > 0
//          HBOOST_SPIRIT_SELECT_LIMIT <= 15
//
//  [Pushed this down a little to make CW happy with HBOOST_STATIC_ASSERT]
//  [Otherwise, it complains: 'hboost_static_assert_test_42' redefined]
//
///////////////////////////////////////////////////////////////////////////////
HBOOST_STATIC_ASSERT(HBOOST_SPIRIT_SELECT_LIMIT <= PHOENIX_LIMIT);
HBOOST_STATIC_ASSERT(HBOOST_SPIRIT_SELECT_LIMIT > 0);
HBOOST_STATIC_ASSERT(HBOOST_SPIRIT_SELECT_LIMIT <= 15);

///////////////////////////////////////////////////////////////////////////////
//
//  Calculate the required amount of tuple members rounded up to the nearest 
//  integer dividable by 3
//
///////////////////////////////////////////////////////////////////////////////
#if HBOOST_SPIRIT_SELECT_LIMIT > 12
#define HBOOST_SPIRIT_SELECT_LIMIT_A     15
#elif HBOOST_SPIRIT_SELECT_LIMIT > 9
#define HBOOST_SPIRIT_SELECT_LIMIT_A     12
#elif HBOOST_SPIRIT_SELECT_LIMIT > 6
#define HBOOST_SPIRIT_SELECT_LIMIT_A     9
#elif HBOOST_SPIRIT_SELECT_LIMIT > 3
#define HBOOST_SPIRIT_SELECT_LIMIT_A     6
#else
#define HBOOST_SPIRIT_SELECT_LIMIT_A     3
#endif

///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
//
//  The select_default_no_fail and select_default_fail structs are used to 
//  distinguish two different behaviours for the select_parser in case that not
//  any of the given sub-parsers match.
//
//  If the select_parser is used with the select_default_no_fail behaviour,
//  then in case of no matching sub-parser the whole select_parser returns an
//  empty match and the value -1.
//
//  If the select_parser is used with the select_default_fail behaviour, then
//  in case of no matching sub-parser the whole select_parser fails to match at 
//  all.
//
///////////////////////////////////////////////////////////////////////////////
struct select_default_no_fail {};
struct select_default_fail {};

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}}  // namespace HBOOST_SPIRIT_CLASSIC_NS

///////////////////////////////////////////////////////////////////////////////
#include <hboost/spirit/home/classic/dynamic/impl/select.ipp>

///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
template <typename TupleT, typename BehaviourT, typename T>
struct select_parser
:   public parser<select_parser<TupleT, BehaviourT, T> >
{
    typedef select_parser<TupleT, BehaviourT, T> self_t;

    select_parser(TupleT const &t_)
    :   t(t_)
    {}
    
    template <typename ScannerT>
    struct result
    {
        typedef typename match_result<ScannerT, T>::type type;
    };

    template <typename ScannerT>
    typename parser_result<self_t, ScannerT>::type
    parse(ScannerT const& scan) const
    {
        typedef typename parser_result<self_t, ScannerT>::type result_t;
        
        if (!scan.at_end()) {
            return impl::parse_tuple_element<
                TupleT::length, result_t, TupleT, BehaviourT>::do_(t, scan);
        }
        return impl::select_match_gen<result_t, BehaviourT>::do_(scan);
    }
        
    TupleT const t;
};

///////////////////////////////////////////////////////////////////////////////
template <typename BehaviourT, typename T = int>
struct select_parser_gen {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  This generates different select_parser_gen::operator()() functions with 
    //  an increasing number of parser parameters:
    //
    //      template <typename ParserT0, ...>
    //      select_parser<
    //          ::hboostphoenix::tuple<
    //              typename impl::as_embedded_parser<ParserT0>::type,
    //              ...
    //          >,
    //          BehaviourT,
    //          T
    //      >
    //      operator()(ParserT0 const &p0, ...) const
    //      {
    //          typedef impl::as_embedded_parser<ParserT0> parser_t0;
    //          ...
    //
    //          typedef ::hboostphoenix::tuple< 
    //                  parser_t0::type,
    //                  ...
    //              > tuple_t; 
    //          typedef select_parser<tuple_t, BehaviourT, T> result_t;
    //
    //          return result_t(tuple_t(
    //                  parser_t0::convert(p0),
    //                  ...
    //              ));
    //      }
    //
    //  The number of generated functions depends on the maximum tuple member 
    //  limit defined by the PHOENIX_LIMIT pp constant. 
    //
    ///////////////////////////////////////////////////////////////////////////
    #define HBOOST_SPIRIT_SELECT_EMBEDDED(z, N, _)                           \
        typename impl::as_embedded_parser<HBOOST_PP_CAT(ParserT, N)>::type   \
        /**/
    #define HBOOST_SPIRIT_SELECT_EMBEDDED_TYPEDEF(z, N, _)                   \
        typedef impl::as_embedded_parser<HBOOST_PP_CAT(ParserT, N)>          \
            HBOOST_PP_CAT(parser_t, N);                                      \
        /**/
    #define HBOOST_SPIRIT_SELECT_CONVERT(z, N, _)                            \
        HBOOST_PP_CAT(parser_t, N)::convert(HBOOST_PP_CAT(p, N))              \
        /**/
        
    #define HBOOST_SPIRIT_SELECT_PARSER(z, N, _)                             \
        template <                                                          \
            HBOOST_PP_ENUM_PARAMS_Z(z, HBOOST_PP_INC(N), typename ParserT)    \
        >                                                                   \
        select_parser<                                                      \
            ::hboostphoenix::tuple<                                                 \
                HBOOST_PP_ENUM_ ## z(HBOOST_PP_INC(N),                        \
                    HBOOST_SPIRIT_SELECT_EMBEDDED, _)                        \
            >,                                                              \
            BehaviourT,                                                     \
            T                                                               \
        >                                                                   \
        operator()(                                                         \
            HBOOST_PP_ENUM_BINARY_PARAMS_Z(z, HBOOST_PP_INC(N),               \
                ParserT, const &p)                                          \
        ) const                                                             \
        {                                                                   \
            HBOOST_PP_REPEAT_ ## z(HBOOST_PP_INC(N),                          \
                HBOOST_SPIRIT_SELECT_EMBEDDED_TYPEDEF, _)                    \
                                                                            \
            typedef ::hboostphoenix::tuple<                                         \
                    HBOOST_PP_ENUM_BINARY_PARAMS_Z(z, HBOOST_PP_INC(N),       \
                        typename parser_t, ::type HBOOST_PP_INTERCEPT)       \
                > tuple_t;                                                  \
            typedef select_parser<tuple_t, BehaviourT, T> result_t;         \
                                                                            \
            return result_t(tuple_t(                                        \
                    HBOOST_PP_ENUM_ ## z(HBOOST_PP_INC(N),                    \
                        HBOOST_SPIRIT_SELECT_CONVERT, _)                     \
                ));                                                         \
        }                                                                   \
        /**/
        
    HBOOST_PP_REPEAT(HBOOST_SPIRIT_SELECT_LIMIT_A, 
        HBOOST_SPIRIT_SELECT_PARSER, _)
        
    #undef HBOOST_SPIRIT_SELECT_PARSER
    #undef HBOOST_SPIRIT_SELECT_CONVERT
    #undef HBOOST_SPIRIT_SELECT_EMBEDDED_TYPEDEF
    #undef HBOOST_SPIRIT_SELECT_EMBEDDED
    ///////////////////////////////////////////////////////////////////////////
};

///////////////////////////////////////////////////////////////////////////////
//
//  Predefined parser generator helper objects
//
///////////////////////////////////////////////////////////////////////////////
select_parser_gen<select_default_no_fail> const select_p = 
    select_parser_gen<select_default_no_fail>();

select_parser_gen<select_default_fail> const select_fail_p = 
    select_parser_gen<select_default_fail>();

#undef HBOOST_SPIRIT_SELECT_LIMIT_A

///////////////////////////////////////////////////////////////////////////////
HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}}  // namespace HBOOST_SPIRIT_CLASSIC_NS

#endif // HBOOST_SPIRIT_SELECT_HPP
