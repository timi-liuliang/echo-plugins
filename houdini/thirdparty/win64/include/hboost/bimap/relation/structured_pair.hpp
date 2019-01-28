// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/structured_pair.hpp
/// \brief Defines the structured_pair class.

#ifndef HBOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP
#define HBOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <utility>

#include <hboost/type_traits/remove_const.hpp>

#include <hboost/mpl/aux_/na.hpp>

#include <hboost/call_traits.hpp>

#include <hboost/utility/enable_if.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/vector.hpp>

#include <hboost/bimap/detail/debug/static_error.hpp>
#include <hboost/bimap/relation/pair_layout.hpp>
#include <hboost/bimap/relation/symmetrical_base.hpp>
#include <hboost/bimap/relation/support/get.hpp>
#include <hboost/bimap/tags/support/value_type_of.hpp>



namespace hboost {
namespace bimaps {
namespace relation {

namespace detail {

/// \brief Storage definition of the left view of a mutant relation.
/**

See also storage_finder, mirror_storage.
                                                                            **/

template< class FirstType, class SecondType >
class normal_storage :
    public symmetrical_base<FirstType,SecondType>
{
    typedef symmetrical_base<FirstType,SecondType> base_;

    public:

    typedef normal_storage storage_;

    typedef HBOOST_DEDUCED_TYPENAME base_::left_value_type  first_type;
    typedef HBOOST_DEDUCED_TYPENAME base_::right_value_type second_type;

    first_type   first;
    second_type  second;

    normal_storage() {}

    normal_storage(HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        first_type >::param_type f,
                   HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        second_type>::param_type s)

        : first(f), second(s) {}

          HBOOST_DEDUCED_TYPENAME base_:: left_value_type &  get_left()      { return first;  }
    const HBOOST_DEDUCED_TYPENAME base_:: left_value_type &  get_left()const { return first;  }
          HBOOST_DEDUCED_TYPENAME base_::right_value_type & get_right()      { return second; }
    const HBOOST_DEDUCED_TYPENAME base_::right_value_type & get_right()const { return second; }
};

/// \brief Storage definition of the right view of a mutant relation.
/**

See also storage_finder, normal_storage.
                                                                            **/

template< class FirstType, class SecondType >
class mirror_storage :
    public symmetrical_base<SecondType,FirstType>
{
    typedef symmetrical_base<SecondType,FirstType> base_;

    public:

    typedef mirror_storage storage_;

    typedef HBOOST_DEDUCED_TYPENAME base_::left_value_type   second_type;
    typedef HBOOST_DEDUCED_TYPENAME base_::right_value_type  first_type;

    second_type  second;
    first_type   first;

    mirror_storage() {}

    mirror_storage(HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<first_type  >::param_type f,
                   HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<second_type >::param_type s)

        : second(s), first(f)  {}

          HBOOST_DEDUCED_TYPENAME base_:: left_value_type &  get_left()      { return second; }
    const HBOOST_DEDUCED_TYPENAME base_:: left_value_type &  get_left()const { return second; }
          HBOOST_DEDUCED_TYPENAME base_::right_value_type & get_right()      { return first;  }
    const HBOOST_DEDUCED_TYPENAME base_::right_value_type & get_right()const { return first;  }
};

/** \struct hboost::bimaps::relation::storage_finder
\brief Obtain the a storage with the correct layout.

\code
template< class FirstType, class SecondType, class Layout >
struct storage_finder
{
    typedef {normal/mirror}_storage<FirstType,SecondType> type;
};
\endcode

See also normal_storage, mirror_storage.
                                                                        **/

#ifndef HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template
<
    class FirstType,
    class SecondType,
    class Layout
>
struct storage_finder
{
    typedef normal_storage<FirstType,SecondType> type;
};

template
<
    class FirstType,
    class SecondType
>
struct storage_finder<FirstType,SecondType,mirror_layout>
{
    typedef mirror_storage<FirstType,SecondType> type;
};

#endif // HBOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES


template< class TA, class TB, class Info, class Layout >
class pair_info_hook :
    public ::hboost::bimaps::relation::detail::storage_finder<TA,TB,Layout>::type
{
    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::detail::storage_finder<TA,TB,Layout>::type base_;

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::tags::support::
        default_tagged<Info,member_at::info>::type tagged_info_type;

    public:
    typedef HBOOST_DEDUCED_TYPENAME tagged_info_type::value_type info_type;
    typedef HBOOST_DEDUCED_TYPENAME tagged_info_type::tag        info_tag;

    info_type info;

    protected:

    pair_info_hook() {}

    pair_info_hook( HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::first_type
                    >::param_type f,
                    HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::second_type
                    >::param_type s,
                    HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        info_type
                    >::param_type i = info_type() )
        : base_(f,s), info(i) {}

    template< class Pair >
    pair_info_hook( const Pair & p) :
        base_(p.first,p.second),
        info(p.info) {}

    template< class Pair >
    void change_to( const Pair & p )
    {
        base_::first  = p.first ;
        base_::second = p.second;
        info          = p.info  ;
    }

    void clear_info()
    {
        info = info_type();
    };
};

template< class TA, class TB, class Layout>
class pair_info_hook<TA,TB,::hboost::mpl::na,Layout> :
    public ::hboost::bimaps::relation::detail::storage_finder<TA,TB,Layout>::type
{
    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::detail::storage_finder<TA,TB,Layout>::type base_;

    public:
    typedef ::hboost::mpl::na info_type;
    typedef member_at::info info_tag;

    protected:

    pair_info_hook() {}

    pair_info_hook( HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::first_type
                    >::param_type f,
                    HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::second_type
                    >::param_type s)

        : base_(f,s) {}

    template< class Pair >
    pair_info_hook( const Pair & p ) :
        base_(p.first,p.second) {}

    template< class Pair >
    void change_to( const Pair & p )
    {
        base_::first  = p.first ;
        base_::second = p.second;
    }

    void clear_info() {};
};



} // namespace detail

template< class TA, class TB, class Info, bool FM >
class mutant_relation;


/// \brief A std::pair signature compatible class that allows you to control
///        the internal structure of the data.
/**
This class allows you to specify the order in wich the two data types will be
in the layout of the class.
                                                                               **/

template< class FirstType, class SecondType, class Info, class Layout = normal_layout >
class structured_pair :

