/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP Library (C++)
 *
 * COMMENTS:	All realtime channels which perform an operation on its inputs
 *		and output a different # of channels should derive from this
 *		class.
 *
 */

#ifndef __CHOP_Realtime_Fan_h__
#define __CHOP_Realtime_Fan_h__

#include "CHOP_API.h"
#include <UT/UT_ValArray.h>
#include "CHOP_Realtime.h"

class CHOP_API CHOP_RealtimeFan : public CHOP_Realtime
{
public:
		 CHOP_RealtimeFan(OP_Network *parent,
				  const char *name,
				  OP_Operator *entry);
    virtual	~CHOP_RealtimeFan();

    virtual void	cacheChannelData(fpreal t);

    virtual void	getChannelOutputNames(UT_ValArray<char*> &str) =0;

private:

};

#endif
