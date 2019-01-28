/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM library (C++)
 *
 * COMMENTS:	Shared functions available to the world
 *
 */

#ifndef __PRM_SharedFunc_h__
#define __PRM_SharedFunc_h__

#include "PRM_API.h"

#include <SYS/SYS_Types.h>
class PRM_Name;
class PRM_SpareData;
class UT_String;
class IMG_TileFileInfo;
class PRM_Parm;
class UT_Ramp;

PRM_API extern void
PRMbuildImageMenu(
	void *, PRM_Name *names, int size,
	const PRM_SpareData *, const PRM_Parm *);

PRM_API extern const PRM_Name *
PRMgetImageName(int index);

PRM_API extern void
PRMbuildResolutionMenu(
	void *, PRM_Name *names, int n,
	const PRM_SpareData *, const PRM_Parm *);

PRM_API extern const PRM_Name *
PRMgetResolutionName(int index);

PRM_API extern void
PRMbuildFilterMenu(
	void *, PRM_Name *names, int,
	const PRM_SpareData *, const PRM_Parm *);

PRM_API extern const PRM_Name *
PRMgetFilterName(int idx);

PRM_API extern void
PRMbuildPixelFormatMenu(
	void *, PRM_Name *names, int,
	const PRM_SpareData *, const PRM_Parm *);

PRM_API extern void
PRMbuildImageDeviceMenu(
	void *, PRM_Name *names, int,
	const PRM_SpareData *, const PRM_Parm *);

PRM_API extern void
PRMbuildRiDeviceMenu(
	void *, PRM_Name *names, int,
	const PRM_SpareData *, const PRM_Parm *);

PRM_API extern void
PRMbuildRiFilterMenu(
	void *, PRM_Name *names, int,
	const PRM_SpareData *, const PRM_Parm *);

PRM_API extern void
PRMbuildRiHiderMenu(
	void *, PRM_Name *names, int,
	const PRM_SpareData *,
	const PRM_Parm *);

PRM_API extern void
PRMbuildTakeMenu(
	void *, PRM_Name *names, int,
	const PRM_SpareData *, const PRM_Parm *);

/// utility method for sorting menu items by their label text
PRM_API extern void
PRMsortMenu(PRM_Name *menuitems, int size);

PRM_API extern const PRM_Name *
PRMgetPixelFormatName(int idx);

//
// The following functions are convenience functions for dealing with the
// PRM_TYPE_IMAGE_FORMAT* templates.  There are three parameters involved in this configuration:
//   1)  A string representing the filename of the image to save
//   2)  A string representing the format of the image to save
//   3)  <TWO> strings representing the options for the format.  The first of
//       these strings is the actual format determined by analyzing the
//       filename and format strings.  The second string contains the optional
//       parameters to the format.
// These functions are intended to be used within the parameter change
// callbacks.  It's up to the caller to evaluate the four strings.
//

//
// The PRMchangeFilename() method is called when the filename parameter is
// changed.  The function may change the option_format string.  If the string
// needs to be changed, then the function returns 1.
//
PRM_API extern int
PRMchangeImageFilename(const UT_String &filename, const UT_String &format,
		  UT_String &option_format, UT_String &options);

//
// When the format parameter is changed, it's possible that the extension on
// the filename will change along with the option-format and the option string.
// If this happens, then this function will return 1.
//
PRM_API extern int
PRMchangeImageFormat(UT_String &filename, const UT_String &format,
		UT_String &option_format, UT_String &options);

#endif
