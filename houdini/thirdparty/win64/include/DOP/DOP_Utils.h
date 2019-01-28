/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __DOP_Utils_h__
#define __DOP_Utils_h__

#include "DOP_API.h"
#include "DOP_InOutInfo.h"

class DOP_Node;

DOP_API bool
DOPfirstInputIsOfType(const DOP_Node *node,
		DOP_InOutDataType checktype, bool defaultreturn);

DOP_API DOP_InOutDataType
DOPgetFirstInputType(const DOP_Node *node,
		DOP_InOutDataType defaultreturn);

DOP_API bool
DOPfirstConnectedInputIsOfType(const DOP_Node *node,
		DOP_InOutDataType checktype, bool defaultreturn);

DOP_API DOP_InOutDataType
DOPgetFirstConnectedInputType(const DOP_Node *node,
		DOP_InOutDataType defaultreturn);

DOP_API DOP_InOutDataType
DOPgetFirstConnectedSpecificInputType(const DOP_Node *node);

#endif

