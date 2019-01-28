/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_Defines.h ( CL Library, C++)
 *
 * COMMENTS:
 *		common definitions
 */

#ifndef __CL_Defines__
#define __CL_Defines__

#include <SYS/SYS_Floor.h>
#include <UT/UT_XformOrder.h>

enum CL_Unit  {  
    UNIT_NONE   = -1,
    UNIT_FRAME  = 0,
    UNIT_SAMPLE = 1,
    UNIT_SECOND = 2 
};

#define CL_RINT(x)		(SYSrint(x))


// These values are used when reading and writing binary data.
// WARNING: Do not modify these values as we save/write them! It is
// only safe to add to this enum.
enum CL_PacketType
{
    CL_PACKET_END		    = 0,
    CL_PACKET_RATE		    = 1,
    CL_PACKET_START		    = 2,
    CL_PACKET_TRACKLENGTH	    = 3,
    CL_PACKET_QUATERNIONS	    = 4,
    CL_PACKET_TRACKS		    = 5,
    CL_PACKET_NCVERSION		    = 6,
    CL_PACKET_COMMENT		    = 7,
    CL_PACKET_TRACKDATATYPE	    = 8,
    CL_PACKET_VERSION		    = 9,
    CL_PACKET_FLOATFIELDDATATYPE    = 10,
    CL_PACKET_LCVERSION		    = 11,
    CL_PACKET_NTYPES		    // sentinel
};

enum CL_DataType
{
    CL_DATATYPE_FP32		    = 0,
    CL_DATATYPE_FP64		    = 1,
};

// WARNING: Do not modify these values as we save/write them!
#define CL_BCLIP_MAGIC_NUMBER (('b'<<24) + ('c'<<16) + ('l'<<8) + ('p'))

// WARNING: Do not modify these values as we save/write them!
#define CL_BCLIP_VERSION	2
#define CL_BCLIP_NCVERSION	CL_BCLIP_VERSION
#define CL_BCLIP_LCVERSION	CL_BCLIP_VERSION


static inline const char *
CLorderToString(UT_XformOrder::xyzOrder ord)
{
    switch(ord)
    {
	case UT_XformOrder::XYZ:
	    return "xyz";
	case UT_XformOrder::XZY:
	    return "xzy";
	case UT_XformOrder::YXZ:
	    return "yxz";
	case UT_XformOrder::YZX:
	    return "yzx";
	case UT_XformOrder::ZXY:
	    return "zxy";
	case UT_XformOrder::ZYX:
	default:
	    return "zyx";
    }
}

static constexpr const char* BCLIP_STREAM = "stdstream.bclip";
static constexpr const char* BCLIP_SC_STREAM = "stdstream.bclip.sc";

#endif
