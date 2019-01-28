/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 *      (based on code from the Microsoft Systems Journal)
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	Function for hooking DLL functions.  This lets you replace
 *      any system (or Houdini) DLL function with something of your
 *      own choosing.
 *
 *      For instance, you might want to replace ::MessageBox() with
 *      a message box function that displays a Houdini logo along
 *      with the message text.
 */

#ifndef __UT_NTHooking_H__
#define __UT_NTHooking_H__

#include "UT_API.h"
#ifdef WIN32
#include <Windows.h>
#include "UT_Defines.h"


// Export these functions using the C calling convention.
#ifdef __cplusplus
extern "C" {
#endif  // _cplusplus



typedef struct tag_HOOKFUNCDESCA
{
    LPCSTR szFunc;  // The name of the function to hook.
    PROC   pProc;   // The procedure to blast in.
} HOOKFUNCDESCA , * LPHOOKFUNCDESCA ;

typedef struct tag_HOOKFUNCDESCW
{
    LPCWSTR szFunc; // The name of the function to hook.
    PROC    pProc;  // The procedure to blast in.
} HOOKFUNCDESCW , * LPHOOKFUNCDESCW ;



#ifdef UNICODE
#define HOOKFUNCDESC   HOOKFUNCDESCW
#define LPHOOKFUNCDESC LPHOOKFUNCDESCW
#else
#define HOOKFUNCDESC   HOOKFUNCDESCA
#define LPHOOKFUNCDESC LPHOOKFUNCDESCA
#endif  // UNICODE



/*----------------------------------------------------------------------

RecursiveHookImportedFunctionsByName()

Chris' all-powerful hooking function.  Descends the symbol tree
from a given HMODULE and hooks everything in each loaded module.

Unless you really want to only hook one HMODULE, call this
instead of the non-recursive version.

See the docs lower down in this file for descriptions of
the parameters.

----------------------------------------------------------------------*/

UT_API extern BOOL
RecursiveHookImportedFunctionsByNameA ( HMODULE         hModule,
					LPCSTR          szImportMod,
					UINT            uiCount,
					LPHOOKFUNCDESCA paHookArray,
					PROC *          paOrigFuncs,
					LPUINT          puiHooked );

UT_API extern BOOL    
RecursiveHookImportedFunctionsByNameW ( HMODULE         hModule,
					LPCWSTR         szImportMod,
					UINT            uiCount,
					LPHOOKFUNCDESCA paHookArray,
					PROC *          paOrigFuncs,
					LPUINT          puiHooked ) ;



/*----------------------------------------------------------------------

HookImportedFunctionsByName()

DISCUSSION:
    Hooks the specified functions imported into hModule by the module
indicated by szImportMod.  This function can be used to hook from one
to 'n' of the functions imported.
    The techniques used in the function are slightly different than
that shown by Matt Pietrek in his book, "Windows 95 System Programming
Secrets."  He uses the address of the function to hook as returned by
GetProcAddress.  Unfortunately, while this works in almost all cases, it
does not work when the program being hooked is running under a debugger
on Windows95 (an presumably, Windows98).  The problem is that
GetProcAddress under a debugger returns a "debug thunk," not the address
that is stored in the Import Address Table (IAT).
    This function gets around that by using the real thunk list in the
PE file, the one not bashed by the loader when the module is loaded and
fixed up, to find where the named import is located.  Once the named
import is found, then the original table is blasted to make the hook.
As the name implies, this function will only hook functions imported by
name.

PARAMETERS:
    hModule     - The module where the imports will be hooked.
    szImportMod - The name of the module whose functions will be
                  imported.
    uiCount     - The number of functions to hook.  This is the size of
                  the paHookArray and paOrigFuncs arrays.
    paHookArray - The array of function descriptors that list which
                  functions to hook.  At this point, the array does not
                  have to be in szFunc name order.  Also, if a
                  particular pProc is NULL, then that item will just be
                  skipped.  This makes it much easier for debugging.
    paOrigFuncs - The array of original addresses that were hooked.  If
                  a function was not hooked, then that item will be
                  NULL.
    puiHooked   - Returns the number of functions hooked out of
                  paHookArray.

RETURNS:
    FALSE - There was a problem, check GetLastError.
    TRUE  - The function succeeded.  See the parameter discussion for
            the output parameters.

----------------------------------------------------------------------*/

UT_API extern BOOL
HookImportedFunctionsByNameA ( HMODULE         hModule,
                               LPCSTR          szImportMod,
                               UINT            uiCount,
                               LPHOOKFUNCDESCA paHookArray,
                               PROC *          paOrigFuncs,
                               LPUINT          puiHooked );

UT_API extern BOOL    
HookImportedFunctionsByNameW ( HMODULE         hModule,
                               LPCWSTR         szImportMod,
                               UINT            uiCount,
                               LPHOOKFUNCDESCA paHookArray,
                               PROC *          paOrigFuncs,
                               LPUINT          puiHooked ) ;



#ifdef UNICODE
#define HookImportedFunctionsByName HookImportedFunctionsByNameW
#define RecursiveHookImportedFunctionsByName RecursiveHookImportedFunctionsByNameW
#else
#define HookImportedFunctionsByName HookImportedFunctionsByNameA
#define RecursiveHookImportedFunctionsByName RecursiveHookImportedFunctionsByNameA
#endif  // UNICODE


#ifdef __cplusplus
}
#endif  // _cplusplus

#endif // WIN32

#endif // __UT_NTHooking_H__
