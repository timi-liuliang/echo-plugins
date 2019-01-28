/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Channel library (C++)
 *
 * COMMENTS:	Collection for channel library
 *
 */

#ifndef __CH_Collection_h__
#define __CH_Collection_h__

#include "CH_API.h"
#include "CH_EventManager.h"
#include "CH_ExprLanguage.h"
#include "CH_Manager.h"
#include "CH_Types.h"
#include <EXPR/EX_Error.h>
#include <UT/UT_Array.h>
#include <UT/UT_ArrayStringMap.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Packet.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>


class CH_Channel;
class CH_LocalVariable;

class UT_IStream;
class UT_String;
class UT_StringArray;
class UT_XformOrder;


typedef UT_ArrayStringMap<CH_Channel *> CH_ChannelMap;

typedef enum {
    CH_OS,
    CH_CH,
    CH_LT,
    CH_IT,
    CH_OT,
    CH_LIT,
    CH_LOT,
    CH_PREV_IT,
    CH_NEXT_OT,
    CH__SEGP,
    CH__PSEGP,
    CH__NSEGP,
    CH_IV,
    CH_OV,
    CH_IM,
    CH_OM,
    CH_IA,
    CH_OA,
    CH_PENDING,
} CH_LOCAL_VARIABLE;

enum {
    CH_LOCAL_VARIABLE_TABLE_INDEX = 1
};

// Forward declare of OP_Channels so that we can provide a mechanism to
// cast to it safely.
class OP_Channels;

class CH_API CH_CollectionIterator
{
// Starting at the current input stream position, tries to load
// the information about the next collection from the chn or bchn file
// that the input stream represents.
public:
    CH_CollectionIterator(UT_IStream &is, int binary);
    bool begin(UT_StringHolder &path);
    bool end();
    bool nextCollection(UT_StringHolder &path);

private:
    UT_IStream *curr_is;
    int is_binary;
};

class CH_API CH_Collection
{
public:
	     CH_Collection(CH_Manager *manager);
	     CH_Collection(CH_Manager *manager, const CH_Collection &from);
    virtual ~CH_Collection();

    virtual int64 getMemoryUsage(bool inclusive) const;

    SETUPCLASSPACKETID("CH_Collection");

    CH_Manager	*getManager()   const { return myManager; }
    fpreal	 getTolerance() const { return myManager->getTolerance(); }

    virtual int	 canAccess(uint mask) const;
    virtual int	 canAccessChannel(uint mask, const CH_Channel *chp) const;

    exint 		 entries() const { return myChannelMap.size(); }
    
    //
    // When getting channels, it's the channel name that is passed,
    // NOT the path
    //
    const CH_Channel	*getChannel(const UT_StringRef &name) const;
    CH_Channel		*getChannel(const UT_StringRef &name);
    const CH_Channel	*getChannel(int idx) const { return myChannels(idx); }
    CH_Channel		*getChannel(int idx)	   { return myChannels(idx); }

    int			 getChannels(CH_ChannelList &chans, 
				     int scope=0) const;

    int			 getChannels(CH_ChannelList &chans,
				     const char *pattern) const;

    int			 getChannelNames(CH_NameList &list, int scope=0) const;
    int			 getChannelCount(int scoped = 0) const;

    //
    // Get a collection or channel given a relative path from this collection
    //
    virtual CH_Collection	*findCollection(const char *path) const = 0;
    CH_Channel			*findChannel   (const char *path) const;
    CH_Channel			*findOrCreateChannel   (const char *path);


    //
    // When adding a channel, adding by name is the safest method since
    //
    CH_Channel		*addChannel(const UT_StringHolder &name);
    CH_Channel		*addChannel(CH_Channel *chp);

    CH_Channel		*dupChannel(const UT_StringRef &name);
    virtual CH_Channel	*dupChannel(const CH_Channel &from);

    // The following method will rename a channel.  Only "spare" channels are
    // allowed to be re-named.  The renaming will fail if the channel is not a
    // spare channel, or if the channel isn't found.
    virtual int		 rename(const char *from_name, const char *to_name,
				const char *to_alias);
    virtual int		 rename(CH_Channel *from, const char *to_name,
				const char *to_alias);

