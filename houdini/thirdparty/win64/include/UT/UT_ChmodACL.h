/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ChmodACL.h (UT Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __UT_CHMODACL_H_INCLUDED__
#define __UT_CHMODACL_H_INCLUDED__

#include "UT_API.h"

/// A version of chmod() that directly manipulates ACLs on supported OSes
///
/// Currently only implemented on Windows and is compatible with cygwin chmod.
/// Note that the CRT chmod() (and therefore os.chmod() in hython) only modify
/// the Read Only file attribute. So if you use this to remove write
/// permission, those other functions won't work anymore.
///
/// On all other platforms, defaults to regular chmod()
///
/// @return 0 on success, -1 on error
UT_API extern int UTchmodACL(const char *path, unsigned int mode);

#endif // __UT_CHMODACL_H_INCLUDED__
