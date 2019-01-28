/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ParmList.h (Parameter Library)
 *
 * COMMENTS:
 *		This class is in charge of organizing PRM_Parms.
 *		It also handles variables associated with a group
 *		of parameters.
 */

#ifndef __PRM_ParmList__
#define __PRM_ParmList__

#include "PRM_API.h"
#include "PRM_ChanState.h"
#include "PRM_Name.h"
#include "PRM_Parm.h"
#include "PRM_Template.h"
#include <CH/CH_EventManager.h>
#include <CH/CH_Types.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Inline.h>

#include <iosfwd>


class UT_TokenString;
class UT_StringArray;
class CH_Collection;
class PRM_Multi;
class PRM_PresetInfo;
class PRM_ParmNameMap;
class PRM_OpSharedInfo;

typedef enum {
    PRM_RALL	= 0004,		/// Read, write, execute all
    PRM_WALL	= 0002,
    PRM_XALL	= 0001,
    PRM_RGRP	= 0040,		/// Read, write, execute group
    PRM_WGRP	= 0020,
    PRM_XGRP	= 0010,
    PRM_RUSR	= 0400,		/// Read, write, execute user
    PRM_WUSR	= 0200,
    PRM_XUSR	= 0100,

    PRM_RWXALL	= 0007,
    PRM_RWXGRP	= 0070,
    PRM_RWXUSR	= 0700,
    PRM_RWX	= 0777,

    PRM_READ_OK		= (PRM_RALL|PRM_RGRP|PRM_RUSR),
    PRM_WRITE_OK	= (PRM_WALL|PRM_WGRP|PRM_WUSR),
    PRM_EXECUTE_OK	= (PRM_XALL|PRM_XGRP|PRM_XUSR)
} PRM_Permission;

class PRM_API PRM_ORCallback
{
public:
    virtual ~PRM_ORCallback() {}

    virtual fpreal getFloatOverride( int data_idx, fpreal time,
	    int *tdepend, int *enable ) = 0;
    virtual bool setFloatOverride( int data_idx, fpreal time,
	    int *tdepend, int *enable, fpreal newvalue ) = 0;

    virtual exint getIntOverride( int data_idx, fpreal time,
	    int *tdepend, int *enable ) = 0;
    virtual bool setIntOverride( int data_idx, fpreal time,
	    int *tdepend, int *enable, exint newvalue ) = 0;

    virtual UT_StringHolder getStringOverride( int data_idx, fpreal time,
	    int *tdepend, int *enable ) = 0;
    virtual bool setStringOverride( int data_idx, fpreal time,
	    int *tdepend, int *enable, const UT_StringHolder &newvalue ) = 0;
};

class PRM_API PRM_ParmList
{
public:
			 PRM_ParmList(PRM_Template *thetemplates,
				      CH_Collection *thechgroupptr,
				      PRM_ParmOwner *node,
				      PRM_PresetInfo *presetinfo,
				      const UT_StringRef &optype,
				      bool clear_channels);
			 /// Constructor used to make a new parm list when
			 /// loading a new PRM_Template.  This assumes that the
			 /// channel collection and variables remains the same.
			 PRM_ParmList(PRM_ParmList *thesrcptr,
				      PRM_Template *thenewtemplate,
				      PRM_ParmOwner *node,
				      PRM_PresetInfo *presetinfo,
				      const UT_StringRef &optype,
				      bool leavedefaultsunchanged);
			~PRM_ParmList();


    int			 getEntries() const { return myParms.entries(); }

    // These two must be redundant now that the constructor has
    // the channel collection in it.
    void		 setChannelGroupPtr(CH_Collection *thechgroupptr)
			    { myChannelGroupPtr = thechgroupptr; }
    CH_Collection	*getChannelGroupPtr() const
			    { return myChannelGroupPtr; }

    fpreal		 getEvalTime(int thread) const;

