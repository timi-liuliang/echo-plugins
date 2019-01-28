/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OpCaller.h ( UT Library, C++)
 *
 */

#ifndef __UT_OpCaller__
#define __UT_OpCaller__

#include "UT_API.h"
class UT_OpCallee;

/// General documentation for the UT_OpCaller class.
/// 
/// In some cases during the cook of an OP Node, calls will be made into low
/// level libraries.  These libraries may have callback mechanisms to reference
/// data from other existing OPs, but without any dependencies on OPs
/// themselves.  For example, in resolving the "op:some-op-path" paths, the
/// low level libraries don't actually know about OPs, but rely on callbacks
/// to perform the evaluation.
///
/// However, the caller OP (the OP cooking) isn't notified of these additional
/// dependencies and so doesn't end up cooking properly.  This pair of classes
/// provides a mechanism for implementing the appropriate dependencies.
/// Prior to cooking, the OP can create a UT_Caller class which acts as a sink
/// for all the OP references which get made.  At the conclusion of the cook,
/// the OP can then call back to all the classes which made the references
/// (the OP_Callee classes) and notify them that the data they built is no
/// longer required.
///
/// This class is primarily used by VEX at the current time.

class UT_API UT_OpCaller {
public:
    /// This class provides a virtual interface to OP_Nodes so that
    /// they can query the op-node which is being evaluated.
			 UT_OpCaller();
    virtual		~UT_OpCaller();

    /// Callee's might want to find out the unique id of the caller.  The /
    //getOpId() method should provide the correct id for the cooking OP
    virtual int		 getOpId() = 0;

    /// The referenceOtherOp() method is called when a callee magically
    /// references another operator.  This might happen when referencing a
    /// file using the "op:soppath" syntax.
    /// At the conclusion of cooking, this cache should be notified that the
    /// OP is done with the data.
    virtual void	 referenceOtherOp(int opid, UT_OpCallee *cache,
					  bool flaginterest) = 0;

    /// Reference a specific parameter
    virtual void	referenceOtherParameter(int opid, int pi, int vi) = 0;
};

class UT_API UT_OpCallee {
public:
    /// The UT_OpCallee class is used as a general mechanism to get callbacks
    /// when a reference to an OP is no longer needed.
			 UT_OpCallee();
    virtual		~UT_OpCallee();

    /// This method should be called by the UT_OpCaller when the data is no
    /// longer required.
    virtual void	 doneWithOp(int opid) = 0;
};

#endif

