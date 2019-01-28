/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_MakeShared.h (UT Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __UT_MAKESHARED_H_INCLUDED__
#define __UT_MAKESHARED_H_INCLUDED__

#if defined(_MSC_VER) 
    #pragma message ("Deprecated in Houdini 17. Use UT_SharedPtr.h instead.")
#else
    #warning "Deprecated in Houdini 17. Use UT_SharedPtr.h instead."
#endif

/// @file
/// This file is deprecated. Include UT_SharedPtr.h instead

#include "UT_SharedPtr.h"

#endif // __UT_MAKESHARED_H_INCLUDED__
