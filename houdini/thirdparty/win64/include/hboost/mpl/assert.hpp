
#ifndef HBOOST_MPL_ASSERT_HPP_INCLUDED
#define HBOOST_MPL_ASSERT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2006
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/not.hpp>
#include <hboost/mpl/aux_/value_wknd.hpp>
#include <hboost/mpl/aux_/nested_type_wknd.hpp>
#include <hboost/mpl/aux_/yes_no.hpp>
#include <hboost/mpl/aux_/na.hpp>
#include <hboost/mpl/aux_/adl_barrier.hpp>

#include <hboost/mpl/aux_/config/nttp.hpp>
#include <hboost/mpl/aux_/config/dtp.hpp>
#include <hboost/mpl/aux_/config/gcc.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/gpu.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>
#include <hboost/mpl/aux_/config/pp_counter.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#include <hboost/preprocessor/cat.hpp>

#include <hboost/config.hpp> // make sure 'size_t' is placed into 'std'
#include <cstddef>

#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1700)
#include <hboost/mpl/if.hpp>
#endif

#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) \
    || (HBOOST_MPL_CFG_GCC != 0) \
    || HBOOST_WORKAROUND(__IBMCPP__, <= 600)
#   define HBOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES
#endif

#if HBOOST_WORKAROUND(__MWERKS__, < 0x3202) \
    || HBOOST_WORKAROUND(__EDG_VERSION__, <= 238) \
    || HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) \
    || HBOOST_WORKAROUND(__DMC__, HBOOST_TESTED_AT(0x840))
#   define HBOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER
#endif

// agurt, 10/nov/06: use enums for Borland (which cannot cope with static constants) 
// and GCC (which issues "unused variable" warnings when static constants are used 
// at a function scope)
#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610)) \
    || (HBOOST_MPL_CFG_GCC != 0) || (HBOOST_MPL_CFG_GPU != 0)
#   define HBOOST_MPL_AUX_ASSERT_CONSTANT(T, expr) enum { expr }
#else
#   define HBOOST_MPL_AUX_ASSERT_CONSTANT(T, expr) HBOOST_STATIC_CONSTANT(T, expr)
#endif


HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

struct failed {};

// agurt, 24/aug/04: MSVC 7.1 workaround here and below: return/accept 
// 'assert<false>' by reference; can't apply it unconditionally -- apparently it
// degrades the quality of GCC diagnostics
#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1310)
#   define AUX778076_ASSERT_ARG(x) x&
#else
#   define AUX778076_ASSERT_ARG(x) x
#endif

template< bool C >  struct assert        { typedef void* type; };
template<>          struct assert<false> { typedef AUX778076_ASSERT_ARG(assert) type; };

template< bool C >
int assertion_failed( typename assert<C>::type );

template< bool C >
struct assertion
{
    static int failed( assert<false> );
};

template<>
struct assertion<true>
{
    static int failed( void* );
};

struct assert_
{
#if !defined(HBOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
    template< typename T1, typename T2 = na, typename T3 = na, typename T4 = na > struct types {};
#endif
    static assert_ const arg;
    enum relations { equal = 1, not_equal, greater, greater_equal, less, less_equal };
};


#if !defined(HBOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)

bool operator==( failed, failed );
bool operator!=( failed, failed );
bool operator>( failed, failed );
bool operator>=( failed, failed );
bool operator<( failed, failed );
bool operator<=( failed, failed );

#if defined(__EDG_VERSION__)
template< bool (*)(failed, failed), long x, long y > struct assert_relation {};
#   define HBOOST_MPL_AUX_ASSERT_RELATION(x, y, r) assert_relation<r,x,y>
#else
template< HBOOST_MPL_AUX_NTTP_DECL(long, x), HBOOST_MPL_AUX_NTTP_DECL(long, y), bool (*)(failed, failed) > 
struct assert_relation {};
#   define HBOOST_MPL_AUX_ASSERT_RELATION(x, y, r) assert_relation<x,y,r>
#endif

#else // HBOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES

hboost::mpl::aux::weighted_tag<1>::type operator==( assert_, assert_ );
hboost::mpl::aux::weighted_tag<2>::type operator!=( assert_, assert_ );
hboost::mpl::aux::weighted_tag<3>::type operator>(  assert_, assert_ );
hboost::mpl::aux::weighted_tag<4>::type operator>=( assert_, assert_ );
hboost::mpl::aux::weighted_tag<5>::type operator<( assert_, assert_ );
hboost::mpl::aux::weighted_tag<6>::type operator<=( assert_, assert_ );

template< assert_::relations r, long x, long y > struct assert_relation {};

#endif 

#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1700)

template<class Pred>
struct extract_assert_pred;

template<class Pred>
struct extract_assert_pred<void(Pred)> { typedef Pred type; };

