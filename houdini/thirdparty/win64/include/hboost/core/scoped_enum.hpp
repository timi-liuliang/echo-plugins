//  scoped_enum.hpp  ---------------------------------------------------------//

//  Copyright Beman Dawes, 2009
//  Copyright (C) 2011-2012 Vicente J. Botet Escriba
//  Copyright (C) 2012 Anthony Williams

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef HBOOST_CORE_SCOPED_ENUM_HPP
#define HBOOST_CORE_SCOPED_ENUM_HPP

#include <hboost/config.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace hboost
{

#ifdef HBOOST_NO_CXX11_SCOPED_ENUMS

  /**
   * Meta-function to get the native enum type associated to an enum class or its emulation.
   */
  template <typename EnumType>
  struct native_type
  {
    /**
     * The member typedef type names the native enum type associated to the scoped enum,
     * which is it self if the compiler supports scoped enums or EnumType::enum_type if it is an emulated scoped enum.
     */
    typedef typename EnumType::enum_type type;
  };

  /**
   * Casts a scoped enum to its underlying type.
   *
   * This function is useful when working with scoped enum classes, which doens't implicitly convert to the underlying type.
   * @param v A scoped enum.
   * @returns The underlying type.
   * @throws No-throws.
   */
  template <typename UnderlyingType, typename EnumType>
  inline
  HBOOST_CONSTEXPR UnderlyingType underlying_cast(EnumType v) HBOOST_NOEXCEPT
  {
    return v.get_underlying_value_();
  }

  /**
   * Casts a scoped enum to its native enum type.
   *
   * This function is useful to make programs portable when the scoped enum emulation can not be use where native enums can.
   *
   * EnumType the scoped enum type
   *
   * @param v A scoped enum.
   * @returns The native enum value.
   * @throws No-throws.
   */
  template <typename EnumType>
  inline
  HBOOST_CONSTEXPR typename EnumType::enum_type native_value(EnumType e) HBOOST_NOEXCEPT
  {
    return e.get_native_value_();
  }

#else  // HBOOST_NO_CXX11_SCOPED_ENUMS

  template <typename EnumType>
  struct native_type
  {
    typedef EnumType type;
  };

  template <typename UnderlyingType, typename EnumType>
  inline
  HBOOST_CONSTEXPR UnderlyingType underlying_cast(EnumType v) HBOOST_NOEXCEPT
  {
    return static_cast<UnderlyingType>(v);
  }

  template <typename EnumType>
  inline
  HBOOST_CONSTEXPR EnumType native_value(EnumType e) HBOOST_NOEXCEPT
  {
    return e;
  }

#endif // HBOOST_NO_CXX11_SCOPED_ENUMS
}


#ifdef HBOOST_NO_CXX11_SCOPED_ENUMS

#ifndef HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS

#define HBOOST_SCOPED_ENUM_UT_DECLARE_CONVERSION_OPERATOR \
     explicit HBOOST_CONSTEXPR operator underlying_type() const HBOOST_NOEXCEPT { return get_underlying_value_(); }

#else

#define HBOOST_SCOPED_ENUM_UT_DECLARE_CONVERSION_OPERATOR

#endif

/**
 * Start a declaration of a scoped enum.
 *
 * @param EnumType The new scoped enum.
 * @param UnderlyingType The underlying type.
 */
#define HBOOST_SCOPED_ENUM_UT_DECLARE_BEGIN(EnumType, UnderlyingType)    \
    struct EnumType {                                                   \
        typedef void is_hboost_scoped_enum_tag;                          \
        typedef UnderlyingType underlying_type;                         \
        EnumType() HBOOST_NOEXCEPT {}                                    \
        explicit HBOOST_CONSTEXPR EnumType(underlying_type v) HBOOST_NOEXCEPT : v_(v) {}                 \
        HBOOST_CONSTEXPR underlying_type get_underlying_value_() const HBOOST_NOEXCEPT { return v_; }    \
        HBOOST_SCOPED_ENUM_UT_DECLARE_CONVERSION_OPERATOR                \
    private:                                                            \
        underlying_type v_;                                             \
        typedef EnumType self_type;                                     \
    public:                                                             \
        enum enum_type

#define HBOOST_SCOPED_ENUM_DECLARE_END2() \
        HBOOST_CONSTEXPR enum_type get_native_value_() const HBOOST_NOEXCEPT { return enum_type(v_); } \
        friend HBOOST_CONSTEXPR bool operator ==(self_type lhs, self_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)==enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator ==(self_type lhs, enum_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)==rhs; } \
        friend HBOOST_CONSTEXPR bool operator ==(enum_type lhs, self_type rhs) HBOOST_NOEXCEPT { return lhs==enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator !=(self_type lhs, self_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)!=enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator !=(self_type lhs, enum_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)!=rhs; } \
        friend HBOOST_CONSTEXPR bool operator !=(enum_type lhs, self_type rhs) HBOOST_NOEXCEPT { return lhs!=enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator <(self_type lhs, self_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)<enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator <(self_type lhs, enum_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)<rhs; } \
        friend HBOOST_CONSTEXPR bool operator <(enum_type lhs, self_type rhs) HBOOST_NOEXCEPT { return lhs<enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator <=(self_type lhs, self_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)<=enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator <=(self_type lhs, enum_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)<=rhs; } \
        friend HBOOST_CONSTEXPR bool operator <=(enum_type lhs, self_type rhs) HBOOST_NOEXCEPT { return lhs<=enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator >(self_type lhs, self_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)>enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator >(self_type lhs, enum_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)>rhs; } \
        friend HBOOST_CONSTEXPR bool operator >(enum_type lhs, self_type rhs) HBOOST_NOEXCEPT { return lhs>enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator >=(self_type lhs, self_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)>=enum_type(rhs.v_); } \
        friend HBOOST_CONSTEXPR bool operator >=(self_type lhs, enum_type rhs) HBOOST_NOEXCEPT { return enum_type(lhs.v_)>=rhs; } \
        friend HBOOST_CONSTEXPR bool operator >=(enum_type lhs, self_type rhs) HBOOST_NOEXCEPT { return lhs>=enum_type(rhs.v_); } \
    };

#define HBOOST_SCOPED_ENUM_DECLARE_END(EnumType) \
    ; \
    HBOOST_CONSTEXPR EnumType(enum_type v) HBOOST_NOEXCEPT : v_(v) {}                 \
    HBOOST_SCOPED_ENUM_DECLARE_END2()

/**
 * Starts a declaration of a scoped enum with the default int underlying type.
 *
 * @param EnumType The new scoped enum.
 */
#define HBOOST_SCOPED_ENUM_DECLARE_BEGIN(EnumType) \
  HBOOST_SCOPED_ENUM_UT_DECLARE_BEGIN(EnumType,int)

/**
 * Name of the native enum type.
 *
 * @param EnumType The new scoped enum.
 */
#define HBOOST_SCOPED_ENUM_NATIVE(EnumType) EnumType::enum_type
/**
 * Forward declares an scoped enum.
 *
 * @param EnumType The scoped enum.
 */
#define HBOOST_SCOPED_ENUM_FORWARD_DECLARE(EnumType) struct EnumType

#else  // HBOOST_NO_CXX11_SCOPED_ENUMS

#define HBOOST_SCOPED_ENUM_UT_DECLARE_BEGIN(EnumType,UnderlyingType) enum class EnumType : UnderlyingType
#define HBOOST_SCOPED_ENUM_DECLARE_BEGIN(EnumType) enum class EnumType
#define HBOOST_SCOPED_ENUM_DECLARE_END2()
#define HBOOST_SCOPED_ENUM_DECLARE_END(EnumType) ;

#define HBOOST_SCOPED_ENUM_NATIVE(EnumType) EnumType
#define HBOOST_SCOPED_ENUM_FORWARD_DECLARE(EnumType) enum class EnumType

#endif  // HBOOST_NO_CXX11_SCOPED_ENUMS

// Deprecated macros
#define HBOOST_SCOPED_ENUM_START(name) HBOOST_SCOPED_ENUM_DECLARE_BEGIN(name)
#define HBOOST_SCOPED_ENUM_END HBOOST_SCOPED_ENUM_DECLARE_END2()
#define HBOOST_SCOPED_ENUM(name) HBOOST_SCOPED_ENUM_NATIVE(name)

#endif  // HBOOST_CORE_SCOPED_ENUM_HPP
