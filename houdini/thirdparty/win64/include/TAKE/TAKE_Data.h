/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TAKE_Data.h ( TAK Library, C++)
 *
 * COMMENTS:	Take data.  Each entity which can be changed on a per-take
 *		basis has take data associated with it.  When the current take
 *		is changed, we lock down the take information for the entity.
 *
 *		The priority is used in application of the data when setting a
 *		take.  For example, a parameter which is a multi-type guy
 *		should occur before the parameters it defines.
 *		At the current time, there should only be two priorities, 10
 *		and 100.
 */

#ifndef __TAKE_Data__
#define __TAKE_Data__

#include "TAKE_API.h"
#include "TAKE_DataId.h"
#include <iosfwd>

class UT_String;
class UT_WorkBuffer;

#define TAKE_HIGH_PRIORITY	10	// Data is applied before other data
#define TAKE_NORMAL_PRIORITY	100
#define TAKE_LOW_PRIORITY	1000	// Data is applied last

class TAKE_API TAKE_Data 
{
public:
	     TAKE_Data(const TAKE_DataId &id, int priority) :
		    myId(id), myRefCount(0), myPriority(priority) { }
    virtual ~TAKE_Data();

    // Create a copy of the data
    virtual TAKE_Data	*duplicate() const = 0;

    // Query whether the take data can be duplicated for the specified
    // node.
    virtual bool	 canDuplicateForNode(int op_id) const = 0;
    // Create a copy of the data for the specified node.
    virtual TAKE_Data	*duplicateForNode(int op_id) const = 0;

    // Store data from the current take
    virtual void	storeCurrent() = 0;
    // Apply the data to make this current
    virtual void	applyData() = 0;

    virtual void	getOpIdString(UT_String &str) = 0;

    // Mark the current take data as being active
    virtual void	markActive() = 0;
    virtual void	markInactive() = 0;

    // Generate a command which will go from the master take to the active take
    virtual void	generateIncludeCommand(UT_WorkBuffer &) = 0;
    virtual void	generateCommand(UT_WorkBuffer &) = 0;

    // Description of what this take data is
    virtual void	getDescription(UT_WorkBuffer &, int brief=0) = 0;
    
    // Amount of memory used (don't forget sizeof(*this))
    virtual int64	getMemoryUsage() const = 0;

    // Query whether or not this data should be saved for the master take.
    // This is used to avoid saving the data in the master take for locked
    // assets, allowing it to be recomputed during the load.
    virtual bool	canSaveInMasterTake() const = 0;

    virtual int		save(std::ostream &os, int binary) const = 0;
    virtual bool	load(UT_IStream &is) = 0;

    const TAKE_DataId	&getId() const	{ return myId; }

    void		bumpRefCount(int dir)	{ myRefCount += dir; }
    int			getRefCount() const	{ return myRefCount; }

    void		setPriority(int p)	{ myPriority = p; }
    int			getPriority() const	{ return myPriority; }

private:
    TAKE_DataId myId;
    int		myRefCount;
    int		myPriority;
};

#endif
