
//  (C) Copyright Edward Diener 2011,2012,2013
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_TTI_DETAIL_MEM_FUN_HPP)
#define HBOOST_TTI_DETAIL_MEM_FUN_HPP

#include <hboost/config.hpp>
#include <hboost/function_types/is_member_function_pointer.hpp>
#include <hboost/function_types/property_tags.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/logical.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/vector.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/type_traits/detail/yes_no_type.hpp>
#include <hboost/type_traits/is_class.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/remove_const.hpp>
#include <hboost/tti/detail/dcomp_mem_fun.hpp>
#include <hboost/tti/detail/ddeftype.hpp>
#include <hboost/tti/detail/dnullptr.hpp>
#include <hboost/tti/detail/dptmf.hpp>
#include <hboost/tti/gen/namespace_gen.hpp>

#if defined(__SUNPRO_CC)

#define HBOOST_TTI_DETAIL_TRAIT_HAS_TYPES_MEMBER_FUNCTION(trait,name) \
  template<class HBOOST_TTI_DETAIL_TP_PMEMF,class HBOOST_TTI_DETAIL_TP_C> \
  struct HBOOST_PP_CAT(trait,_detail_types) \
    { \
    template<HBOOST_TTI_DETAIL_TP_PMEMF> \
    struct helper {}; \
    \
    template<class HBOOST_TTI_DETAIL_TP_EC> \
    static ::hboost::type_traits::yes_type chkt(helper<&HBOOST_TTI_DETAIL_TP_EC::name> *); \
    \
    template<class HBOOST_TTI_DETAIL_TP_EC> \
    static ::hboost::type_traits::no_type chkt(...); \
    \
    typedef hboost::mpl::bool_<sizeof(chkt<HBOOST_TTI_DETAIL_TP_C>(HBOOST_TTI_DETAIL_NULLPTR))==sizeof(::hboost::type_traits::yes_type)> type; \
    }; \
/**/

#else

#define HBOOST_TTI_DETAIL_TRAIT_HAS_TYPES_MEMBER_FUNCTION(trait,name) \
  template<class HBOOST_TTI_DETAIL_TP_PMEMF,class HBOOST_TTI_DETAIL_TP_C> \
  struct HBOOST_PP_CAT(trait,_detail_types) \
    { \
    template<HBOOST_TTI_DETAIL_TP_PMEMF> \
    struct helper; \
    \
    template<class HBOOST_TTI_DETAIL_TP_EC> \
    static ::hboost::type_traits::yes_type chkt(helper<&HBOOST_TTI_DETAIL_TP_EC::name> *); \
    \
    template<class HBOOST_TTI_DETAIL_TP_EC> \
    static ::hboost::type_traits::no_type chkt(...); \
    \
    typedef hboost::mpl::bool_<sizeof(chkt<HBOOST_TTI_DETAIL_TP_C>(HBOOST_TTI_DETAIL_NULLPTR))==sizeof(::hboost::type_traits::yes_type)> type; \
    }; \
/**/

#endif

#define HBOOST_TTI_DETAIL_TRAIT_CTMF_INVOKE(trait,name) \
  HBOOST_TTI_DETAIL_TRAIT_HAS_TYPES_MEMBER_FUNCTION(trait,name) \
  template<class HBOOST_TTI_DETAIL_TP_T,class HBOOST_TTI_DETAIL_TP_R,class HBOOST_TTI_DETAIL_TP_FS,class HBOOST_TTI_DETAIL_TP_TAG> \
  struct HBOOST_PP_CAT(trait,_detail_ctmf_invoke) : \
    HBOOST_PP_CAT(trait,_detail_types) \
      < \
      typename HBOOST_TTI_NAMESPACE::detail::ptmf_seq<HBOOST_TTI_DETAIL_TP_T,HBOOST_TTI_DETAIL_TP_R,HBOOST_TTI_DETAIL_TP_FS,HBOOST_TTI_DETAIL_TP_TAG>::type, \
      HBOOST_TTI_DETAIL_TP_T \
      > \
    { \
    }; \
/**/

#define HBOOST_TTI_DETAIL_TRAIT_HAS_CALL_TYPES_MEMBER_FUNCTION(trait,name) \
  HBOOST_TTI_DETAIL_TRAIT_CTMF_INVOKE(trait,name) \
  template<class HBOOST_TTI_DETAIL_TP_T,class HBOOST_TTI_DETAIL_TP_R,class HBOOST_TTI_DETAIL_TP_FS,class HBOOST_TTI_DETAIL_TP_TAG> \
  struct HBOOST_PP_CAT(trait,_detail_call_types) : \
	hboost::mpl::eval_if \
		< \
 		hboost::is_class<HBOOST_TTI_DETAIL_TP_T>, \
 		HBOOST_PP_CAT(trait,_detail_ctmf_invoke) \
 			< \
 			HBOOST_TTI_DETAIL_TP_T, \
 			HBOOST_TTI_DETAIL_TP_R, \
 			HBOOST_TTI_DETAIL_TP_FS, \
 			HBOOST_TTI_DETAIL_TP_TAG \
 			>, \
 		hboost::mpl::false_ \
		> \
    { \
    }; \
/**/

#define HBOOST_TTI_DETAIL_TRAIT_CHECK_HAS_COMP_MEMBER_FUNCTION(trait,name) \
  HBOOST_TTI_DETAIL_TRAIT_HAS_COMP_MEMBER_FUNCTION(trait,name) \
  template<class HBOOST_TTI_DETAIL_TP_T> \
  struct HBOOST_PP_CAT(trait,_detail_check_comp) : \
    HBOOST_PP_CAT(trait,_detail_hcmf)<HBOOST_TTI_DETAIL_TP_T> \
    { \
    HBOOST_MPL_ASSERT((hboost::function_types::is_member_function_pointer<HBOOST_TTI_DETAIL_TP_T>)); \
    }; \
/**/

#define HBOOST_TTI_DETAIL_TRAIT_HAS_MEMBER_FUNCTION(trait,name) \
  HBOOST_TTI_DETAIL_TRAIT_HAS_CALL_TYPES_MEMBER_FUNCTION(trait,name) \
  HBOOST_TTI_DETAIL_TRAIT_CHECK_HAS_COMP_MEMBER_FUNCTION(trait,name) \
  template<class HBOOST_TTI_DETAIL_TP_T,class HBOOST_TTI_DETAIL_TP_R,class HBOOST_TTI_DETAIL_TP_FS,class HBOOST_TTI_DETAIL_TP_TAG> \
  struct HBOOST_PP_CAT(trait,_detail_hmf) : \
    hboost::mpl::eval_if \
      < \
      hboost::mpl::and_ \
        < \
        hboost::is_same<HBOOST_TTI_DETAIL_TP_R,HBOOST_TTI_NAMESPACE::detail::deftype>, \
        hboost::is_same<HBOOST_TTI_DETAIL_TP_FS,hboost::mpl::vector<> >, \
        hboost::is_same<HBOOST_TTI_DETAIL_TP_TAG,hboost::function_types::null_tag> \
        >, \
      HBOOST_PP_CAT(trait,_detail_check_comp)<HBOOST_TTI_DETAIL_TP_T>, \
      HBOOST_PP_CAT(trait,_detail_call_types)<HBOOST_TTI_DETAIL_TP_T,HBOOST_TTI_DETAIL_TP_R,HBOOST_TTI_DETAIL_TP_FS,HBOOST_TTI_DETAIL_TP_TAG> \
      > \
    { \
    }; \
/**/

#endif // HBOOST_TTI_DETAIL_MEM_FUN_HPP
