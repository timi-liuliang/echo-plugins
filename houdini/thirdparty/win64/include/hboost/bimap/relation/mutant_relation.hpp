// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/mutant_relation.hpp
/// \brief Defines the mutant_relation class

#ifndef HBOOST_BIMAP_RELATION_MUTANT_RELATION_HPP
#define HBOOST_BIMAP_RELATION_MUTANT_RELATION_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/mpl/vector.hpp>
#include <hboost/operators.hpp>
#include <hboost/call_traits.hpp>

#ifndef HBOOST_BIMAP_DISABLE_SERIALIZATION
  #include <hboost/serialization/nvp.hpp>
#endif // HBOOST_BIMAP_DISABLE_SERIALIZATION

#include <hboost/functional/hash/hash.hpp>

#include <hboost/mpl/aux_/na.hpp>

// Boost.Bimap
#include <hboost/bimap/tags/tagged.hpp>
#include <hboost/bimap/tags/support/default_tagged.hpp>
#include <hboost/bimap/tags/support/tag_of.hpp>
#include <hboost/bimap/tags/support/value_type_of.hpp>

#include <hboost/bimap/relation/member_at.hpp>
#include <hboost/bimap/relation/detail/mutant.hpp>
#include <hboost/bimap/relation/structured_pair.hpp>
#include <hboost/bimap/relation/symmetrical_base.hpp>
#include <hboost/bimap/relation/support/get.hpp>

