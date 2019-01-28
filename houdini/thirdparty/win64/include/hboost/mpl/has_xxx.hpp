
#ifndef HBOOST_MPL_HAS_XXX_HPP_INCLUDED
#define HBOOST_MPL_HAS_XXX_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2006
// Copyright David Abrahams 2002-2003
// Copyright Daniel Walker 2007
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/type_wrapper.hpp>
#include <hboost/mpl/aux_/yes_no.hpp>
#include <hboost/mpl/aux_/config/gcc.hpp>
#include <hboost/mpl/aux_/config/has_xxx.hpp>
#include <hboost/mpl/aux_/config/msvc_typename.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#include <hboost/preprocessor/array/elem.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>

#if HBOOST_WORKAROUND( __BORLANDC__, HBOOST_TESTED_AT(0x590) )
# include <hboost/type_traits/is_class.hpp>
#endif

#if !defined(HBOOST_MPL_CFG_NO_HAS_XXX)

#   if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300)

// agurt, 11/sep/02: MSVC-specific version (< 7.1), based on a USENET 
// newsgroup's posting by John Madsen (comp.lang.c++.moderated, 
// 1999-11-12 19:17:06 GMT); the code is _not_ standard-conforming, but 
// it works way more reliably than the SFINAE-based implementation

// Modified dwa 8/Oct/02 to handle reference types.

#   include <hboost/mpl/if.hpp>
#   include <hboost/mpl/bool.hpp>

namespace hboost { namespace mpl { namespace aux {

struct has_xxx_tag;

#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1300)
template< typename U > struct msvc_incomplete_array
{
    typedef char (&type)[sizeof(U) + 1];
};
#endif

template< typename T >
struct msvc_is_incomplete
{
    // MSVC is capable of some kinds of SFINAE.  If U is an incomplete
    // type, it won't pick the second overload
    static char tester(...);

#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1300)
    template< typename U >
    static typename msvc_incomplete_array<U>::type tester(type_wrapper<U>);
#else
    template< typename U >
    static char (& tester(type_wrapper<U>) )[sizeof(U)+1];
#endif 
    
    HBOOST_STATIC_CONSTANT(bool, value = 
          sizeof(tester(type_wrapper<T>())) == 1
        );
};

template<>
struct msvc_is_incomplete<int>
{
    HBOOST_STATIC_CONSTANT(bool, value = false);
};

}}}

#   define HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF_(trait, name, default_) \
template< typename T, typename name = ::hboost::mpl::aux::has_xxx_tag > \
struct HBOOST_PP_CAT(trait,_impl) : T \
{ \
    static hboost::mpl::aux::no_tag \
    test(void(*)(::hboost::mpl::aux::has_xxx_tag)); \
    \
    static hboost::mpl::aux::yes_tag test(...); \
    \
    HBOOST_STATIC_CONSTANT(bool, value = \
          sizeof(test(static_cast<void(*)(name)>(0))) \
            != sizeof(hboost::mpl::aux::no_tag) \
        ); \
    typedef hboost::mpl::bool_<value> type; \
}; \
\
template< typename T, typename fallback_ = hboost::mpl::bool_<default_> > \
struct trait \
    : hboost::mpl::if_c< \
          hboost::mpl::aux::msvc_is_incomplete<T>::value \
        , hboost::mpl::bool_<false> \
        , HBOOST_PP_CAT(trait,_impl)<T> \
        >::type \
{ \
}; \
\
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, void) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, bool) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, char) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, signed char) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, unsigned char) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, signed short) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, unsigned short) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, signed int) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, unsigned int) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, signed long) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, unsigned long) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, float) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, double) \
HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, long double) \
/**/

#   define HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, T) \
template<> struct trait<T> \
{ \
    HBOOST_STATIC_CONSTANT(bool, value = false); \
    typedef hboost::mpl::bool_<false> type; \
}; \
/**/

#if !defined(HBOOST_NO_INTRINSIC_WCHAR_T)
#   define HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, unused) \
    HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF_(trait, name, unused) \
    HBOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, wchar_t) \
/**/
#else
#   define HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, unused) \
    HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF_(trait, name, unused) \
/**/
#endif


// SFINAE-based implementations below are derived from a USENET newsgroup's 
// posting by Rani Sharoni (comp.lang.c++.moderated, 2002-03-17 07:45:09 PST)

#   elif HBOOST_WORKAROUND(HBOOST_MSVC, <= 1400) \
      || (HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1800)) && defined(__CUDACC__)) \
      || HBOOST_WORKAROUND(__IBMCPP__, <= 700)

