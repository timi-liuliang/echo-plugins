/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH library (C++)
 *
 * COMMENTS:	Convenient types used by the channel library.
 *
 */

#ifndef __CH_Types_h__
#define __CH_Types_h__

#include "CH_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_ValArray.h>

class CH_Group;
class CH_Segment;
class CH_Channel;
class CH_ChannelRef;
class CH_Collection;

typedef UT_ValArray<CH_Group *>		CH_GroupList;
typedef UT_ValArray<CH_Segment *>	CH_SegmentList;
typedef UT_ValArray<CH_Channel *>	CH_ChannelList;
typedef UT_Array<CH_ChannelRef>	CH_ChannelRefList;
typedef UT_ValArray<CH_Collection *>	CH_CollectionList;
typedef UT_ValArray<const char *>	CH_NameList;
typedef int	(*CHfindCallBack)(CH_Collection *grp, CH_Channel *chp, void *);

typedef enum {
    CH_SCOPEMASK_NONE = 0x0,

    CH_SCOPED = 0x1,
    CH_SELECTED = 0x2,
    CH_PINNED = 0x4,
    CH_DISPLAY = 0x8,
    CH_TEMPLATE = 0x10,

    CH_LAYER_SCOPED = 0x20,
    CH_LAYER_SELECTED = 0x40,
    CH_LAYER_PINNED = 0x80,
    CH_LAYER_DISPLAY = 0x100,

    CH_HIDDEN = 0x200,
    CH_VECTOR_HIDDEN = 0x400,

    CH_GRAPH_SELECTED = 0x800,
    CH_PARM_SELECTED = 0x1000,

    CH_NUM_FLAGS = 13,
    CH_ALL_FLAGS = ((1<<13)-1)

} CH_ScopeMasks;

CH_API extern const char *const
chScopeFlagNames[];

//
//  This enum is used when calling the changeSegLength method and provides
//  a hint as to how the change is to be done. This is currently only
//  necessary when changing the length of raw segments and the channel
//  manager raw interpolation option is turned off.
//
typedef enum {
    CH_SCALE_ANY,		// Don't care.
    CH_SCALE_START,		// Adjusting the segment start time.
    CH_SCALE_END		// Adjusting the segment end time.
} CH_SegmentScale;

typedef enum {
    CH_VALUE = 0,
    CH_SLOPE,
    CH_ACCEL,
    CH_NUM_VALUES
} CH_ValueTypes;

#endif
