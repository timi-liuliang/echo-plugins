/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_CreateVariants_h__
#define __HUSD_CreateVariants_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_UniquePtr.h>

class HUSD_API HUSD_CreateVariants
{
public:
			 HUSD_CreateVariants();
			~HUSD_CreateVariants();

    bool		 addHandle(const HUSD_DataHandle &src,
				const UT_StringHolder &srcpath,
				const UT_StringHolder &variantname);
    bool		 execute(HUSD_AutoWriteLock &lock,
				const UT_StringRef &primpath,
				const UT_StringRef &variantset) const;

private:
    class husd_CreateVariantsPrivate;

    UT_UniquePtr<husd_CreateVariantsPrivate>	 myPrivate;
    UT_StringArray				 mySrcPaths;
    UT_StringArray				 myVariantNames;
};

#endif

