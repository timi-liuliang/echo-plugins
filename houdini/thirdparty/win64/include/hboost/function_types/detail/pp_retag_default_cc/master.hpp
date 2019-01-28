
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is guarded externally

#ifdef __WAVE__
// this file has been generated from the master.hpp file in the same directory
#   pragma wave option(preserve: 0)
#endif

#if !defined(HBOOST_PP_VALUE)
#   include <hboost/preprocessor/slot/slot.hpp>
#   include <hboost/preprocessor/iteration/self.hpp>

#   include <hboost/function_types/detail/encoding/def.hpp>
#   include <hboost/function_types/detail/encoding/aliases_def.hpp>

namespace hboost { namespace function_types {

namespace detail
{
  template<class Tag, class RefTag> struct selector_bits
  {
#   define  HBOOST_PP_VALUE non_member|member|non_variadic|variadic
#   include HBOOST_PP_ASSIGN_SLOT(1)

    HBOOST_STATIC_CONSTANT(bits_t, value = (
        (::hboost::function_types::detail::bits<Tag>::value & HBOOST_FT_default_cc) 
      | (::hboost::function_types::detail::bits<RefTag>::value & HBOOST_PP_SLOT(1))
    ));
  };

  template<bits_t SelectorBits> struct default_cc_tag; 
  
  template<class Tag, class RefTag> struct retag_default_cc
    : detail::compound_tag
      < Tag, detail::default_cc_tag< 
          ::hboost::function_types::detail::selector_bits<Tag,RefTag>::value > >
  { };

  template<bits_t SelectorBits> struct default_cc_tag
  {
    typedef null_tag::bits bits;
    typedef null_tag::mask mask;
  };

  class test_class;
  typedef constant<HBOOST_FT_cc_mask> cc_mask_constant;

#   define HBOOST_FT_self \
      <hboost/function_types/detail/pp_retag_default_cc/master.hpp>

#   define  default_cc_ HBOOST_FT_default_cc

#   define  HBOOST_PP_VALUE default_cc_|non_member|non_variadic
#   define  HBOOST_FT_tester void (*tester)()
#   define  HBOOST_PP_INDIRECT_SELF HBOOST_FT_self
#   include HBOOST_PP_INCLUDE_SELF()

#   define  HBOOST_PP_VALUE default_cc_|non_member|variadic
#   define  HBOOST_FT_tester void (*tester)(...)
#   define  HBOOST_PP_INDIRECT_SELF HBOOST_FT_self
#   include HBOOST_PP_INCLUDE_SELF()

#   define  HBOOST_PP_VALUE default_cc_|member|non_variadic
#   define  HBOOST_FT_tester void (test_class::*tester)()
#   define  HBOOST_PP_INDIRECT_SELF HBOOST_FT_self
#   include HBOOST_PP_INCLUDE_SELF()

#   define  HBOOST_PP_VALUE default_cc_|member|variadic
#   define  HBOOST_FT_tester void (test_class::*tester)(...)
#   define  HBOOST_PP_INDIRECT_SELF HBOOST_FT_self
#   include HBOOST_PP_INCLUDE_SELF()

#   undef   default_cc_

#   undef HBOOST_FT_self

} } } // namespace ::hboost::function_types::detail

#   include <hboost/function_types/detail/encoding/aliases_undef.hpp>
#   include <hboost/function_types/detail/encoding/undef.hpp>

#else // if defined(HBOOST_PP_VALUE)

#   include HBOOST_PP_ASSIGN_SLOT(1)

  template<> struct default_cc_tag<HBOOST_PP_SLOT(1)> 
  {
    typedef HBOOST_FT_tester;
    typedef mpl::bitand_<components<tester>::bits,cc_mask_constant> bits;
    typedef cc_mask_constant mask;
  };

#   undef HBOOST_FT_tester

#endif