    void		 deleteChannel(CH_Channel *chp);
    int			 deleteChannels();

    //
    // Methods to get a channel evaluated
    //
    fpreal		 evaluate(CH_Channel *chp, fpreal time,
				  bool no_disabling /*=false*/, int thread);
    void		 evaluate(CH_Channel *chp, fpreal from, fpreal to,
				  fpreal *vals, int nsamples,
				  bool no_disabling /*=false*/,
				  bool use_cache /*=false*/, int thread);
    void		 evaluateString(UT_String &result,
					CH_Channel *chp, fpreal time,
					int thread);

    //
    // Reparses all the expressions.  Should be done if the local
    // variables have changed.
    //
    void		 unresolveLocalVars(int thread);

    //
    // Now, some common mirrors for system calls
    //
    virtual UT_StringHolder getFullPath() const = 0;
    void	 	 getFullPath(UT_String &path) const;
    void		 getFullPath(const CH_Channel *chp, UT_StringHolder &p,
				     bool use_alias=false) const;
    void		 getFullPath(const CH_Channel *chp, UT_String &p,
				     bool use_alias=false) const
			 {
			     UT_StringHolder s;
			     getFullPath(chp, s, use_alias);
			     p.adopt(s);
			 }

    //
    // channelFind() & collectionFind() will traverse the channel tree
    // for the given path, calling the callback on every match of the
    // specified type.  For example:  /o*/g*/t?
    //
    void		 channelFind(const char *path, CHfindCallBack callback,
				void *data = nullptr) const;

    // These methods will do word finding/replacement in all expressions of
    // all channels.
    void		 findString(const char *word, bool fullword,
				    bool usewildcards, CHfindCallBack callback,
				    void *data = nullptr);
    int			 findString(const char *word, bool fullword,
				    bool usewildcards) const;
    int			 changeString(const char *from, const char *to,
				      bool fullword, int thread);

    //
    // The following pure virtuals must be defined by a sub-class.
    // These provide the collection name and the channel hierarchy.
    //
    virtual const UT_String	&getName() const = 0;

    virtual int			 getUniqueId() const;
    virtual CH_Collection *	 lookupCollection(int id) const;

    virtual const CH_Collection	*getParent() const = 0;
    virtual	  CH_Collection	*getParent() = 0;

    virtual const CH_Collection	*getChild(const char *name) const = 0;
    virtual	  CH_Collection	*getChild(const char *name) = 0;


    // This is a mean of accessing the node owner from CH library
    // It's used only the know if the owner is valid and OP_Node isn't used
    virtual const void	*getOwner() const = 0;
    virtual	  void	*getOwner() = 0;

    //
    //  These methods only get child collections, not channels
    virtual int		 getChildren(CH_NameList &names) const =0;
    virtual int		 getChildren(CH_CollectionList &grps) const =0;


    virtual bool	 getVariableValue(UT_String &str,
					  int table, int index,
					  int thread);
    virtual bool	 getVariableValue(fpreal &val,
					  int table, int index,
					  int thread);

    virtual const CH_LocalVariable *resolveVariable(const char *name, int &tab);

    //
    // I/O methods
    //
    int			 save(std::ostream &os, int binary, bool compiled, 
       			      int scope, 
       			      const CH_ChannelList *channels = nullptr) const;
    int			 saveToFile(const char *nm) const;

    template <typename FPREAL_TYPE>
    bool		 load(UT_IStream &is, const char *path = 0,
			      const CH_LoadInfo *load_info = nullptr,
			      CH_ChannelList *loadedChannels = nullptr,
			      bool delete_keys = false);
    int			 loadFromFile(const char *nm);

    //
    //  Keyframe management & convenience function
    //
    fpreal		 findKey(fpreal gtime, int dir) const;
    virtual void	 saveKeyFrameForUndo(CH_Channel *chp, fpreal gtime) = 0;

    // int			 anyModified() const;
    // void		 clearModified();

    int			 getChanged() const { return myModifiedFlag; }
    virtual void	 setChanged(CH_CHANGE_TYPE how, void *data = nullptr);

    void		 setExpose( bool onoff );
    bool		 getExpose() const  { return myExpose; }

    //
    // Remove and delete all channels
    //
    void		 clearAndDestroy();

