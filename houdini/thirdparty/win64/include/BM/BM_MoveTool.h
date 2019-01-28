/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BM_MoveTool.h (BM Library, C++)
 *
 * COMMENTS:	Abstract mix-in class for move tool states
 */

#ifndef __BM_MOVETOOL_H__
#define __BM_MOVETOOL_H__

#include "BM_API.h"

class BM_API BM_MoveTool
{
public:
    enum Type { ALL, TRANSLATE, ROTATE, SCALE, NUM_TOOLS }; 

    virtual ~BM_MoveTool()  { }

    virtual void    setMoveToolType(Type move_type) = 0;
    virtual Type    getMoveToolType() const = 0;
};

#endif // __BM_MOVETOOL_H__