    fpreal		 findNextKey(fpreal theoldtime);
    fpreal		 findPrevKey(fpreal theoldtime);

    /// returns 1 if all the parms in the list are at their default values
    /// returns 0 otherwise
    int			 allDefaults() const;

    // This will call revertToDefaults on parms which haven't had their
    // defaults set, thereby delaying the creation of multiparm entries until
    // after we have built all our templates.
    void		 buildDelayedDefaults( bool clear_channels = true );

    PRM_Parm		*getParmPtr(const UT_StringRef &theparmname,
				    int &theparmidx);
    PRM_Parm		*getParmPtr(const UT_StringRef &theparmname);
    const PRM_Parm	*getParmPtr(const UT_StringRef &theparmname) const;
    PRM_Parm		*getParmPtr(const PRM_Name &name);
    const PRM_Parm	*getParmPtr(const PRM_Name &name) const;

    PRM_Parm		*getParmPtr(const int theindex)
			 {
			     return (theindex >= 0 && theindex < myParms.entries())
					     ? myParms(theindex)
					     : 0;
			 }
    const PRM_Parm	*getParmPtr(const int theindex) const
			 {
			     return (theindex >= 0 && theindex < myParms.entries())
					     ? myParms(theindex)
					     : 0;
			 }


    int			 getParmIndex(const PRM_Parm *parm) const;
    int			 getParmIndex(const UT_StringRef &parmname) const;
    int			 getParmIndex(const PRM_Name &name) const
			    { return getParmIndex(name.getTokenRef()); }

    SYS_FORCE_INLINE
    const PRM_Parm	*getParmPtrFromChannel(
			    const UT_StringRef &channel,
			    int *vecidx = nullptr) const
			 {
			     int i = getParmIndexFromChannel(
					channel, vecidx, /*allow_alias*/true);
			     if (i < 0)
				 return nullptr;
			     return myParms(i);
			 }
    SYS_FORCE_INLINE
    PRM_Parm		*getParmPtrFromChannel(
			    const UT_StringRef &channel,
			    int *vecidx = nullptr)
			 {
			     int i = getParmIndexFromChannel(
					channel, vecidx, /*allow_alias*/true);
			     if (i < 0)
				 return nullptr;
			     return myParms(i);
			 }
    int			 getParmIndex(const UT_StringRef &channel_name,
				      int &subidx,
				      bool allow_alias = true) const
			 {
			    return getParmIndexFromChannel(
					    channel_name, &subidx, allow_alias);
			 }
    int			 getParmIndexFromChannel(const UT_StringRef &channel,
						 int *vecidx,
						 bool allow_alias) const;

    // Clear the dirty flags for all parameters, or query if all dirty flags
    // are clear.
    void		 clearAllDirtyFlags();
    int			 areAllFlagsClean() const;
    void		 clearAllUndoSavedFlags();

    void		 revertToDefaults(fpreal time);

    void		 revertValuesToFactoryDefaults();

    // When the PRM_Template changes (i.e. reloading a new operator
    // definition), the new PRM_ParmList is created and then this method is
    // called to copy over any pertinent information from the old PRM_ParmList.
    void		 updateFromOldTemplate(PRM_ParmList *oldlist);
    // When the spare parameter definitions for a node changes, this function
    // is called to update the parameters to be in line with the new set of
    // templates. The old templates must still exist at this point.
    void		 updateSpareParmTemplates(PRM_Template *newtemplates,
						  bool leavedefaultsunchanged);

    void		 saveSingle(PRM_Parm *, std::ostream &os,
				int binary) const;
    void		 save(std::ostream &os,
				int binary, bool compiled) const;
    bool		 load(UT_IStream &is, const char *path,
				PRM_ParmList *obsoleteparms);

    void		 saveUndoData(PRM_UndoDataList &data) const;
    void		 saveSingleUndoData(const PRM_Parm *parm,
					    PRM_UndoDataList &data) const;
    void		 loadUndoData(const PRM_UndoDataList &data,
				      const char *path,
				      PRM_ParmList *obsoleteparms);