template<class Pred>
struct eval_assert {
    typedef typename extract_assert_pred<Pred>::type P;
    typedef typename P::type p_type;
    typedef typename ::hboost::mpl::if_c<p_type::value,
        AUX778076_ASSERT_ARG(assert<false>),
        failed ************ P::************
    >::type type;
};

template<class Pred>
struct eval_assert_not {
    typedef typename extract_assert_pred<Pred>::type P;
    typedef typename P::type p_type;
    typedef typename ::hboost::mpl::if_c<!p_type::value,
        AUX778076_ASSERT_ARG(assert<false>),
        failed ************ ::hboost::mpl::not_<P>::************
    >::type type;
};

template< typename T >
T make_assert_arg();

#elif !defined(HBOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)

template< bool > struct assert_arg_pred_impl { typedef int type; };
template<> struct assert_arg_pred_impl<true> { typedef void* type; };

template< typename P > struct assert_arg_pred
{
    typedef typename P::type p_type;
    typedef typename assert_arg_pred_impl< p_type::value >::type type;
};

template< typename P > struct assert_arg_pred_not
{
    typedef typename P::type p_type;
    HBOOST_MPL_AUX_ASSERT_CONSTANT( bool, p = !p_type::value );
    typedef typename assert_arg_pred_impl<p>::type type;
};

template< typename Pred >
failed ************ (Pred::************ 
      assert_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type )
    );

template< typename Pred >
failed ************ (hboost::mpl::not_<Pred>::************ 
      assert_not_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type )
    );

template< typename Pred >
AUX778076_ASSERT_ARG(assert<false>)
assert_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type );

template< typename Pred >
AUX778076_ASSERT_ARG(assert<false>)
assert_not_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type );


#else // HBOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER
        
template< bool c, typename Pred > struct assert_arg_type_impl
{
    typedef failed      ************ Pred::* mwcw83_wknd;
    typedef mwcw83_wknd ************* type;
};

template< typename Pred > struct assert_arg_type_impl<true,Pred>
{
    typedef AUX778076_ASSERT_ARG(assert<false>) type;
};

template< typename Pred > struct assert_arg_type
    : assert_arg_type_impl< HBOOST_MPL_AUX_VALUE_WKND(HBOOST_MPL_AUX_NESTED_TYPE_WKND(Pred))::value, Pred >
{
};

template< typename Pred >
typename assert_arg_type<Pred>::type 
assert_arg(void (*)(Pred), int);

template< typename Pred >
typename assert_arg_type< hboost::mpl::not_<Pred> >::type 
assert_not_arg(void (*)(Pred), int);

#   if !defined(HBOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)
template< long x, long y, bool (*r)(failed, failed) >
typename assert_arg_type_impl< false,HBOOST_MPL_AUX_ASSERT_RELATION(x,y,r) >::type
assert_rel_arg( HBOOST_MPL_AUX_ASSERT_RELATION(x,y,r) );
#   else
template< assert_::relations r, long x, long y >
typename assert_arg_type_impl< false,assert_relation<r,x,y> >::type
assert_rel_arg( assert_relation<r,x,y> );
#   endif

#endif // HBOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER

#undef AUX778076_ASSERT_ARG

HBOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1700)

// HBOOST_MPL_ASSERT((pred<x,...>))

#define HBOOST_MPL_ASSERT(pred) \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,HBOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          hboost::mpl::assertion_failed<false>( \
              hboost::mpl::make_assert_arg< \
                  typename hboost::mpl::eval_assert<void pred>::type \
                >() \
            ) \
        ) \
    ) \
/**/

// HBOOST_MPL_ASSERT_NOT((pred<x,...>))

#define HBOOST_MPL_ASSERT_NOT(pred) \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,HBOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          hboost::mpl::assertion_failed<false>( \
              hboost::mpl::make_assert_arg< \
                  typename hboost::mpl::eval_assert_not<void pred>::type \
                >() \
            ) \
        ) \
    ) \
/**/

#else

// HBOOST_MPL_ASSERT((pred<x,...>))

#define HBOOST_MPL_ASSERT(pred) \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,HBOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          hboost::mpl::assertion_failed<false>( \
              hboost::mpl::assert_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
    ) \
/**/

// HBOOST_MPL_ASSERT_NOT((pred<x,...>))

