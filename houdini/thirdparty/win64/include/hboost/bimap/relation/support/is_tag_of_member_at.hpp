// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/support/is_tag_of_member_at.hpp
/// \brief is_tag_of_member_at<tag,relation> metafunction

#ifndef HBOOST_BIMAP_RELATION_SUPPORT_IS_TAG_OF_MEMBER_AT_HPP
#define HBOOST_BIMAP_RELATION_SUPPORT_IS_TAG_OF_MEMBER_AT_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/bimap/relation/member_at.hpp>
#include <hboost/type_traits/is_same.hpp>

#include <hboost/mpl/bool.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/bimap/relation/support/member_with_tag.hpp>

/** \struct hboost::bimaps::relation::support::is_tag_of_member_at_left

\brief Metafunction to test if a user tag is refering to the left member.

\code

template< class Tag, class Relation >
struct is_tag_of_member_at_left : {true_|false_} {};

\endcode

This metafunction is somewhat redundant with member_with_tag, but it is included
because it is a lot easier to metaprogram with it. The result type is the
same that:

\code

 is_same< member_with_tag<Tag,Relation>::type , member_at::left >::type

\endcode

See also member_with_tag, member_at, is_tag_of_member_at_right.
\ingroup relation_group
                                                                             **/


/** \struct hboost::bimaps::relation::support::is_tag_of_member_at_right

\brief Metafunction to test if a user tag is refering to the left member.

\code

template< class Tag, class Relation >
struct is_tag_of_member_at_right : {true_|false_} {};

\endcode

This metafunction is somewhat redundat with member_with_tag, but it is included
because it is a lot easier to metaprogram with it. The result type is the
same that:

\code

 is_same< member_with_tag<Tag,Relation>::type , member_at::right >::type

\endcode

See also member_with_tag, member_at, is_tag_of_member_at_left.
\ingroup relation_group
                                                                             **/


#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace hboost {
namespace bimaps {
namespace relation {
namespace support {

// Metafunction is_tag_of_member_at_left
// Easiear metaprogramming

template
<
    class Tag,
    class Relation,
    class Enable = void
>
struct is_tag_of_member_at_left :
    ::hboost::mpl::false_ {};

template< class Tag, class Relation >
struct is_tag_of_member_at_left
<
    Tag, Relation,
    HBOOST_DEDUCED_TYPENAME enable_if
    <
        is_same
        <
            HBOOST_DEDUCED_TYPENAME member_with_tag<Tag,Relation>::type,
            member_at::left
        >

    >::type
> :
    ::hboost::mpl::true_ {};

// Metafunction is_tag_of_member_at_right
// Easiear metaprogramming

template
<
    class Tag,
    class Relation,
    class Enable = void
>
struct is_tag_of_member_at_right :
    ::hboost::mpl::false_ {};

template< class Tag, class Relation >
struct is_tag_of_member_at_right
<
    Tag, Relation,
    HBOOST_DEDUCED_TYPENAME enable_if
    <
        is_same
        <
            HBOOST_DEDUCED_TYPENAME member_with_tag<Tag,Relation>::type,
            member_at::right
        >

    >::type
> :
    ::hboost::mpl::true_ {};


// Metafunction is_tag_of_member_at_info
// Easiear metaprogramming

template
<
    class Tag,
    class Relation,
    class Enable = void
>
struct is_tag_of_member_at_info :
    ::hboost::mpl::false_ {};

template< class Tag, class Relation >
struct is_tag_of_member_at_info
<
    Tag, Relation,
    HBOOST_DEDUCED_TYPENAME enable_if
    <
        is_same
        <
            HBOOST_DEDUCED_TYPENAME member_with_tag<Tag,Relation>::type,
            member_at::info
        >

    >::type
> :
    ::hboost::mpl::true_ {};

} // namespace support
} // namespace relation
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // HBOOST_BIMAP_RELATION_SUPPORT_IS_TAG_OF_MEMBER_AT_HPP


