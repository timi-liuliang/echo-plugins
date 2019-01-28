// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//

#ifndef HBOOST_ASSIGN_LIST_INSERTER_HPP
#define HBOOST_ASSIGN_LIST_INSERTER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/detail/workaround.hpp>

#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/config.hpp>
#include <cstddef>

#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/iteration/local.hpp>
#include <hboost/preprocessor/arithmetic/inc.hpp>

namespace hboost
{
namespace assign_detail
{
    template< class T >
    struct repeater
    {
        std::size_t  sz;
        T            val;

        repeater( std::size_t sz_, T r ) : sz( sz_ ), val( r )
        { }
    };
    
    template< class Fun >
    struct fun_repeater
    {
        std::size_t  sz;
        Fun          val;
        
        fun_repeater( std::size_t sz_, Fun r ) : sz( sz_ ), val( r )
        { }
    };
    
    template< class C >
    class call_push_back
    {
        C& c_;
    public:
        call_push_back( C& c ) : c_( c )
        { }
        
        template< class T >
        void operator()( T r ) 
        {
            c_.push_back( r );
        }
    };
    
    template< class C >
    class call_push_front
    {
        C& c_;
    public:
        call_push_front( C& c ) : c_( c )
        { }
        
        template< class T >
        void operator()( T r ) 
        {
            c_.push_front( r );
        }
    };
    
    template< class C >
    class call_push
    {
        C& c_;
    public:
        call_push( C& c ) : c_( c )
        { }
    
        template< class T >
        void operator()( T r ) 
        {
            c_.push( r );
        }
    };
    
    template< class C >
    class call_insert
    {
        C& c_;
    public:
        call_insert( C& c ) : c_( c )
        { }
    
        template< class T >
        void operator()( T r ) 
        {
            c_.insert( r );
        }
    };

    template< class C >
    class call_add_edge
    {
        C& c_;
    public:
        call_add_edge( C& c ) : c_(c)
        { }

        template< class T >
        void operator()( T l, T r )
        {
            add_edge( l, r, c_ );
        }

        template< class T, class EP >
        void operator()( T l, T r, const EP& ep )
        {
            add_edge( l, r, ep, c_ );
        }

    };
    
    struct forward_n_arguments {};
    
} // namespace 'assign_detail'

namespace assign
{

    template< class T >
    inline assign_detail::repeater<T>
    repeat( std::size_t sz, T r )
    {
        return assign_detail::repeater<T>( sz, r );
    }
    
    template< class Function >
    inline assign_detail::fun_repeater<Function>
    repeat_fun( std::size_t sz, Function r )
    {
        return assign_detail::fun_repeater<Function>( sz, r );
    }
    

    template< class Function, class Argument = assign_detail::forward_n_arguments > 
    class list_inserter
    {
        struct single_arg_type {};
        struct n_arg_type      {};

        typedef HBOOST_DEDUCED_TYPENAME mpl::if_c< is_same<Argument,assign_detail::forward_n_arguments>::value,
                                                  n_arg_type,
                                                  single_arg_type >::type arg_type;  
            
    public:
        
        list_inserter( Function fun ) : insert_( fun )
        {}
        
        template< class Function2, class Arg >
        list_inserter( const list_inserter<Function2,Arg>& r ) 
        : insert_( r.fun_private() ) 
        {}

        list_inserter( const list_inserter& r ) : insert_( r.insert_ )
        {}

        list_inserter& operator()()
        {
            insert_( Argument() );
            return *this;
        }
        
        template< class T >
        list_inserter& operator=( const T& r )
        {
            insert_( r );
            return *this;
        }
        
        template< class T >
        list_inserter& operator=( assign_detail::repeater<T> r )
        {
            return operator,( r );
        }
        
        template< class Nullary_function >
        list_inserter& operator=( const assign_detail::fun_repeater<Nullary_function>& r )
        {
            return operator,( r );
        }
        
        template< class T >
        list_inserter& operator,( const T& r )
        {
            insert_( r  );
            return *this;
        }

#if HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3205))
        template< class T >
        list_inserter& operator,( const assign_detail::repeater<T> & r )
        {
            return repeat( r.sz, r.val ); 
        }
#else
        template< class T >
        list_inserter& operator,( assign_detail::repeater<T> r )
        {
            return repeat( r.sz, r.val ); 
        }
#endif
        
        template< class Nullary_function >
        list_inserter& operator,( const assign_detail::fun_repeater<Nullary_function>& r )
        {
            return repeat_fun( r.sz, r.val ); 
        }

        template< class T >
        list_inserter& repeat( std::size_t sz, T r )
        {
            std::size_t i = 0;
            while( i++ != sz )
                insert_( r );
            return *this;
        }
        
        template< class Nullary_function >
        list_inserter& repeat_fun( std::size_t sz, Nullary_function fun )
        {
            std::size_t i = 0;
            while( i++ != sz )
                insert_( fun() );
            return *this;
        }

        template< class SinglePassIterator >
        list_inserter& range( SinglePassIterator first, 
                              SinglePassIterator last )
        {
            for( ; first != last; ++first )
                insert_( *first );
            return *this;
        }
        