    /// Methods to load and save channel aliases
    /// @{
    void		 saveChannelAliases(std::ostream &os, int binary);
    bool		 loadChannelAliases(UT_IStream &is, const char *path);
    /// @}
    /// Returns true if any of our parms have an alias and false otherwise
    bool		 hasChannelAliases() const;

    void		 clearAndDestroyChannels();
    int			 hasAnyChannels();
    int			 addChannel(const UT_StringRef &name);
    void		 reloadChannelPtrs();
    int			 channelPtrModified(const UT_StringRef &name);

    unsigned		 getUserMask() const	{ return myUserMask; }
    void		 setUserMask(unsigned m)
			    { myUserMask = m & PRM_RWX; }
    unsigned		 getPermissions() const	{ return myPermissions; }
    void		 setPermissions(unsigned mask)
			    { mask &= PRM_RWX;
			      if (mask != myPermissions)
			      {
				  myPermissions = mask;
				  sendChangeEvent(CH_PERMISSION_CHANGED);
			      }
			    }
    unsigned		 getAssetPermissionMask() const
			    { return myAssetPermissionMask; }
    void		 setAssetPermissionMask(unsigned mask)
			    { mask &= PRM_RWX;
			      if (mask != myAssetPermissionMask)
			      {
				  myAssetPermissionMask = mask;
				  sendChangeEvent(CH_PERMISSION_CHANGED);
			      }
			    }

    // By default, the canAccess method just tests to see if the OP is
    // writeable.
    int			 canAccess(unsigned mask=PRM_WRITE_OK,
				    const PRM_Type &type=PRM_TYPE_ZERO) const;
    int			 canAccessIgnoringAssetMask(
				    unsigned mask=PRM_WRITE_OK,
				    const PRM_Type &type=PRM_TYPE_ZERO) const;
    int			 canAccessCheckingOnlyAssetMask(
				    unsigned mask=PRM_WRITE_OK,
				    const PRM_Type &type=PRM_TYPE_ZERO) const;

    static void		 beginLoading();
    static void		 endLoading();
    static bool		 isLoading();

    void		 hardenChanges(fpreal time, bool forceflag = 0,
	    			       const char *patt=0,
				       CH_ChannelRefList *list=0);

    PRM_ChanState	 getChanState(fpreal time);
    void		 sendChangeEvent(CH_CHANGE_TYPE what, PRM_Parm *who=0);
    void		 setEventHandler(void *data,
				void (*handler)(void *, CH_CHANGE_TYPE, int))
			 {
			     myEventObject = data;
			     myEventHandler = handler;
			 }

    /// If the parm list is not already flagged as time dependent then
    /// checkTimeDependence will scan the parms for time dependencies and
    /// return the new time dependence flag.
    /// @note Expressions will not be flagged as time dependent until
    /// they're evaluated.  So, if it's possible that the channels have not
    /// been evaluated, set evaluate_channels to true.
    int			 checkTimeDependence(bool evaluate_channels = false);

    SYS_FORCE_INLINE
    void		 setTimeDependent(bool onOff)
    			 { myTimeDependent=onOff;}
    SYS_FORCE_INLINE
    bool		 getTimeDependent() const
			 { return myTimeDependent; }
    SYS_FORCE_INLINE
    void		 setCookTimeDependent(bool onOff)
    			 { myCookTimeDependent=onOff;}
    SYS_FORCE_INLINE
    bool		 getCookTimeDependent() const
			 { return myCookTimeDependent; }

