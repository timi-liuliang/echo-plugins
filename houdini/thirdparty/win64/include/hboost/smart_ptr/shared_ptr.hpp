#ifndef HBOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED
#define HBOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED

//
//  shared_ptr.hpp
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001-2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/shared_ptr.htm for documentation.
//

#include <hboost/config.hpp>   // for broken compiler workarounds

// In order to avoid circular dependencies with Boost.TR1
// we make sure that our include of <memory> doesn't try to
// pull in the TR1 headers: that's why we use this header 
// rather than including <memory> directly:
#include <hboost/config/no_tr1/memory.hpp>  // std::auto_ptr

#include <hboost/assert.hpp>
#include <hboost/checked_delete.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/smart_ptr/detail/shared_count.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/smart_ptr/detail/sp_convertible.hpp>
#include <hboost/smart_ptr/detail/sp_nullptr_t.hpp>
#include <hboost/smart_ptr/detail/sp_disable_deprecated.hpp>

#if !defined(HBOOST_SP_NO_ATOMIC_ACCESS)
#include <hboost/smart_ptr/detail/spinlock_pool.hpp>
#endif

#include <algorithm>            // for std::swap
#include <functional>           // for std::less
#include <typeinfo>             // for std::bad_cast
#include <cstddef>              // for std::size_t

#if !defined(HBOOST_NO_IOSTREAM)
#if !defined(HBOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif

#if defined( HBOOST_SP_DISABLE_DEPRECATED )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

namespace hboost
{

template<class T> class shared_ptr;
template<class T> class weak_ptr;
template<class T> class enable_shared_from_this;
class enable_shared_from_raw;

namespace movelib
{

