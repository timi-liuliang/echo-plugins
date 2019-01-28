//
// Copyright (c) Antony Polukhin, 2012-2014.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_TYPE_INDEX_HPP
#define HBOOST_TYPE_INDEX_HPP

/// \file hboost/type_index.hpp
/// \brief Includes minimal set of headers required to use the Boost.TypeIndex library.
///
/// By inclusion of this file most optimal type index classes will be included and used 
/// as a hboost::typeindex::type_index and hboost::typeindex::type_info.

#include <hboost/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

#if defined(HBOOST_TYPE_INDEX_USER_TYPEINDEX)
#   include HBOOST_TYPE_INDEX_USER_TYPEINDEX
#   ifdef HBOOST_HAS_PRAGMA_DETECT_MISMATCH
#       pragma detect_mismatch( "hboost__type_index__abi", "user defined type_index class is used: " HBOOST_STRINGIZE(HBOOST_TYPE_INDEX_USER_TYPEINDEX))
#   endif
#elif (!defined(HBOOST_NO_RTTI) && !defined(HBOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)) || defined(HBOOST_MSVC)
#   include <hboost/type_index/stl_type_index.hpp>
#   if defined(HBOOST_NO_RTTI) || defined(HBOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)
#       include <hboost/type_index/detail/stl_register_class.hpp>
#       ifdef HBOOST_HAS_PRAGMA_DETECT_MISMATCH
#           pragma detect_mismatch( "hboost__type_index__abi", "RTTI is off - typeid() is used only for templates")
#       endif
#   else
#       ifdef HBOOST_HAS_PRAGMA_DETECT_MISMATCH
#           pragma detect_mismatch( "hboost__type_index__abi", "RTTI is used")
#       endif
#   endif
#else
#   include <hboost/type_index/ctti_type_index.hpp>
#   include <hboost/type_index/detail/ctti_register_class.hpp>
#   ifdef HBOOST_HAS_PRAGMA_DETECT_MISMATCH
#       pragma detect_mismatch( "hboost__type_index__abi", "RTTI is off - using CTTI")
#   endif
#endif

#ifndef HBOOST_TYPE_INDEX_REGISTER_CLASS
#define HBOOST_TYPE_INDEX_REGISTER_CLASS
#endif

