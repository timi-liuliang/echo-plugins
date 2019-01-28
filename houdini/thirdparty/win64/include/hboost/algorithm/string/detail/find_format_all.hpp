//  Boost string_algo library find_format_all.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef HBOOST_STRING_FIND_FORMAT_ALL_DETAIL_HPP
#define HBOOST_STRING_FIND_FORMAT_ALL_DETAIL_HPP

#include <hboost/algorithm/string/config.hpp>
#include <hboost/range/iterator_range_core.hpp>
#include <hboost/range/const_iterator.hpp>
#include <hboost/range/value_type.hpp>
#include <hboost/algorithm/string/detail/find_format_store.hpp>
#include <hboost/algorithm/string/detail/replace_storage.hpp>

namespace hboost {
    namespace algorithm {
        namespace detail {

// find_format_all_copy (iterator variant) implementation ---------------------------//

           template< 
                typename OutputIteratorT,
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline OutputIteratorT find_format_all_copy_impl2(
                OutputIteratorT Output,
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult )
            {       
                typedef HBOOST_STRING_TYPENAME 
                    range_const_iterator<InputT>::type input_iterator_type; 

                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

                // Initialize last match
                input_iterator_type LastMatch=::hboost::begin(Input);

                // Iterate through all matches
                while( M )
                {
                    // Copy the beginning of the sequence
                    Output = std::copy( LastMatch, M.begin(), Output );
                    // Copy formatted result
                    Output = std::copy( ::hboost::begin(M.format_result()), ::hboost::end(M.format_result()), Output );

                    // Proceed to the next match
                    LastMatch=M.end();
                    M=Finder( LastMatch, ::hboost::end(Input) );
                }

                // Copy the rest of the sequence
                Output = std::copy( LastMatch, ::hboost::end(Input), Output );

                return Output;
            }

            template< 
                typename OutputIteratorT,
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline OutputIteratorT find_format_all_copy_impl(
                OutputIteratorT Output,
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult )
            {   
                if( ::hboost::algorithm::detail::check_find_result(Input, FindResult) ) {
                    return ::hboost::algorithm::detail::find_format_all_copy_impl2( 
                        Output,
                        Input,
                        Finder,
                        Formatter,
                        FindResult,
                        Formatter(FindResult) );
                } else {
                    return std::copy( ::hboost::begin(Input), ::hboost::end(Input), Output );
                }
            }

 // find_format_all_copy implementation ----------------------------------------------//

           template< 
                typename InputT, 
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline InputT find_format_all_copy_impl2(
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult)
            {
                typedef HBOOST_STRING_TYPENAME 
                    range_const_iterator<InputT>::type input_iterator_type; 

                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

                // Initialize last match
                input_iterator_type LastMatch=::hboost::begin(Input);

                // Output temporary
                InputT Output;

                // Iterate through all matches
                while( M )
                {
                    // Copy the beginning of the sequence
                    hboost::algorithm::detail::insert( Output, ::hboost::end(Output), LastMatch, M.begin() );
                    // Copy formatted result
                    hboost::algorithm::detail::insert( Output, ::hboost::end(Output), M.format_result() );

                    // Proceed to the next match
                    LastMatch=M.end();
                    M=Finder( LastMatch, ::hboost::end(Input) );
                }

                // Copy the rest of the sequence
                ::hboost::algorithm::detail::insert( Output, ::hboost::end(Output), LastMatch, ::hboost::end(Input) );

                return Output;
            }

            template< 
                typename InputT, 
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline InputT find_format_all_copy_impl(
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult)
            {
                if( ::hboost::algorithm::detail::check_find_result(Input, FindResult) ) {
                    return ::hboost::algorithm::detail::find_format_all_copy_impl2(
                        Input,
                        Finder,
                        Formatter,
                        FindResult,
                        Formatter(FindResult) );
                } else {
                    return Input;
                }
            }

 // find_format_all implementation ------------------------------------------------//
        
            template<
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline void find_format_all_impl2( 
                InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                FindResultT FindResult,
                FormatResultT FormatResult)
            {
                typedef HBOOST_STRING_TYPENAME 
                    range_iterator<InputT>::type input_iterator_type; 
                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );
          
                // Instantiate replacement storage
                std::deque<
                    HBOOST_STRING_TYPENAME range_value<InputT>::type> Storage;

                // Initialize replacement iterators
                input_iterator_type InsertIt=::hboost::begin(Input);
                input_iterator_type SearchIt=::hboost::begin(Input);
                
                while( M )
                {
                    // process the segment
                    InsertIt=process_segment( 
                        Storage,
                        Input,
                        InsertIt,
                        SearchIt,
                        M.begin() );
                    
                    // Adjust search iterator
                    SearchIt=M.end();

                    // Copy formatted replace to the storage
                    ::hboost::algorithm::detail::copy_to_storage( Storage, M.format_result() );

                    // Find range for a next match
                    M=Finder( SearchIt, ::hboost::end(Input) );
                }

                // process the last segment
                InsertIt=::hboost::algorithm::detail::process_segment( 
                    Storage,
                    Input,
                    InsertIt,
                    SearchIt,
                    ::hboost::end(Input) );
                
                if ( Storage.empty() )
                {
                    // Truncate input
                    ::hboost::algorithm::detail::erase( Input, InsertIt, ::hboost::end(Input) );
                }
                else
                {
                    // Copy remaining data to the end of input
                    ::hboost::algorithm::detail::insert( Input, ::hboost::end(Input), Storage.begin(), Storage.end() );
                }
            }

            template<
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline void find_format_all_impl( 
                InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                FindResultT FindResult)
            {
                if( ::hboost::algorithm::detail::check_find_result(Input, FindResult) ) {
                    ::hboost::algorithm::detail::find_format_all_impl2(
                        Input,
                        Finder,
                        Formatter,
                        FindResult,
                        Formatter(FindResult) );
                }
            }

        } // namespace detail
    } // namespace algorithm
} // namespace hboost

#endif  // HBOOST_STRING_FIND_FORMAT_ALL_DETAIL_HPP
