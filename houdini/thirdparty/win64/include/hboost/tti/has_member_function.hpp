
//  (C) Copyright Edward Diener 2011,2012,2013
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_TTI_HAS_MEMBER_FUNCTION_HPP)
#define HBOOST_TTI_HAS_MEMBER_FUNCTION_HPP

#include <hboost/config.hpp>
#include <hboost/function_types/property_tags.hpp>
#include <hboost/mpl/vector.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/tti/detail/ddeftype.hpp>
#include <hboost/tti/detail/dmem_fun.hpp>
#include <hboost/tti/gen/has_member_function_gen.hpp>
#include <hboost/tti/gen/namespace_gen.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

/// Expands to a metafunction which tests whether a member function with a particular name and signature exists.
/**

    trait = the name of the metafunction within the tti namespace.
    
    name  = the name of the inner member.

    generates a metafunction called "trait" where 'trait' is the macro parameter.<br />
    
              template<class HBOOST_TTI_TP_T,class HBOOST_TTI_R,class HBOOST_TTI_FS,class HBOOST_TTI_TAG>
              struct trait
                {
                static const value = unspecified;
                typedef mpl::bool_<true-or-false> type;
                };

              The metafunction types and return:
    
                HBOOST_TTI_TP_T   = the enclosing type in which to look for our 'name'
                                            OR
                          a pointer to member function as a single type.
                
                HBOOST_TTI_TP_R   = (optional) the return type of the member function
                          if the first parameter is the enclosing type.
                
                HBOOST_TTI_TP_FS  = (optional) the parameters of the member function as a hboost::mpl forward sequence
                          if the first parameter is the enclosing type and the member function parameters
                          are not empty.
                
                HBOOST_TTI_TP_TAG = (optional) a hboost::function_types tag to apply to the member function
                          if the first parameter is the enclosing type and a tag is needed.
                
                returns = 'value' is true if the 'name' exists, 
                          with the appropriate member function type,
                          otherwise 'value' is false.
                          
*/
#define HBOOST_TTI_TRAIT_HAS_MEMBER_FUNCTION(trait,name) \
  HBOOST_TTI_DETAIL_TRAIT_HAS_MEMBER_FUNCTION(trait,name) \
  template<class HBOOST_TTI_TP_T,class HBOOST_TTI_TP_R = HBOOST_TTI_NAMESPACE::detail::deftype,class HBOOST_TTI_TP_FS = hboost::mpl::vector<>,class HBOOST_TTI_TP_TAG = hboost::function_types::null_tag> \
  struct trait \
    { \
    typedef typename \
    HBOOST_PP_CAT(trait,_detail_hmf)<HBOOST_TTI_TP_T,HBOOST_TTI_TP_R,HBOOST_TTI_TP_FS,HBOOST_TTI_TP_TAG>::type type; \
    HBOOST_STATIC_CONSTANT(bool,value=type::value); \
    }; \
/**/

/// Expands to a metafunction which tests whether a member function with a particular name and signature exists.
/**

    name  = the name of the inner member.

    generates a metafunction called "has_member_function_name" where 'name' is the macro parameter.
    
              template<class HBOOST_TTI_TP_T,class HBOOST_TTI_TP_R,class HBOOST_TTI_TP_FS,class HBOOST_TTI_TP_TAG>
              struct has_member_function_name
                {
                static const value = unspecified;
                typedef mpl::bool_<true-or-false> type;
                };

              The metafunction types and return:
    
                HBOOST_TTI_TP_T   = the enclosing type in which to look for our 'name'
                                            OR
                          a pointer to member function as a single type.
                
                HBOOST_TTI_TP_R   = (optional) the return type of the member function
                          if the first parameter is the enclosing type.
                
                HBOOST_TTI_TP_FS  = (optional) the parameters of the member function as a hboost::mpl forward sequence
                          if the first parameter is the enclosing type and the member function parameters
                          are not empty.
                
                HBOOST_TTI_TP_TAG = (optional) a hboost::function_types tag to apply to the member function
                          if the first parameter is the enclosing type and a tag is needed.
                
                returns = 'value' is true if the 'name' exists, 
                          with the appropriate member function type,
                          otherwise 'value' is false.
                          
*/
#define HBOOST_TTI_HAS_MEMBER_FUNCTION(name) \
  HBOOST_TTI_TRAIT_HAS_MEMBER_FUNCTION \
  ( \
  HBOOST_TTI_HAS_MEMBER_FUNCTION_GEN(name), \
  name \
  ) \
/**/

#endif // HBOOST_TTI_HAS_MEMBER_FUNCTION_HPP
