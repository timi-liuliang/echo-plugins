/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveFamilyMask.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveFamilyMask__
#define __GA_PrimitiveFamilyMask__

// Convenience family masks for grouping the factory types to allow for fast
// iteration and detection.
enum GA_PrimitiveFamilyMask
{
    GA_FAMILY_NONE	    = 0,

    GA_FAMILY_FACE   	    = 0x00000001,
    GA_FAMILY_CURVE	    = 0x00000002,
    GA_FAMILY_HULL	    = 0x00000004,
    GA_FAMILY_TPSURF	    = 0x00000008,
    GA_FAMILY_QUADRIC	    = 0x00000010,
    GA_FAMILY_META	    = 0x00000020,
    GA_FAMILY_VOLUMEELEMENT = 0x00000040,

    GA_FAMILY_SPLINE	= (GA_FAMILY_CURVE|GA_FAMILY_TPSURF),
};

#endif
