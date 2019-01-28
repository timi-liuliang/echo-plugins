/*
 *
 * Copyright (c) 1998-2002
 * John Maddock
 *
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         instances.cpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: Defines those template instances that are placed in the
  *                library rather than in the users object files.
  */

//
// note no include guard, we may include this multiple times:
//
#ifndef HBOOST_REGEX_NO_EXTERNAL_TEMPLATES

namespace hboost{

//
// this header can be included multiple times, each time with
// a different character type, HBOOST_REGEX_CHAR_T must be defined
// first:
//
#ifndef HBOOST_REGEX_CHAR_T
#  error "HBOOST_REGEX_CHAR_T not defined"
#endif

#ifndef HBOOST_REGEX_TRAITS_T
#  define HBOOST_REGEX_TRAITS_T , hboost::regex_traits<HBOOST_REGEX_CHAR_T >
#endif

//
// what follows is compiler specific:
//

#if  defined(__BORLANDC__) && (__BORLANDC__ < 0x600)

#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_PREFIX
#endif

#  ifndef HBOOST_REGEX_INSTANTIATE
#     pragma option push -Jgx
#  endif

template class HBOOST_REGEX_DECL basic_regex< HBOOST_REGEX_CHAR_T HBOOST_REGEX_TRAITS_T >;
template class HBOOST_REGEX_DECL match_results< const HBOOST_REGEX_CHAR_T* >;
#ifndef HBOOST_NO_STD_ALLOCATOR
template class HBOOST_REGEX_DECL ::hboost::HBOOST_REGEX_DETAIL_NS::perl_matcher<HBOOST_REGEX_CHAR_T const *, match_results< const HBOOST_REGEX_CHAR_T* >::allocator_type HBOOST_REGEX_TRAITS_T >;
#endif

#  ifndef HBOOST_REGEX_INSTANTIATE
#     pragma option pop
#  endif

#ifdef HBOOST_HAS_ABI_HEADERS
#  include HBOOST_ABI_SUFFIX
#endif

#elif defined(HBOOST_MSVC) || defined(__ICL)

#  ifndef HBOOST_REGEX_INSTANTIATE
#     ifdef __GNUC__
#        define template __extension__ extern template
#     else
#        if HBOOST_MSVC > 1310
#           define HBOOST_REGEX_TEMPLATE_DECL
#        endif
#        define template extern template
#     endif
#  endif

#ifndef HBOOST_REGEX_TEMPLATE_DECL
#  define HBOOST_REGEX_TEMPLATE_DECL HBOOST_REGEX_DECL
#endif

#  ifdef HBOOST_MSVC
#     pragma warning(push)
#     pragma warning(disable : 4251 4231)
#     if HBOOST_MSVC < 1600
#     pragma warning(disable : 4660)
#     endif
#  endif

template class HBOOST_REGEX_TEMPLATE_DECL basic_regex< HBOOST_REGEX_CHAR_T HBOOST_REGEX_TRAITS_T >;

template class HBOOST_REGEX_TEMPLATE_DECL match_results< const HBOOST_REGEX_CHAR_T* >;
#ifndef HBOOST_NO_STD_ALLOCATOR
template class HBOOST_REGEX_TEMPLATE_DECL ::hboost::HBOOST_REGEX_DETAIL_NS::perl_matcher<HBOOST_REGEX_CHAR_T const *, match_results< const HBOOST_REGEX_CHAR_T* >::allocator_type HBOOST_REGEX_TRAITS_T >;
#endif
#if !(defined(HBOOST_DINKUMWARE_STDLIB) && (HBOOST_DINKUMWARE_STDLIB <= 1))\
   && !(defined(HBOOST_INTEL_CXX_VERSION) && (HBOOST_INTEL_CXX_VERSION <= 800))\
   && !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION))\
   && !defined(HBOOST_REGEX_ICU_INSTANCES)
template class HBOOST_REGEX_TEMPLATE_DECL match_results< std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator >;
#ifndef HBOOST_NO_STD_ALLOCATOR
template class HBOOST_REGEX_TEMPLATE_DECL ::hboost::HBOOST_REGEX_DETAIL_NS::perl_matcher< std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator, match_results< std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator >::allocator_type, hboost::regex_traits<HBOOST_REGEX_CHAR_T > >;
#endif
#endif


#  ifdef HBOOST_MSVC
#     pragma warning(pop)
#  endif

#  ifdef template
#     undef template
#  endif

#undef HBOOST_REGEX_TEMPLATE_DECL

#elif (defined(__GNUC__) && (__GNUC__ >= 3)) || !defined(HBOOST_NO_CXX11_EXTERN_TEMPLATE)

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#endif

#  ifndef HBOOST_REGEX_INSTANTIATE
#     ifdef __GNUC__
#        define template __extension__ extern template
#     else
#        define template extern template
#     endif
#  endif

