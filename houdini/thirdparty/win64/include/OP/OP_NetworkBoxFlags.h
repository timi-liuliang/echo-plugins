/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:	A simple class to hold flags for network boxes
 *
 */

#ifndef __OP_NetworkBoxFlags_h__
#define __OP_NetworkBoxFlags_h__

#include "OP_API.h"
#include <iosfwd>

#include <UT/UT_String.h>

class OP_NetworkBox;

#define OP_NETWORKBOX_PICK_FLAG         'p'
#define OP_NETWORKBOX_MINIMIZED_FLAG    'm'

class OP_API OP_NetworkBoxFlags
{
public:
    
    void		setUndoComment(bool flag)
			{ myUndoComment = flag; }
    void		setUndoLayout(bool flag)
			{ myUndoLayout = flag; }
    void		setUndoMinimize(bool flag)
			{ myUndoMinimize = flag; }
    void		setUndoRename(bool flag)
			{ myUndoRename = flag; }
    void		setUndoResize(bool flag)
			{ myUndoResize = flag; }

    void		setMarked(bool flag)
			{ myMarked = flag; }
    void		setMinimized(bool flag)
			{ myMinimized = flag; }
    void		setPicked(bool flag)
			{ myPicked = flag; }
    void		setResizing(bool flag)
			{ myResizing = flag; }
    void		setAutoFit(bool flag)
			{ myAutoFit = flag; }
    
    bool		getUndoComment() const
			{ return myUndoComment; }
    bool		getUndoLayout() const
			{ return myUndoLayout; }
    bool		getUndoMinimize() const
			{ return myUndoMinimize; }
    bool		getUndoRename() const
			{ return myUndoRename; }
    bool		getUndoResize() const
			{ return myUndoResize; }
    
    bool		getMarked() const
			{ return myMarked; }
    bool	  	getMinimized() const
			{ return myMinimized; }
    bool		getPicked() const
			{ return myPicked; }
    bool		getResizing() const
			{ return myResizing; }
    bool		getAutoFit() const
			{ return myAutoFit; }

    void	 	getFlagOptions(UT_String &options) const;

private:
    bool   	myMarked:1, 	// Temporary marking for various purposes
		myMinimized:1,	// Netbox is minimized
		myPicked:1,	// Picked by user
		myResizing:1, 	// Netbox is being resized
		myAutoFit:1, 	// Netbox should auto-resize to fit contents

		// These flags are used to save whether a particular
    	        // type of change for the node has been saved.
		myUndoComment:1,
		myUndoLayout:1,
		myUndoMinimize:1,
		myUndoRename:1,
		myUndoResize:1;

private:
    OP_NetworkBoxFlags();
    OP_NetworkBoxFlags(const OP_NetworkBoxFlags &source);

    friend class OP_NetworkBox;
    
    ///Writes out the values of the flags specified by 'flags' to 'os'. If 
    ///'flags' is NULL, writes out values for all our flags
    void	appendCommandString(std::ostream &os, const char *flags,
				    bool save_to_hip) const;

    void	shallowCopy(const OP_NetworkBoxFlags &source);

    ///Initializes all the flags to their default values
    void	setDefaults(void);

};

#endif