    void		 addContextOptionDep(const UT_StringHolder &opt)
			 { if (!opt.isstring()) return;
			     DEPcreateContextOptionDepsFromPtr(
				 myContextOptionDeps,
				 &myContextOptionDepsLock).insert(opt); }
    void		 clearContextOptionDeps()
			 { DEP_ContextOptionDepsLockScope scope(
			    myContextOptionDepsLock);
			    myContextOptionDeps.reset(); }
    const DEP_ContextOptionDeps	&getContextOptionDeps() const
			 { return DEPgetContextOptionDepsFromPtr(
				myContextOptionDeps); }
    void		 addCookContextOptionDep(const UT_StringHolder &opt)
			 { if (!opt.isstring()) return;
			     DEPcreateContextOptionDepsFromPtr(
				 myCookContextOptionDeps,
				 &myContextOptionDepsLock).insert(opt); }
    void		 clearCookContextOptionDeps()
			 { DEP_ContextOptionDepsLockScope scope(
			    myContextOptionDepsLock);
			    myCookContextOptionDeps.reset(); }
    const DEP_ContextOptionDeps	&getCookContextOptionDeps() const
			 { return DEPgetContextOptionDepsFromPtr(
				myCookContextOptionDeps); }

    // the override callback is static
    static PRM_ORCallback	*getORCallback(void) { return theORCallback; }
    static void		 	 setORCallback(PRM_ORCallback *impl) 
    			 	 { theORCallback = impl; }

    void		 pushVariables();	// These should only be used
    void		 popVariables();	// by PSI for syntax reasons

    int			 findString(const char *str, bool fullword,
				    bool usewildcards) const;
    int			 changeString(const char *from, const char *to,
				      bool fullword);

    bool		 notifyVarChange(const char *varname);

    /// This used to be called getSendExtraEvent()
    int			 getNumSendExtra() const
			    { return myNumSendExtra; }
    void		 bumpNumSendExtra(int n)
			    { myNumSendExtra += n; }

    /// Return a unique identifier for the current parameter settings. If this
    /// is called outside of a cook, place a PRM_AutoBlockErrors around the
    /// call.
    void		 getMagicString(UT_TokenString &string,
					fpreal t, int parm_group = 1);

    void		 setUndoSavedFlag()	{ myUndoSavedFlag = true; }
    void		 setAutoTakeUndoSavedFlag()
					{ myAutoTakeUndoSavedFlag = true; }

    // Appends a block of multiparm instance parameters.
    void		 appendMultiParms(const PRM_Multi &multiparmowner,
				     PRM_Template *templ, int num, bool spare,
				     const UT_IntArray &instance,
				     UT_ValArray<PRM_Parm *> &newparms);

    void		 removeAndDestroyParm(int index);

    void		 insertMultiParmItem(int index, int i);
    void		 removeMultiParmItem(int index, int i);

    PRM_ParmOwner	*getParmOwner() const	{ return myParmOwner; }
    PRM_PresetInfo	*getPresetInfo() const	{ return myPresetInfo; }
    void		 clearParmOwner();

    /// checkMultiParmInstance() will return true if the given parm name matches
    /// a multi-parm instance name. If the multi-parm instance isn't created
    /// yet, then it will be created before this function returns. In that case,
    /// do_owner_init specifies if the new instance should be initialized.
    bool		 checkMultiParmInstance(const UT_StringRef &parmname,
						bool do_owner_init);

    /// This method is used to alias a channel name to a another name
    /// Returns true if successful and false otherwise.
    /// NOTE: you should not need to use this directly, you should instead
    /// call the OP_Node version.
    bool		 setChannelAlias(PRM_Parm &parm, int subindex,
					 const UT_StringHolder &chan_alias);

    int64		 getMemoryUsage(bool inclusive) const;

    unsigned long	 getAddRemoveCount() const
			    { return myAddRemoveCount; }
    void		 setAddRemoveCount(unsigned long count)
			    { myAddRemoveCount = count; }

