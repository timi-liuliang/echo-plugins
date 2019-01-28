
#ifndef __UT_API_h__
#define __UT_API_h__

#include <SYS/SYS_Visibility.h>

#ifdef UT_EXPORTS
#define UT_API SYS_VISIBILITY_EXPORT
#define UT_API_TMPL SYS_VISIBILITY_EXPORT_TMPL
#define UT_API_TINST SYS_VISIBILITY_EXPORT_TINST
#define UT_EXTERN_TEMPLATE(CLASS_) SYS_VISIBILITY_EXPORT_EXTERN_TEMPLATE(CLASS_)
#else
#define UT_API SYS_VISIBILITY_IMPORT
#define UT_API_TMPL SYS_VISIBILITY_IMPORT_TMPL
#define UT_API_TINST SYS_VISIBILITY_IMPORT_TINST
#define UT_EXTERN_TEMPLATE(CLASS_) SYS_VISIBILITY_IMPORT_EXTERN_TEMPLATE(CLASS_)
#endif

// Check that we are being built with valid compiler options.
#ifdef WIN32
    #if _MSC_VER < 1300
	#error "You must compile with MS Visual C++ .NET 2003"
    #endif
    #ifndef __cplusplus
	#error "You must compile with the /TP switch (C++ Compilation)"
    #endif
    #ifndef _MT
	#error "You must compile with the /MD or /MT switch (Multithreaded CRT)"
    #endif
    #ifndef _CPPUNWIND
	#error "You must compile with the /GX switch (Exception Handling)"
    #endif
    #ifndef _CPPRTTI
	#error "You must compile with the /GR switch (RTTI)"
    #endif
    #ifndef WIN32
	#error "You must define WIN32"
    #endif
    #ifdef _M_IX86
	#ifndef SESI_LITTLE_ENDIAN
	    #error "You must define SESI_LITTLE_ENDIAN for IX86 compilation"
	#endif
    #endif
#endif

#endif