    public ::hboost::bimaps::relation::detail::pair_info_hook
    <
        FirstType, SecondType,
        Info,
        Layout

    >

{
    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::detail::pair_info_hook
    <
        FirstType, SecondType,
        Info,
        Layout

    > base_;

    public:

    typedef ::hboost::mpl::vector3<
        structured_pair< FirstType, SecondType, Info, normal_layout >,
        structured_pair< FirstType, SecondType, Info, mirror_layout >,
        HBOOST_DEDUCED_TYPENAME ::hboost::mpl::if_<
            HBOOST_DEDUCED_TYPENAME ::hboost::is_same<Layout, normal_layout>::type,
            mutant_relation< FirstType, SecondType, Info, true >,
            mutant_relation< SecondType, FirstType, Info, true >
        >::type

    > mutant_views;

    structured_pair() {}

    structured_pair(HBOOST_DEDUCED_TYPENAME hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::first_type  >::param_type f,
                    HBOOST_DEDUCED_TYPENAME hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::second_type >::param_type s)
        : base_(f,s) {}

    structured_pair(HBOOST_DEDUCED_TYPENAME hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::first_type  >::param_type f,
                    HBOOST_DEDUCED_TYPENAME hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::second_type >::param_type s,
                    HBOOST_DEDUCED_TYPENAME hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::info_type   >::param_type i)
        : base_(f,s,i) {}

    template< class OtherLayout >
    structured_pair(
        const structured_pair<FirstType,SecondType,Info,OtherLayout> & p)
        : base_(p) {}

    template< class OtherLayout >
    structured_pair& operator=(
        const structured_pair<FirstType,SecondType,OtherLayout> & p)
    {
        base_::change_to(p);
        return *this;
    }

    template< class First, class Second >
    structured_pair(const std::pair<First,Second> & p) :
        base_(p.first,p.second)
    {}

    template< class First, class Second >
    structured_pair& operator=(const std::pair<First,Second> & p)
    {
        base_::first  = p.first;
        base_::second = p.second;
        base_::clear_info();
        return *this;
    }

    template< class Tag >
    const HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
        result_of::get<Tag,const structured_pair>::type
    get() const
    {
        return ::hboost::bimaps::relation::support::get<Tag>(*this);
    }

    template< class Tag >
    HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
        result_of::get<Tag,structured_pair>::type
    get()
    {
        return ::hboost::bimaps::relation::support::get<Tag>(*this);
    }
};

// structured_pair - structured_pair

template< class FirstType, class SecondType, class Info, class Layout1, class Layout2 >
bool operator==(const structured_pair<FirstType,SecondType,Info,Layout1> & a,
                const structured_pair<FirstType,SecondType,Info,Layout2> & b)
{
    return ( ( a.first  == b.first  ) &&
             ( a.second == b.second ) );
}

template< class FirstType, class SecondType, class Info, class Layout1, class Layout2 >
bool operator!=(const structured_pair<FirstType,SecondType,Info,Layout1> & a,
                const structured_pair<FirstType,SecondType,Info,Layout2> & b)
{
    return ! ( a == b );
}

template< class FirstType, class SecondType, class Info, class Layout1, class Layout2 >
bool operator<(const structured_pair<FirstType,SecondType,Info,Layout1> & a,
               const structured_pair<FirstType,SecondType,Info,Layout2> & b)
{
    return (  ( a.first  <  b.first  ) ||
             (( a.first == b.first ) && ( a.second < b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout1, class Layout2 >
bool operator<=(const structured_pair<FirstType,SecondType,Info,Layout1> & a,
                const structured_pair<FirstType,SecondType,Info,Layout2> & b)
{
    return (  ( a.first  <  b.first  ) ||
             (( a.first == b.first ) && ( a.second <= b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout1, class Layout2 >
bool operator>(const structured_pair<FirstType,SecondType,Info,Layout1> & a,
               const structured_pair<FirstType,SecondType,Info,Layout2> & b)
{
    return ( ( a.first  >  b.first  ) ||
             (( a.first == b.first ) && ( a.second > b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout1, class Layout2 >
bool operator>=(const structured_pair<FirstType,SecondType,Info,Layout1> & a,
                const structured_pair<FirstType,SecondType,Info,Layout2> & b)
{
    return ( ( a.first  >  b.first  ) ||
             (( a.first == b.first ) && ( a.second >= b.second )));
}

// structured_pair - std::pair

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator==(const structured_pair<FirstType,SecondType,Info,Layout> & a,
                const std::pair<F,S> & b)
{
    return ( ( a.first  == b.first  ) &&
             ( a.second == b.second ) );
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator!=(const structured_pair<FirstType,SecondType,Info,Layout> & a,
                const std::pair<F,S> & b)
{
    return ! ( a == b );
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator<(const structured_pair<FirstType,SecondType,Info,Layout> & a,
               const std::pair<F,S> & b)
{
    return (  ( a.first  <  b.first  ) ||
             (( a.first == b.first ) && ( a.second < b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator<=(const structured_pair<FirstType,SecondType,Info,Layout> & a,
                const std::pair<F,S> & b)
{
    return (  ( a.first  <  b.first  ) ||
             (( a.first == b.first ) && ( a.second <= b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator>(const structured_pair<FirstType,SecondType,Info,Layout> & a,
               const std::pair<F,S> & b)
{
    return ( ( a.first  >  b.first  ) ||
             (( a.first == b.first ) && ( a.second > b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator>=(const structured_pair<FirstType,SecondType,Info,Layout> & a,
                const std::pair<F,S> & b)
{
    return ( ( a.first  >  b.first  ) ||
             (( a.first == b.first ) && ( a.second >= b.second )));
}

// std::pair - sturctured_pair

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator==(const std::pair<F,S> & a,
                const structured_pair<FirstType,SecondType,Info,Layout> & b)
{
    return ( ( a.first  == b.first  ) &&
             ( a.second == b.second ) );
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator!=(const std::pair<F,S> & a,
                const structured_pair<FirstType,SecondType,Info,Layout> & b)
{
    return ! ( a == b );
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator<(const std::pair<F,S> & a,
               const structured_pair<FirstType,SecondType,Info,Layout> & b)
{
    return (  ( a.first  <  b.first  ) ||
             (( a.first == b.first ) && ( a.second < b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator<=(const std::pair<F,S> & a,
                const structured_pair<FirstType,SecondType,Info,Layout> & b)
{
    return (  ( a.first  <  b.first  ) ||
             (( a.first == b.first ) && ( a.second <= b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator>(const std::pair<F,S> & a,
               const structured_pair<FirstType,SecondType,Info,Layout> & b)
{
    return ( ( a.first  >  b.first  ) ||
             (( a.first == b.first ) && ( a.second > b.second )));
}

template< class FirstType, class SecondType, class Info, class Layout, class F, class S >
bool operator>=(const std::pair<F,S> & a,
                const structured_pair<FirstType,SecondType,Info,Layout> & b)
{
    return ( ( a.first  >  b.first  ) ||
             (( a.first == b.first ) && ( a.second >= b.second )));
}


namespace detail {

template< class FirstType, class SecondType, class Info, class Layout>
structured_pair<FirstType,SecondType,Info,Layout> 
    copy_with_first_replaced(structured_pair<FirstType,SecondType,Info,Layout> const& p,
        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits< HBOOST_DEDUCED_TYPENAME 
            structured_pair<FirstType,SecondType,Info,Layout>::first_type>
                ::param_type f)
{
    return structured_pair<FirstType,SecondType,Info,Layout>(f,p.second,p.info);
}
    
template< class FirstType, class SecondType, class Layout>
structured_pair<FirstType,SecondType,::hboost::mpl::na,Layout> 
    copy_with_first_replaced(structured_pair<FirstType,SecondType,::hboost::mpl::na,Layout> const& p,
        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits< HBOOST_DEDUCED_TYPENAME 
            structured_pair<FirstType,SecondType,::hboost::mpl::na,Layout>::first_type>
                ::param_type f)
{
    return structured_pair<FirstType,SecondType,::hboost::mpl::na,Layout>(f,p.second);
}
    
template< class FirstType, class SecondType, class Info, class Layout>
structured_pair<FirstType,SecondType,Info,Layout> 
    copy_with_second_replaced(structured_pair<FirstType,SecondType,Info,Layout> const& p,
        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits< HBOOST_DEDUCED_TYPENAME 
            structured_pair<FirstType,SecondType,Info,Layout>::second_type>
                ::param_type s)
{
    return structured_pair<FirstType,SecondType,Info,Layout>(p.first,s,p.info);
}
    
template< class FirstType, class SecondType, class Layout>
structured_pair<FirstType,SecondType,::hboost::mpl::na,Layout> 
    copy_with_second_replaced(structured_pair<FirstType,SecondType,::hboost::mpl::na,Layout> const& p,
        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits< HBOOST_DEDUCED_TYPENAME 
            structured_pair<FirstType,SecondType,::hboost::mpl::na,Layout>::second_type>
                ::param_type s)
{
    return structured_pair<FirstType,SecondType,::hboost::mpl::na,Layout>(p.first,s);
}

} // namespace detail


} // namespace relation
} // namespace bimaps
} // namespace hboost

#endif // HBOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP

