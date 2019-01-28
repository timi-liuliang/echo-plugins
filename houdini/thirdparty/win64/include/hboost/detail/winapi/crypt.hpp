//  crypt.hpp  --------------------------------------------------------------//

//  Copyright 2014 Antony Polukhin
//  Copyright 2015 Andrey Semashev

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef HBOOST_DETAIL_WINAPI_CRYPT_HPP
#define HBOOST_DETAIL_WINAPI_CRYPT_HPP

#include <hboost/detail/winapi/basic_types.hpp>
#include <hboost/detail/winapi/detail/cast_ptr.hpp>
#if defined( HBOOST_USE_WINDOWS_H )
// This header is not always included as part of windows.h
#include <wincrypt.h>
#endif

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if !defined( HBOOST_USE_WINDOWS_H )
namespace hboost { namespace detail { namespace winapi {
typedef ULONG_PTR_ HCRYPTPROV_;
}}}

// Some versions of MinGW (including the latest ones) contain buggy declarations of CryptEnumProvidersA and CryptEnumProvidersW.
// We cannot detect those broken versions, and we can't include the system header because it's incomplete.
// So below we duplicate the broken declarations here and work around the problem with cast_ptr. These declarations
// will have to be removed when MinGW is fixed.

extern "C" {
#if !defined( HBOOST_NO_ANSI_APIS )
#if !defined( HBOOST_WINAPI_IS_MINGW ) || !defined( UNICODE )
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
CryptEnumProvidersA(
    hboost::detail::winapi::DWORD_ dwIndex,
    hboost::detail::winapi::DWORD_ *pdwReserved,
    hboost::detail::winapi::DWORD_ dwFlags,
    hboost::detail::winapi::DWORD_ *pdwProvType,
    hboost::detail::winapi::LPSTR_ szProvName,
    hboost::detail::winapi::DWORD_ *pcbProvName);
#else
// Broken declaration in MinGW
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
CryptEnumProvidersA(
    hboost::detail::winapi::DWORD_ dwIndex,
    hboost::detail::winapi::DWORD_ *pdwReserved,
    hboost::detail::winapi::DWORD_ dwFlags,
    hboost::detail::winapi::DWORD_ *pdwProvType,
    hboost::detail::winapi::LPWSTR_ szProvName,
    hboost::detail::winapi::DWORD_ *pcbProvName);
#endif

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
CryptAcquireContextA(
    hboost::detail::winapi::HCRYPTPROV_ *phProv,
    hboost::detail::winapi::LPCSTR_ pszContainer,
    hboost::detail::winapi::LPCSTR_ pszProvider,
    hboost::detail::winapi::DWORD_ dwProvType,
    hboost::detail::winapi::DWORD_ dwFlags);
#endif // !defined( HBOOST_NO_ANSI_APIS )

#if !defined( HBOOST_WINAPI_IS_MINGW ) || defined( UNICODE )
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
CryptEnumProvidersW(
    hboost::detail::winapi::DWORD_ dwIndex,
    hboost::detail::winapi::DWORD_ *pdwReserved,
    hboost::detail::winapi::DWORD_ dwFlags,
    hboost::detail::winapi::DWORD_ *pdwProvType,
    hboost::detail::winapi::LPWSTR_ szProvName,
    hboost::detail::winapi::DWORD_ *pcbProvName);
#else
// Broken declaration in MinGW
HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
CryptEnumProvidersW(
    hboost::detail::winapi::DWORD_ dwIndex,
    hboost::detail::winapi::DWORD_ *pdwReserved,
    hboost::detail::winapi::DWORD_ dwFlags,
    hboost::detail::winapi::DWORD_ *pdwProvType,
    hboost::detail::winapi::LPSTR_ szProvName,
    hboost::detail::winapi::DWORD_ *pcbProvName);
#endif

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
CryptAcquireContextW(
    hboost::detail::winapi::HCRYPTPROV_ *phProv,
    hboost::detail::winapi::LPCWSTR_ szContainer,
    hboost::detail::winapi::LPCWSTR_ szProvider,
    hboost::detail::winapi::DWORD_ dwProvType,
    hboost::detail::winapi::DWORD_ dwFlags);

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
CryptGenRandom(
    hboost::detail::winapi::HCRYPTPROV_ hProv,
    hboost::detail::winapi::DWORD_ dwLen,
    hboost::detail::winapi::BYTE_ *pbBuffer);

HBOOST_SYMBOL_IMPORT hboost::detail::winapi::BOOL_ WINAPI
CryptReleaseContext(
    hboost::detail::winapi::HCRYPTPROV_ hProv,
    hboost::detail::winapi::DWORD_ dwFlags);
}
#endif // !defined( HBOOST_USE_WINDOWS_H )

