
//  (C) Copyright Edward Diener 2011,2012,2013
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_TTI_DETAIL_COMP_MEM_FUN_HPP)
#define HBOOST_TTI_DETAIL_COMP_MEM_FUN_HPP

#include <hboost/config.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/tti/detail/dftclass.hpp>
#include <hboost/tti/detail/dnullptr.hpp>
#include <hboost/tti/gen/namespace_gen.hpp>
#include <hboost/type_traits/remove_const.hpp>
#include <hboost/type_traits/detail/yes_no_type.hpp>

#if defined(__SUNPRO_CC)

#define HBOOST_TTI_DETAIL_TRAIT_HAS_COMP_MEMBER_FUNCTION(trait,name) \
  template<class HBOOST_TTI_DETAIL_TP_T> \
  struct HBOOST_PP_CAT(trait,_detail_hcmf) \
    { \
    template<class HBOOST_TTI_DETAIL_TP_F> \
    struct cl_type : \
      hboost::remove_const \
        < \
        typename HBOOST_TTI_NAMESPACE::detail::class_type<HBOOST_TTI_DETAIL_TP_F>::type \
        > \
      { \
      }; \
    \
    template<HBOOST_TTI_DETAIL_TP_T> \
    struct helper {}; \
    \
    template<class HBOOST_TTI_DETAIL_TP_U> \
    static ::hboost::type_traits::yes_type chkt(helper<&HBOOST_TTI_DETAIL_TP_U::name> *); \
    \
    template<class HBOOST_TTI_DETAIL_TP_U> \
    static ::hboost::type_traits::no_type chkt(...); \
    \
    typedef hboost::mpl::bool_<sizeof(chkt<typename cl_type<HBOOST_TTI_DETAIL_TP_T>::type>(HBOOST_TTI_DETAIL_NULLPTR))==sizeof(::hboost::type_traits::yes_type)> type; \
    }; \
/**/

#else

#define HBOOST_TTI_DETAIL_TRAIT_HAS_COMP_MEMBER_FUNCTION(trait,name) \
  template<class HBOOST_TTI_DETAIL_TP_T> \
  struct HBOOST_PP_CAT(trait,_detail_hcmf) \
    { \
    template<class HBOOST_TTI_DETAIL_TP_F> \
    struct cl_type : \
      hboost::remove_const \
        < \
        typename HBOOST_TTI_NAMESPACE::detail::class_type<HBOOST_TTI_DETAIL_TP_F>::type \
        > \
      { \
      }; \
    \
    template<HBOOST_TTI_DETAIL_TP_T> \
    struct helper; \
    \
    template<class HBOOST_TTI_DETAIL_TP_U> \
    static ::hboost::type_traits::yes_type chkt(helper<&HBOOST_TTI_DETAIL_TP_U::name> *); \
    \
    template<class HBOOST_TTI_DETAIL_TP_U> \
    static ::hboost::type_traits::no_type chkt(...); \
    \
    typedef hboost::mpl::bool_<sizeof(chkt<typename cl_type<HBOOST_TTI_DETAIL_TP_T>::type>(HBOOST_TTI_DETAIL_NULLPTR))==sizeof(::hboost::type_traits::yes_type)> type; \
    }; \
/**/

#endif


#endif // HBOOST_TTI_DETAIL_COMP_MEM_FUN_HPP