#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300)
#   define HBOOST_MPL_ASSERT_NOT(pred) \
enum { \
      HBOOST_PP_CAT(mpl_assertion_in_line_,HBOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          hboost::mpl::assertion<false>::failed( \
              hboost::mpl::assert_not_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
}\
/**/
#else
#   define HBOOST_MPL_ASSERT_NOT(pred) \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,HBOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
          hboost::mpl::assertion_failed<false>( \
              hboost::mpl::assert_not_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
   ) \
/**/
#endif

#endif

// HBOOST_MPL_ASSERT_RELATION(x, ==|!=|<=|<|>=|>, y)

#if defined(HBOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)

#   if !defined(HBOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)
// agurt, 9/nov/06: 'enum' below is a workaround for gcc 4.0.4/4.1.1 bugs #29522 and #29518
#   define HBOOST_MPL_ASSERT_RELATION_IMPL(counter, x, rel, y)      \
enum { HBOOST_PP_CAT(mpl_assert_rel_value,counter) = (x rel y) }; \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,counter) = sizeof( \
        hboost::mpl::assertion_failed<HBOOST_PP_CAT(mpl_assert_rel_value,counter)>( \
            (hboost::mpl::failed ************ ( hboost::mpl::assert_relation< \
                  hboost::mpl::assert_::relations( sizeof( \
                      hboost::mpl::assert_::arg rel hboost::mpl::assert_::arg \
                    ) ) \
                , x \
                , y \
                >::************)) 0 ) \
        ) \
    ) \
/**/
#   else
#   define HBOOST_MPL_ASSERT_RELATION_IMPL(counter, x, rel, y)    \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assert_rel,counter) = sizeof( \
          hboost::mpl::assert_::arg rel hboost::mpl::assert_::arg \
        ) \
    ); \
HBOOST_MPL_AUX_ASSERT_CONSTANT( bool, HBOOST_PP_CAT(mpl_assert_rel_value,counter) = (x rel y) ); \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,counter) = sizeof( \
        hboost::mpl::assertion_failed<HBOOST_PP_CAT(mpl_assert_rel_value,counter)>( \
              hboost::mpl::assert_rel_arg( hboost::mpl::assert_relation< \
                  hboost::mpl::assert_::relations(HBOOST_PP_CAT(mpl_assert_rel,counter)) \
                , x \
                , y \
                >() ) \
            ) \
        ) \
    ) \
/**/
#   endif

#   define HBOOST_MPL_ASSERT_RELATION(x, rel, y) \
HBOOST_MPL_ASSERT_RELATION_IMPL(HBOOST_MPL_AUX_PP_COUNTER(), x, rel, y) \
/**/

#else // !HBOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES

#   if defined(HBOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)
#   define HBOOST_MPL_ASSERT_RELATION(x, rel, y) \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,HBOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
        hboost::mpl::assertion_failed<(x rel y)>( hboost::mpl::assert_rel_arg( \
              hboost::mpl::HBOOST_MPL_AUX_ASSERT_RELATION(x,y,(&hboost::mpl::operator rel))() \
            ) ) \
        ) \
    ) \
/**/
#   else
#   define HBOOST_MPL_ASSERT_RELATION(x, rel, y) \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,HBOOST_MPL_AUX_PP_COUNTER()) = sizeof( \
        hboost::mpl::assertion_failed<(x rel y)>( (hboost::mpl::failed ************ ( \
            hboost::mpl::HBOOST_MPL_AUX_ASSERT_RELATION(x,y,(&hboost::mpl::operator rel))::************))0 ) \
        ) \
    ) \
/**/
#   endif

#endif


// HBOOST_MPL_ASSERT_MSG( (pred<x,...>::value), USER_PROVIDED_MESSAGE, (types<x,...>) ) 

#if HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3202))
#   define HBOOST_MPL_ASSERT_MSG_IMPL( counter, c, msg, types_ ) \
struct msg; \
typedef struct HBOOST_PP_CAT(msg,counter) : hboost::mpl::assert_ \
{ \
    using hboost::mpl::assert_::types; \
    static hboost::mpl::failed ************ (msg::************ assert_arg()) types_ \
    { return 0; } \
} HBOOST_PP_CAT(mpl_assert_arg,counter); \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,counter) = sizeof( \
        hboost::mpl::assertion<(c)>::failed( HBOOST_PP_CAT(mpl_assert_arg,counter)::assert_arg() ) \
        ) \
    ) \
/**/
#else
#   define HBOOST_MPL_ASSERT_MSG_IMPL( counter, c, msg, types_ )  \
struct msg; \
typedef struct HBOOST_PP_CAT(msg,counter) : hboost::mpl::assert_ \
{ \
    static hboost::mpl::failed ************ (msg::************ assert_arg()) types_ \
    { return 0; } \
} HBOOST_PP_CAT(mpl_assert_arg,counter); \
HBOOST_MPL_AUX_ASSERT_CONSTANT( \
      std::size_t \
    , HBOOST_PP_CAT(mpl_assertion_in_line_,counter) = sizeof( \
        hboost::mpl::assertion_failed<(c)>( HBOOST_PP_CAT(mpl_assert_arg,counter)::assert_arg() ) \
        ) \
    ) \
/**/
#endif

#define HBOOST_MPL_ASSERT_MSG( c, msg, types_ ) \
HBOOST_MPL_ASSERT_MSG_IMPL( HBOOST_MPL_AUX_PP_COUNTER(), c, msg, types_ ) \
/**/

#endif // HBOOST_MPL_ASSERT_HPP_INCLUDED