namespace hboost { namespace typeindex {

#if defined(HBOOST_TYPE_INDEX_DOXYGEN_INVOKED)

/// \def HBOOST_TYPE_INDEX_FUNCTION_SIGNATURE
/// HBOOST_TYPE_INDEX_FUNCTION_SIGNATURE is used by hboost::typeindex::ctti_type_index class to
/// deduce the name of a type. If your compiler is not recognized
/// by the TypeIndex library and you wish to work with hboost::typeindex::ctti_type_index, you may
/// define this macro by yourself.
///
/// HBOOST_TYPE_INDEX_FUNCTION_SIGNATURE must be defined to a compiler specific macro
/// that outputs the \b whole function signature \b including \b template \b parameters.
///
/// If your compiler is not recognised and HBOOST_TYPE_INDEX_FUNCTION_SIGNATURE is not defined,
/// then a compile-time error will arise at any attempt to use hboost::typeindex::ctti_type_index classes.
///
/// See HBOOST_TYPE_INDEX_REGISTER_CTTI_PARSING_PARAMS and HBOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING
/// for an information of how to tune the implementation to make a nice pretty_name() output.
#define HBOOST_TYPE_INDEX_FUNCTION_SIGNATURE HBOOST_CURRENT_FUNCTION

/// \def HBOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING
/// This is a helper macro for making correct pretty_names() with RTTI off.
///
/// HBOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING macro may be defined to
/// '(begin_skip, end_skip, runtime_skip, runtime_skip_until)' with parameters for adding a
/// support for compilers, that by default are not recognized by TypeIndex library.
///
/// \b Example:
///
/// Imagine the situation when
/// \code hboost::typeindex::ctti_type_index::type_id<int>().pretty_name() \endcode
/// returns the following string:
/// \code "static const char *hboost::detail::ctti<int>::n() [T = int]" \endcode
/// and \code hboost::typeindex::ctti_type_index::type_id<short>().pretty_name() \endcode returns the following:
/// \code "static const char *hboost::detail::ctti<short>::n() [T = short]" \endcode
///
/// As we may see first 39 characters are "static const char *hboost::detail::ctti<" and they do not depend on
/// the type T. After first 39 characters we have a human readable type name which is duplicated at the end
/// of a string. String always ends on ']', which consumes 1 character.
///
/// Now if we define `HBOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING` to
/// `(39, 1, false, "")` we'll be getting \code "int>::n() [T = int" \endcode
/// for `hboost::typeindex::ctti_type_index::type_id<int>().pretty_name()` and \code "short>::n() [T = short" \endcode
/// for `hboost::typeindex::ctti_type_index::type_id<short>().pretty_name()`.
///
/// Now we need to take additional care of the characters that go before the last mention of our type. We'll
/// do that by telling the macro that we need to cut off everything that goes before the "T = " including the "T = "
/// itself:
///
/// \code (39, 1, true, "T = ") \endcode
///
/// In case of GCC or Clang command line we need to add the following line while compiling all the sources:
///
/// \code
/// -DHBOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING='(39, 1, true, "T = ")'
/// \endcode
/// \param begin_skip How many characters must be skipped at the beginning of the type holding string.
/// Must be a compile time constant.
/// \param end_skip How many characters must be skipped at the end of the type holding string.
/// Must be a compile time constant.
/// \param runtime_skip Do we need additional checks at runtime to cut off the more characters.
/// Must be `true` or `false`.
/// \param runtime_skip_until Skip all the characters before the following string (including the string itself).
/// Must be a compile time array of characters.
///
/// See [RTTI emulation limitations](hboost_typeindex/rtti_emulation_limitations.html) for more info.
#define HBOOST_TYPE_INDEX_CTTI_USER_DEFINED_PARSING (0, 0, false, "")


