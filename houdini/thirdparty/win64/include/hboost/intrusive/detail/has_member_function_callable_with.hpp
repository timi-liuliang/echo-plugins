//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2014. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_CALLABLE_WITH_HPP
#define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_CALLABLE_WITH_HPP

//Mark that we don't support 0 arg calls due to compiler ICE in GCC 3.4/4.0/4.1 and
//wrong SFINAE for GCC 4.2/4.3
#if defined(__GNUC__) && !defined(__clang__) && ((__GNUC__*100 + __GNUC_MINOR__*10) >= 340) && ((__GNUC__*100 + __GNUC_MINOR__*10) <= 430)
   #define HBOOST_INTRUSIVE_DETAIL_HAS_MEMBER_FUNCTION_CALLABLE_WITH_0_ARGS_UNSUPPORTED
#elif defined(HBOOST_INTEL) && (HBOOST_INTEL < 1200 )
   #define HBOOST_INTRUSIVE_DETAIL_HAS_MEMBER_FUNCTION_CALLABLE_WITH_0_ARGS_UNSUPPORTED
#endif
#include <cstddef>
#include <hboost/move/utility_core.hpp>
#include <hboost/move/detail/fwd_macros.hpp>

namespace hboost_intrusive_hmfcw {

typedef char yes_type;
struct no_type{ char dummy[2]; };

#if defined(HBOOST_NO_CXX11_DECLTYPE)

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

template<class T>
struct make_dontcare
{
   typedef dont_care type;
};

#endif

struct dont_care
{
   dont_care(...);
};

struct private_type
{
   static private_type p;
   private_type const &operator,(int) const;
};

template<typename T>
no_type is_private_type(T const &);
yes_type is_private_type(private_type const &);

#endif   //#if defined(HBOOST_NO_CXX11_DECLTYPE)

#if defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T> struct remove_cv                    {  typedef T type;   };
template<typename T> struct remove_cv<const T>           {  typedef T type;   };
template<typename T> struct remove_cv<const volatile T>  {  typedef T type;   };
template<typename T> struct remove_cv<volatile T>        {  typedef T type;   };

#endif

}  //namespace hboost_intrusive_hmfcw {

#endif  //HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_CALLABLE_WITH_HPP

#ifndef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME
   #error "You MUST define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME before including this header!"
#endif

#ifndef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN
   #error "You MUST define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN before including this header!"
#endif

#ifndef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX
   #error "You MUST define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX before including this header!"
#endif

#if HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX < HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN
   #error "HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX value MUST be greater or equal than HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN!"
#endif

#if HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX == 0
   #define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_COMMA_IF
#else
   #define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_COMMA_IF ,
#endif

#ifndef  HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_BEG
   #error "HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_BEG not defined!"
#endif

#ifndef  HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_END
   #error "HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_END not defined!"
#endif

HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_BEG

#if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_CXX11_DECLTYPE)
   //With decltype and variadic templaes, things are pretty easy
   template<typename Fun, class ...Args>
   struct HBOOST_MOVE_CAT(has_member_function_callable_with_,HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)
   {
      template<class U>
      static decltype(hboost::move_detail::declval<U>().
         HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME(::hboost::move_detail::declval<Args>()...)
            , hboost_intrusive_hmfcw::yes_type()) Test(U* f);
      template<class U>
      static hboost_intrusive_hmfcw::no_type Test(...);
      static const bool value = sizeof(Test<Fun>((Fun*)0)) == sizeof(hboost_intrusive_hmfcw::yes_type);
   };

