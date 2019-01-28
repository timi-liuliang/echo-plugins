/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:	A simple class to hold flags for post it notes
 *
 */

#ifndef __OP_PostItNoteFlags_h__
#define __OP_PostItNoteFlags_h__

#include "OP_API.h"
#include <iosfwd>


class OP_PostIt;
class UT_IStream;
class UT_String;

#define OP_POSTITNOTE_MINIMIZED_FLAG		'm'
#define OP_POSTITNOTE_DRAWBACKGROUND_FLAG	'd'

class OP_API OP_PostItNoteFlags
{
public:
    void	setMinimized(bool flag)
		{ myMinimized = flag; }
    void	setDrawBackground(bool flag)
		{ myDrawBackground = flag; }
    void	setPicked(bool flag)
		{ myPicked = flag; }
    void	setResizing(bool flag)
		{ myResizing = flag; }
    
    bool	getMinimized() const
		{ return myMinimized; }
    bool	getDrawBackground() const
		{ return myDrawBackground; }
    bool	getPicked() const
		{ return myPicked; }
    bool	getResizing() const
		{ return myResizing; }

private:
    bool   	myPicked:1,	// Picked by user
		myResizing:1, 	// Post-it is being resized
		myMinimized:1,
		myDrawBackground:1;

private:
    OP_PostItNoteFlags();

    void	save(std::ostream &os);
    bool	load(UT_IStream &os, const char *path=0);

    bool	loadAsciiFlag(const char *name, const char *val);
    bool	loadBinaryFlag(int tag, int val);

    ///Initializes all the flags to their default values
    void	setDefaults();

    friend class OP_PostIt;
};

#endif