// MSVC 7.1 & MSVC 8.0 & VACPP

// agurt, 15/jun/05: replace overload-based SFINAE implementation with SFINAE
// applied to partial specialization to fix some apparently random failures 
// (thanks to Daniel Wallin for researching this!)

#   define HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, default_) \
template< typename T > \
struct HBOOST_PP_CAT(trait, _msvc_sfinae_helper) \
{ \
    typedef void type; \
};\
\
template< typename T, typename U = void > \
struct HBOOST_PP_CAT(trait,_impl_) \
{ \
    HBOOST_STATIC_CONSTANT(bool, value = false); \
    typedef hboost::mpl::bool_<value> type; \
}; \
\
template< typename T > \
struct HBOOST_PP_CAT(trait,_impl_)< \
      T \
    , typename HBOOST_PP_CAT(trait, _msvc_sfinae_helper)< typename T::name >::type \
    > \
{ \
    HBOOST_STATIC_CONSTANT(bool, value = true); \
    typedef hboost::mpl::bool_<value> type; \
}; \
\
template< typename T, typename fallback_ = hboost::mpl::bool_<default_> > \
struct trait \
    : HBOOST_PP_CAT(trait,_impl_)<T> \
{ \
}; \
/**/

#   elif HBOOST_WORKAROUND( __BORLANDC__, HBOOST_TESTED_AT(0x590) )

#   define HBOOST_MPL_HAS_XXX_TRAIT_NAMED_BCB_DEF(trait, trait_tester, name, default_) \
template< typename T, bool IS_CLASS > \
struct trait_tester \
{ \
    HBOOST_STATIC_CONSTANT( bool,  value = false ); \
}; \
template< typename T > \
struct trait_tester< T, true > \
{ \
    struct trait_tester_impl \
    { \
        template < class U > \
        static int  resolve( hboost::mpl::aux::type_wrapper<U> const volatile * \
                           , hboost::mpl::aux::type_wrapper<typename U::name >* = 0 ); \
        static char resolve( ... ); \
    }; \
    typedef hboost::mpl::aux::type_wrapper<T> t_; \
    HBOOST_STATIC_CONSTANT( bool, value = ( sizeof( trait_tester_impl::resolve( static_cast< t_ * >(0) ) ) == sizeof(int) ) ); \
}; \
template< typename T, typename fallback_ = hboost::mpl::bool_<default_> > \
struct trait           \
{                      \
    HBOOST_STATIC_CONSTANT( bool, value = (trait_tester< T, hboost::is_class< T >::value >::value) );     \
    typedef hboost::mpl::bool_< trait< T, fallback_ >::value > type; \
};

#   define HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, default_) \
    HBOOST_MPL_HAS_XXX_TRAIT_NAMED_BCB_DEF( trait \
                                         , HBOOST_PP_CAT(trait,_tester)      \
                                         , name       \
                                         , default_ ) \
/**/

#   else // other SFINAE-capable compilers

#   define HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, default_) \
template< typename T, typename fallback_ = hboost::mpl::bool_<default_> > \
struct trait \
{ \
    struct gcc_3_2_wknd \
    { \
        template< typename U > \
        static hboost::mpl::aux::yes_tag test( \
              hboost::mpl::aux::type_wrapper<U> const volatile* \
            , hboost::mpl::aux::type_wrapper<HBOOST_MSVC_TYPENAME U::name>* = 0 \
            ); \
    \
        static hboost::mpl::aux::no_tag test(...); \
    }; \
    \
    typedef hboost::mpl::aux::type_wrapper<T> t_; \
    HBOOST_STATIC_CONSTANT(bool, value = \
          sizeof(gcc_3_2_wknd::test(static_cast<t_*>(0))) \
            == sizeof(hboost::mpl::aux::yes_tag) \
        ); \
    typedef hboost::mpl::bool_<value> type; \
}; \
/**/

#   endif // HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300)


#else // HBOOST_MPL_CFG_NO_HAS_XXX

// placeholder implementation

#   define HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, default_) \
template< typename T, typename fallback_ = hboost::mpl::bool_<default_> > \
struct trait \
{ \
    HBOOST_STATIC_CONSTANT(bool, value = fallback_::value); \
    typedef fallback_ type; \
}; \
/**/

#endif

#define HBOOST_MPL_HAS_XXX_TRAIT_DEF(name) \
    HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(HBOOST_PP_CAT(has_,name), name, false) \
/**/


#if !defined(HBOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE)

