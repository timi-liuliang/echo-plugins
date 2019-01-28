// Boost.Range library
//
//  Copyright Neil Groves 2009.
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_SUB_RANGE_HPP
#define HBOOST_RANGE_SUB_RANGE_HPP

#include <hboost/detail/workaround.hpp>

#if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1500)) 
    #pragma warning( push )
    #pragma warning( disable : 4996 )
#endif

#include <hboost/range/config.hpp>
#include <hboost/range/iterator_range.hpp>
#include <hboost/range/value_type.hpp>
#include <hboost/range/size_type.hpp>
#include <hboost/range/difference_type.hpp>
#include <hboost/range/reference.hpp>
#include <hboost/range/algorithm/equal.hpp>
#include <hboost/assert.hpp>
#include <hboost/type_traits/is_reference.hpp>
#include <hboost/type_traits/remove_reference.hpp>

namespace hboost
{
    namespace range_detail
    {

template<class ForwardRange, class TraversalTag>
class sub_range_base
        : public iterator_range<
            HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type
        >
{
    typedef iterator_range<
        HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type
    > base;

protected:
    typedef HBOOST_DEDUCED_TYPENAME base::iterator_range_ iterator_range_;

public:
    typedef HBOOST_DEDUCED_TYPENAME range_value<ForwardRange>::type value_type;
    typedef HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type iterator;
    typedef HBOOST_DEDUCED_TYPENAME range_iterator<const ForwardRange>::type const_iterator;
    typedef HBOOST_DEDUCED_TYPENAME range_difference<ForwardRange>::type difference_type;
    typedef HBOOST_DEDUCED_TYPENAME range_size<ForwardRange>::type size_type;
    typedef HBOOST_DEDUCED_TYPENAME range_reference<ForwardRange>::type reference;
    typedef HBOOST_DEDUCED_TYPENAME range_reference<const ForwardRange>::type const_reference;

    sub_range_base()
    {
    }

    template<class Iterator>
    sub_range_base(Iterator first, Iterator last)
        : base(first, last)
    {
    }

    reference front()
    {
        return base::front();
    }

    const_reference front() const
    {
        return base::front();
    }
};

template<class ForwardRange>
class sub_range_base<ForwardRange, bidirectional_traversal_tag>
        : public sub_range_base<ForwardRange, forward_traversal_tag>
{
    typedef sub_range_base<ForwardRange, forward_traversal_tag> base;
public:
    sub_range_base()
    {
    }

    template<class Iterator>
    sub_range_base(Iterator first, Iterator last)
        : base(first, last)
    {
    }

    HBOOST_DEDUCED_TYPENAME base::reference back()
    {
        return base::back();
    }

    HBOOST_DEDUCED_TYPENAME base::const_reference back() const
    {
        return base::back();
    }
};

template<class ForwardRange>
class sub_range_base<ForwardRange, random_access_traversal_tag>
        : public sub_range_base<ForwardRange, bidirectional_traversal_tag>
{
    typedef sub_range_base<ForwardRange, bidirectional_traversal_tag> base;

public:
    sub_range_base()
    {
    }

    template<class Iterator>
    sub_range_base(Iterator first, Iterator last)
        : base(first, last)
    {
    }

    HBOOST_DEDUCED_TYPENAME base::reference
    operator[](HBOOST_DEDUCED_TYPENAME base::difference_type n)
    {
        return this->begin()[n];
    }

    HBOOST_DEDUCED_TYPENAME base::const_reference
    operator[](HBOOST_DEDUCED_TYPENAME base::difference_type n) const
    {
        return this->begin()[n];
    }
};

    } // namespace range_detail

    template<class ForwardRange>
    class sub_range
        : public range_detail::sub_range_base<
                ForwardRange,
                HBOOST_DEDUCED_TYPENAME iterator_traversal<
                    HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type
                >::type
            >
    {
        typedef HBOOST_DEDUCED_TYPENAME range_iterator<
            ForwardRange
        >::type iterator_t;

        typedef range_detail::sub_range_base<
            ForwardRange,
            HBOOST_DEDUCED_TYPENAME iterator_traversal<
                HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type
            >::type
        > base;

        typedef HBOOST_DEDUCED_TYPENAME base::impl impl;

    protected:
        typedef HBOOST_DEDUCED_TYPENAME base::iterator_range_ iterator_range_;

    private:
        template<class Source>
        struct is_compatible_range
            : is_convertible<
                HBOOST_DEDUCED_TYPENAME mpl::eval_if<
                    has_range_iterator<Source>,
                    range_iterator<Source>,
                    mpl::identity<void>
                >::type,
                HBOOST_DEDUCED_TYPENAME base::iterator
            >
        {
        };

    public:
        sub_range()
        { }

#if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1500) ) 
        sub_range(const sub_range& r)
            : base(impl::adl_begin(const_cast<base&>(static_cast<const base&>(r))),
                   impl::adl_end(const_cast<base&>(static_cast<const base&>(r))))
        { }  
#endif

        template< class ForwardRange2 >
        sub_range(
            ForwardRange2& r,
            HBOOST_DEDUCED_TYPENAME ::hboost::enable_if<
                is_compatible_range<ForwardRange2>
            >::type* = 0
        )
        : base(impl::adl_begin(r), impl::adl_end(r))
        {
        }

        template< class ForwardRange2 >
        sub_range(
            const ForwardRange2& r,
            HBOOST_DEDUCED_TYPENAME ::hboost::enable_if<
                is_compatible_range<const ForwardRange2>
            >::type* = 0
        )
        : base(impl::adl_begin(r), impl::adl_end(r))
        {
        }

        HBOOST_DEDUCED_TYPENAME base::const_iterator begin() const
        {
            return base::begin();
        }

        HBOOST_DEDUCED_TYPENAME base::iterator begin()
        {
            return base::begin();
        }

        HBOOST_DEDUCED_TYPENAME base::const_iterator end() const
        {
            return base::end();
        }

        HBOOST_DEDUCED_TYPENAME base::iterator end()
        {
            return base::end();
        }

        template< class Iter >
        sub_range( Iter first, Iter last ) :
            base( first, last )
        { }

        template<class ForwardRange2>
        HBOOST_DEDUCED_TYPENAME ::hboost::enable_if<
            is_compatible_range<ForwardRange2>,
            sub_range&
        >::type
        operator=(ForwardRange2& r)
        {
            iterator_range_::operator=( r );
            return *this;
        }

        template<class ForwardRange2>
        HBOOST_DEDUCED_TYPENAME ::hboost::enable_if<
            is_compatible_range<const ForwardRange2>,
            sub_range&
        >::type
        operator=( const ForwardRange2& r )
        {
            iterator_range_::operator=( r );
            return *this;
        }   

        sub_range& operator=( const sub_range& r )
        {
            iterator_range_::operator=( static_cast<const iterator_range_&>(r) );
            return *this;            
        }
        
        sub_range& advance_begin(
            HBOOST_DEDUCED_TYPENAME base::difference_type n)
        {
            std::advance(this->m_Begin, n);
            return *this;
        }
        
        sub_range& advance_end(
            HBOOST_DEDUCED_TYPENAME base::difference_type n)
        {
            std::advance(this->m_End, n);
            return *this;
        }
    };

} // namespace 'boost'

#if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1500)) 
    #pragma warning( pop )
#endif

#endif