    /// Convenience functions for evaluating and setting parameters
    /// @{
    void		 evalStringRaw(UT_String &value,
				       const UT_StringRef &pn, int vi,
				       fpreal t);
    void		 evalString(UT_String &value,
				    const UT_StringRef &pn, int vi, fpreal t);
    void		 setString(const UT_String &value,
				   CH_StringMeaning meaning,
				   const UT_StringRef &pn, int vi, fpreal t,
				   bool propagate);
    int			 evalInt(const UT_StringRef &pn, int vi, fpreal t);
    void		 setInt(int value, const UT_StringRef &pn, int vi,
				fpreal t, bool propagate);
    fpreal		 evalFloat(const UT_StringRef &pn, int vi, fpreal t);
    void		 setFloat(fpreal value, const UT_StringRef &pn, int vi,
				fpreal t, bool propagate);
    /// @}


    DEP_MicroNode &	 parmMicroNode(int parm_idx, int vi);
    bool		 hasParmMicroNode(int parm_idx) const;

    /// Get all parm micronodes for the entire parm list
    void		 getParmMicroNodes(DEP_MicroNodeList &micronodes) const;

    void		 propagateDirtyParmMicroNodes(
			    int parm_index, // can be -1 for all
			    DEP_MicroNode::Visitor client_visit,
			    DEP_MicroNode *parmlist_micronode,
			    bool force);

    void		 clearMicroNodeInputs();
    void		 getParmInputList(
			    DEP_MicroNodeList &inputs,
			    bool cooking_only = false) const;
    int			 getNumParmInputs() const;
    void		 getParmOutputList(
			    DEP_MicroNodeList &outputs,
			    bool cooking_only = false) const;

    void		 dumpMicroNodes(
			    std::ostream &os,
			    bool as_DOT,
			    int indent_level=0) const;
    int64		 getMicroNodesMemoryUsage() const;

    // This function removes the shared parmhash info for the specified
    // operator from the shared info table. The data will get deleted
    // as soon as the last PRM_ParmList with a pointer to the shared
    // info gets deleted.
    static void		 removeOpSharedInfo(const UT_StringRef &optoken);
    // This functions changes the name under which some shred parm
    // hash info is stored in our table. This is done when the name
    // of an operator type changes.
    static void		 renameOpSharedInfo(const UT_StringRef &oldoptoken,
					    const UT_StringHolder &newoptoken);

    PRM_Parm	*getParmPtrInst(const UT_StringRef &parm_name,
				const int *instance,
				int nest_level)
    {
	int unused;
	const PRM_Parm *p = static_cast<const PRM_ParmList*>(this)
	    ->getParmPtrInst(parm_name, instance, nest_level, unused);
	return const_cast<PRM_Parm*>(p);
    }
    PRM_Parm	*getParmPtrInst(const UT_StringRef &parm_name,
				const int *instance,
				int nest_level,
				int &theparmidx)
    {
	const PRM_Parm *p = static_cast<const PRM_ParmList*>(this)
	    ->getParmPtrInst(parm_name, instance, nest_level, theparmidx);
	return const_cast<PRM_Parm*>(p);
    }
    const PRM_Parm	*getParmPtrInst(const UT_StringRef &parm_name,
				const int *instance,
				int nest_level) const
    {
	int unused;
	return getParmPtrInst(parm_name, instance, nest_level, unused);
    }
    const PRM_Parm	*getParmPtrInst(const UT_StringRef &parm_name,
				const int *instance,
				int nest_level,
				int &theparmidx) const;

private: // forbid usage of these functions
    PRM_ParmList(const PRM_ParmList &copy);
    PRM_ParmList &operator =(const PRM_ParmList &copy);

    void		 moveMultiParmAlias(int index, int i, bool up);
    void		 loadUnknownParmNames(UT_IStream &is,
					UT_StringArray &names,
					PRM_ParmList *obsoleteparms);
    PRM_Parm		*findLoadParm(const UT_StringRef &name,
					PRM_ParmList *obsoleteparms,
					PRM_ParmNameMap *nmap,
					bool try_again=true);