// Create a boolean Metafunction to detect a nested template
// member. This implementation is based on a USENET newsgroup's
// posting by Aleksey Gurtovoy (comp.lang.c++.moderated, 2002-03-19),
// Rani Sharoni's USENET posting cited above, the non-template has_xxx
// implementations above, and discussion on the Boost mailing list.

#   if !defined(HBOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES)
#     if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1400)
#       define HBOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES 1
#     else
#       define HBOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES 0
#     endif
#   endif

#   if !defined(HBOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION)
#     if (defined(HBOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS))
#       define HBOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION 1
#     else
#       define HBOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION 0
#     endif
#   endif

#   if !defined(HBOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE)
#     if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1400)
#       define HBOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE 1
#     else
#       define HBOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE 0
#     endif
#   endif

// NOTE: Many internal implementation macros take a Boost.Preprocessor
// array argument called args which is of the following form.
//           ( 4, ( trait, name, max_arity, default_ ) )

#   define HBOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args) \
      HBOOST_PP_CAT(HBOOST_PP_ARRAY_ELEM(0, args) , _introspect) \
    /**/

#   define HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) \
      HBOOST_PP_CAT(HBOOST_PP_CAT(HBOOST_PP_ARRAY_ELEM(0, args) , _substitute), n) \
    /**/

#   define HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args) \
      HBOOST_PP_CAT(HBOOST_PP_ARRAY_ELEM(0, args) , _test) \
    /**/

// Thanks to Guillaume Melquiond for pointing out the need for the
// "substitute" template as an argument to the overloaded test
// functions to get SFINAE to work for member templates with the
// correct name but different number of arguments.
#   define HBOOST_MPL_HAS_MEMBER_MULTI_SUBSTITUTE(z, n, args) \
      template< \
          template< HBOOST_PP_ENUM_PARAMS(HBOOST_PP_INC(n), typename V) > class V \
       > \
      struct HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) { \
      }; \
    /**/

#   define HBOOST_MPL_HAS_MEMBER_SUBSTITUTE(args, substitute_macro) \
      HBOOST_PP_REPEAT( \
          HBOOST_PP_ARRAY_ELEM(2, args) \
        , HBOOST_MPL_HAS_MEMBER_MULTI_SUBSTITUTE \
        , args \
      ) \
    /**/

#   if !HBOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION
#     define HBOOST_MPL_HAS_MEMBER_REJECT(args, member_macro) \
        template< typename V > \
        static hboost::mpl::aux::no_tag \
        HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)(...); \
      /**/
#   else
#     define HBOOST_MPL_HAS_MEMBER_REJECT(args, member_macro) \
        static hboost::mpl::aux::no_tag \
        HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)(...); \
      /**/
#   endif

#   if !HBOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES
#     define HBOOST_MPL_HAS_MEMBER_MULTI_ACCEPT(z, n, args) \
        template< typename V > \
        static hboost::mpl::aux::yes_tag \
        HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)( \
            hboost::mpl::aux::type_wrapper< V > const volatile* \
          , HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) < \
                V::template HBOOST_PP_ARRAY_ELEM(1, args) \
            >* = 0 \
        ); \
      /**/
#     define HBOOST_MPL_HAS_MEMBER_ACCEPT(args, member_macro) \
        HBOOST_PP_REPEAT( \
            HBOOST_PP_ARRAY_ELEM(2, args) \
          , HBOOST_MPL_HAS_MEMBER_MULTI_ACCEPT \
          , args \
        ) \
      /**/
#   else
#     define HBOOST_MPL_HAS_MEMBER_ACCEPT(args, member_macro) \
        template< typename V > \
        static hboost::mpl::aux::yes_tag \
        HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)( \
            V const volatile* \
          , member_macro(args, V, T)* = 0 \
        ); \
      /**/
#   endif

#   if !HBOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION
#     define HBOOST_MPL_HAS_MEMBER_TEST(args) \
          sizeof(HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)< U >(0)) \
              == sizeof(hboost::mpl::aux::yes_tag) \
      /**/
#   else
#     if !HBOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES
#       define HBOOST_MPL_HAS_MEMBER_TEST(args) \
          sizeof( \
              HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)( \
                  static_cast< hboost::mpl::aux::type_wrapper< U >* >(0) \
              ) \
          ) == sizeof(hboost::mpl::aux::yes_tag) \
        /**/
#     else
#       define HBOOST_MPL_HAS_MEMBER_TEST(args) \
          sizeof( \
              HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)( \
                  static_cast< U* >(0) \
              ) \
          ) == sizeof(hboost::mpl::aux::yes_tag) \
        /**/
#     endif
#   endif

