/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_SubRange.h ( CL Library, C++)
 *
 * COMMENTS:
 *		class to determine effect rolloff
 */

#ifndef __CL_SubRange__
#define __CL_SubRange__

#include "CL_API.h"
#include "CL_Sequence.h"
#include <SYS/SYS_Types.h>

class CL_API CL_SubRange
{

public:

     CL_SubRange();
    ~CL_SubRange();

    void		set(fpreal s, fpreal p, fpreal r, fpreal e, fpreal sc,
	    		    CL_BlendFunction rise, CL_BlendFunction fall);
    fpreal		getEffect(fpreal index) const;

private:

    void		sort(void);

    fpreal		myStart;
    fpreal		myPeak;
    fpreal		myRelease;
    fpreal		myEnd;
    fpreal		myScale;

    CL_BlendFunction	myRise;
    CL_BlendFunction	myFall;

};


#endif

