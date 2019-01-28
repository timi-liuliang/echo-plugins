/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_UndoCaptureLayerPaint_h__
#define __SOP_UndoCaptureLayerPaint_h__

#include "SOP_API.h"
#include <UT/UT_Undo.h>

#include "SOP_CaptureLayerPaint.h"

class SOP_API SOP_UndoCaptureLayerPaint : public UT_Undo
{
public:
		     SOP_UndoCaptureLayerPaint(SOP_CaptureLayerPaint *node,
					       SOP_CaptureLayerData &delta,
					       UT_String &oldrootpath,
					       UT_String &newrootpath,
					       UT_StringArray &oldmapping,
					       UT_StringArray &newmapping);
    virtual	    ~SOP_UndoCaptureLayerPaint();

    virtual void     undo();
    virtual void     redo();

private:
    int				 myOpId;
    SOP_CaptureLayerData	 myDelta;
    UT_String			 myOldRootPath;
    UT_String			 myNewRootPath;
    UT_StringArray		 myOldStencilMapping;
    UT_StringArray		 myNewStencilMapping;
};

#endif
