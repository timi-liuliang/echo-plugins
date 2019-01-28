/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Take.h ( OP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OP_Take__
#define __OP_Take__

#include "OP_API.h"
#include "OP_Node.h"
#include <UT/UT_String.h>
#include <UT/UT_Functor.h>
#include <TAKE/TAKE_Manager.h>
#include <iosfwd>

class OP_Node;
class PRM_Parm;
class TAKE_Take;
class UT_StringArray;
class UT_WorkBuffer;
class OP_UndoSync;

#define OP_TAKE_FLAG_TOKEN	".flags."
#define OP_TAKE_CHANNEL_TOKEN	".chan"

#define OP_TAKE_TAKESET_CPIO_PACKET	".cwd"

class OP_API OP_Take : public TAKE_Manager {
public:
	     OP_Take();
    virtual ~OP_Take();

    // Methods called to store master take data
    void	storeTakeFlag(OP_Node *node, uchar flag);
    void	storeTakeParm(OP_Node *node, PRM_Parm *parm, bool join=1,
			      bool auto_take=false);

    void	clearTakeFlag(OP_Node *node, uchar flag);
    void	clearTakeParm(OP_Node *node, PRM_Parm *parm);

    void	clearTakeData(int id, const char *label);

    virtual void	switchToTake(TAKE_Take *take, bool force=false);

    void	storeCurrentTake(OP_Node *root, bool force_master=false)
		{
		    // We don't want to store the master take.  Its data is
		    // always set by the other takes.  When we recursively
		    // store the take data, we only care about branches which
		    // actually have active take data.
		    if (force_master || !isMasterTake())
			recurseStoreCurrent(root, true);
		}

    // Clear the active parameters for the current take
    static void	clearActiveFlags(OP_Node *root);

    // Utility method to check whether the current take manager is in
    // a state which could automatically activate parameters in the
    // current take.
    static bool	autoActivationActive();

    // Utility method to check whether the specified parameter needs to
    // be auto activated.
    static bool needsParmAutoActivation(PRM_Parm *parm);

    // Utility method to check whether the specified flag needs to
    // be auto activated.
    static bool needsFlagAutoActivation(OP_Node *node, uchar flag);

    // Utility method to extract nodes and tokens (parm and flag) from a given
    // take.  The caller must specify a mapping functor to convert flag chars
    // into appropriate tokens.
    static void	getNodesAndTokens(const TAKE_Take *take,
				  UT_Functor1<const char *, uchar> flag_map,
				  OP_NodeList &nodes, UT_StringArray &tokens);

    virtual TAKE_Data	*dataFactory(const char *oppath, const char *label);
    virtual void	 notifyObservers(TAKE_Event::TAKE_EventType type,
					 const char *name,
					 const TAKE_Take *take = 0);

    // Command callbacks
    bool	 takeSet(const char *name);
    bool	 takeMove(const char *name, const char *newparent,
			  bool skip_current_restore = false);
    bool	 takeMerge(const char *dest, UT_StringArray &sources,
			   bool override, UT_StringArray *missed = 0);
    bool	 takeRemove(const char *name, bool recursive);
    bool	 takeRename(const char *oldname, const char *newname);
    TAKE_Take	*takeAdd(const char *name, const char *parent=0);
    TAKE_Take	*takeInsert(const char *name, const char *parent=0);
    bool	 takeInfo(const char *takename, UT_WorkBuffer &buf,
			    int indent=0, bool contents=true,
			    bool memusage=true);
    bool	 takeCommands(const char *takename, std::ostream &os);
    bool	 takeCopy(const char *takename, std::ostream &os, bool recurse);
    bool	 takePaste(UT_IStream &is, const char *parent=0,
			    UT_ValArray<TAKE_Take *> *created_list=0);

    void	 takeRestoreCurrent(TAKE_Take *take);

    void	 nodesDeleted(OP_NodeList &nodes);

    void	 removeSpareParm(OP_Node *node, const char *parmname);

    // A helper class to aid when merging in another hip file.
    class OP_API MergeContext
    {
	public:
	    MergeContext() : myDelayTakeSwitch(false) {}

	    // The merge prefix is the string prepended to all merged takes.
	    const UT_String	&getMergePrefix() const
					{ return myMergePrefix; }
	    void		 setMergePrefix(const char *s)
					{ harden(myMergePrefix, s); }

	    // The merge master take string records the name of the master
	    // take from the merged file.
	    const UT_String	&getMergeMasterTake() const
					{ return myMergeMasterTake; }
	    void		 setMergeMasterTake(const char *s)
					{ harden(myMergeMasterTake, s); }

	    // A flag instructing OP_Take::takeSet() to delay any non-trivial
	    // take switch.
	    bool		 getDelayTakeSwitch() const
					{ return myDelayTakeSwitch; }
	    void		 setDelayTakeSwitch(bool flag)
					{ myDelayTakeSwitch = flag; }

	    // A string recording the delayed take switch, if any.
	    const UT_String	&getDelayedTake() const
					{ return myDelayedTake; }
	    void		 setDelayedTake(const char *s)
					{ harden(myDelayedTake, s); }
	private:
	    void		 harden(UT_String &str, const char *s)
					{ if (s) str.harden(s); else str = 0; }

	    UT_String	 myMergePrefix;
	    UT_String	 myMergeMasterTake;
	    UT_String	 myDelayedTake;
	    bool	 myDelayTakeSwitch;
    };

    // The following method is used to copy the data in all the takes for
    // a particular node to another another node, whenever possible.  Note
    // that this method does not actually change any parameters, and so it
    // may be necessary to call the takeRestoreCurrent() method after calling
    // this.
    void	 copyDataFromNode(OP_Node *dest, const OP_Node *src);

    // The following method is used to merge in the takes loaded during the
    // OP_Network::loadNetwork() call while merging.  A new child take will
    // be added to the master take to act as the root for all merged takes.
    // All new takes are prefixed with a string derived from the supplied
    // prefix string.  The provided merge context is updated to store the
    // prefix string used as well as the name of the master take from the
    // merged hierarchy.
    void	 networkLoadMerge(OP_Take &src, const char *prefix,
				  MergeContext &context);

    // Register/unregister a merge context to be used during the takeSet()
    // method.
    void	 setMergeContext(MergeContext *context);

    // The main idea behind the "Sync Contexts" is very simple.  We have to
    // be in the master take in order to properly synchronize an asset with
    // its definition.  These "Sync Contexts" allow us to potentially group
    // multiple synchronizations together and minimize the number of context
    // switches.  As such, it is critical that any calls to syncingNode be
    // made only while in a "Sync Context".
    void	 pushSyncContext();
    void	 syncingNode(OP_Node *node);
    void	 popSyncContext();

    // Normally the take manager is not notified of node deletions during
    // an undo/redo operation.  The following methods can be used to override
    // this behaviour.
    void	 setHandleNodeDeletionDuringUndoFlag(bool flag);
    bool	 handlesNodeDeletionDuringUndo() const
			{ return myHandleNodeDeletionDuringUndoFlag; }

    // The following methods allow the user to set or query the current
    // sync undo in which deletion of take data will be recorded if we
    // are unable to add an undo ourselves.  It is an error to attempt
    // to set a sync undo while one is currently set.
    void	 setSyncUndo(OP_UndoSync *);
    OP_UndoSync	*getSyncUndo() const { return mySyncUndo; }

    // The following method is used for internal changing of the current
    // take, necessary during asset synchronization.  It is only public
    // because OP_UndoSync needs access to it.
    bool	internalTakeSetForSync(const char *name);
    virtual void	renameTake(TAKE_Take *take, const char *name);

protected:
    void	setACTIVETAKE(TAKE_Take *take);
    void	recurseStoreCurrent(OP_Node *node, bool partial_recurse);
    void	recursiveDestroy(TAKE_Take *take,
				 bool *cleaned_dead_data_from_master_take = 0);
    bool	recursiveNodeDeleted(TAKE_Take *take, OP_NodeList &node,
			    UT_ValArray<TAKE_Data *> &work);


private:
    int				 mySyncDepth;		 
    UT_String			 mySyncTake;

    OP_UndoSync			*mySyncUndo;
    bool			 myHandleNodeDeletionDuringUndoFlag;

    MergeContext		*myMergeContext;
};

#endif