namespace hboost {
namespace bimaps {
namespace relation {

namespace detail {

// This class is included so structured_pair and mutant_relation share
// exactly the same class layout

template< class LeftType, class RightType, bool force_mutable >
class relation_storage :
    public symmetrical_base<LeftType,RightType,force_mutable>
{
    typedef symmetrical_base<LeftType,RightType,force_mutable> base_;

    typedef relation_storage storage_;

    public:

    typedef relation_storage<LeftType,RightType,false> non_mutable_storage;

    typedef ::hboost::mpl::vector2
    <
        relation_storage< LeftType, RightType, true  >,
        relation_storage< LeftType, RightType, false >

    > mutant_views;

    //@{
        /// data
        HBOOST_DEDUCED_TYPENAME base_::left_value_type  left;
        HBOOST_DEDUCED_TYPENAME base_::right_value_type right;
    //@}

    relation_storage() {}

    relation_storage(HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                         HBOOST_DEDUCED_TYPENAME base_::left_value_type
                     >::param_type l,
                     HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::right_value_type
                     >::param_type r)

        : left(l), right(r) {}

          HBOOST_DEDUCED_TYPENAME base_:: left_value_type &  get_left()      { return left;  }
    const HBOOST_DEDUCED_TYPENAME base_:: left_value_type &  get_left()const { return left;  }
          HBOOST_DEDUCED_TYPENAME base_::right_value_type & get_right()      { return right; }
    const HBOOST_DEDUCED_TYPENAME base_::right_value_type & get_right()const { return right; }
};



template< class TA, class TB, class Info, bool force_mutable >
class relation_info_hook : public
 ::hboost::bimaps::relation::detail::relation_storage<TA,TB,force_mutable>
{
    typedef ::hboost::bimaps::relation::detail::
                relation_storage<TA,TB,force_mutable> base_;

    typedef HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::tags::support::
        default_tagged<Info,member_at::info>::type tagged_info_type;

    public:
    typedef HBOOST_DEDUCED_TYPENAME tagged_info_type::value_type info_type;
    typedef HBOOST_DEDUCED_TYPENAME tagged_info_type::tag        info_tag;

    info_type info;

    protected:

    relation_info_hook() {}

    relation_info_hook( HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                           HBOOST_DEDUCED_TYPENAME base_::left_value_type
                        >::param_type l,
                        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                            HBOOST_DEDUCED_TYPENAME base_::right_value_type
                        >::param_type r,
                        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                            info_type
                        >::param_type i = info_type() )

        : base_(l,r), info(i) {}

    template< class Relation >
    relation_info_hook( const Relation & rel ) :
        base_(rel.left,rel.right),
        info(rel.info) {}

    template< class Relation >
    void change_to( const Relation & rel )
    {
        base_::left  = rel.left ;
        base_::right = rel.right;
        info         = rel.info ;
    }

    #ifndef HBOOST_BIMAP_DISABLE_SERIALIZATION
    template< class Archive >
    void serialize(Archive & ar, const unsigned int)
    {
        ar & ::hboost::serialization::make_nvp("left" , base_::left );
        ar & ::hboost::serialization::make_nvp("right", base_::right);
        ar & ::hboost::serialization::make_nvp("info" , info        );
    }
    #endif // HBOOST_BIMAP_DISABLE_SERIALIZATION
};

template< class TA, class TB, bool force_mutable>
class relation_info_hook<TA,TB,::hboost::mpl::na,force_mutable> :
    public ::hboost::bimaps::relation::detail::relation_storage<TA,TB,force_mutable>
{
    typedef ::hboost::bimaps::relation::detail::
                relation_storage<TA,TB,force_mutable> base_;

    public:
    typedef ::hboost::mpl::na info_type;
    typedef member_at::info info_tag;

    protected:

    relation_info_hook() {}

    relation_info_hook( HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                           HBOOST_DEDUCED_TYPENAME base_::left_value_type
                        >::param_type l,
                        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                            HBOOST_DEDUCED_TYPENAME base_::right_value_type
                        >::param_type r)

        : base_(l,r) {}

    template< class Relation >
    relation_info_hook( const Relation & rel ) :
        base_(rel.left,rel.right) {}

    template< class Relation >
    void change_to( const Relation & rel )
    {
        base_::left  = rel.left ;
        base_::right = rel.right;
    }

    #ifndef HBOOST_BIMAP_DISABLE_SERIALIZATION
    template< class Archive >
    void serialize(Archive & ar, const unsigned int)
    {
        ar & ::hboost::serialization::make_nvp("left" , base_::left );
        ar & ::hboost::serialization::make_nvp("right", base_::right);
    }
    #endif // HBOOST_BIMAP_DISABLE_SERIALIZATION
};


} // namespace detail

/// \brief Abstraction of a related pair of values, that extends the std::pair class.
/**
The mutant_relation is a mutant class. A mutant class can mutate
with zero overhead in other classes that are called views.
Each view has to be StorageCompatible with the base class
of the mutant. Note that all the views have the following
storage structure:

\verbatim
                        __________
                       |          |
                       |    TA    |
                       |__________|
                       |          |
                       |    TB    |
                       |__________|

\endverbatim

See also select_relation, standard_relation.
\ingroup relation_group
                                                           **/


template< class TA, class TB, class Info = ::hboost::mpl::na, bool force_mutable = false >
class mutant_relation : public
    ::hboost::bimaps::relation::detail::
        relation_info_hook<TA,TB,Info,force_mutable>
{
    typedef ::hboost::bimaps::relation::detail::
        relation_info_hook<TA,TB,Info,force_mutable> base_;

    public:

    // We have to know the type of the base where the types are
    // defined because Boost.MultiIndex requires it.

    typedef ::hboost::bimaps::relation::detail::
                relation_storage<TA,TB,force_mutable> storage_base;

    /// Above view, non mutable view of the relation

    typedef mutant_relation<TA,TB,Info,false> above_view;

    //@{
        /// A signature compatible std::pair that is a view of the relation.

        typedef structured_pair< TA, TB, Info, normal_layout >  left_pair;
        typedef structured_pair< TB, TA, Info, mirror_layout > right_pair;
    //@}

    typedef ::hboost::mpl::vector4
    <
         left_pair,
        right_pair,

        mutant_relation< TA, TB, Info, true  >,
        mutant_relation< TA, TB, Info, false >

    > mutant_views;

    mutant_relation() {}

    mutant_relation(HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_:: left_value_type
                    >::param_type l,
                    HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::right_value_type
                    >::param_type r) :
        base_(l,r) {}

    mutant_relation(HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_:: left_value_type
                    >::param_type l,
                    HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::right_value_type
                    >::param_type r,
                    HBOOST_DEDUCED_TYPENAME ::hboost::call_traits<
                        HBOOST_DEDUCED_TYPENAME base_::info_type
                    >::param_type i) :
        base_(l,r,i) {}

    mutant_relation(const mutant_relation<TA,TB,Info,false> & rel) :
        base_(rel) {}

    mutant_relation(const mutant_relation<TA,TB,Info,true> & rel) :
        base_(rel) {}

    // Operators

    template< bool FM >
    mutant_relation& operator=(const mutant_relation<TA,TB,Info,FM> & rel)
    {
        base_::change_to(rel);
        return *this;
    }

    // The following functions are redundant if you only consider this class.
    // They are included to make easier the construction of the get and the
    // pair_by metafunction. Remember that not all compiler supports the mutant
    // idiom.

    left_pair & get_left_pair()
    {
        return ::hboost::bimaps::relation::detail::mutate<left_pair>(*this);
    }

    const left_pair & get_left_pair() const
    {
        return ::hboost::bimaps::relation::detail::mutate<left_pair>(*this);
    }

    right_pair & get_right_pair()
    {
        return ::hboost::bimaps::relation::detail::mutate<right_pair>(*this);
    }

    const right_pair & get_right_pair() const
    {
        return ::hboost::bimaps::relation::detail::mutate<right_pair>(*this);
    }

    above_view & get_view()
    {
        return ::hboost::bimaps::relation::detail::mutate<above_view>(*this);
    }

    const above_view & get_view() const
    {
        return ::hboost::bimaps::relation::detail::mutate<above_view>(*this);
    }

    template< class Tag >
    const HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
        result_of::get<Tag,const mutant_relation>::type
    get() const
    {
        return ::hboost::bimaps::relation::support::get<Tag>(*this);
    }

    template< class Tag >
    HBOOST_DEDUCED_TYPENAME ::hboost::bimaps::relation::support::
        result_of::get<Tag,mutant_relation>::type
    get()
    {
        return ::hboost::bimaps::relation::support::get<Tag>(*this);
    }
    
    #ifndef HBOOST_BIMAP_DISABLE_SERIALIZATION

    private:
    friend class ::hboost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        base_::serialize(ar,version);
    }