namespace hboost {
namespace detail {
namespace winapi {

#if defined( HBOOST_USE_WINDOWS_H )

typedef ::HCRYPTPROV HCRYPTPROV_;

const DWORD_ PROV_RSA_FULL_         = PROV_RSA_FULL;

const DWORD_ CRYPT_VERIFYCONTEXT_   = CRYPT_VERIFYCONTEXT;
const DWORD_ CRYPT_NEWKEYSET_       = CRYPT_NEWKEYSET;
const DWORD_ CRYPT_DELETEKEYSET_    = CRYPT_DELETEKEYSET;
const DWORD_ CRYPT_MACHINE_KEYSET_  = CRYPT_MACHINE_KEYSET;
const DWORD_ CRYPT_SILENT_          = CRYPT_SILENT;

#else

const DWORD_ PROV_RSA_FULL_         = 1;

const DWORD_ CRYPT_VERIFYCONTEXT_   = 0xF0000000;
const DWORD_ CRYPT_NEWKEYSET_       = 8;
const DWORD_ CRYPT_DELETEKEYSET_    = 16;
const DWORD_ CRYPT_MACHINE_KEYSET_  = 32;
const DWORD_ CRYPT_SILENT_          = 64;

#endif

#if !defined( HBOOST_NO_ANSI_APIS )
using ::CryptEnumProvidersA;
using ::CryptAcquireContextA;
#endif
using ::CryptEnumProvidersW;
using ::CryptAcquireContextW;
using ::CryptGenRandom;
using ::CryptReleaseContext;

#if !defined( HBOOST_NO_ANSI_APIS )
HBOOST_FORCEINLINE BOOL_ crypt_enum_providers(
    DWORD_ dwIndex,
    DWORD_ *pdwReserved,
    DWORD_ dwFlags,
    DWORD_ *pdwProvType,
    LPSTR_ szProvName,
    DWORD_ *pcbProvName)
{
    return ::CryptEnumProvidersA(dwIndex, pdwReserved, dwFlags, pdwProvType, winapi::detail::cast_ptr(szProvName), pcbProvName);
}

HBOOST_FORCEINLINE BOOL_ crypt_acquire_context(
    HCRYPTPROV_ *phProv,
    LPCSTR_ pszContainer,
    LPCSTR_ pszProvider,
    DWORD_ dwProvType,
    DWORD_ dwFlags)
{
    return ::CryptAcquireContextA(phProv, pszContainer, pszProvider, dwProvType, dwFlags);
}
#endif

HBOOST_FORCEINLINE BOOL_ crypt_enum_providers(
    DWORD_ dwIndex,
    DWORD_ *pdwReserved,
    DWORD_ dwFlags,
    DWORD_ *pdwProvType,
    LPWSTR_ szProvName,
    DWORD_ *pcbProvName)
{
    return ::CryptEnumProvidersW(dwIndex, pdwReserved, dwFlags, pdwProvType, winapi::detail::cast_ptr(szProvName), pcbProvName);
}

HBOOST_FORCEINLINE BOOL_ crypt_acquire_context(
    HCRYPTPROV_ *phProv,
    LPCWSTR_ szContainer,
    LPCWSTR_ szProvider,
    DWORD_ dwProvType,
    DWORD_ dwFlags)
{
    return ::CryptAcquireContextW(phProv, szContainer, szProvider, dwProvType, dwFlags);
}

}
}
}

#endif // HBOOST_DETAIL_WINAPI_CRYPT_HPP