#   define HBOOST_MPL_HAS_MEMBER_INTROSPECT( \
               args, substitute_macro, member_macro \
           ) \
      template< typename U > \
      struct HBOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args) { \
          HBOOST_MPL_HAS_MEMBER_SUBSTITUTE(args, substitute_macro) \
          HBOOST_MPL_HAS_MEMBER_REJECT(args, member_macro) \
          HBOOST_MPL_HAS_MEMBER_ACCEPT(args, member_macro) \
          HBOOST_STATIC_CONSTANT( \
              bool, value = HBOOST_MPL_HAS_MEMBER_TEST(args) \
          ); \
          typedef hboost::mpl::bool_< value > type; \
      }; \
    /**/

#   define HBOOST_MPL_HAS_MEMBER_IMPLEMENTATION( \
               args, introspect_macro, substitute_macro, member_macro \
           ) \
      template< \
          typename T \
        , typename fallback_ \
              = hboost::mpl::bool_< HBOOST_PP_ARRAY_ELEM(3, args) > \
      > \
      class HBOOST_PP_ARRAY_ELEM(0, args) { \
          introspect_macro(args, substitute_macro, member_macro) \
      public: \
          static const bool value \
              = HBOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args)< T >::value; \
          typedef typename HBOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args)< \
              T \
          >::type type; \
      }; \
    /**/

// HBOOST_MPL_HAS_MEMBER_WITH_FUNCTION_SFINAE expands to the full
// implementation of the function-based metafunction. Compile with -E
// to see the preprocessor output for this macro.
#   define HBOOST_MPL_HAS_MEMBER_WITH_FUNCTION_SFINAE( \
               args, substitute_macro, member_macro \
           ) \
      HBOOST_MPL_HAS_MEMBER_IMPLEMENTATION( \
          args \
        , HBOOST_MPL_HAS_MEMBER_INTROSPECT \
        , substitute_macro \
        , member_macro \
      ) \
    /**/

#   if HBOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE

#     if !defined(HBOOST_MPL_HAS_XXX_NEEDS_NAMESPACE_LEVEL_SUBSTITUTE)
#       if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1400)
#         define HBOOST_MPL_HAS_XXX_NEEDS_NAMESPACE_LEVEL_SUBSTITUTE 1
#       endif
#     endif

#     if !HBOOST_MPL_HAS_XXX_NEEDS_NAMESPACE_LEVEL_SUBSTITUTE
#       define HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                   args, n \
               ) \
          HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) \
        /**/
#     else
#       define HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                   args, n \
               ) \
          HBOOST_PP_CAT( \
              hboost_mpl_has_xxx_ \
            , HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) \
          ) \
        /**/
#     endif

#     define HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_TAG_NAME( \
                 args \
             ) \
        HBOOST_PP_CAT( \
            HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                args, 0 \
            ) \
          , _tag \
        ) \
      /**/

#     define HBOOST_MPL_HAS_MEMBER_MULTI_SUBSTITUTE_WITH_TEMPLATE_SFINAE( \
                 z, n, args \
             ) \
        template< \
             template< HBOOST_PP_ENUM_PARAMS(HBOOST_PP_INC(n), typename U) > class U \
        > \
        struct HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                args, n \
               ) { \
            typedef \
                HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_TAG_NAME(args) \
                type; \
        }; \
      /**/

#     define HBOOST_MPL_HAS_MEMBER_SUBSTITUTE_WITH_TEMPLATE_SFINAE( \
                 args, substitute_macro \
             ) \
        typedef void \
            HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_TAG_NAME(args); \
        HBOOST_PP_REPEAT( \
            HBOOST_PP_ARRAY_ELEM(2, args) \
          , HBOOST_MPL_HAS_MEMBER_MULTI_SUBSTITUTE_WITH_TEMPLATE_SFINAE \
          , args \
        ) \
      /**/

#     define HBOOST_MPL_HAS_MEMBER_REJECT_WITH_TEMPLATE_SFINAE( \
                 args, member_macro \
             ) \
        template< \
            typename U \
          , typename V \
                = HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_TAG_NAME(args) \
        > \
        struct HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args) { \
            HBOOST_STATIC_CONSTANT(bool, value = false); \
            typedef hboost::mpl::bool_< value > type; \
        }; \
      /**/

#     define HBOOST_MPL_HAS_MEMBER_MULTI_ACCEPT_WITH_TEMPLATE_SFINAE( \
                 z, n, args \
             ) \
        template< typename U > \
        struct HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)< \
            U \
          , typename \
                HBOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                    args, n \
                )< \
                    HBOOST_MSVC_TYPENAME U::HBOOST_PP_ARRAY_ELEM(1, args)< > \
                >::type \
        > { \
            HBOOST_STATIC_CONSTANT(bool, value = true); \
            typedef hboost::mpl::bool_< value > type; \
        }; \
      /**/

