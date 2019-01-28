/*=============================================================================
    Copyright (c) 2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef HBOOST_SPIRIT_SELECT_IPP
#define HBOOST_SPIRIT_SELECT_IPP

#include <hboost/spirit/home/classic/core/parser.hpp>
#include <hboost/spirit/home/classic/core/composite/composite.hpp>
#include <hboost/spirit/home/classic/meta/as_parser.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace spirit {

HBOOST_SPIRIT_CLASSIC_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
namespace impl {

///////////////////////////////////////////////////////////////////////////////
template <typename ParserT>
struct as_embedded_parser : public as_parser<ParserT>
{
    typedef typename as_parser<ParserT>::type::derived_t::embed_t type;
};

///////////////////////////////////////////////////////////////////////////////

// no implementation here to catch unknown BehaviourT template arguments
template <typename ResultT, typename BehaviourT>
struct select_match_gen;

// implementation for the select_default_no_fail behaviour
template <typename ResultT>
struct select_match_gen<ResultT, select_default_no_fail> {

    template <typename ScannerT>
    static ResultT
    do_ (ScannerT const &scan)
    {
        return scan.create_match(0, -1, scan.first, scan.first);
    }
};

// implementation for the select_default_fail behaviour
template <typename ResultT>
struct select_match_gen<ResultT, select_default_fail> {

    template <typename ScannerT>
    static ResultT
    do_ (ScannerT const &scan)
    {
        return scan.no_match();
    }
};

///////////////////////////////////////////////////////////////////////////////
template <int N, typename ResultT, typename TupleT, typename BehaviourT>
struct parse_tuple_element {

    HBOOST_STATIC_CONSTANT(int, index = (TupleT::length - N));
    
    template <typename ScannerT>
    static ResultT
    do_(TupleT const &t, ScannerT const &scan)
    {
        typedef typename ::hboostphoenix::tuple_element<index, TupleT>::type parser_t;
        typedef typename ScannerT::iterator_t                       iterator_t;
        typedef typename parser_result<parser_t, ScannerT>::type    result_t;
    
    iterator_t save(scan.first);
    result_t result(t[::hboostphoenix::tuple_index<index>()].parse(scan));

        if (result) {
            return scan.create_match(result.length(), TupleT::length - N, 
                save, scan.first);
        }
        scan.first = save;    // reset the input stream 
        return parse_tuple_element<N-1, ResultT, TupleT, BehaviourT>::
            do_(t, scan);
    }
};

template <typename ResultT, typename TupleT, typename BehaviourT>
struct parse_tuple_element<1, ResultT, TupleT, BehaviourT> {

    HBOOST_STATIC_CONSTANT(int, index = (TupleT::length - 1));
    
    template <typename ScannerT>
    static ResultT
    do_(TupleT const &t, ScannerT const &scan)
    {
        typedef typename ::hboostphoenix::tuple_element<index, TupleT>::type  parser_t;
        typedef typename ScannerT::iterator_t                       iterator_t;
        typedef typename parser_result<parser_t, ScannerT>::type    result_t;
        
    iterator_t save(scan.first);
    result_t result(t[::hboostphoenix::tuple_index<index>()].parse(scan));

        if (result) {
            return scan.create_match(result.length(), TupleT::length - 1, 
                save, scan.first);
        }
        scan.first = save;    // reset the input stream 
        return select_match_gen<ResultT, BehaviourT>::do_(scan);
    }
};

///////////////////////////////////////////////////////////////////////////////
}   // namespace impl

HBOOST_SPIRIT_CLASSIC_NAMESPACE_END

}}  // namespace hboost::spirit

#endif  // HBOOST_SPIRIT_SELECT_IPP
