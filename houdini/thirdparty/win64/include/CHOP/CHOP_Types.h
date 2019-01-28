/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP_Types.h (CHOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CHOP_TYPES_H_INCLUDED__
#define __CHOP_TYPES_H_INCLUDED__

#include "CHOP_API.h"
#include <UT/UT_Array.h>

class CHOP_Handle;
class CHOP_Node;
class orUnit;

typedef UT_Array<CHOP_Handle *>		CHOP_HandleList;
typedef UT_Array<CHOP_Node *>		CHOP_NodeList;
typedef UT_Array<const CHOP_Node *>	CHOP_NodeListC;
typedef UT_Array<orUnit *>		orUnitList;

#endif // __CHOP_TYPES_H_INCLUDED__
