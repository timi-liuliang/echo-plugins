/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_SaveVersion.h (CL Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CL_SAVEVERSION_H_INCLUDED__
#define __CL_SAVEVERSION_H_INCLUDED__

#include "CL_API.h"

/// Generic version number for CL state data
///
/// Version 1:	    Initial version, all float data is fpreal32
/// Version 2/3:    These were intermediate version numbers used by CHOP_Filter
///		    and CHOP_Gesture.
/// Version 4:	    All floating point data is fpreal64
///
#define CL_SAVE_VERSION 4

#endif // __CL_SAVEVERSION_H_INCLUDED__