#     define HBOOST_MPL_HAS_MEMBER_ACCEPT_WITH_TEMPLATE_SFINAE( \
                 args, member_macro \
             ) \
        HBOOST_PP_REPEAT( \
            HBOOST_PP_ARRAY_ELEM(2, args) \
          , HBOOST_MPL_HAS_MEMBER_MULTI_ACCEPT_WITH_TEMPLATE_SFINAE \
          , args \
        ) \
      /**/

#     define HBOOST_MPL_HAS_MEMBER_INTROSPECT_WITH_TEMPLATE_SFINAE( \
                 args, substitute_macro, member_macro \
             ) \
        HBOOST_MPL_HAS_MEMBER_REJECT_WITH_TEMPLATE_SFINAE(args, member_macro) \
        HBOOST_MPL_HAS_MEMBER_ACCEPT_WITH_TEMPLATE_SFINAE(args, member_macro) \
        template< typename U > \
        struct HBOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args) \
            : HBOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)< U > { \
        }; \
      /**/
 
// HBOOST_MPL_HAS_MEMBER_WITH_TEMPLATE_SFINAE expands to the full
// implementation of the template-based metafunction. Compile with -E
// to see the preprocessor output for this macro.
//
// Note that if HBOOST_MPL_HAS_XXX_NEEDS_NAMESPACE_LEVEL_SUBSTITUTE is
// defined HBOOST_MPL_HAS_MEMBER_SUBSTITUTE_WITH_TEMPLATE_SFINAE needs
// to be expanded at namespace level before
// HBOOST_MPL_HAS_MEMBER_WITH_TEMPLATE_SFINAE can be used.
#     define HBOOST_MPL_HAS_MEMBER_WITH_TEMPLATE_SFINAE( \
                 args, substitute_macro, member_macro \
             ) \
        HBOOST_MPL_HAS_MEMBER_SUBSTITUTE_WITH_TEMPLATE_SFINAE( \
            args, substitute_macro \
        ) \
        HBOOST_MPL_HAS_MEMBER_IMPLEMENTATION( \
            args \
          , HBOOST_MPL_HAS_MEMBER_INTROSPECT_WITH_TEMPLATE_SFINAE \
          , substitute_macro \
          , member_macro \
        ) \
      /**/

#   endif // HBOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE

// Note: In the current implementation the parameter and access macros
// are no longer expanded.
#   if !HBOOST_WORKAROUND(HBOOST_MSVC, <= 1400)
#     define HBOOST_MPL_HAS_XXX_TEMPLATE_NAMED_DEF(trait, name, default_) \
        HBOOST_MPL_HAS_MEMBER_WITH_FUNCTION_SFINAE( \
            ( 4, ( trait, name, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, default_ ) ) \
          , HBOOST_MPL_HAS_MEMBER_TEMPLATE_SUBSTITUTE_PARAMETER \
          , HBOOST_MPL_HAS_MEMBER_TEMPLATE_ACCESS \
        ) \
      /**/
#   else
#     define HBOOST_MPL_HAS_XXX_TEMPLATE_NAMED_DEF(trait, name, default_) \
        HBOOST_MPL_HAS_MEMBER_WITH_TEMPLATE_SFINAE( \
            ( 4, ( trait, name, HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, default_ ) ) \
          , HBOOST_MPL_HAS_MEMBER_TEMPLATE_SUBSTITUTE_PARAMETER \
          , HBOOST_MPL_HAS_MEMBER_TEMPLATE_ACCESS \
        ) \
      /**/
#   endif

#else // HBOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE

// placeholder implementation

#   define HBOOST_MPL_HAS_XXX_TEMPLATE_NAMED_DEF(trait, name, default_) \
      template< typename T \
              , typename fallback_ = hboost::mpl::bool_< default_ > > \
      struct trait { \
          HBOOST_STATIC_CONSTANT(bool, value = fallback_::value); \
          typedef fallback_ type; \
      }; \
    /**/

#endif // HBOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE

#   define HBOOST_MPL_HAS_XXX_TEMPLATE_DEF(name) \
      HBOOST_MPL_HAS_XXX_TEMPLATE_NAMED_DEF( \
          HBOOST_PP_CAT(has_, name), name, false \
      ) \
    /**/

#endif // HBOOST_MPL_HAS_XXX_HPP_INCLUDED
