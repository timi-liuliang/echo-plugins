// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/manage_additional_parameters.hpp
/// \brief Utility class to extract the additional parameters from the template parameters.

#ifndef HBOOST_BIMAP_DETAIL_MANAGE_ADDITIONAL_PARAMETERS_HPP
#define HBOOST_BIMAP_DETAIL_MANAGE_ADDITIONAL_PARAMETERS_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <memory>

// Boost.MPL
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/aux_/na.hpp>
#include <hboost/type_traits/is_same.hpp>

#include <hboost/bimap/detail/is_set_type_of.hpp>

namespace hboost {
namespace bimaps {

template< class Type >
struct with_info
{
    typedef Type value_type;
};

namespace detail {

/// \brief Metafunction to check if a given type is a data_hook specification.

template< class Type >
struct is_with_info : ::hboost::mpl::false_ {};

template< class ValueType >
struct is_with_info< with_info<ValueType> > : ::hboost::mpl::true_ {};

/** \struct hboost::bimaps::detail::manage_additional_parameters
\brief Utility class to extract the additional parameters from the template parameters.

\code
template< class AP1, class AP2, class AP3 >
struct manage_additional_parameters
{
    struct parameters
    {
        typedef -unspecified- set_type_of_relation;
        typedef -unspecified- data_hook;
        typedef -unspecified- allocator;
    };

    typedef parameters type;
};
\endcode

See also bimap, bimap_core.
                                                                                **/

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class AP1, class AP2, class AP3 >
struct manage_additional_parameters
{
    // (1) manage_additional_parameters<
    //         not_specified,not_specified,not_specified>
    //
    //     set_type_of_relation: based on the left key type
    //     info_hook:            no additional info
    //     allocator:            default allocator

    struct case_NNN
    {
        typedef left_based set_type_of_relation;
        typedef std::allocator<void> allocator;
        typedef ::hboost::mpl::na additional_info;
    };

    // (2) manage_additional_parameters<Allocator,not_specified,not_specified>
    //
    //     set_type_of_relation: based on the left key type
    //     info_hook:            no additional info
    //     allocator:            Allocator

    struct case_ANN
    {
        typedef left_based set_type_of_relation;
        typedef AP1 allocator;
        typedef ::hboost::mpl::na additional_info;
    };

    // (3) manage_additional_parameters<
    //         SetOfRelationType,not_specified,not_specified>
    //
    //     set_type_of_relation: SetTypeOfRelation
    //     info_hook:            no additional info
    //     allocator:            default allocator

    struct case_SNN
    {
        typedef AP1 set_type_of_relation;
        typedef std::allocator<void> allocator;
        typedef ::hboost::mpl::na additional_info;
    };

    // (4) manage_additional_parameters<
    //          SetTypeOfRelation,Allocator,not_specified>
    //
    //     set_type_of_relation: SetTypeOfRelation
    //     info_hook:            no additional info
    //     allocator:            Allocator

    struct case_SAN
    {
        typedef AP1 set_type_of_relation;
        typedef AP2 allocator;
        typedef ::hboost::mpl::na additional_info;
    };

    // (5) manage_additional_parameters<InfoToHook,not_specified,not_specified>
    //
    //     set_type_of_relation: based on the left key type
    //     info_hook:            InfoToHook
    //     allocator:            default allocator

    struct case_HNN
    {
        typedef left_based set_type_of_relation;
        typedef std::allocator<void> allocator;
        typedef HBOOST_DEDUCED_TYPENAME AP1::value_type additional_info;
    };

    // (6) manage_additional_parameters<
    //         SetTypeOfRelation,InfoToHook,not_specified>
    //
    //     set_type_of_relation: SetTypeOfRelation
    //     info_hook:            InfoToHook
    //     allocator:            default allocator

    struct case_SHN
    {
        typedef AP1 set_type_of_relation;
        typedef std::allocator<void> allocator;
        typedef HBOOST_DEDUCED_TYPENAME AP2::value_type additional_info;
    };

    // (7) manage_additional_parameters<
    //         DataToHook,Allocator,not_specified>
    //
    //     set_type_of_relation: SetTypeOfRelation
    //     info_hook:            InfoToHook
    //     allocator:            default allocator

    struct case_HAN
    {
        typedef left_based set_type_of_relation;
        typedef AP2 allocator;
        typedef HBOOST_DEDUCED_TYPENAME AP1::value_type additional_info;
    };

    // (8) manage_additional_parameters<
    //         SetTypeOfRelation,DataToHook,Allocator>
    //
    //     set_type_of_relation: SetTypeOfRelation
    //     info_hook:            InfoToHook
    //     allocator:            Allocator

    struct case_SHA
    {
        typedef AP1 set_type_of_relation;
        typedef AP2 allocator;
        typedef HBOOST_DEDUCED_TYPENAME AP2::value_type additional_info;
    };

    // Some annidated mpl::if_ and we are done!

    typedef HBOOST_DEDUCED_TYPENAME mpl::if_
    <
        ::hboost::mpl::is_na<AP1>,
        case_NNN, // (1)
        HBOOST_DEDUCED_TYPENAME mpl::if_
        <
            ::hboost::mpl::is_na<AP2>,
            HBOOST_DEDUCED_TYPENAME mpl::if_
            <
                is_set_type_of_relation<AP1>,
                case_SNN, // (3)
                HBOOST_DEDUCED_TYPENAME mpl::if_
                <
                    is_with_info<AP1>,
                    case_HNN, // (5)
                    case_ANN  // (2)

                >::type

            >::type,
            HBOOST_DEDUCED_TYPENAME mpl::if_
            <
                ::hboost::mpl::is_na<AP3>,
                HBOOST_DEDUCED_TYPENAME mpl::if_
                <
                    is_with_info<AP1>,
                    case_HAN, // (7)
                    HBOOST_DEDUCED_TYPENAME mpl::if_
                    <
                        is_with_info<AP2>,
                        case_SHN, // (6)
                        case_SAN  // (4)

                    >::type

                >::type,

                case_SHA // (8)

            >::type

        >::type

    >::type type;

};

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

} // namespace detail
} // namespace bimaps
} // namespace hboost


#endif // HBOOST_BIMAP_DETAIL_MANAGE_ADDITIONAL_PARAMETERS_HPP

