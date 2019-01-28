/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_UndoTimeGroup.h ( OP Library, C++)
 *
 * COMMENTS:	This class handles the undoing of time group 
 * 		operations. 
 * 		
 * 		This class handles the updating of internal data
 * 		when a channel group operation is undone/redone.
 */

#ifndef __CH_UndoTimeGroup__
#define __CH_UndoTimeGroup__

#include "CH_API.h"
#include <UT/UT_Undo.h>
#include <SYS/SYS_Types.h>

class CH_TimeGroup;

enum CH_UndoTimeGroupOperation
{
    CH_UNDO_CREATE,
    CH_UNDO_MODIFY,
    CH_UNDO_DELETE
};

class CH_API CH_UndoTimeGroup : public UT_Undo
{
public:
		 CH_UndoTimeGroup(CH_TimeGroup *, CH_UndoTimeGroupOperation);
    virtual	~CH_UndoTimeGroup();

    virtual void undo();
    virtual void redo();

private:
    void	 swap();

    UT_String			 myGroupName;
    fpreal			 myTime;
    CH_UndoTimeGroupOperation 	 myOperation;
};

#endif

