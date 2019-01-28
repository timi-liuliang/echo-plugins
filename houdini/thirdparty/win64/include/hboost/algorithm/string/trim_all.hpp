//  Boost string_algo library trim.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef HBOOST_STRING_TRIM_ALL_HPP
#define HBOOST_STRING_TRIM_ALL_HPP

#include <hboost/algorithm/string/config.hpp>

#include <hboost/algorithm/string/trim.hpp>
#include <hboost/algorithm/string/classification.hpp>
#include <hboost/algorithm/string/find_format.hpp>
#include <hboost/algorithm/string/formatter.hpp>
#include <hboost/algorithm/string/finder.hpp>
#include <locale>

/*! \file
    Defines trim_all algorithms.
    
    Just like \c trim, \c trim_all removes all trailing and leading spaces from a 
    sequence (string). In addition, spaces in the middle of the sequence are truncated
    to just one character. Space is recognized using given locales.

    \c trim_fill acts as trim_all, but the spaces in the middle are replaces with 
    a user-define sequence of character.

    Parametric (\c _if) variants use a predicate (functor) to select which characters
    are to be trimmed.. 
    Functions take a selection predicate as a parameter, which is used to determine 
    whether a character is a space. Common predicates are provided in classification.hpp header.

*/

namespace hboost {
    namespace algorithm {

        // multi line trim  ----------------------------------------------- //

        //! Trim All - parametric
        /*!
            Remove all leading and trailing spaces from the input and
            compress all other spaces to a single character.
            The result is a trimmed copy of the input

            \param Input An input sequence
             \param IsSpace A unary predicate identifying spaces
            \return A trimmed copy of the input
        */
        template<typename SequenceT, typename PredicateT>
        inline SequenceT trim_all_copy_if(const SequenceT& Input, PredicateT IsSpace)
        {
            return 
                ::hboost::find_format_all_copy(      
                    ::hboost::trim_copy_if(Input, IsSpace),
                    ::hboost::token_finder(IsSpace, ::hboost::token_compress_on),
                    ::hboost::dissect_formatter(::hboost::head_finder(1)));
        }


        //! Trim All
        /*!
            Remove all leading and trailing spaces from the input and
            compress all other spaces to a single character.
            The input sequence is modified in-place.

            \param Input An input sequence
            \param IsSpace A unary predicate identifying spaces
        */
        template<typename SequenceT, typename PredicateT>
        inline void trim_all_if(SequenceT& Input, PredicateT IsSpace)
        {
            ::hboost::trim_if(Input, IsSpace);
            ::hboost::find_format_all(       
                Input,          
                ::hboost::token_finder(IsSpace, ::hboost::token_compress_on),
                ::hboost::dissect_formatter(::hboost::head_finder(1)));
        }


        //! Trim All
        /*!
            Remove all leading and trailing spaces from the input and
            compress all other spaces to a single character.
            The result is a trimmed copy of the input

            \param Input An input sequence
            \param Loc A locale used for 'space' classification
            \return A trimmed copy of the input
        */
        template<typename SequenceT>
        inline SequenceT trim_all_copy(const SequenceT& Input, const std::locale& Loc =std::locale())
        {
            return trim_all_copy_if(Input, ::hboost::is_space(Loc));
        }


        //! Trim All
        /*!
            Remove all leading and trailing spaces from the input and
            compress all other spaces to a single character.
            The input sequence is modified in-place.

            \param Input An input sequence
            \param Loc A locale used for 'space' classification
            \return A trimmed copy of the input
        */
        template<typename SequenceT>
        inline void trim_all(SequenceT& Input, const std::locale& Loc =std::locale())
        {
            trim_all_if(Input, ::hboost::is_space(Loc));
        }


        //! Trim Fill - parametric
        /*!
            Remove all leading and trailing spaces from the input and
            replace all every block of consecutive spaces with a fill string
            defined by user.
            The result is a trimmed copy of the input

            \param Input An input sequence
            \param Fill A string used to fill the inner spaces
            \param IsSpace A unary predicate identifying spaces
            \return A trimmed copy of the input
        */
        template<typename SequenceT, typename RangeT, typename PredicateT>
        inline SequenceT trim_fill_copy_if(const SequenceT& Input, const RangeT& Fill, PredicateT IsSpace)
        {
            return 
                ::hboost::find_format_all_copy(      
                    ::hboost::trim_copy_if(Input, IsSpace),
                    ::hboost::token_finder(IsSpace, ::hboost::token_compress_on),
                    ::hboost::const_formatter(::hboost::as_literal(Fill)));
        }


        //! Trim Fill
        /*!
            Remove all leading and trailing spaces from the input and
            replace all every block of consecutive spaces with a fill string
            defined by user.
            The input sequence is modified in-place.

            \param Input An input sequence
            \param Fill A string used to fill the inner spaces
            \param IsSpace A unary predicate identifying spaces
        */
        template<typename SequenceT, typename RangeT, typename PredicateT>
        inline void trim_fill_if(SequenceT& Input, const RangeT& Fill, PredicateT IsSpace)
        {
            ::hboost::trim_if(Input, IsSpace);
            ::hboost::find_format_all(       
                Input,          
                ::hboost::token_finder(IsSpace, ::hboost::token_compress_on),
                ::hboost::const_formatter(::hboost::as_literal(Fill)));
        }


        //! Trim Fill
        /*!
            Remove all leading and trailing spaces from the input and
            replace all every block of consecutive spaces with a fill string
            defined by user.
            The result is a trimmed copy of the input

            \param Input An input sequence
            \param Fill A string used to fill the inner spaces
            \param Loc A locale used for 'space' classification
            \return A trimmed copy of the input
        */
        template<typename SequenceT, typename RangeT>
        inline SequenceT trim_fill_copy(const SequenceT& Input, const RangeT& Fill, const std::locale& Loc =std::locale())
        {
            return trim_fill_copy_if(Input, Fill, ::hboost::is_space(Loc));
        }


        //! Trim Fill
        /*!
            Remove all leading and trailing spaces from the input and
            replace all every block of consecutive spaces with a fill string
            defined by user.
            The input sequence is modified in-place.

            \param Input An input sequence
            \param Fill A string used to fill the inner spaces
            \param Loc A locale used for 'space' classification
            \return A trimmed copy of the input
        */
        template<typename SequenceT, typename RangeT>
        inline void trim_fill(SequenceT& Input, const RangeT& Fill, const std::locale& Loc =std::locale())
        {
            trim_fill_if(Input, Fill, ::hboost::is_space(Loc));
        }


    } // namespace algorithm    

    // pull names to the boost namespace
    using algorithm::trim_all;
    using algorithm::trim_all_if;
    using algorithm::trim_all_copy;
    using algorithm::trim_all_copy_if;
    using algorithm::trim_fill;
    using algorithm::trim_fill_if;
    using algorithm::trim_fill_copy;
    using algorithm::trim_fill_copy_if;

} // namespace hboost

#endif  // HBOOST_STRING_TRIM_ALL_HPP
