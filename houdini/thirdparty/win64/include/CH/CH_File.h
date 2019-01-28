/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_File.h ( CH Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CH_File__
#define __CH_File__

#include "CH_API.h"
#include "CH_Manager.h"
#include <SYS/SYS_Types.h>

class CH_API CH_File
{
public:
    enum Error
    {
	INVALID_FRAME_RANGE = -1,
	FILE_NOT_LOADED = -2,
	FILE_NOT_SAVED = -3,
	NO_DATA_IN_FILE = -4,
	NO_CHANNELS = -5,
        NO_PARAMETERS = -6,
	ALLOCATION_FAILED = -7
    };

    // from, to are assumed to be frames
    // results will be saved to tstart, tend
    static void         getTimeRange(const fpreal *from, const fpreal *to,
                                     fpreal &tstart, fpreal &tend);

    // from and to are supplied as frames
    static int		loadRawChannels(CH_ChannelRefList *chrefs,
				const char *filename,
                                CH_CollectionList &parents,
                                const fpreal *from = NULL, const fpreal *to = NULL,
                                bool full_replace = false);

    // from and to are supplied as frames
    static int		saveRawChannels(CH_ChannelList &list,
				const char *filename, bool binary,
                                const fpreal *from = NULL, const fpreal *to = NULL);
};

#endif

