/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Memory.h ( PRM Library, C++)
 *
 * COMMENTS:	Memory tracking for PRM
 */

#ifndef __PRM_Memory__
#define __PRM_Memory__

#include "PRM_API.h"
#include <UT/UT_Memory.h>

enum PRM_MemUser {
    PRM_MEM_DEBUG = 0,

    PRM_MEM_SETVALUE,		// Set value evaluation

    PRM_MAX_MEM_USERS
};

class PRM_API PRM_Memory {
public:
    SYS_MEMORY_SUBCLASS(static, , thePRMMemoryTable)

    static void		printUsage(UT_WorkBuffer &buf);

private:
    static SYS_MemoryTable	thePRMMemoryTable;
};

#endif