#else //defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(HBOOST_NO_CXX11_DECLTYPE)

   /////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////
   //
   //    has_member_function_callable_with_impl_XXX
   //    declaration, special case and 0 arg specializaton
   //
   /////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////

   #if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
      /////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////
      //
      //    has_member_function_callable_with_impl_XXX for 1 to N arguments
      //
      /////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////

      //defined(HBOOST_NO_CXX11_DECLTYPE) must be true
      template<class Fun, class ...DontCares>
      struct FunWrapTmpl : Fun
      {
         using Fun::HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME;
         hboost_intrusive_hmfcw::private_type HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME(DontCares...) const;
      };

      template<typename Fun, class ...Args>
      struct HBOOST_MOVE_CAT(has_member_function_callable_with_,HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<Fun, Args...>
      {
         typedef FunWrapTmpl<typename hboost_intrusive_hmfcw::make_dontcare<Args>::type...> FunWrap;

         static bool const value = (sizeof(hboost_intrusive_hmfcw::no_type) ==
                                    sizeof(hboost_intrusive_hmfcw::is_private_type
                                             ( (::hboost::move_detail::declval< FunWrap<Fun> >().
                                                   HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME(::hboost::move_detail::declval<Args>()...), 0) )
                                          )
                                    );
      };
   #else //defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

      //Preprocessor must be used to generate specializations instead of variadic templates

      template <typename Type>
      class HBOOST_MOVE_CAT(has_member_function_named_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)
      {
         struct BaseMixin
         {
            void HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME()
            {} //Some compilers require the definition or linker errors happen
         };

         struct Base
            : public hboost_intrusive_hmfcw::remove_cv<Type>::type, public BaseMixin
         {  //Declare the unneeded default constructor as some old compilers wrongly require it with is_convertible
            Base(){}
         };
         template <typename T, T t> class Helper{};

         template <typename U>
         static hboost_intrusive_hmfcw::no_type  deduce
            (U*, Helper<void (BaseMixin::*)(), &U::HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME>* = 0);
         static hboost_intrusive_hmfcw::yes_type deduce(...);

         public:
         static const bool value = sizeof(hboost_intrusive_hmfcw::yes_type) == sizeof(deduce((Base*)0));
      };

      /////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////
      //
      //    has_member_function_callable_with_impl_XXX specializations
      //
      /////////////////////////////////////////////////////////

      template<typename Fun, bool HasFunc HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_COMMA_IF HBOOST_MOVE_CAT(HBOOST_MOVE_CLASSDFLT,HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX)>
      struct HBOOST_MOVE_CAT(has_member_function_callable_with_impl_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME);

      //No HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME member specialization
      template<typename Fun HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_COMMA_IF HBOOST_MOVE_CAT(HBOOST_MOVE_CLASS,HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX)>
      struct HBOOST_MOVE_CAT(has_member_function_callable_with_impl_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)
         <Fun, false HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_COMMA_IF HBOOST_MOVE_CAT(HBOOST_MOVE_TARG,HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX)>
      {
         static const bool value = false;
      };

      #if HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN == 0
         //0 arg specialization when HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME is present
         #if !defined(HBOOST_NO_CXX11_DECLTYPE)

            template<typename Fun>
            struct HBOOST_MOVE_CAT(has_member_function_callable_with_impl_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<Fun, true>
            {
               template<class U>
               static decltype(hboost::move_detail::declval<U>().HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME()
                  , hboost_intrusive_hmfcw::yes_type()) Test(U* f);

               template<class U>
               static hboost_intrusive_hmfcw::no_type Test(...);
               static const bool value = sizeof(Test<Fun>((Fun*)0)) == sizeof(hboost_intrusive_hmfcw::yes_type);
            };

         #else //defined(HBOOST_NO_CXX11_DECLTYPE)

            #if !defined(HBOOST_INTRUSIVE_DETAIL_HAS_MEMBER_FUNCTION_CALLABLE_WITH_0_ARGS_UNSUPPORTED)

            template<class F, std::size_t N = sizeof(hboost::move_detail::declval<F>().HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME(), 0)>
            struct HBOOST_MOVE_CAT(zeroarg_checker_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)
            {  hboost_intrusive_hmfcw::yes_type dummy[N ? 1 : 2];   };

            template<typename Fun>
            struct HBOOST_MOVE_CAT(has_member_function_callable_with_impl_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<Fun, true>
            {
               template<class U> static HBOOST_MOVE_CAT(zeroarg_checker_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<U>
                  Test(HBOOST_MOVE_CAT(zeroarg_checker_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<U>*);
               template<class U> static hboost_intrusive_hmfcw::no_type Test(...);
               static const bool value = sizeof(Test< Fun >(0)) == sizeof(hboost_intrusive_hmfcw::yes_type);
            };

            #else //defined(HBOOST_INTRUSIVE_DETAIL_HAS_MEMBER_FUNCTION_CALLABLE_WITH_0_ARGS_UNSUPPORTED)

               template<typename Fun>
               struct HBOOST_MOVE_CAT(has_member_function_callable_with_impl_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<Fun, true>
               {//GCC [3.4-4.3) gives ICE when instantiating the 0 arg version so it is not supported.
                  static const bool value = true;
               };

            #endif//!defined(HBOOST_INTRUSIVE_DETAIL_HAS_MEMBER_FUNCTION_CALLABLE_WITH_0_ARGS_UNSUPPORTED)
         #endif   //!defined(HBOOST_NO_CXX11_DECLTYPE)
      #endif   //#if HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN == 0

      #if HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX > 0
         //1 to N arg specialization when HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME is present
         //Declare some unneeded default constructor as some old compilers wrongly require it with is_convertible
         #if defined(HBOOST_NO_CXX11_DECLTYPE)
            #define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_ITERATION(N)\
            \
            template<class Fun>\
            struct HBOOST_MOVE_CAT(FunWrap##N, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)\
               : Fun\
            {\
               using Fun::HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME;\
               HBOOST_MOVE_CAT(FunWrap##N, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)();\
               hboost_intrusive_hmfcw::private_type HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME\
                  (HBOOST_MOVE_REPEAT##N(hboost_intrusive_hmfcw::dont_care)) const;\
            };\
            \
            template<typename Fun, HBOOST_MOVE_CLASS##N>\
            struct HBOOST_MOVE_CAT(has_member_function_callable_with_impl_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<Fun, true, HBOOST_MOVE_TARG##N>\
            {\
               static bool const value = (sizeof(hboost_intrusive_hmfcw::no_type) == sizeof(hboost_intrusive_hmfcw::is_private_type\
                                                   ( (::hboost::move_detail::declval\
                                                         < HBOOST_MOVE_CAT(FunWrap##N, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<Fun> >().\
                                                      HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME(HBOOST_MOVE_DECLVAL##N), 0) )\
                                                )\
                                          );\
            };\
            //
         #else
            #define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_ITERATION(N)\
            template<typename Fun, HBOOST_MOVE_CLASS##N>\
            struct HBOOST_MOVE_CAT(has_member_function_callable_with_impl_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)\
               <Fun, true, HBOOST_MOVE_TARG##N>\
            {\
               template<class U>\
               static decltype(hboost::move_detail::declval<U>().\
                  HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME(HBOOST_MOVE_DECLVAL##N)\
                     , hboost_intrusive_hmfcw::yes_type()) Test(U* f);\
               template<class U>\
               static hboost_intrusive_hmfcw::no_type Test(...);\
               static const bool value = sizeof(Test<Fun>((Fun*)0)) == sizeof(hboost_intrusive_hmfcw::yes_type);\
            };\
            //
         #endif
         ////////////////////////////////////
         // Build and invoke HBOOST_MOVE_ITERATE_NTOM macrofunction, note that N has to be at least 1
         ////////////////////////////////////
         #if HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN == 0
            #define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_ITERATE_MIN 1
         #else
            #define HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_ITERATE_MIN HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN
         #endif
         HBOOST_MOVE_CAT
            (HBOOST_MOVE_CAT(HBOOST_MOVE_CAT(HBOOST_MOVE_ITERATE_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_ITERATE_MIN), TO)
            ,HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX)
               (HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_ITERATION)
         #undef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_ITERATION
         #undef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_ITERATE_MIN
         ////////////////////////////////////
         // End of HBOOST_MOVE_ITERATE_NTOM
         ////////////////////////////////////
      #endif   //HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX > 0

      /////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////
      //
      //       has_member_function_callable_with_FUNC
      //
      /////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////

      //Otherwise use the preprocessor
      template<typename Fun HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_COMMA_IF HBOOST_MOVE_CAT(HBOOST_MOVE_CLASSDFLT,HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX)>
      struct HBOOST_MOVE_CAT(has_member_function_callable_with_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)
         : public HBOOST_MOVE_CAT(has_member_function_callable_with_impl_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)
            <Fun
            , HBOOST_MOVE_CAT(has_member_function_named_, HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME)<Fun>::value
            HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_COMMA_IF HBOOST_MOVE_CAT(HBOOST_MOVE_TARG,HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX)>
      {};
   #endif   //defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#endif

HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_END

//Undef local macros
#undef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_COMMA_IF

//Undef user defined macros
#undef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME
#undef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MIN
#undef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_MAX
#undef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_BEG
#undef HBOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_END
