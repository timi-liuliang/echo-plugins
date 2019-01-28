/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Xform_h__
#define __HUSD_Xform_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_TimeCode.h"
#include <UT/UT_Matrix4.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>

class HUSD_FindPrims;

enum HUSD_XformStyle
{
    HUSD_XFORM_LOCALSPACE,
    HUSD_XFORM_WORLDSPACE,
    HUSD_XFORM_ABSOLUTE
};

class HUSD_API HUSD_XformEntry
{
public:
    UT_Matrix4D		 myXform;
    HUSD_TimeCode	 myTimeCode;
};
typedef UT_Array<HUSD_XformEntry> HUSD_XformEntryArray;
typedef UT_StringMap<HUSD_XformEntryArray> HUSD_XformEntryMap;

class HUSD_API HUSD_Xform
{
public:
			 HUSD_Xform(HUSD_AutoWriteLock &dest);
			~HUSD_Xform();

    // Apply a single transform to all primitives
    bool		 applyXforms(const HUSD_FindPrims &findprims,
				const UT_StringRef &name,
				const UT_Matrix4D &xform,
				const HUSD_TimeCode &timecode,
				HUSD_XformStyle xform_style) const;

    // For each primpath apply the corresponding xform
    bool		 applyXforms(const HUSD_XformEntryMap &xform_map,
				const UT_StringRef &name,
				HUSD_XformStyle xform_style) const;

    bool		 getIsTimeVarying() const
			 { return myTimeVarying; }

private:
    HUSD_AutoWriteLock	&myWriteLock;
    mutable bool	 myTimeVarying;
};

#endif

