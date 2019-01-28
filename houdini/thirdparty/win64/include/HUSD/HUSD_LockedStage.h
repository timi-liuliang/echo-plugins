/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_LockedStage_h__
#define __HUSD_LockedStage_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_WeakPtr.h>
#include <UT/UT_UniquePtr.h>

class HUSD_API HUSD_LockedStage
{
public:
				 HUSD_LockedStage();
				~HUSD_LockedStage();

    bool			 isValid() const;
    const UT_StringHolder	&getRootLayerIdentifier() const
				 { return myRootLayerIdentifier; }

    void			 clear();
    bool			 lockStage(const HUSD_DataHandle &data,
					const UT_StringHolder &save_path);

private:
    class husd_LockedStagePrivate;

    UT_UniquePtr<husd_LockedStagePrivate>	 myPrivate;
    UT_StringHolder				 myRootLayerIdentifier;
};

typedef UT_SharedPtr<HUSD_LockedStage> HUSD_LockedStagePtr;
typedef UT_WeakPtr<HUSD_LockedStage> HUSD_LockedStageWeakPtr;

#endif

