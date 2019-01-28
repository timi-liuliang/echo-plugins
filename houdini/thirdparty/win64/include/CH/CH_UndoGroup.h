/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_UndoGroup.h ( OP Library, C++)
 *
 * COMMENTS:	This class handles the undoing of channel group 
 * 		operations (from JIVE applications).
 *
 * 		This class handles the updating of internal data
 * 		when a channel group operation is undone/redone.
 */

#ifndef __CH_UndoGroup__
#define __CH_UndoGroup__

#include "CH_API.h"
#include <UT/UT_Undo.h>
#include "CH_ChannelRef.h"

class CH_Group;
class CH_Manager;

class CH_API CH_UndoGroup : public UT_Undo
{
public:
    CH_UndoGroup(const CH_Group *chg, bool reverse);
    virtual	~CH_UndoGroup();

    virtual void undo();
    virtual void redo();

protected:
    virtual void	 swap();
    CH_Group		*getGroup() const;

    UT_String		 myPath;
    bool		 myReverse;
};

class CH_API CH_UndoGroupCreateDelete: public CH_UndoGroup
{
public:
    CH_UndoGroupCreateDelete(const CH_Group *chg, bool reverse);

private:
    virtual void	 swap();
};

class CH_API CH_UndoGroupRename: public CH_UndoGroup
{
public:
    CH_UndoGroupRename(
	    const CH_Group *chg,
	    const char *oldName, const char *newName );
private:
    virtual void	 swap();

private:
    UT_String		 myOldName;
    UT_String		 myNewName;
};

class CH_API CH_UndoGroupLoadClear: public CH_UndoGroup
{
public:
    CH_UndoGroupLoadClear(const CH_Group *chg, bool reverse);
    ~CH_UndoGroupLoadClear();

private:
    virtual void	 swap();

private:
    void		 saveChannels();

    char		*myBuffer;
    size_t		 myBufSize;
};

class CH_API CH_UndoGroupAddRemoveChannel: public CH_UndoGroup
{
public:
    CH_UndoGroupAddRemoveChannel(
	    const CH_Group *chg, bool reverse,
	    const CH_ChannelRef &chref );
private:
    virtual void	 swap();

private:
    CH_ChannelRef	 myChanRef;
};

class CH_API CH_UndoGroupMove: public CH_UndoGroup
{
public:
    CH_UndoGroupMove(
	    const CH_Group *chgsp, CH_Group *chgtp,
	    const char *old_name, const char *new_name);
private:
    virtual void	 swap();

private:
    UT_String		 myOldName;
    UT_String		 myNewName;
    UT_String		 myTargetParentPath;
};

class CH_API CH_UndoGroupChangeIndex: public CH_UndoGroup
{
public:
    CH_UndoGroupChangeIndex(const CH_Group *chg, int old_index, int new_index);

private:
    virtual void	 swap();

private:
    int			 myOldIndex;
    int			 myNewIndex;
};
#endif