    #endif // HBOOST_BIMAP_DISABLE_SERIALIZATION
};

// hash value

template< class FirstType, class SecondType, bool FM >
std::size_t hash_value(const detail::relation_storage<FirstType,SecondType,FM> & r)
{
    std::size_t seed = 0;
    ::hboost::hash_combine(seed, r. left );
    ::hboost::hash_combine(seed, r.right );

    return seed;
}

// mutant_relation - mutant_relation

template< class FirstType, class SecondType, bool FM1, bool FM2 >
bool operator==(const detail::relation_storage<FirstType,SecondType,FM1> & a,
                const detail::relation_storage<FirstType,SecondType,FM2> & b)
{
    return ( ( a.left  == b.left  ) &&
             ( a.right == b.right ) );
}

template< class FirstType, class SecondType, bool FM1, bool FM2 >
bool operator!=(const detail::relation_storage<FirstType,SecondType,FM1> & a,
                const detail::relation_storage<FirstType,SecondType,FM2> & b)
{
    return ! ( a == b );
}

template< class FirstType, class SecondType, bool FM1, bool FM2 >
bool operator<(const detail::relation_storage<FirstType,SecondType,FM1> & a,
               const detail::relation_storage<FirstType,SecondType,FM2> & b)
{
    return (  ( a.left  <  b.left  ) ||
             (( a.left == b.left ) && ( a.right < b.right )));
}

template< class FirstType, class SecondType, bool FM1, bool FM2 >
bool operator<=(const detail::relation_storage<FirstType,SecondType,FM1> & a,
                const detail::relation_storage<FirstType,SecondType,FM2> & b)
{
    return (  ( a.left  <  b.left  ) ||
             (( a.left == b.left ) && ( a.right <= b.right )));
}

template< class FirstType, class SecondType, bool FM1, bool FM2 >
bool operator>(const detail::relation_storage<FirstType,SecondType,FM1> & a,
               const detail::relation_storage<FirstType,SecondType,FM2> & b)
{
    return ( ( a.left  >  b.left  ) ||
             (( a.left == b.left ) && ( a.right > b.right )));
}

template< class FirstType, class SecondType, bool FM1, bool FM2 >
bool operator>=(const detail::relation_storage<FirstType,SecondType,FM1> & a,
                const detail::relation_storage<FirstType,SecondType,FM2> & b)
{
    return ( ( a.left  >  b.left  ) ||
             (( a.left == b.left ) && ( a.right >= b.right )));
}

namespace detail {

template< class TA, class TB, class Info, bool force_mutable>
mutant_relation<TA,TB,Info,force_mutable> 
    copy_with_left_replaced(mutant_relation<TA,TB,Info,force_mutable> const& rel,
        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits< HBOOST_DEDUCED_TYPENAME 
            mutant_relation<TA,TB,Info,force_mutable>::left_value_type>
                ::param_type l)
{
    return mutant_relation<TA,TB,Info,force_mutable>(l,rel.right,rel.info);
}
    
template< class TA, class TB, bool force_mutable>
mutant_relation<TA,TB,::hboost::mpl::na,force_mutable>
    copy_with_left_replaced(mutant_relation<TA,TB,::hboost::mpl::na,force_mutable> const& rel,
        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits< HBOOST_DEDUCED_TYPENAME 
            mutant_relation<TA,TB,::hboost::mpl::na,force_mutable>::left_value_type>
                ::param_type l)
{
    return mutant_relation<TA,TB,::hboost::mpl::na,force_mutable>(l,rel.right);  
}
    
template< class TA, class TB, class Info, bool force_mutable>
mutant_relation<TA,TB,Info,force_mutable> 
    copy_with_right_replaced(mutant_relation<TA,TB,Info,force_mutable> const& rel,
        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits< HBOOST_DEDUCED_TYPENAME 
            mutant_relation<TA,TB,Info,force_mutable>::right_value_type>
                ::param_type r)
{
    return mutant_relation<TA,TB,Info,force_mutable>(rel.left,r,rel.info);
}
    
template< class TA, class TB, bool force_mutable>
mutant_relation<TA,TB,::hboost::mpl::na,force_mutable>
    copy_with_right_replaced(mutant_relation<TA,TB,::hboost::mpl::na,force_mutable> const& rel,
        HBOOST_DEDUCED_TYPENAME ::hboost::call_traits< HBOOST_DEDUCED_TYPENAME 
            mutant_relation<TA,TB,::hboost::mpl::na,force_mutable>::right_value_type>
                ::param_type r)
{
    return mutant_relation<TA,TB,::hboost::mpl::na,force_mutable>(rel.left,r);  
}

} // namespace detail

} // namespace relation
} // namespace bimaps
} // namespace hboost


#endif // HBOOST_BIMAP_RELATION_MUTANT_RELATION_HPP