    void		 initManager(CH_Manager *manager);

    fpreal		getGlobalStart() const		
			{ return myManager->getGlobalStart(); }
    fpreal		getGlobalEnd() const	
			{ return myManager->getGlobalEnd(); }

    // returns the 'range' of the collection - the lowest start time and the
    // highest end time. Returns false if the collection has no 'range'.
    // Single expressions do not set the range, but will set the timedep
    // flag if necessary. The timedep flag is set if even one of the channels
    // is time dependent.
    bool		 getCollectionRange(fpreal &start, fpreal &end,
					    bool &timedep, int thread);

    void		 addDynamicVariables(CH_LocalVariable *table)
			 { myVariables[3] = table; }


    // This method should only be called by CH_Manager. It signifies
    // this collection is dependent on a param function and will be
    // calledback on modification until removeParamDependency is called.

    void		 addedParamDependency();
    void		 removeParamDependency();

    // Returns the xform order for this collection given a specific channel.
    // This defaults to XYZ, but can be overridden by OP_Node types
    // to handle their specific orders, such as with Objects.
    virtual void 	 getXformOrder(UT_XformOrder &xord, const CH_Channel *chp) const;

    // The "external" flag marks this CH_Collection object as not belonging to
    // any node (ie. standalone).
    // @{
    bool    		 getExternal() const
			    { return myExternal; }
    void		 setExternal(bool yesno)
			    { myExternal = yesno; }
    // @}

    // Set the alias on a channel -- returns true if the alias was 
    // successfully set and false otherwise
    bool		 setChannelAlias(CH_Channel &chp,
					 const UT_StringHolder &alias);

    // Returns true if s represents a valid Channel name and false otherwise
    static bool		 isValidChannelName(const char *s);

    // Returns a valid pointer if this instance is of an OP_Channel and
    // nullptr otherwise.
    virtual OP_Channels	*castToOPChannels() { return nullptr; }
    virtual const OP_Channels	*castToOPChannels() const { return nullptr; }

    // Overridden by OP_Channels to add dependencies for local variables
    virtual void	 addLocalVarDependency(int table, int index,
					       void *ref_id);

    CH_ExprLanguage	 getExprLanguage() const;
    void		 setExprLanguage(CH_ExprLanguage language);
    
    virtual void	 getLocalVariables(UT_StringArray &out_vars);

protected:

    // checkExpressionError() is split up into a checking part and an actual
    // error handling part, evaluationError() so that the checking part can
    // be inlined.
    SYS_FORCE_INLINE void
    checkEvaluationError(const CH_Channel *chp, int thread) const
    {
	int err = ev_GetErrNo(thread);
	if (err != EV_NOERROR)
	    evaluationError(chp, err, thread);
    }

    void		 addLocalVariables(CH_LocalVariable *table)
			 { myVariables[2] = table; }
    const CH_Collection	*getRoot() const;
    //
    // Don't publicize our implementation:
    //
    const CH_ChannelList *getChannels() const { return &myChannels; }

    virtual void	 notifyChannelRenameDependents(const char *old_name,
						       const char *new_name);

private:
    CH_Manager		*myManager;
    CH_LocalVariable	*myVariables[4];	// Local var tables
    CH_ChannelMap 	 myChannelMap;
    CH_ChannelList	 myChannels;		// For easy traversal
    UT_UniquePtr<CH_ChannelMap>
			 myChannelAliasesTable;

    int			 myModifiedFlag;	// My modified flag

    CH_ExprLanguage	 myExprLanguage;

    char		 myParamDependent;	// relies on param function

    bool		 myExternal;
    bool		 myExpose;

    // evaluationError() generates the appropriate call to opError() and then
    // clears the expression error state.
    void		 evaluationError(const CH_Channel *chp, int err,
					 int thread) const;

    void		 deleteChannel(exint index);	// Delete from array

    template <typename FPREAL_TYPE>
    CH_Channel *	 loadChannelRange(UT_IStream &is, const char *name,
					  const CH_LoadInfo &load_info);

    friend class	 CH_Manager;
};

CH_API extern void
CHcollectionsToOPchannels( CH_CollectionList const& collections,
				UT_ValArray< OP_Channels* > &op_channels );

#endif