    template< class T, class D > class unique_ptr;

} // namespace movelib

namespace detail
{

// sp_element, element_type

template< class T > struct sp_element
{
    typedef T type;
};

#if !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

template< class T > struct sp_element< T[] >
{
    typedef T type;
};

#if !defined( __BORLANDC__ ) || !HBOOST_WORKAROUND( __BORLANDC__, < 0x600 )

template< class T, std::size_t N > struct sp_element< T[N] >
{
    typedef T type;
};

#endif

#endif // !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

// sp_dereference, return type of operator*

template< class T > struct sp_dereference
{
    typedef T & type;
};

template<> struct sp_dereference< void >
{
    typedef void type;
};

#if !defined(HBOOST_NO_CV_VOID_SPECIALIZATIONS)

template<> struct sp_dereference< void const >
{
    typedef void type;
};

template<> struct sp_dereference< void volatile >
{
    typedef void type;
};

template<> struct sp_dereference< void const volatile >
{
    typedef void type;
};

#endif // !defined(HBOOST_NO_CV_VOID_SPECIALIZATIONS)

#if !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

template< class T > struct sp_dereference< T[] >
{
    typedef void type;
};

#if !defined( __BORLANDC__ ) || !HBOOST_WORKAROUND( __BORLANDC__, < 0x600 )

template< class T, std::size_t N > struct sp_dereference< T[N] >
{
    typedef void type;
};

#endif

#endif // !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

// sp_member_access, return type of operator->

template< class T > struct sp_member_access
{
    typedef T * type;
};

#if !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

template< class T > struct sp_member_access< T[] >
{
    typedef void type;
};

#if !defined( __BORLANDC__ ) || !HBOOST_WORKAROUND( __BORLANDC__, < 0x600 )

template< class T, std::size_t N > struct sp_member_access< T[N] >
{
    typedef void type;
};

#endif

#endif // !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

// sp_array_access, return type of operator[]

template< class T > struct sp_array_access
{
    typedef void type;
};

#if !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

template< class T > struct sp_array_access< T[] >
{
    typedef T & type;
};

#if !defined( __BORLANDC__ ) || !HBOOST_WORKAROUND( __BORLANDC__, < 0x600 )

template< class T, std::size_t N > struct sp_array_access< T[N] >
{
    typedef T & type;
};

#endif

#endif // !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

// sp_extent, for operator[] index check

template< class T > struct sp_extent
{
    enum _vt { value = 0 };
};

#if !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

template< class T, std::size_t N > struct sp_extent< T[N] >
{
    enum _vt { value = N };
};

#endif // !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

// enable_shared_from_this support

template< class X, class Y, class T > inline void sp_enable_shared_from_this( hboost::shared_ptr<X> const * ppx, Y const * py, hboost::enable_shared_from_this< T > const * pe )
{
    if( pe != 0 )
    {
        pe->_internal_accept_owner( ppx, const_cast< Y* >( py ) );
    }
}

template< class X, class Y > inline void sp_enable_shared_from_this( hboost::shared_ptr<X> * ppx, Y const * py, hboost::enable_shared_from_raw const * pe );

#ifdef _MANAGED

// Avoid C4793, ... causes native code generation

struct sp_any_pointer
{
    template<class T> sp_any_pointer( T* ) {}
};

inline void sp_enable_shared_from_this( sp_any_pointer, sp_any_pointer, sp_any_pointer )
{
}

#else // _MANAGED

inline void sp_enable_shared_from_this( ... )
{
}

#endif // _MANAGED

#if !defined( HBOOST_NO_SFINAE ) && !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION ) && !defined( HBOOST_NO_AUTO_PTR )

// rvalue auto_ptr support based on a technique by Dave Abrahams

template< class T, class R > struct sp_enable_if_auto_ptr
{
};

template< class T, class R > struct sp_enable_if_auto_ptr< std::auto_ptr< T >, R >
{
    typedef R type;
}; 

#endif

// sp_assert_convertible

template< class Y, class T > inline void sp_assert_convertible()
{
#if !defined( HBOOST_SP_NO_SP_CONVERTIBLE )

    // static_assert( sp_convertible< Y, T >::value );
    typedef char tmp[ sp_convertible< Y, T >::value? 1: -1 ];
    (void)sizeof( tmp );

#else

    T* p = static_cast< Y* >( 0 );
    (void)p;

#endif
}

// pointer constructor helper

template< class T, class Y > inline void sp_pointer_construct( hboost::shared_ptr< T > * ppx, Y * p, hboost::detail::shared_count & pn )
{
    hboost::detail::shared_count( p ).swap( pn );
    hboost::detail::sp_enable_shared_from_this( ppx, p, p );
}

#if !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

template< class T, class Y > inline void sp_pointer_construct( hboost::shared_ptr< T[] > * /*ppx*/, Y * p, hboost::detail::shared_count & pn )
{
    sp_assert_convertible< Y[], T[] >();
    hboost::detail::shared_count( p, hboost::checked_array_deleter< T >() ).swap( pn );
}

template< class T, std::size_t N, class Y > inline void sp_pointer_construct( hboost::shared_ptr< T[N] > * /*ppx*/, Y * p, hboost::detail::shared_count & pn )
{
    sp_assert_convertible< Y[N], T[N] >();
    hboost::detail::shared_count( p, hboost::checked_array_deleter< T >() ).swap( pn );
}

#endif // !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

// deleter constructor helper

template< class T, class Y > inline void sp_deleter_construct( hboost::shared_ptr< T > * ppx, Y * p )
{
    hboost::detail::sp_enable_shared_from_this( ppx, p, p );
}

#if !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

template< class T, class Y > inline void sp_deleter_construct( hboost::shared_ptr< T[] > * /*ppx*/, Y * /*p*/ )
{
    sp_assert_convertible< Y[], T[] >();
}

template< class T, std::size_t N, class Y > inline void sp_deleter_construct( hboost::shared_ptr< T[N] > * /*ppx*/, Y * /*p*/ )
{
    sp_assert_convertible< Y[N], T[N] >();
}

#endif // !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

} // namespace detail


//
//  shared_ptr
//
//  An enhanced relative of scoped_ptr with reference counted copy semantics.
//  The object pointed to is deleted when the last shared_ptr pointing to it
//  is destroyed or reset.
//

template<class T> class shared_ptr
{
private:

    // Borland 5.5.1 specific workaround
    typedef shared_ptr<T> this_type;

public:

    typedef typename hboost::detail::sp_element< T >::type element_type;

    shared_ptr() HBOOST_NOEXCEPT : px( 0 ), pn() // never throws in 1.30+
    {
    }

#if !defined( HBOOST_NO_CXX11_NULLPTR )

    shared_ptr( hboost::detail::sp_nullptr_t ) HBOOST_NOEXCEPT : px( 0 ), pn() // never throws
    {
    }

#endif

    template<class Y>
    explicit shared_ptr( Y * p ): px( p ), pn() // Y must be complete
    {
        hboost::detail::sp_pointer_construct( this, p, pn );
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared_ptr will release p by calling d(p)
    //

    template<class Y, class D> shared_ptr( Y * p, D d ): px( p ), pn( p, d )
    {
        hboost::detail::sp_deleter_construct( this, p );
    }

#if !defined( HBOOST_NO_CXX11_NULLPTR )

    template<class D> shared_ptr( hboost::detail::sp_nullptr_t p, D d ): px( p ), pn( p, d )
    {
    }

#endif

    // As above, but with allocator. A's copy constructor shall not throw.

    template<class Y, class D, class A> shared_ptr( Y * p, D d, A a ): px( p ), pn( p, d, a )
    {
        hboost::detail::sp_deleter_construct( this, p );
    }

#if !defined( HBOOST_NO_CXX11_NULLPTR )

    template<class D, class A> shared_ptr( hboost::detail::sp_nullptr_t p, D d, A a ): px( p ), pn( p, d, a )
    {
    }

#endif

//  generated copy constructor, destructor are fine...

#if !defined( HBOOST_NO_CXX11_RVALUE_REFERENCES )

// ... except in C++0x, move disables the implicit copy

    shared_ptr( shared_ptr const & r ) HBOOST_NOEXCEPT : px( r.px ), pn( r.pn )
    {
    }

#endif

    template<class Y>
    explicit shared_ptr( weak_ptr<Y> const & r ): pn( r.pn ) // may throw
    {
        hboost::detail::sp_assert_convertible< Y, T >();

        // it is now safe to copy r.px, as pn(r.pn) did not throw
        px = r.px;
    }

    template<class Y>
    shared_ptr( weak_ptr<Y> const & r, hboost::detail::sp_nothrow_tag )
    HBOOST_NOEXCEPT : px( 0 ), pn( r.pn, hboost::detail::sp_nothrow_tag() )
    {
        if( !pn.empty() )
        {
            px = r.px;
        }
    }

    template<class Y>
#if !defined( HBOOST_SP_NO_SP_CONVERTIBLE )

    shared_ptr( shared_ptr<Y> const & r, typename hboost::detail::sp_enable_if_convertible<Y,T>::type = hboost::detail::sp_empty() )

#else

    shared_ptr( shared_ptr<Y> const & r )

#endif
    HBOOST_NOEXCEPT : px( r.px ), pn( r.pn )
    {
        hboost::detail::sp_assert_convertible< Y, T >();
    }

    // aliasing
    template< class Y >
    shared_ptr( shared_ptr<Y> const & r, element_type * p ) HBOOST_NOEXCEPT : px( p ), pn( r.pn )
    {
    }

#ifndef HBOOST_NO_AUTO_PTR

    template<class Y>
    explicit shared_ptr( std::auto_ptr<Y> & r ): px(r.get()), pn()
    {
        hboost::detail::sp_assert_convertible< Y, T >();

        Y * tmp = r.get();
        pn = hboost::detail::shared_count( r );

        hboost::detail::sp_deleter_construct( this, tmp );
    }

#if !defined( HBOOST_NO_CXX11_RVALUE_REFERENCES )

    template<class Y>
    shared_ptr( std::auto_ptr<Y> && r ): px(r.get()), pn()
    {
        hboost::detail::sp_assert_convertible< Y, T >();

        Y * tmp = r.get();
        pn = hboost::detail::shared_count( r );

        hboost::detail::sp_deleter_construct( this, tmp );
    }

#elif !defined( HBOOST_NO_SFINAE ) && !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class Ap>
    explicit shared_ptr( Ap r, typename hboost::detail::sp_enable_if_auto_ptr<Ap, int>::type = 0 ): px( r.get() ), pn()
    {
        typedef typename Ap::element_type Y;

        hboost::detail::sp_assert_convertible< Y, T >();

        Y * tmp = r.get();
        pn = hboost::detail::shared_count( r );

        hboost::detail::sp_deleter_construct( this, tmp );
    }

#endif // HBOOST_NO_SFINAE, HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // HBOOST_NO_AUTO_PTR

#if !defined( HBOOST_NO_CXX11_SMART_PTR ) && !defined( HBOOST_NO_CXX11_RVALUE_REFERENCES )

    template< class Y, class D >
    shared_ptr( std::unique_ptr< Y, D > && r ): px( r.get() ), pn()
    {
        hboost::detail::sp_assert_convertible< Y, T >();

        typename std::unique_ptr< Y, D >::pointer tmp = r.get();
        pn = hboost::detail::shared_count( r );

        hboost::detail::sp_deleter_construct( this, tmp );
    }

#endif

    template< class Y, class D >
    shared_ptr( hboost::movelib::unique_ptr< Y, D > r ): px( r.get() ), pn()
    {
        hboost::detail::sp_assert_convertible< Y, T >();

        typename hboost::movelib::unique_ptr< Y, D >::pointer tmp = r.get();
        pn = hboost::detail::shared_count( r );

        hboost::detail::sp_deleter_construct( this, tmp );
    }

    // assignment

    shared_ptr & operator=( shared_ptr const & r ) HBOOST_NOEXCEPT
    {
        this_type(r).swap(*this);
        return *this;
    }

#if !defined(HBOOST_MSVC) || (HBOOST_MSVC >= 1400)

    template<class Y>
    shared_ptr & operator=(shared_ptr<Y> const & r) HBOOST_NOEXCEPT
    {
        this_type(r).swap(*this);
        return *this;
    }

#endif

#ifndef HBOOST_NO_AUTO_PTR

    template<class Y>
    shared_ptr & operator=( std::auto_ptr<Y> & r )
    {
        this_type( r ).swap( *this );
        return *this;
    }

#if !defined( HBOOST_NO_CXX11_RVALUE_REFERENCES )

    template<class Y>
    shared_ptr & operator=( std::auto_ptr<Y> && r )
    {
        this_type( static_cast< std::auto_ptr<Y> && >( r ) ).swap( *this );
        return *this;
    }

#elif !defined( HBOOST_NO_SFINAE ) && !defined( HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class Ap>
    typename hboost::detail::sp_enable_if_auto_ptr< Ap, shared_ptr & >::type operator=( Ap r )
    {
        this_type( r ).swap( *this );
        return *this;
    }

#endif // HBOOST_NO_SFINAE, HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // HBOOST_NO_AUTO_PTR

#if !defined( HBOOST_NO_CXX11_SMART_PTR ) && !defined( HBOOST_NO_CXX11_RVALUE_REFERENCES )

    template<class Y, class D>
    shared_ptr & operator=( std::unique_ptr<Y, D> && r )
    {
        this_type( static_cast< std::unique_ptr<Y, D> && >( r ) ).swap(*this);
        return *this;
    }

#endif

    template<class Y, class D>
    shared_ptr & operator=( hboost::movelib::unique_ptr<Y, D> r )
    {
        // this_type( static_cast< unique_ptr<Y, D> && >( r ) ).swap( *this );

        hboost::detail::sp_assert_convertible< Y, T >();

        typename hboost::movelib::unique_ptr< Y, D >::pointer p = r.get();

        shared_ptr tmp;

        tmp.px = p;
        tmp.pn = hboost::detail::shared_count( r );

        hboost::detail::sp_deleter_construct( &tmp, p );

        tmp.swap( *this );

        return *this;
    }

// Move support

#if !defined( HBOOST_NO_CXX11_RVALUE_REFERENCES )

    shared_ptr( shared_ptr && r ) HBOOST_NOEXCEPT : px( r.px ), pn()
    {
        pn.swap( r.pn );
        r.px = 0;
    }

    template<class Y>
#if !defined( HBOOST_SP_NO_SP_CONVERTIBLE )

    shared_ptr( shared_ptr<Y> && r, typename hboost::detail::sp_enable_if_convertible<Y,T>::type = hboost::detail::sp_empty() )

#else

    shared_ptr( shared_ptr<Y> && r )

#endif
    HBOOST_NOEXCEPT : px( r.px ), pn()
    {
        hboost::detail::sp_assert_convertible< Y, T >();

        pn.swap( r.pn );
        r.px = 0;
    }

    shared_ptr & operator=( shared_ptr && r ) HBOOST_NOEXCEPT
    {
        this_type( static_cast< shared_ptr && >( r ) ).swap( *this );
        return *this;
    }

    template<class Y>
    shared_ptr & operator=( shared_ptr<Y> && r ) HBOOST_NOEXCEPT
    {
        this_type( static_cast< shared_ptr<Y> && >( r ) ).swap( *this );
        return *this;
    }

#endif

#if !defined( HBOOST_NO_CXX11_NULLPTR )

    shared_ptr & operator=( hboost::detail::sp_nullptr_t ) HBOOST_NOEXCEPT // never throws
    {
        this_type().swap(*this);
        return *this;
    }

#endif

    void reset() HBOOST_NOEXCEPT // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    template<class Y> void reset( Y * p ) // Y must be complete
    {
        HBOOST_ASSERT( p == 0 || p != px ); // catch self-reset errors
        this_type( p ).swap( *this );
    }

    template<class Y, class D> void reset( Y * p, D d )
    {
        this_type( p, d ).swap( *this );
    }

    template<class Y, class D, class A> void reset( Y * p, D d, A a )
    {
        this_type( p, d, a ).swap( *this );
    }

    template<class Y> void reset( shared_ptr<Y> const & r, element_type * p )
    {
        this_type( r, p ).swap( *this );
    }
    
    // never throws (but has a HBOOST_ASSERT in it, so not marked with HBOOST_NOEXCEPT)
    typename hboost::detail::sp_dereference< T >::type operator* () const
    {
        HBOOST_ASSERT( px != 0 );
        return *px;
    }
    
    // never throws (but has a HBOOST_ASSERT in it, so not marked with HBOOST_NOEXCEPT)
    typename hboost::detail::sp_member_access< T >::type operator-> () const 
    {
        HBOOST_ASSERT( px != 0 );
        return px;
    }
    
    // never throws (but has a HBOOST_ASSERT in it, so not marked with HBOOST_NOEXCEPT)
    typename hboost::detail::sp_array_access< T >::type operator[] ( std::ptrdiff_t i ) const
    {
        HBOOST_ASSERT( px != 0 );
        HBOOST_ASSERT( i >= 0 && ( i < hboost::detail::sp_extent< T >::value || hboost::detail::sp_extent< T >::value == 0 ) );

        return static_cast< typename hboost::detail::sp_array_access< T >::type >( px[ i ] );
    }

    element_type * get() const HBOOST_NOEXCEPT
    {
        return px;
    }

// implicit conversion to "bool"
#include <hboost/smart_ptr/detail/operator_bool.hpp>

    bool unique() const HBOOST_NOEXCEPT
    {
        return pn.unique();
    }

    long use_count() const HBOOST_NOEXCEPT
    {
        return pn.use_count();
    }

    void swap( shared_ptr & other ) HBOOST_NOEXCEPT
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    template<class Y> bool owner_before( shared_ptr<Y> const & rhs ) const HBOOST_NOEXCEPT
    {
        return pn < rhs.pn;
    }

    template<class Y> bool owner_before( weak_ptr<Y> const & rhs ) const HBOOST_NOEXCEPT
    {
        return pn < rhs.pn;
    }

    void * _internal_get_deleter( hboost::detail::sp_typeinfo const & ti ) const HBOOST_NOEXCEPT
    {
        return pn.get_deleter( ti );
    }

    void * _internal_get_untyped_deleter() const HBOOST_NOEXCEPT
    {
        return pn.get_untyped_deleter();
    }

    bool _internal_equiv( shared_ptr const & r ) const HBOOST_NOEXCEPT
    {
        return px == r.px && pn == r.pn;
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<class Y> friend class shared_ptr;
    template<class Y> friend class weak_ptr;


#endif

    element_type * px;                 // contained pointer
    hboost::detail::shared_count pn;    // reference counter

};  // shared_ptr

template<class T, class U> inline bool operator==(shared_ptr<T> const & a, shared_ptr<U> const & b) HBOOST_NOEXCEPT
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(shared_ptr<T> const & a, shared_ptr<U> const & b) HBOOST_NOEXCEPT
{
    return a.get() != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<class T> inline bool operator!=(shared_ptr<T> const & a, shared_ptr<T> const & b) HBOOST_NOEXCEPT
{
    return a.get() != b.get();
}

#endif

#if !defined( HBOOST_NO_CXX11_NULLPTR )

template<class T> inline bool operator==( shared_ptr<T> const & p, hboost::detail::sp_nullptr_t ) HBOOST_NOEXCEPT
{
    return p.get() == 0;
}

template<class T> inline bool operator==( hboost::detail::sp_nullptr_t, shared_ptr<T> const & p ) HBOOST_NOEXCEPT
{
    return p.get() == 0;
}

template<class T> inline bool operator!=( shared_ptr<T> const & p, hboost::detail::sp_nullptr_t ) HBOOST_NOEXCEPT
{
    return p.get() != 0;
}

template<class T> inline bool operator!=( hboost::detail::sp_nullptr_t, shared_ptr<T> const & p ) HBOOST_NOEXCEPT
{
    return p.get() != 0;
}

#endif

template<class T, class U> inline bool operator<(shared_ptr<T> const & a, shared_ptr<U> const & b) HBOOST_NOEXCEPT
{
    return a.owner_before( b );
}

template<class T> inline void swap(shared_ptr<T> & a, shared_ptr<T> & b) HBOOST_NOEXCEPT
{
    a.swap(b);
}

template<class T, class U> shared_ptr<T> static_pointer_cast( shared_ptr<U> const & r ) HBOOST_NOEXCEPT
{
    (void) static_cast< T* >( static_cast< U* >( 0 ) );

    typedef typename shared_ptr<T>::element_type E;

    E * p = static_cast< E* >( r.get() );
    return shared_ptr<T>( r, p );
}

template<class T, class U> shared_ptr<T> const_pointer_cast( shared_ptr<U> const & r ) HBOOST_NOEXCEPT
{
    (void) const_cast< T* >( static_cast< U* >( 0 ) );

    typedef typename shared_ptr<T>::element_type E;

    E * p = const_cast< E* >( r.get() );
    return shared_ptr<T>( r, p );
}

template<class T, class U> shared_ptr<T> dynamic_pointer_cast( shared_ptr<U> const & r ) HBOOST_NOEXCEPT
{
    (void) dynamic_cast< T* >( static_cast< U* >( 0 ) );

    typedef typename shared_ptr<T>::element_type E;

    E * p = dynamic_cast< E* >( r.get() );
    return p? shared_ptr<T>( r, p ): shared_ptr<T>();
}

template<class T, class U> shared_ptr<T> reinterpret_pointer_cast( shared_ptr<U> const & r ) HBOOST_NOEXCEPT
{
    (void) reinterpret_cast< T* >( static_cast< U* >( 0 ) );

    typedef typename shared_ptr<T>::element_type E;

    E * p = reinterpret_cast< E* >( r.get() );
    return shared_ptr<T>( r, p );
}

// get_pointer() enables hboost::mem_fn to recognize shared_ptr

template<class T> inline typename shared_ptr<T>::element_type * get_pointer(shared_ptr<T> const & p) HBOOST_NOEXCEPT
{
    return p.get();
}

// operator<<

#if !defined(HBOOST_NO_IOSTREAM)

#if defined(HBOOST_NO_TEMPLATED_IOSTREAMS) || ( defined(__GNUC__) &&  (__GNUC__ < 3) )

template<class Y> std::ostream & operator<< (std::ostream & os, shared_ptr<Y> const & p)
{
    os << p.get();
    return os;
}

#else

// in STLport's no-iostreams mode no iostream symbols can be used
#ifndef _STLP_NO_IOSTREAMS

# if defined(HBOOST_MSVC) && HBOOST_WORKAROUND(HBOOST_MSVC, < 1300 && __SGI_STL_PORT)
// MSVC6 has problems finding std::basic_ostream through the using declaration in namespace _STL
using std::basic_ostream;
template<class E, class T, class Y> basic_ostream<E, T> & operator<< (basic_ostream<E, T> & os, shared_ptr<Y> const & p)
# else
template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, shared_ptr<Y> const & p)
# endif
{
    os << p.get();
    return os;
}

#endif // _STLP_NO_IOSTREAMS

#endif // __GNUC__ < 3

#endif // !defined(HBOOST_NO_IOSTREAM)

// get_deleter

namespace detail
{

#if ( defined(__GNUC__) && HBOOST_WORKAROUND(__GNUC__, < 3) ) || \
    ( defined(__EDG_VERSION__) && HBOOST_WORKAROUND(__EDG_VERSION__, <= 238) ) || \
    ( defined(__HP_aCC) && HBOOST_WORKAROUND(__HP_aCC, <= 33500) )

// g++ 2.9x doesn't allow static_cast<X const *>(void *)
// apparently EDG 2.38 and HP aCC A.03.35 also don't accept it

template<class D, class T> D * basic_get_deleter(shared_ptr<T> const & p)
{
    void const * q = p._internal_get_deleter(HBOOST_SP_TYPEID(D));
    return const_cast<D *>(static_cast<D const *>(q));
}

#else

template<class D, class T> D * basic_get_deleter( shared_ptr<T> const & p ) HBOOST_NOEXCEPT
{
    return static_cast<D *>( p._internal_get_deleter(HBOOST_SP_TYPEID(D)) );
}

#endif

class esft2_deleter_wrapper
{
private:

    shared_ptr<void const volatile> deleter_;

public:

    esft2_deleter_wrapper()
    {
    }

    template< class T > void set_deleter( shared_ptr<T> const & deleter )
    {
        deleter_ = deleter;
    }

    template<typename D> D* get_deleter() const HBOOST_NOEXCEPT
    {
        return hboost::detail::basic_get_deleter<D>( deleter_ );
    }

    template< class T> void operator()( T* )
    {
        HBOOST_ASSERT( deleter_.use_count() <= 1 );
        deleter_.reset();
    }
};

} // namespace detail

template<class D, class T> D * get_deleter( shared_ptr<T> const & p ) HBOOST_NOEXCEPT
{
    D *del = hboost::detail::basic_get_deleter<D>(p);

    if(del == 0)
    {
        hboost::detail::esft2_deleter_wrapper *del_wrapper = hboost::detail::basic_get_deleter<hboost::detail::esft2_deleter_wrapper>(p);
// The following get_deleter method call is fully qualified because
// older versions of gcc (2.95, 3.2.3) fail to compile it when written del_wrapper->get_deleter<D>()
        if(del_wrapper) del = del_wrapper->::hboost::detail::esft2_deleter_wrapper::get_deleter<D>();
    }

    return del;
}

// atomic access

#if !defined(HBOOST_SP_NO_ATOMIC_ACCESS)

template<class T> inline bool atomic_is_lock_free( shared_ptr<T> const * /*p*/ ) HBOOST_NOEXCEPT
{
    return false;
}

template<class T> shared_ptr<T> atomic_load( shared_ptr<T> const * p )
{
    hboost::detail::spinlock_pool<2>::scoped_lock lock( p );
    return *p;
}

template<class T> inline shared_ptr<T> atomic_load_explicit( shared_ptr<T> const * p, /*memory_order mo*/ int )
{
    return atomic_load( p );
}

template<class T> void atomic_store( shared_ptr<T> * p, shared_ptr<T> r )
{
    hboost::detail::spinlock_pool<2>::scoped_lock lock( p );
    p->swap( r );
}

template<class T> inline void atomic_store_explicit( shared_ptr<T> * p, shared_ptr<T> r, /*memory_order mo*/ int )
{
    atomic_store( p, r ); // std::move( r )
}

template<class T> shared_ptr<T> atomic_exchange( shared_ptr<T> * p, shared_ptr<T> r )
{
    hboost::detail::spinlock & sp = hboost::detail::spinlock_pool<2>::spinlock_for( p );

    sp.lock();
    p->swap( r );
    sp.unlock();

    return r; // return std::move( r )
}

template<class T> shared_ptr<T> atomic_exchange_explicit( shared_ptr<T> * p, shared_ptr<T> r, /*memory_order mo*/ int )
{
    return atomic_exchange( p, r ); // std::move( r )
}

template<class T> bool atomic_compare_exchange( shared_ptr<T> * p, shared_ptr<T> * v, shared_ptr<T> w )
{
    hboost::detail::spinlock & sp = hboost::detail::spinlock_pool<2>::spinlock_for( p );

    sp.lock();

    if( p->_internal_equiv( *v ) )
    {
        p->swap( w );

        sp.unlock();

        return true;
    }
    else
    {
        shared_ptr<T> tmp( *p );

        sp.unlock();

        tmp.swap( *v );
        return false;
    }
}

template<class T> inline bool atomic_compare_exchange_explicit( shared_ptr<T> * p, shared_ptr<T> * v, shared_ptr<T> w, /*memory_order success*/ int, /*memory_order failure*/ int )
{
    return atomic_compare_exchange( p, v, w ); // std::move( w )
}

#endif // !defined(HBOOST_SP_NO_ATOMIC_ACCESS)

// hash_value

template< class T > struct hash;

template< class T > std::size_t hash_value( hboost::shared_ptr<T> const & p ) HBOOST_NOEXCEPT
{
    return hboost::hash< typename hboost::shared_ptr<T>::element_type* >()( p.get() );
}

} // namespace hboost

#if defined( HBOOST_SP_DISABLE_DEPRECATED )
#pragma GCC diagnostic pop
#endif

#endif  // #ifndef HBOOST_SMART_PTR_SHARED_PTR_HPP_INCLUDED