    /// Depending on a compiler flags, optimal implementation of type_index will be used 
    /// as a default hboost::typeindex::type_index.
    ///
    /// Could be a hboost::typeindex::stl_type_index, hboost::typeindex::ctti_type_index or 
    /// user defined type_index class.
    ///
    /// \b See hboost::typeindex::type_index_facade for a full description of type_index functions.
    typedef platform_specific type_index;
#elif defined(HBOOST_TYPE_INDEX_USER_TYPEINDEX)
    // Nothing to do
#elif (!defined(HBOOST_NO_RTTI) && !defined(HBOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY)) || defined(HBOOST_MSVC)
    typedef hboost::typeindex::stl_type_index type_index;
#else 
    typedef hboost::typeindex::ctti_type_index type_index;
#endif

/// Depending on a compiler flags, optimal implementation of type_info will be used 
/// as a default hboost::typeindex::type_info.
///
/// Could be a std::type_info, hboost::typeindex::detail::ctti_data or 
/// some user defined class.
///
/// type_info \b is \b not copyable or default constructible. It is \b not assignable too!
typedef type_index::type_info_t type_info;

#if defined(HBOOST_TYPE_INDEX_DOXYGEN_INVOKED)

/// \def HBOOST_TYPE_INDEX_USER_TYPEINDEX
/// HBOOST_TYPE_INDEX_USER_TYPEINDEX can be defined to the path to header file
/// with user provided implementation of type_index.
///
/// See [Making a custom type_index](hboost_typeindex/making_a_custom_type_index.html) section
/// of documentation for usage example.
#define HBOOST_TYPE_INDEX_USER_TYPEINDEX <full/absolute/path/to/header/with/type_index.hpp>


/// \def HBOOST_TYPE_INDEX_REGISTER_CLASS
/// HBOOST_TYPE_INDEX_REGISTER_CLASS is used to help to emulate RTTI.
/// Put this macro into the public section of polymorphic class to allow runtime type detection.
///
/// Depending on the typeid() availability this macro will expand to nothing or to virtual helper function
/// `virtual const type_info& hboost_type_info_type_id_runtime_() const noexcept`.
///
/// \b Example:
/// \code
/// class A {
/// public:
///     HBOOST_TYPE_INDEX_REGISTER_CLASS
///     virtual ~A(){}
/// };
///
/// struct B: public A {
///     HBOOST_TYPE_INDEX_REGISTER_CLASS
/// };
///
/// struct C: public B {
///     HBOOST_TYPE_INDEX_REGISTER_CLASS
/// };
///
/// ...
///
/// C c1;
/// A* pc1 = &c1;
/// assert(hboost::typeindex::type_id<C>() == hboost::typeindex::type_id_runtime(*pc1));
/// \endcode
#define HBOOST_TYPE_INDEX_REGISTER_CLASS nothing-or-some-virtual-functions

/// \def HBOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY
/// HBOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY is a helper macro that must be defined if mixing
/// RTTI on/off modules. See
/// [Mixing sources with RTTI on and RTTI off](hboost_typeindex/mixing_sources_with_rtti_on_and_.html)
/// section of documentation for more info.
#define HBOOST_TYPE_INDEX_FORCE_NO_RTTI_COMPATIBILITY

#endif // defined(HBOOST_TYPE_INDEX_DOXYGEN_INVOKED)


/// Function to get hboost::typeindex::type_index for a type T.
/// Removes const, volatile && and & modifiers from T.
///
/// \b Example:
/// \code
/// type_index ti = type_id<int&>();
/// std::cout << ti.pretty_name();  // Outputs 'int'
/// \endcode
///
/// \tparam T Type for which type_index must be created.
/// \throw Nothing.
/// \return hboost::typeindex::type_index with information about the specified type T.
template <class T>
inline type_index type_id() HBOOST_NOEXCEPT {
    return type_index::type_id<T>();
}

/// Function for constructing hboost::typeindex::type_index instance for type T. 
/// Does not remove const, volatile, & and && modifiers from T.
///
/// If T has no const, volatile, & and && modifiers, then returns exactly 
/// the same result as in case of calling `type_id<T>()`.
///
/// \b Example:
/// \code
/// type_index ti = type_id_with_cvr<int&>();
/// std::cout << ti.pretty_name();  // Outputs 'int&'
/// \endcode
///
/// \tparam T Type for which type_index must be created.
/// \throw Nothing.
/// \return hboost::typeindex::type_index with information about the specified type T.
template <class T>
inline type_index type_id_with_cvr() HBOOST_NOEXCEPT {
    return type_index::type_id_with_cvr<T>();
}

/// Function that works exactly like C++ typeid(rtti_val) call, but returns hboost::type_index.
///
/// Retunrs runtime information about specified type.
///
/// \b Requirements: RTTI available or Base and Derived classes must be marked with HBOOST_TYPE_INDEX_REGISTER_CLASS.
///
/// \b Example:
/// \code
/// struct Base { virtual ~Base(){} };
/// struct Derived: public Base  {};
/// ...
/// Derived d;
/// Base& b = d;
/// type_index ti = type_id_runtime(b);
/// std::cout << ti.pretty_name();  // Outputs 'Derived'
/// \endcode
///
/// \param runtime_val Varaible which runtime type must be returned.
/// \throw Nothing.
/// \return hboost::typeindex::type_index with information about the specified variable.
template <class T>
inline type_index type_id_runtime(const T& runtime_val) HBOOST_NOEXCEPT {
    return type_index::type_id_runtime(runtime_val);
}

}} // namespace hboost::typeindex



#endif // HBOOST_TYPE_INDEX_HPP

