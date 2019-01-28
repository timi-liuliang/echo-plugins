/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_EditVariants_h__
#define __HUSD_EditVariants_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>

class HUSD_API HUSD_EditVariants
{
public:
			 HUSD_EditVariants(HUSD_AutoWriteLock &lock,
				const UT_StringHolder &primpath,
				const UT_StringHolder &variantset);
			~HUSD_EditVariants();

    bool		 setVariant(const UT_StringHolder &variantname);

private:
    HUSD_AutoWriteLock	&myWriteLock;
    UT_StringHolder	 myPrimPath;
    UT_StringHolder	 myVariantSet;
};

#endif