#if !defined(HBOOST_NO_STD_LOCALE) && !defined(HBOOST_REGEX_ICU_INSTANCES)
namespace HBOOST_REGEX_DETAIL_NS{
template HBOOST_REGEX_DECL
std::locale cpp_regex_traits_base<HBOOST_REGEX_CHAR_T>::imbue(const std::locale& l);

template HBOOST_REGEX_DECL
cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::string_type 
   cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::transform_primary(const HBOOST_REGEX_CHAR_T* p1, const HBOOST_REGEX_CHAR_T* p2) const;
template HBOOST_REGEX_DECL
cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::string_type 
   cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::transform(const HBOOST_REGEX_CHAR_T* p1, const HBOOST_REGEX_CHAR_T* p2) const;
template HBOOST_REGEX_DECL
cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::string_type 
   cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::lookup_collatename(const HBOOST_REGEX_CHAR_T* p1, const HBOOST_REGEX_CHAR_T* p2) const;
template HBOOST_REGEX_DECL
void cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::init();
template HBOOST_REGEX_DECL
cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::char_class_type 
   cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::lookup_classname_imp(const HBOOST_REGEX_CHAR_T* p1, const HBOOST_REGEX_CHAR_T* p2) const;
#ifdef HBOOST_REGEX_BUGGY_CTYPE_FACET
template HBOOST_REGEX_DECL
bool cpp_regex_traits_implementation<HBOOST_REGEX_CHAR_T>::isctype(const HBOOST_REGEX_CHAR_T c, char_class_type mask) const;
#endif
} // namespace
template HBOOST_REGEX_DECL
int cpp_regex_traits<HBOOST_REGEX_CHAR_T>::toi(const HBOOST_REGEX_CHAR_T*& first, const HBOOST_REGEX_CHAR_T* last, int radix)const;
template HBOOST_REGEX_DECL
std::string cpp_regex_traits<HBOOST_REGEX_CHAR_T>::catalog_name(const std::string& name);
template HBOOST_REGEX_DECL
std::string& cpp_regex_traits<HBOOST_REGEX_CHAR_T>::get_catalog_name_inst();
template HBOOST_REGEX_DECL
std::string cpp_regex_traits<HBOOST_REGEX_CHAR_T>::get_catalog_name();
#ifdef HBOOST_HAS_THREADS
template HBOOST_REGEX_DECL
static_mutex& cpp_regex_traits<HBOOST_REGEX_CHAR_T>::get_mutex_inst();
#endif
#endif

template HBOOST_REGEX_DECL basic_regex<HBOOST_REGEX_CHAR_T HBOOST_REGEX_TRAITS_T >& 
   basic_regex<HBOOST_REGEX_CHAR_T HBOOST_REGEX_TRAITS_T >::do_assign(
      const HBOOST_REGEX_CHAR_T* p1, 
      const HBOOST_REGEX_CHAR_T* p2, 
      flag_type f);
template HBOOST_REGEX_DECL basic_regex<HBOOST_REGEX_CHAR_T HBOOST_REGEX_TRAITS_T >::locale_type HBOOST_REGEX_CALL 
   basic_regex<HBOOST_REGEX_CHAR_T HBOOST_REGEX_TRAITS_T >::imbue(locale_type l);

template HBOOST_REGEX_DECL void HBOOST_REGEX_CALL 
   match_results<const HBOOST_REGEX_CHAR_T*>::maybe_assign(
      const match_results<const HBOOST_REGEX_CHAR_T*>& m);

namespace HBOOST_REGEX_DETAIL_NS{
template HBOOST_REGEX_DECL void perl_matcher<HBOOST_REGEX_CHAR_T const *, match_results< const HBOOST_REGEX_CHAR_T* >::allocator_type HBOOST_REGEX_TRAITS_T >::construct_init(
      const basic_regex<HBOOST_REGEX_CHAR_T HBOOST_REGEX_TRAITS_T >& e, match_flag_type f);
template HBOOST_REGEX_DECL bool perl_matcher<HBOOST_REGEX_CHAR_T const *, match_results< const HBOOST_REGEX_CHAR_T* >::allocator_type HBOOST_REGEX_TRAITS_T >::match();
template HBOOST_REGEX_DECL bool perl_matcher<HBOOST_REGEX_CHAR_T const *, match_results< const HBOOST_REGEX_CHAR_T* >::allocator_type HBOOST_REGEX_TRAITS_T >::find();
} // namespace

#if (defined(__GLIBCPP__) || defined(__GLIBCXX__)) \
   && !defined(HBOOST_REGEX_ICU_INSTANCES)\
   && !defined(__SGI_STL_PORT)\
   && !defined(_STLPORT_VERSION)
// std:basic_string<>::const_iterator instances as well:
template HBOOST_REGEX_DECL void HBOOST_REGEX_CALL 
   match_results<std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator>::maybe_assign(
      const match_results<std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator>& m);

namespace HBOOST_REGEX_DETAIL_NS{
template HBOOST_REGEX_DECL void perl_matcher<std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator, match_results< std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator >::allocator_type, hboost::regex_traits<HBOOST_REGEX_CHAR_T > >::construct_init(
      const basic_regex<HBOOST_REGEX_CHAR_T>& e, match_flag_type f);
template HBOOST_REGEX_DECL bool perl_matcher<std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator, match_results< std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator >::allocator_type, hboost::regex_traits<HBOOST_REGEX_CHAR_T > >::match();
template HBOOST_REGEX_DECL bool perl_matcher<std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator, match_results< std::basic_string<HBOOST_REGEX_CHAR_T>::const_iterator >::allocator_type, hboost::regex_traits<HBOOST_REGEX_CHAR_T > >::find();
} // namespace
#endif

#  ifdef template
#     undef template
#  endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

} // namespace hboost

#endif // HBOOST_REGEX_NO_EXTERNAL_TEMPLATES