    // Adopt the common parms (including animating parms) from the source.
    // Note that the source parm list will be changed.
    void		 adoptParmList(
				PRM_ParmList &thesource,
				bool leavedefaultsunchanged);

    static bool		 shouldCopyOnAdopt(
				PRM_Template &dst_template,
				PRM_Parm &srcparm,
				bool leavedefaultsunchanged);

    // Adopt the values and other important information from one parm into
    // another. The parms don't have to be from the same PRM_ParmList, but
    // they can be. This allows this function to be used from the adopt()
    // function above, or from updateSpareParms(). Note that the source
    // parm will be changed.
    void		 adoptParm(
				PRM_Parm &dstparm,
				PRM_Parm &srcparm,
				bool leavedefaultsunchanged);

    void		 adoptChannelAlias(
				PRM_Parm &dst_parm, 
				const PRM_Parm &src_parm);

    PRM_Parm		*appendParm(
				PRM_Parm *parm,
				const PRM_Template &temp);
    PRM_Parm		*createMultiInstanceParm(
				PRM_Template &temp,
				bool spare,
				const PRM_Multi &multiparmowner);
    PRM_Parm		*createSpareParm(
				PRM_Template &temp);

    int			 findChannelIndex(const UT_StringRef &chan_name,
					  bool allow_alias) const;

private: // data
    UT_ValArray<PRM_Parm *>	 myParms;

    unsigned long		 myAddRemoveCount;

    // The following member data holds a pointer to shared hash tables.
    // This is a hash table of all the parameters for this op type. This
    // table is shared by all operators of this type. This implies two
    // things. First, it doesn't contain spare parms or multiparm instances.
    // Second, all operators must keep their "standard" parms always at
    // the start of myParms (before any spare or multi parms). Otherwise
    // The indexes for these parms would be different for each node.
    // NOTE: The channel table is not a list of active animated channels,
    // but a list of all possible channel names.
    UT_SharedPtr<PRM_OpSharedInfo> myOpSharedInfo;

    // This is the hash table for holding the parm indices of spare parms
    // and multiparm instances for this node. It is not shared, obviously.
    // NOTE: The channel table is not a list of active animated channels,
    // but a list of all possible channel names.
    UT_ArrayStringMap<int>	 mySpareAndMultiParmHashTable;
    UT_ArrayStringMap<int>	 mySpareAndMultiChannelTable;

    // This is the symbol table of channel aliases. It is also not shared,
    // as channel aliases are per-node not per-type. Because channel aliases
    // are always per-node, we don't need separate tables for regular parms
    // and spare/multi parms.
    UT_ArrayStringMap<int>	 myChannelAliasesTable;

    // Names of state vars on which parms in this parm list are dependent.
    DEP_ContextOptionDepsLock	 myContextOptionDepsLock;
    DEP_ContextOptionDepsPtr	 myContextOptionDeps;
    DEP_ContextOptionDepsPtr	 myCookContextOptionDeps;

    unsigned			 myPermissions;
    unsigned			 myUserMask;
    unsigned			 myAssetPermissionMask;

    CH_Collection		*myChannelGroupPtr;
    PRM_ParmOwner		*myParmOwner;
    PRM_PresetInfo		*myPresetInfo;

    void			(*myEventHandler)(void *, CH_CHANGE_TYPE, int);
    void			*myEventObject;

    // myNumSendExtra is incremented whenever a parameter that has its dirty
    // and sendExtra flags set. It should be set from the PRM_Parm::setValue
    // code paths and it should be decremented after a OP_PRM_CHANGED or
    // equivalent event has been sent out.
    int				 myNumSendExtra;

    // This are promoted to full bool's for better performance
    bool			 myTimeDependent;
    bool			 myCookTimeDependent;

    bool			 myUndoSavedFlag;
    bool			 myAutoTakeUndoSavedFlag;

    static PRM_ORCallback	*theORCallback;
    static fpreal32		 theFileVersion;
    static int			 thePRMLoadCount;
};

#endif
