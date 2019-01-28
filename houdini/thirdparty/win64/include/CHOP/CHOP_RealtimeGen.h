/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP Library (C++)
 *
 * COMMENTS:	All realtime Channel Generators should derive from this class.
 *
 */

#ifndef __CHOP_Realtime_Generator_h__
#define __CHOP_Realtime_Generator_h__

#include "CHOP_API.h"
#include <UT/UT_ValArray.h>
#include "CHOP_Realtime.h"

class CHOP_API CHOP_RealtimeGenerator : public CHOP_Realtime
{
public:
		 CHOP_RealtimeGenerator(OP_Network *parent,
					const char *name,
					OP_Operator *entry);
    virtual	~CHOP_RealtimeGenerator();

    virtual void	cacheChannelData(fpreal t);
    virtual void	getChannelOutputNames(UT_ValArray<char*> &str) =0;
};

#endif
