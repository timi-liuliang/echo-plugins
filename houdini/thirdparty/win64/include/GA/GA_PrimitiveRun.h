/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveRun.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveRun__
#define __GA_PrimitiveRun__

#include "GA_API.h"

#include <UT/UT_JSONValue.h>
#include <UT/UT_StringArray.h>
#include <SYS/SYS_Types.h>


class GA_Primitive;
class GA_SaveMap;

class UT_JSONWriter;


class GA_API GA_PrimitiveRun 
{
public:
     GA_PrimitiveRun(UT_JSONWriter &w, const GA_SaveMap &map);
    ~GA_PrimitiveRun();

    // Write a primitive
    bool	write(const GA_Primitive *prim);
    bool	flush();

private:
    bool		match(const GA_Primitive *prim) const;
    bool		runPrimitive(const GA_Primitive *prim);
    bool		singlePrimitive(const GA_Primitive *prim);

    UT_JSONWriter	&myW;
    const GA_SaveMap	&mySave;
    const GA_Primitive	**myQueue;	// Queue of primitives in run
    UT_JSONValue	 myUniform;
    UT_StringArray	 myVarying;
    int			 myQCapacity;
    int			 myQEntries;
    uint		 myRunFlags;
    bool		 myDoRun;
};

#endif