        template< class SinglePassRange >
        list_inserter& range( const SinglePassRange& r )
        {
            return range( hboost::begin(r), hboost::end(r) );
        }
        
        template< class T >
        list_inserter& operator()( const T& t )
        {
            insert_( t );
            return *this;
        }

#ifndef HBOOST_ASSIGN_MAX_PARAMS // use user's value
#define HBOOST_ASSIGN_MAX_PARAMS 5        
#endif
#define HBOOST_ASSIGN_MAX_PARAMETERS (HBOOST_ASSIGN_MAX_PARAMS - 1)
#define HBOOST_ASSIGN_PARAMS1(n) HBOOST_PP_ENUM_PARAMS(n, class T)
#define HBOOST_ASSIGN_PARAMS2(n) HBOOST_PP_ENUM_BINARY_PARAMS(n, T, const& t)
#define HBOOST_ASSIGN_PARAMS3(n) HBOOST_PP_ENUM_PARAMS(n, t)
        
#define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_ASSIGN_MAX_PARAMETERS)
#define HBOOST_PP_LOCAL_MACRO(n) \
    template< class T, HBOOST_ASSIGN_PARAMS1(n) > \
    list_inserter& operator()(T t, HBOOST_ASSIGN_PARAMS2(n) ) \
        { \
            HBOOST_PP_CAT(insert, HBOOST_PP_INC(n))(t, HBOOST_ASSIGN_PARAMS3(n), arg_type()); \
            return *this; \
        } \
        /**/

#include HBOOST_PP_LOCAL_ITERATE()
        

#define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_ASSIGN_MAX_PARAMETERS)
#define HBOOST_PP_LOCAL_MACRO(n) \
    template< class T, HBOOST_ASSIGN_PARAMS1(n) > \
    void HBOOST_PP_CAT(insert, HBOOST_PP_INC(n))(T const& t, HBOOST_ASSIGN_PARAMS2(n), single_arg_type) \
    { \
        insert_( Argument(t, HBOOST_ASSIGN_PARAMS3(n) )); \
    } \
    /**/
        
#include HBOOST_PP_LOCAL_ITERATE()

#define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_ASSIGN_MAX_PARAMETERS)
#define HBOOST_PP_LOCAL_MACRO(n) \
    template< class T, HBOOST_ASSIGN_PARAMS1(n) > \
    void HBOOST_PP_CAT(insert, HBOOST_PP_INC(n))(T const& t, HBOOST_ASSIGN_PARAMS2(n), n_arg_type) \
    { \
        insert_(t, HBOOST_ASSIGN_PARAMS3(n) ); \
    } \
    /**/
        
#include HBOOST_PP_LOCAL_ITERATE()

        
        Function fun_private() const
        {
            return insert_;
        }

    private:
        
        list_inserter& operator=( const list_inserter& );
        Function insert_;
    };
    
    template< class Function >
    inline list_inserter< Function >
    make_list_inserter( Function fun )
    {
        return list_inserter< Function >( fun );
    }
    
    template< class Function, class Argument >
    inline list_inserter<Function,Argument>
    make_list_inserter( Function fun, Argument* )
    {
        return list_inserter<Function,Argument>( fun );
    }

    template< class C >
    inline list_inserter< assign_detail::call_push_back<C>, 
                          HBOOST_DEDUCED_TYPENAME C::value_type >
    push_back( C& c )
    {
        static HBOOST_DEDUCED_TYPENAME C::value_type* p = 0;
        return make_list_inserter( assign_detail::call_push_back<C>( c ), 
                                   p );
    }
    
    template< class C >
    inline list_inserter< assign_detail::call_push_front<C>,
                          HBOOST_DEDUCED_TYPENAME C::value_type >
    push_front( C& c )
    {
        static HBOOST_DEDUCED_TYPENAME C::value_type* p = 0;
        return make_list_inserter( assign_detail::call_push_front<C>( c ),
                                   p );
    }

    template< class C >
    inline list_inserter< assign_detail::call_insert<C>, 
                          HBOOST_DEDUCED_TYPENAME C::value_type >
    insert( C& c )
    {
        static HBOOST_DEDUCED_TYPENAME C::value_type* p = 0;
        return make_list_inserter( assign_detail::call_insert<C>( c ),
                                   p );
    }

    template< class C >
    inline list_inserter< assign_detail::call_push<C>, 
                          HBOOST_DEDUCED_TYPENAME C::value_type >
    push( C& c )
    {
        static HBOOST_DEDUCED_TYPENAME C::value_type* p = 0;
        return make_list_inserter( assign_detail::call_push<C>( c ),
                                   p );
    }

    template< class C >
    inline list_inserter< assign_detail::call_add_edge<C> >
    add_edge( C& c )   
    {
        return make_list_inserter( assign_detail::call_add_edge<C>( c ) );
    }
    
} // namespace 'assign'
} // namespace 'boost'

#undef HBOOST_ASSIGN_PARAMS1
#undef HBOOST_ASSIGN_PARAMS2
#undef HBOOST_ASSIGN_PARAMS3
#undef HBOOST_ASSIGN_MAX_PARAMETERS

#endif
