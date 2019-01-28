/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ObjectSolveInfo_h__
#define __SIM_ObjectSolveInfo_h__

#include "SIM_API.h"
#include <UT/UT_ValArray.h>

class SIM_Solver;
class SIM_Object;
class SIM_ObjectSolveInfo;
class SIM_Engine;

typedef UT_ValArray<SIM_ObjectSolveInfo *> SIM_ObjectSolveInfoArray;

/// This is a simple virtual base class for information that can be stored
/// on a SIM_Object during a solve. This object specific solve info is
/// automatically deleted by the engine at the end of a solve. All objects
/// involved in the solve in any way have this data cleared, so the solver
/// can use this data for objects being solved, feedback object, and one
/// way affector objects.
class SIM_API SIM_ObjectSolveInfo
{
public:
			 SIM_ObjectSolveInfo(const SIM_Solver *createdbysolver,
					     const SIM_Object *object);
    virtual		~SIM_ObjectSolveInfo();

    const SIM_Solver	*getCreatedBySolver() const;

    static void		 clearAllSolveInfo(const SIM_Engine *engine);

private:
    const SIM_Solver	*myCreatedBySolver;
    const SIM_Object	*myObject;

    static bool				 theClearingSolveInfos;
};

#endif
