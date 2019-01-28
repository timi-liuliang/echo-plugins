/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_UndoEventHandler.h (JIVE Library, C++)
 *
 * COMMENTS:	Creates an undo marker to minimize sent events
 */

#ifndef __CH_UNDOEVENTHANDLER_H_INCLUDED__
#define __CH_UNDOEVENTHANDLER_H_INCLUDED__

#include "CH_API.h"
#include <UT/UT_Undo.h>
#include <UT/UT_UndoManager.h>


/// Creates an event handler into the undo manager to minimize sent events. To
/// use this, create a CH_UndoEventHandler::Scope object on the stack to
/// encapsulate the undo actions that we want to batch events for. If the
/// actions span multiple function calls, use matching undoBegin()/undoEnd()
/// invocations.
class CH_API CH_UndoEventHandler : public UT_Undo
{
public:

    class Scope
    {
    public:
	Scope()
	{
	    myBlockLevel = CH_UndoEventHandler::start();
	}
	~Scope()
	{
	    CH_UndoEventHandler::stop(myBlockLevel);
	}
    private:
	int	myBlockLevel;
    };

    static int		start()
			{
			    if (!UTwillAcceptUndoAddition())
				return -1;

			    UTaddToUndoBlock(new CH_UndoEventHandler(false));
			    return UTgetUndoManager()->blockLevel();
			}
    static void		stop(int level)
			{
			    if (level <= -1)
				return;
			    UTaddToUndoBlock(new CH_UndoEventHandler(true));
			}

    virtual void	undo();
    virtual void	redo();

private:
			CH_UndoEventHandler(bool start_marker);
    void		swap();

private:
    bool		myStartMarker;

    friend class CH_UndoEventHandler::Scope;
};

#endif // __CH_UNDOEVENTHANDLER_H_INCLUDED__
