/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * 	NAME:			 DD_Source.h
 *
 * COMMENTS:
 *
 *
 */

#ifndef __DD_Source_
#define __DD_Source_

#include "DD_API.h"
#include "DD_ChoiceList.h"

class DD_API DD_Source
{

public:

    DD_Source();
    virtual ~DD_Source();

    // Generalized query method to retrieve data items
    // from the source (dragged object)
    //  0 = not available
    //  1 = available
    // -1 = reject and abort search (eg dragging onto source object)

    virtual void *getSourceData(const char *label, exint index = 0) = 0;

    // Similar to above, but subclass to avoid calculating the actual value
    virtual int  hasSourceData(const char *label);

    // Mechanism to allow the source to modify itself
    // whether or not its currently being accepted (change cursors, etc).

    virtual void	saveDragDropState();
    virtual void	dragDropAccepted(bool onoff);
    virtual void	restoreDragDropState();


    // Set up these parameters for receiving tests

    void		setSourceXY(int x, int y)
			{ mySourceX = x; mySourceY = y; }

    int			getSourceX() const
			{ return mySourceX; }

    int			getSourceY() const
			{ return mySourceY; }

private:

    int			mySourceX;
    int			mySourceY;

};

#endif 

