/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: This class stores deltas (a GDT_Detail object) which are
 * 	     used to complete an undo/redo operation for a class
 * 	     derived from SOP_GDT.
 * 	     Depending on the type of undo, we may apply the deltas
 * 	     inversely or not on an undo.
 */

#ifndef __SOP_UndoGDT_h__
#define __SOP_UndoGDT_h__

#include "SOP_API.h"
#include <UT/UT_Undo.h>
#include <GDT/GDT_Detail.h>

class SOP_GDT;

enum SOP_UndoGDTType {
    SOP_UNDOGDTTYPE_APPLY_CURRENT_DELTA		= 0,
    SOP_UNDOGDTTYPE_CLEAR_PERMANENT_DELTA	= 1
};

class SOP_API SOP_UndoGDT : public UT_Undo
{
    public:
	// Upon creation of a SOP_UndoGDT object, the node id
	// and a copy of the given GDT are stored.
			     SOP_UndoGDT(SOP_GDT *gdtnode,
					 GDT_Detail *delta,
					 SOP_UndoGDTType undotype);
	virtual             ~SOP_UndoGDT();

	virtual void         undo(void);
	virtual void         redo(void);

    private:
	int                  myOpId;
	GDT_Detail	    *myDelta;
	SOP_UndoGDTType      myUndoType;
	GDT_CoordinateFrames *myCoordinateFrames;
};

#endif
