// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef HBOOST_PROPERTY_TREE_DETAIL_PTREE_UTILS_HPP_INCLUDED
#define HBOOST_PROPERTY_TREE_DETAIL_PTREE_UTILS_HPP_INCLUDED

#include <hboost/limits.hpp>
#include <hboost/type_traits/integral_constant.hpp>
#include <hboost/mpl/has_xxx.hpp>
#include <hboost/mpl/and.hpp>
#include <string>
#include <algorithm>
#include <locale>

namespace hboost { namespace property_tree { namespace detail
{

    template<class T>
    struct less_nocase
    {
        typedef typename T::value_type Ch;
        std::locale m_locale;
        inline bool operator()(Ch c1, Ch c2) const
        {
            return std::toupper(c1, m_locale) < std::toupper(c2, m_locale);
        }
        inline bool operator()(const T &t1, const T &t2) const
        {
            return std::lexicographical_compare(t1.begin(), t1.end(),
                                                t2.begin(), t2.end(), *this);
        }
    };

    template <typename Ch>
    struct is_character : public hboost::false_type {};
    template <>
    struct is_character<char> : public hboost::true_type {};
    template <>
    struct is_character<wchar_t> : public hboost::true_type {};


    HBOOST_MPL_HAS_XXX_TRAIT_DEF(internal_type)
    HBOOST_MPL_HAS_XXX_TRAIT_DEF(external_type)
    template <typename T>
    struct is_translator : public hboost::mpl::and_<
        has_internal_type<T>, has_external_type<T> > {};



    // Naively convert narrow string to another character type
    template<typename Str>
    Str widen(const char *text)
    {
        Str result;
        while (*text)
        {
            result += typename Str::value_type(*text);
            ++text;
        }
        return result;
    }

    // Naively convert string to narrow character type
    template<typename Str, typename char_type>
    Str narrow(const char_type *text)
    {
        Str result;
        while (*text)
        {
            if (*text < 0 || *text > (std::numeric_limits<char>::max)())
                result += '*';
            else
                result += typename Str::value_type(*text);
            ++text;
        }
        return result;
    }

    // Remove trailing and leading spaces
    template<class Str>
    Str trim(const Str &s, const std::locale &loc = std::locale())
    {
        typename Str::const_iterator first = s.begin();
        typename Str::const_iterator end = s.end();
        while (first != end && std::isspace(*first, loc))
            ++first;
        if (first == end)
            return Str();
        typename Str::const_iterator last = end;
        do --last; while (std::isspace(*last, loc));
        if (first != s.begin() || last + 1 != end)
            return Str(first, last + 1);
        else
            return s;
    }

} } }

#endif
