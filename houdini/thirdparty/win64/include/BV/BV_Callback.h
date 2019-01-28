/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BV_Callback.h
 */

#ifndef __BV_Callback_h__
#define __BV_Callback_h__

#include "BV_API.h"
class BV_Node;
class BV_NodeLeaf;

/// Callback for bounding volume hierarchy intersection operation.
class BV_API BV_Callback
{
public:
		     BV_Callback();
    virtual	    ~BV_Callback();

    enum Action
    {
	/// Continue intersection process
	BV_CONTINUE,
	/// Stop intersection process
	BV_STOP
    };
    virtual Action   callback(int primIdA, int primIdB) = 0;

private:
    /// Disallowed.
    /// @{
		     BV_Callback(const BV_Callback &);
    BV_Callback	    &operator=(const BV_Callback &);
    /// @}
};

#endif
