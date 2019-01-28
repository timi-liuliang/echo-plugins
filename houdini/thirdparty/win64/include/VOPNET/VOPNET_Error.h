/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __VOPNET_Error__
#define __VOPNET_Error__

enum VOPNET_ErrorCodes
{
    VOPNET_MESSAGE = 0,

    VOPNET_OPERATOR_IN_USE,	// operator type %s is in use.
    VOPNET_CONFLICTING_OPTYPE	// The VOPNET name doesn't match the name of
				// the operator it defines.
};

#endif
