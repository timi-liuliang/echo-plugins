/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Stat.h ( OP Library, C++)
 *
 * COMMENTS:	Stats for an OP.
 *	Creation Time is the time the OP was first created
 *	The following operations on an OP_Node should touch the modify time:
 *		Parameter change/animated
 *		Spare channel modified
 *		Node renamed
 *		Child created/destroyed
 *		Input changed
 *		Locked contents changed (i.e. modelled)
 *		The operator is "touched"
 *	The cook time is modified whenever the OP is cooked
 */

#ifndef __OP_Stat__
#define __OP_Stat__

#include "OP_API.h"
#include <sys/types.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class UT_IStream;
class UT_StringHolder;
class UT_StringRef;

class OP_API OP_Stat {
public:
     OP_Stat();
    ~OP_Stat();

    bool	load(UT_IStream &is, unsigned *permission);
    bool	save(std::ostream &os, unsigned  permission=0777);

    void	touch();		// Change modify time

    time_t	 getCreateTime() const	{ return myCreateTime; }
    time_t	 getModifyTime() const	{ return myModifyTime; }

    bool	 isAuthorCurrentUser() const { return (myAuthor == 0); }

    const UT_StringHolder
		&getAuthor() const;

    void	 setAuthor(const UT_StringRef &name);

    int64	 getMemoryUsage(bool inclusive) const
    { return inclusive ? sizeof(*this) : 0; }

private:
    time_t	myCreateTime;
    time_t	myModifyTime;
    int		myAuthor;
};

#endif
