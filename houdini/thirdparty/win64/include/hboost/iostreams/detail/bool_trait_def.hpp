// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_BOOL_TRAIT_DEF_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_BOOL_TRAIT_DEF_HPP_INCLUDED     

#include <hboost/config.hpp> // HBOOST_STATIC_CONSTANT.
#include <hboost/iostreams/detail/template_params.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/type_traits/detail/yes_no_type.hpp>
 
// 
// Macro name: HBOOST_IOSTREAMS_BOOL_TRAIT_DEF
// Description: Used to generate the traits classes is_istream, is_ostream,
//      etc.
//
#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x582))
# define HBOOST_IOSTREAMS_TRAIT_NAMESPACE(trait)
#else
# define HBOOST_IOSTREAMS_TRAIT_NAMESPACE(trait) HBOOST_PP_CAT(trait, _impl_):: 
#endif
#define HBOOST_IOSTREAMS_BOOL_TRAIT_DEF(trait, type, arity) \
    namespace HBOOST_PP_CAT(trait, _impl_) { \
      HBOOST_IOSTREAMS_TEMPLATE_PARAMS(arity, T) \
      type_traits::yes_type helper \
          (const volatile type HBOOST_IOSTREAMS_TEMPLATE_ARGS(arity, T)*); \
      type_traits::no_type helper(...); \
      template<typename T> \
      struct impl { \
           HBOOST_STATIC_CONSTANT(bool, value = \
           (sizeof(HBOOST_IOSTREAMS_TRAIT_NAMESPACE(trait) \
              helper(static_cast<T*>(0))) == \
                sizeof(type_traits::yes_type))); \
      }; \
    } \
    template<typename T> \
    struct trait \
        : mpl::bool_<HBOOST_PP_CAT(trait, _impl_)::impl<T>::value> \
    { HBOOST_MPL_AUX_LAMBDA_SUPPORT(1, trait, (T)) }; \
    /**/

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_BOOL_TRAIT_DEF_HPP_INCLUDED
