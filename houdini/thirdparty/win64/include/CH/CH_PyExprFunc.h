/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __CH_PyExprFunc_h__
#define __CH_PyExprFunc_h__

#include <EXPR/EXPR.h>
#include "CH_API.h"

CH_API void CHpythonFunctionCallback(EV_FUNCTION *func, EV_SYMBOL *result,
				     EV_SYMBOL **argv, int);

#endif
