/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Channels.h (C++)
 *
 * COMMENTS:	OP_Channels is a channel collection used by an OP_Node
 *		object. This class implements the pure virtuals defined
 *		in CH_Collection to access local variables and the channel
 *		collection hierarchy.
 *
 *		there are two uses of this class
 *		1: as a channel holder for each OP_Node
 *		2: as a channel holder to do copying and pasting
 *
 *		appropriate methods/variables are marked accordingly
 *		this should be two or three classes maybe, but there is no
 *		reason to do this now.
 *
 */

#ifndef __OP_Channels__
#define __OP_Channels__

#include "OP_API.h"
#include <CH/CH_Collection.h>
#include <CH/CH_EventManager.h>
#include <CH/CH_Types.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Types.h>


class OP_Node;
class CH_Channel;
class CH_LocalVariable;
class CH_Manager;
class UT_String;
class UT_XformOrder;


class OP_API OP_Channels : public CH_Collection
{
public:
	     OP_Channels(OP_Node *owner, CH_Manager *chman,
			 CH_LocalVariable *vars);
    virtual ~OP_Channels();

    virtual int64 getMemoryUsage(bool inclusive) const;

    // PATH/ID RELATED (1)
    // these functions rely on the one-to-one mapping between
    // OP_Node and OP_Channels
    virtual const UT_String		&getName() const;

    virtual int				 getUniqueId() const;
    virtual CH_Collection *		 lookupCollection(int id) const;

    virtual const CH_Collection		*getParent() const;
    virtual	  CH_Collection		*getParent();
    virtual const CH_Collection		*getChild(const char *name) const;
    virtual	  CH_Collection		*getChild(const char *name);
    virtual const void			*getOwner() const;
    virtual void			*getOwner();

    virtual int		 getChildren(CH_NameList &names) const;
    virtual int		 getChildren(CH_CollectionList &grps) const;

    virtual UT_StringHolder	 getFullPath() const;
    virtual CH_Collection	*findCollection(const char *name) const;

    // EVENTS (1)
    virtual void	 setChanged(CH_CHANGE_TYPE how, void *data=0);

    // CHANNEL EVALUATION (1 & 2)
    OP_Node		*getNode() const { return myOwner; }
    void		 setNode(OP_Node *new_owner);

    virtual bool	 getVariableValue(fpreal &val, int table, int index,
					  int thread);
    virtual bool	 getVariableValue(UT_String &str, int table, int index,
					  int thread);

    enum { OP_VARIABLE_TABLE_INDEX = 4 };

    virtual const CH_LocalVariable *resolveVariable(const char *name, int &tab);
    const CH_LocalVariable *resolveExtraVariableForSyntaxHighlight(
							    const char *name);

    virtual void	 getXformOrder(UT_XformOrder &xord, const CH_Channel *chp) const;

    // CHANNEL BUFFER STUFF (2)
    // this sets the node so that contained channels can evaluate channel
    // references as if they are in the same node as target
    void		 setTargetChannel( const CH_Channel *target ) const;

    // UTILITIES (1)
    virtual int		 canAccess(unsigned mask) const;
    virtual int		 canAccessChannel(uint mask,
					  const CH_Channel *chp) const;
    virtual void	 saveKeyFrameForUndo(CH_Channel *chp, fpreal gtime);

    virtual void	 addLocalVarDependency(int table, int index,
					       void *ref_id);

    virtual OP_Channels		*castToOPChannels() { return this; }
    virtual const OP_Channels	*castToOPChannels() const { return this; }

protected:

    virtual void	 notifyChannelRenameDependents(const char *old_name,
						       const char *new_name);

private:
    OP_Node			*myOwner;	// (1 & 2)
};

#endif
