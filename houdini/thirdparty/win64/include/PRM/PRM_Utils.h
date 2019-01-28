/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Utils.h (Parameter Library)
 */

#ifndef __PRM_Utils_h__
#define __PRM_Utils_h__

#include "PRM_API.h"
#include <UT/UT_ErrorManager.h>
#include <UT/UT_Thread.h>
#include <EXPR/EX_Error.h>

class UT_BitArray;
class PRM_ParmList;
class PRM_Parm;

/// A set of useful functions for working with parameters.
class PRM_API PRM_Utils
{
public:
    typedef void (*TraverseParmFunc)(const PRM_ParmList &, PRM_Parm *,
				     int, void *data);

    /// Traverse the parameters in the specified folder of the given switcher
    /// and optionally execute a function on each parameter.
    ///
    ///    folder_start_index
    ///      - index into the parmlist pointing to first eligible parameter
    ///        for the folder
    ///    traverse_multi_instances
    ///      - call the specified function on the instances of any embedded
    ///        multiparms
    ///
    /// Returns the number of parameters to skip in the parmlist to step over
    /// the folder.  Note that this happens whether or not a traverse function
    /// is provided, and does not account for any embedded multiparm instances.
    static int	 traverseFolder(PRM_ParmList &parmlist, PRM_Parm *switcher,
				int folder, int folder_start_index,
				bool traverse_multi_instances,
				TraverseParmFunc function, void *data);

    // Flag the instances associated with the given multiparm in the provided
    // bit array.
    static void	 flagMultiParmInstances(const PRM_ParmList &parmlist,
					PRM_Parm *multi, UT_BitArray &flags);
};

/// Use this to block evaluation errors. This is necessary whenever setting
/// or getting parameters outside of the cooking code path
class PRM_API PRM_AutoBlockErrors
{
public:
    PRM_AutoBlockErrors(int thread = -1)
	: myScope(thread)
    {
	myErrNo = ev_GetErrNo(myScope.getThread());
	myScope.getErrorManager().setDisabled(true);
    }
    ~PRM_AutoBlockErrors()
    {
	ev_SetErrNo(myErrNo, myScope.getThread());
	ev_ClearCycles(myScope.getThread());
    }

private:
    UT_ErrorManager::Scope	 myScope;
    int				 myErrNo;
};


#endif
