/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Instance.h (Parameter Library)
 *
 * COMMENTS:
 *		A PRM_Instance contains all the variable data relating to
 *		parameters. In other words it contains the actual values
 *		of the parameter. The types and format of the values are
 *		specified in the PRM_Template portion of a parameter. A
 *		PRM_Instance is to a PRM_Template as an object is to a
 *		class in C++.
 */

#ifndef __PRM_Instance_h__
#define __PRM_Instance_h__

#include "PRM_API.h"
#include "PRM_Callback.h"
#include "PRM_ChanState.h"
#include "PRM_Default.h"
#include "PRM_Error.h"
#include "PRM_KeySetType.h"
#include "PRM_Parm.h"
#include "PRM_ParmList.h"
#include "PRM_Template.h"
#include "PRM_Type.h"

#include <CH/CH_Collection.h>
#include <CH/CH_ExprLanguage.h>
#include <CH/CH_Manager.h>
#include <CH/CH_Types.h>
#include <DEP/DEP_MicroNode.h>

#include <EXPR/EX_Error.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Color.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_SmallObject.h>
#include <UT/UT_TBBSpinLock.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>


class PRM_ChoiceList;
class PRM_Multi;
class PRM_ParmMicroNode;
class PRM_ParmOwner;
class PRM_Range;
class PRM_RefId;
class PRM_Value;
class CH_Channel;
class CH_FullKey;
class UT_IStream;
class UT_JSONValueMap;
class UT_Packet;
class UT_TokenString;
class UT_WorkBuffer;


typedef UT_ValArray<CH_Channel *> CH_ChannelList;

class PRM_API PRM_Instance : public UT_SmallObject<PRM_Instance,
                                           UT_SMALLOBJECT_CLEANPAGES_DEFAULT,
                                           UT_SMALLOBJECT_PAGESIZE_DEFAULT,
					   UT_SMALLOBJECT_THREADSAFE_OFF>
{
public:
			  PRM_Instance(PRM_Template *thetemplateptr,
				       PRM_Parm *parm);
			  PRM_Instance(PRM_Instance *thesrcptr,
				       PRM_Parm *parm);
    virtual		 ~PRM_Instance();

    virtual void	  adopt(PRM_Instance &thesrcinstance);
    virtual void	  adoptOverrides(PRM_Instance &thesrcinstance);

    virtual PRM_Instance *duplicateSelf(PRM_Parm *node) = 0;

    virtual void	  setValueToDefault(fpreal time, int index,
					    PRM_AddKeyType add_key) = 0;
    virtual void	  revertToDefault(int index) = 0;
    virtual void	  revertToDefaults(bool clear_channels =true) = 0;
    virtual void	  restoreFactoryDefault(int index) = 0;
    virtual void	  overwriteDefault(fpreal time, int index) = 0;
    virtual void	  overwriteDefaults(fpreal time) = 0;
    virtual int		  isDefault(int index, enum PRM_DefaultType deftype,
				    bool basiccheck = true) const;
    bool		  isAllDefault(enum PRM_DefaultType deftype) const;
		    
    virtual void          save(std::ostream &os, int binary, bool compiled) const=0;
    virtual bool          load(UT_IStream &is) = 0;

    virtual void	  saveUndoData(PRM_UndoDataHandle &data) const;
    virtual bool	  loadUndoData(const PRM_UndoDataHandle &data);

    virtual void	  saveCommand(std::ostream &os, int values=0, int index = -1)
				      const = 0;
    virtual int		  loadCommand(int argc, const char *const argv[],
					int index,
					bool values_only, 
					PRM_AddKeyType add_key,
					PRM_KeySetType set_type) = 0;

    virtual void	  getValue(PRM_ParmList *parm_list, fpreal time,
				   int32 &value, int index,
				   int thread) const = 0;
    virtual void	  getValue(PRM_ParmList *parm_list, fpreal time,
				   int64 &value, int index,
				   int thread) const = 0;
    virtual void	  getValue(PRM_ParmList *parm_list, fpreal time,
				   fpreal &value, int index,
				   int thread) const = 0;
    virtual void	  getValue(PRM_ParmList *parm_list, fpreal time,
				   UT_String &value, int index,
				   bool expand, int thread) const = 0;
    virtual void	  getValue(PRM_ParmList *parm_list, fpreal time,
				   PRM_DataItemHandle &value, int index,
				   int thread) const = 0;
    virtual void	  getValue(PRM_ParmList *parm_list, fpreal time,
				   UT_JSONValueMap &value, int index,
				   bool expand, int thread) const = 0;
    virtual void	  getValues(PRM_ParmList *parm_list, fpreal time,
				   fpreal32 *value, int thread) const;
    virtual void	  getValues(PRM_ParmList *parm_list, fpreal time,
				   fpreal64 *value, int thread) const;
    virtual void	  getValues(PRM_ParmList *parm_list, fpreal time,
				   int32 *theints, int thread) const;
    virtual void	  getValues(PRM_ParmList *parm_list, fpreal time,
				   int64 *theints, int thread) const;
    virtual void	  getDefaultValue(fpreal &value, int index) const;
    virtual void	  getDefaultValue(int32 &value, int index) const;
    virtual void	  getDefaultValue(int64 &value, int index) const;
    virtual void	  getDefaultValue(UT_String &value, int index) const;

    virtual void	  getExpressionStringForUI(fpreal time,
						   UT_String &value,
						   int index,
						   int thread) const = 0;
    virtual void	  getExpression(fpreal time, UT_String &value,
					int index, bool allow_value,
					int thread) const = 0;
    virtual void          setExpression(fpreal time,
					const char *value,
					CH_ExprLanguage language,
					int index, bool evaluate = true,
					bool rmchannel = false);

    CH_StringMeaning	  getStringMeaning(fpreal time, int index) const;

    // If this parm contains an expression this function returns that
    // expression's language.  Otherwise, it returns the language that it
    // would be if it was turned into an expression.
    CH_ExprLanguage	  getExprLanguageIfMadeAnExpression(fpreal time,
							    int index) const;

    // If there is an expression, this function returns whether the
    // expression's language for the segment at the given time is the same as
    // the node's langauge.  If there is no expression, this function returns
    // true.
    bool		  languageIsNodesLanguage(fpreal time, int index) const;

    // set value of a parameter
    //   setkey - determines how the value is set at keyframes.
    //        Whether the preference is used or always set as pending
    virtual void	  setValue(fpreal time,
				   const char *value,
				   CH_StringMeaning meaning,
				   bool kill_expr = false,
				   int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValue(fpreal time,
				   fpreal value,
				   bool kill_expr = false,
				   const int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValue(fpreal time,
				   int32 value,
				   bool kill_expr = false,
				   const int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValue(fpreal time,
				   int64 value,
				   bool kill_expr = false,
				   const int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValue(fpreal time,
				   const PRM_DataItemHandle &value,
				   bool kill_expr = false,
				   const int index = 0,
				   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    //
    // The setValues methods set all entries in the vector...
    //
    virtual void	  setValues(fpreal time,
				    const fpreal *values,
				    bool kill_expr = false,
				    PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				   PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValues(fpreal time,
				    const int32 *values,
				    bool kill_expr = false,
				    PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				    PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setValues(fpreal time,
				    const int64 *values,
				    bool kill_expr = false,
				    PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
				    PRM_KeySetType setkey = PRM_USE_PREFERENCES);

    virtual void	  setDefaultChannelValue(CH_Channel *channelptr,
						 int index = 0) = 0;

    virtual bool	  isMultiParm() const;
    virtual int		  getMultiParmNumItems() const;
    virtual int		  getMultiParmInstancesPerItem() const;
    virtual const char	 *getMultiParmToken(int p, int index) const;
    // Only PRM_ParmList should directly call these insert/remove methods.
    // All other users, should use the insert/remove methods in OP_Parameter.
    virtual void	  insertMultiParmItem(int index);
    virtual void	  removeMultiParmItem(int index);

    virtual void	  buildOpDependencies(const PRM_RefId &ref, int thread);
    virtual void	  changeOpRef(const char *new_fullpath,
				      const char *old_fullpath,
				      const char *old_cwd,
				      const char *chan_name,
				      const char *old_chan_name,
				      void (*undo_callback)(void *),
				      void *undo_data,
				      int thread);

    fpreal		  findNextKey(fpreal theoldtime, int index=-1);
    fpreal		  findPrevKey(fpreal theoldtime, int index=-1);

    virtual int		  findString(const char *str, bool fullword,
				     bool usewildcards) const;
    virtual int		  changeString(const char *from, const char *to,
				       bool fullword);

    virtual bool	  notifyVarChange(int subindex, const char *varname);

    virtual bool	  getMagicString(UT_TokenString &magic,
					 PRM_ParmList *list, int size,
					 fpreal t, bool add_if_default /*=true*/,
					 int thread) = 0;

    unsigned		  getEnableState(int comp = -1) const; 
    bool		  setEnableState(bool f, int comp = -1);

    int			  getExpressionState() const
			  {
			      return myBitField & BITFLAG_EXPRESSIONSTATE;
			  }
    void		  setExpressionState(int state)
			  {
			      if (state)
				  myBitField |= BITFLAG_EXPRESSIONSTATE;
			      else
				  myBitField &= ~BITFLAG_EXPRESSIONSTATE;
			  }
    int			  getValueState() const
			  {
			      return myBitField & BITFLAG_VALUESTATE;
			  }
    void		  setValueState(int state)
			  {
			      if (state)
				  myBitField |= BITFLAG_VALUESTATE;
			      else
				  myBitField &= ~BITFLAG_VALUESTATE;
			  }

    virtual bool	  isEditable(PRM_ParmList *thelist, fpreal time,
				     int index) const { return true; }
    virtual bool	  isEditableByUI(PRM_ParmList *thelist, fpreal time,
					 int index) const { return true; }

    CH_Collection	 *getChannelGroupPtr() const
			    {
				return getParmList()
				    ? getParmList()->getChannelGroupPtr()
				    : nullptr;
			    }
    fpreal		 getEvalTime(int thread) const
			    {
				CH_Collection *coll = getChannelGroupPtr();
				if (!coll)
				    return 0.0;
				return coll->getManager()
						    ->getEvaluateTime(thread);
			    }
    fpreal		 getGlobalStartTime() const
			    {
				CH_Collection *coll = getChannelGroupPtr();
				if (!coll)
				    return 0.0;
				return coll->getManager()->getGlobalStart();
			    }

    CH_Channel		 *getChannel(int subindex) const
			    {
				UT_ASSERT(subindex >= 0
					    && subindex < getVectorSize());
				return myChannelPtrs[subindex];
			    }
    void		  setChannel(int subindex, CH_Channel *chp)
			    {
				myChannelPtrs[subindex] = chp;
			    }

    void		  reloadChannelPtrs();
    int			  channelPtrModified(const UT_StringRef &name);
    void		  internalAddChannel(int index,
					     const char *theexpr,
					     CH_ExprLanguage language,
					     bool add_seg = true);
    void		  addChannel(int index);
    int			  addChannel(const UT_StringRef &name);
    void		  addChannel(int index, const char *theexpr,
				     CH_ExprLanguage language);
    int			  addChannel(const UT_StringRef &name, const char *expr,
				     CH_ExprLanguage language);
    bool		  removeChannel(int index);
    int			  getChannelCount() const;

    // Determine if a given component if time/data dependent. Use -1 if you
    // want to know this for any of the components in the parm.
    virtual bool	  isTimeDependent(int subindex) const;
    virtual bool	  isDataDependent(fpreal gtime, int subindex) const;

    virtual void	  setTimeDependent(int subindex, bool timedep);
    // Form for setTimeDependent() for the evaluation code path
    void		  setTimeDependentForEval(int vi, bool timedep) const
			  { SYSconst_cast(this)->setTimeDependent(vi, timedep);}

    bool		  hasContextOptionDeps(int subindex) const;
    const DEP_ContextOptionDeps &getContextOptionDeps(int subindex) const;
    void		  addContextOptionDep(int subindex,
				const UT_StringHolder &opt);

    int			  changePending(int index);
    virtual PRM_ChanState getChanState(fpreal time, int index) const;
    virtual PRM_ChanState getChanState(fpreal time) const;

    // add keyframes if changes are pending
    virtual void	  hardenChanges(fpreal time,
					int index,
					bool theforceflag,
					const char *patt = 0,
					CH_ChannelRefList *list = 0) = 0;

    // sets a key even if no channel exists
    void		  setKey(fpreal time, int index);
    void		  setKey(fpreal time, int index, const char *expr,
				 CH_ExprLanguage language,
				 const char *patt = 0,
				 CH_ChannelRefList *list = 0);

    void		  setFullKey(fpreal gtime, int index,
				CH_FullKey const& key, 
				bool accel_ratios = true);

    unsigned int	  getSaveFlags() const
			  { return myBitField & BITFLAG_SAVE_MASK; }
    void		  setSaveFlags(unsigned int f)
			  { myBitField |= (f & BITFLAG_SAVE_MASK); }

    bool		  getActiveTakeFlag() const
			  { return myBitField & BITFLAG_ACTIVETAKE; }
    bool		  getAlwaysTakeFlag() const
			  { return myBitField & BITFLAG_TAKEALWAYSACTIVE; }
    void		  setActiveTakeFlag(int onoff)
			  {
			      if (onoff) myBitField |= BITFLAG_ACTIVETAKE;
			      else
			      {
				  if (!(myBitField & BITFLAG_TAKEALWAYSACTIVE))
				       myBitField &= ~BITFLAG_ACTIVETAKE;
				  else myBitField |= BITFLAG_ACTIVETAKE;
			      }
			  }

    bool		  getBypassFlag() const
			  { return myBitField & BITFLAG_BYPASS; }
    void		  setBypassFlag(bool onoff)
			  {
			      // Only allow bypass on spare parameters
			      if (isSpareParm() && onoff)
				  myBitField |=  BITFLAG_BYPASS;
			      else
				  myBitField &= ~BITFLAG_BYPASS;
			  }

    bool		  getVisibleState(int comp = -1) const;
    void 		  setVisibleState(bool f, int comp = -1);

    bool		  getUndoSavedFlag()
			  { return myBitField & BITFLAG_UNDOSAVED; }
    void		  setUndoSavedFlag(bool f)
			  { if (f) myBitField |= BITFLAG_UNDOSAVED;
			    else myBitField &= ~BITFLAG_UNDOSAVED;}
    bool		  getAutoTakeUndoSavedFlag()
			  { return myBitField & BITFLAG_AUTOTAKEUNDOSAVED; }
    void		  setAutoTakeUndoSavedFlag(bool f)
			  { if (f) myBitField |= BITFLAG_AUTOTAKEUNDOSAVED;
			    else myBitField &= ~BITFLAG_AUTOTAKEUNDOSAVED;}

    bool		  getSaveAutoScopeFlag() const
			  { return myBitField & BITFLAG_SAVEAUTOSCOPE; }

    bool		  getSaveAutoSelectFlag() const
			  { return myBitField & BITFLAG_SAVEAUTOSELECT; }

    void		  setSaveLabelFlag(bool f);
    bool		  getSaveLabelFlag() const
			  { return myBitField & BITFLAG_SAVELABEL; }

    bool		  getLockedFlag(int vec_idx) const;
    bool		  areAllComponentsLocked() const;
    void		  setLockedFlag(int vec_idx, bool f);
    bool		  getAutoScopedFlag(int vec_idx) const;
    void		  setAutoScopedFlag(int vec_idx, bool f);

    bool		  getDefaultsInitFlag() const
			  { return (myBitField & BITFLAG_DEFAULTSINIT) != 0; }
    void		  setDefaultsInitFlag(bool f)
			  { if (f) myBitField |= BITFLAG_DEFAULTSINIT;
			    else myBitField &= ~BITFLAG_DEFAULTSINIT;}

    unsigned int	  getLockField() const
				{ return myLockField; }
    unsigned int	  getAutoScopeField() const
				{ return myAutoScopeField; }

    virtual int		  setOverride(int index, int data_idx, const char *source, PRM_OverrideType type);
    virtual int		  setOverrideDisable(int index, int onoff);
    virtual const char	 *getOverride(int index) const;
    virtual bool	  getOverrideType(int index, PRM_OverrideType& type) const;
    virtual int		  getOverrideDataIndex(int index) const;

    ///Sometimes we may want to make an existing and enabled override 
    ///temporarily inactive, so that we can "shut off" the override without 
    ///losing any dependencies between the parm and the source of its exported 
    ///override values. (e.g. might want to temporarily change the parm's 
    ///display colour)
    virtual int		  getIsOverrideActive(int index) const; 

    virtual void	  destroyChildParms() { }

    // A parameter becomes dirty whenever its value is set.
    bool		  isDirty(int vec_idx) const
			  { return getFieldBit(myDirtyParmFlags, vec_idx); }
    bool		  areAllFlagsClean() const
			  { return !myDirtyParmFlags; }
    void		  clearAllDirtyFlags()
			  { myDirtyParmFlags = 0; }
    void		  setDirtyFlags(int vec_idx)
			  {
			      setFieldBit(myDirtyParmFlags, vec_idx, true);
			      setSendExtra(vec_idx);
			  }

    // mySendExtraFlags is a per-component bitfield that has its bit set
    // if it needs to be dirty propagated.
    bool		  isSendExtra(int vi) const
			  { return getFieldBit(mySendExtraFlags, vi); }
    bool		  hasAnySendExtraFlags() const
			  { return (mySendExtraFlags != 0); }
    void		  setSendExtra(int vi)
			  {
			      if (!getFieldBit(mySendExtraFlags, vi))
			      {
				  setFieldBit(mySendExtraFlags, vi, true);
				  getParmList()->bumpNumSendExtra(
					vi < 0 ? getVectorSize() : +1);
			      }
			  }

    // The input notify value is not created by default.  Call
    // createInputNotifyValue when you need it.  Handles use this
    // notify value to add an interest in input fields so they can know
    // when the input has changed.  Op's trigger this notify value when
    // they generate their input groups, if this value exists.
    void		  createInputNotifyValue();
	  PRM_Value	 *inputNotifyValue()	   { return myNotifyValue; }
    const PRM_Value	 *inputNotifyValue() const { return myNotifyValue; }

    const PRM_Type	&getType() const
			 { return myTemplatePtr->getType(); }
    PRM_TypeExtended	 getTypeExtended() const
			 { return myTemplatePtr->getTypeExtended(); }
    PRM_MultiType	 getMultiType() const
			 { return myTemplatePtr->getMultiType(); }
    fpreal		 getMultiSize() const
			 { return myTemplatePtr->getMultiSize(); }
    int			 getMultiStartOffset() const
			 { return myTemplatePtr->getMultiStartOffset(); }
    const PRM_Template	*getMultiParmTemplate() const
			 { return myTemplatePtr->getMultiParmTemplate(); }
    PRM_Template	*getMultiParmTemplate()
			 { return myTemplatePtr->getMultiParmTemplate(); }
    const char		*getToken() const
			 { return myTemplatePtr->getToken(); }
    const char		*getLabel() const
			 { return myTemplatePtr->getLabel(); }
    PRM_Callback	 getCallback() const
			 { return myTemplatePtr->getCallback(); }
    int			 getVectorSize() const
			 { return myTemplatePtr->getVectorSize(); }
    const PRM_Range *	 getRangePtr() const
			 { return myTemplatePtr->getRangePtr(); }
    void		 getChannelToken(UT_String &chname, int i) const
			 { myTemplatePtr->getChannelToken(chname, i); }
    const UT_StringHolder& getChannelToken(int i) const
			 { return myTemplatePtr->getChannelToken(i); }
    const PRM_ChoiceList *getChoiceListPtr() const
			 { return myTemplatePtr->getChoiceListPtr(); }

    const PRM_Template	*getTemplatePtr() const { return myTemplatePtr; }
    PRM_Template	*getTemplatePtr() { return myTemplatePtr; }

    // get/setChannelAlias allow the manipulation of channel aliases.  
    // setChannelAlias will return false if the operation fails and true
    // otherwise.  Do not use the set method directly, only PRM_ParmList should
    // do that.  To set a channel alias, use the OP_Node method.
    bool		 hasChannelAlias(int index) const;
    void		 getChannelAlias(UT_String &strref, int index) const;
    const UT_StringHolder& getChannelAlias(int index) const;
    bool		 setChannelAlias(const UT_StringHolder &token_alias,
					 int index);

    // Returns the sub-index of the parm given by the name thechannelname.  If
    // allow_alias is true, the channel name aliases are also searched, 
    // otherwise they are ignored.
    int			 findSubIndex(const UT_StringRef &thechannelname,
				      bool allow_alias) const;

    PRM_ParmOwner	*getParmOwner() const
			    { return myParm->getParmOwner(); }
    PRM_ParmList	*getParmList() const
			    { return myParm->getOwner(); }
    PRM_Parm		*getParm() const
			    { return myParm; }

    static bool		 isChanRefString(const char *str, bool is_expr,
					 UT_String *chref = 0);
    static bool		 isObsoleteDirectChanRefString(const char *str,
						       UT_String *chref = 0);

    // Returns true if the channame is the token (or alias) of the channel at 
    // vec_index, and returns false otherwise.
    // The length of channame can be given in channamelen to speed up rejections
    // if the caller caches the string length.
    bool		 hasChannelName(const UT_StringRef &s, int vi) const;

    // Methods for accessing and setting the active handle binding flag
    bool                 isActive(const int index) const;
    void                 setActive(const int index, const bool active);

    // Spare parameter flag
    bool		 isSpareParm() const;
    void		 setSpareParm(bool spare);

    // Multiparm instance information
    const PRM_Instance	*getMultiParmOwner() const;
    void		 setMultiParmOwner(const PRM_Multi *multiparmowner);

    /// Return the list of instance indicies for this parameter.
    /// If this parameter is part of a multi-parameter, then its index in the
    /// multi-parm array will be inserted into the indices array.  If the
    /// multi-parm is nested in other multi-parms, then the indices array will
    /// have multiple entries (with the outer multi-parm listed first, and the
    /// inner-most multi-parm last in the array).
    /// \param indices The list of index instances.
    /// \param instance_index Each multi-parm can have multiple parameters in
    /// each instance.  If instance_index is true, the instance number will be
    /// returned.  Otherwise the raw offset into the multi-parm will be
    /// extracted.
    int			 getMultiInstanceIndex(UT_IntArray &indices,
				bool instance_index=true) const;

    bool		 validIndex(int index) const;

    virtual int64	 getMemoryUsage(bool inclusive) const;

    virtual bool	 isRotationParm() const
			 { return false; }

    /// During loading we need to validate that the channels tied to the
    /// parameter indices are correclty mapped.
    void		validateChannelNames(PRM_ParmList &list);

    /// Thread-safe access to get or create a micronode
    DEP_MicroNode &	 microNode(int vi);
    
    /// Thread-safe access to get micronodes
    void		 getMicroNodes(DEP_MicroNodeList &micronodes) const;

    /// Return true if this parameter has micronodes, NOT thread-safe
    bool		 hasMicroNodes() const
			    { return (myMicroNodes != nullptr); }
    
    void		 clearMicroNodeInputs();
    void		 getMicroNodeInputList(DEP_MicroNodeList &inputs) const;
    int			 getMicroNodeNumInputs() const;
    void		 getMicroNodeOutputList(DEP_MicroNodeList &outputs) const;

    void		 propagateDirty(
			    DEP_MicroNode::Visitor client_visit,
			    DEP_MicroNode *parmlist_micronode,
			    bool force,
			    bool *dirtied_data = nullptr);
    void		 dumpMicroNodes(
			    std::ostream &os,
			    bool as_DOT,
			    int indent_level) const;
    int64		 getMicroNodesMemoryUsage() const;

    /// Custom Channel Color
    bool		 hasChannelCustomColor(int index) const;
    const UT_Color	&getChannelCustomColor(int index) const;
    void		 setChannelCustomColor(int index, const UT_Color &c);
    void		 resetChannelCustomColor(int index);

    bool	 getAutoSelectFlag(int vec_idx) const;
    void	 setAutoSelectFlag(int vec_idx, bool f);
    unsigned int getAutoSelectField() const
				{ return myAutoSelectField; }

protected:
    void		 clearAndDestroyChannels();

    void		 addParmWarning(PRM_ErrorCode code) const;
    void		 indexError(int index);

    void		 setLockField(unsigned int locks)
			 { myLockField = locks; }
    void		 setAutoScopeField(unsigned int field)
			 {
			     myAutoScopeField = field;
			     myBitField |= BITFLAG_SAVEAUTOSCOPE;
			 }

    void		 setAutoSelectField(unsigned int field)
			 {
			     myAutoSelectField = field;
			     myBitField |= BITFLAG_SAVEAUTOSELECT;
			 }

    SYS_FORCE_INLINE void
    checkForExpressionErrors(
	    const PRM_ParmList *thelist, int vector_index, int thread) const
    {
	int err;
	err = ev_GetErrNo(thread);
	if (err != EV_NOERROR)
	    evaluationError(thelist, vector_index, err, thread);
    }

    bool		 validateAccess();

    // This helper method is called by loadCommand() in derived classes to
    // load the token either as a reference to our channel, or failing that,
    // as an expression.
    void		 loadTokenAsChannelNameOrExpression(
				const UT_String &token, int index);

    // This method loads the options of the parm that are within '[]' before
    // the parm value.  The istream must be positioned after the first '['
    // and this method will leave it after the ']' and return true.  If it
    // could not correctly parse the options, it will return false.
    bool		 loadAsciiOptions(UT_IStream &is, uint &pending_mask);

    // This method checks the given binary packet id against the list of 
    // options it supports, if successful, it loads the packet and returns
    // true, otherwise returns false.
    bool		 loadBinaryOption(UT_IStream &is, short packet_id);

    // This method will save the options of the parm if there are any to set.
    // Currently, only the locked flag is saved.  If the parm is not locked,
    // this method won't do anything in an effort to maintain forwards 
    // compatibility with older versions of Houdini.
    // Returns true if anything was written to the stream.
    void		 saveAsciiOptions(std::ostream &os) const;
    bool		 saveBinaryOptions(int class_packet_id,
					   UT_Packet &packet,
					   std::ostream &os) const;

    CH_ExprLanguage	 stringMeaningToExprLanguage(
					CH_StringMeaning meaning) const;

    enum PRM_ParmIOToken {
	PRM_IO_TOKEN_DELIMITER,
	PRM_IO_TOKEN_BRACKET_SPACE,
	PRM_IO_TOKEN_OPTS_OPEN,
	PRM_IO_TOKEN_OPTS_CLOSE,
	PRM_IO_TOKEN_OPTS_LOCKS,
	PRM_IO_TOKEN_OPTS_AUTOSCOPE,
	PRM_IO_TOKEN_OPTS_BYPASS,
	PRM_IO_TOKEN_OPTS_LABEL,
	PRM_IO_TOKEN_OPTS_COLOR,
	PRM_IO_TOKEN_OPTS_AUTOSELECT,
	PRM_IO_TOKEN_NUM
    };

    // Returns the token used in parm IO.
    static const char	*getPacketIOToken(PRM_ParmIOToken token);
    static int		 getPacketIOTokenLen(PRM_ParmIOToken token);
    static int		 getPacketIOBinaryId(PRM_ParmIOToken token);

    // Compares if the given text matches a given token.  You
    // must provide the length of the text, which has the virtue
    // of avoiding having to harden buffers!
    static bool		 comparePacketIOToken(PRM_ParmIOToken token,
				    const char *text,
				    int textlen);

private:

    inline bool		getFieldBit(unsigned int field, int i) const
			{
			    if (i < 0)
				return (field != 0);
			    else
				return ((field & (1 << (unsigned)i)) != 0);
			}
    inline void		setFieldBit(unsigned int &field, int i, bool f)
			{
			    if (i < 0)
			    {
				if (f) // enable lowermost vector size bits
				    field = (1<<(unsigned)getVectorSize()) - 1;
				else
				    field = 0;
			    }
			    else
			    {
				if (f)
				    field |=  (1 << (unsigned)i);
				else
				    field &= ~(1 << (unsigned)i);
			    }
			}

    void		 getFullParmPath(UT_WorkBuffer &path) const;
    
    /// When loading a file, it's possible that the channel mapping doesn't
    /// match the expected channel name.  In this case, we want to either
    /// rename the channel, or delete the channel (if there are name
    /// collisions).
    void		 validateChannelName(PRM_ParmList &list, int subindex);

    void		 evaluationError(
			    const PRM_ParmList *thelist,
			    int vector_index, int err, int thread) const;

    // This method is *NOT* thread-safe wrt microNode() so do not access the
    // source and destination PRM_Instance pointers from multiple threads when
    // calling this.
    void		 adoptMicroNodes(PRM_Instance &thesrcinstance);

    /// Thread-safe version of hasMicroNodes() that can be called at the same
    /// time as microNode().
    inline PRM_ParmMicroNode *threadSafeMicroNodes() const;

private:

    enum { // bitflags for myBitField
	    /*BITFLAG_UNUSED_BIT		= 0x0002,*/
	    /*BITFLAG_UNUSED_BIT		= 0x0004,*/
	    BITFLAG_EXPRESSIONSTATE		= 0x0008,
	    BITFLAG_UNDOSAVED			= 0x0010,
	    BITFLAG_ACTIVETAKE			= 0x0020,
	    BITFLAG_TAKEALWAYSACTIVE		= 0x0040,
	    BITFLAG_SAVEAUTOSCOPE		= 0x0080,
	    BITFLAG_SPAREPARM			= 0x0100,
	    BITFLAG_AUTOTAKEUNDOSAVED		= 0x0400,
	    BITFLAG_DEFAULTSINIT		= 0x0800,
	    BITFLAG_VALUESTATE			= 0x1000,
	    BITFLAG_BYPASS			= 0x2000,
	    BITFLAG_SAVELABEL			= 0x4000,
	    BITFLAG_SAVEAUTOSELECT		= 0x8000,

	    // Flags that should be saved
	    BITFLAG_SAVE_MASK			= (BITFLAG_EXPRESSIONSTATE)
    };

    unsigned int	  myBitField;
    unsigned int	  myEnableField;
    unsigned int	  myLockField;
    unsigned int	  myAutoScopeField;
    unsigned int	  myVisibleField;
    unsigned int	  myAutoSelectField;

    // Flag to indicate that the handle binded to this parm is active
    unsigned int	  myActiveField;

    PRM_Template	 *myTemplatePtr;

    PRM_Value		 *myNotifyValue;
    unsigned		  myDirtyParmFlags;
    unsigned		  mySendExtraFlags;

    CH_Channel		**myChannelPtrs;
    UT_UniquePtr<UT_StringHolder[]>
			  myChannelAliases;

    struct ChannelCustomColor
    {
	ChannelCustomColor()
	    { myHasCustomColor = false; }
	UT_Color	  myColor;
	bool	          myHasCustomColor;
    };
    ChannelCustomColor	 *myChannelColors;

    PRM_Parm	 	 *myParm;
    const PRM_Multi	 *myMultiParmOwner;

    PRM_ParmMicroNode	 *myMicroNodes;
    mutable UT_TBBSpinLock  myMicroNodesLock;
};

inline int	
PRM_Instance::findSubIndex(const UT_StringRef &thechannelname,
			    bool allow_alias) const
{
    int     size   = myTemplatePtr->getVectorSize();
    int	    retval = -1;
    int	    i	   = 0;

    if (allow_alias && myChannelAliases)
    {
	for (i = 0; i < size; i++)
	{
	    if (myChannelAliases[i] == thechannelname)
	    {
		retval = i;
		break;
	    }
	}
    }

    if (retval == -1)
	retval = myTemplatePtr->findSubIndex(thechannelname);

    return retval;
}

inline unsigned
PRM_Instance::getEnableState(int comp) const
{
    if(comp < 0)
	return myEnableField;

    return ( myEnableField & (1<<comp) ) ? 1 : 0;
}

inline bool
PRM_Instance::getVisibleState(int comp) const
{
    // If the template declares a parameter invisible, it stays invisible.
    if (myTemplatePtr->getInvisible())
	return true;

    // Separators, labels and headings have no values, so we use the entire
    // visible field for them.
    if (!getVectorSize())
	return (myVisibleField != 0);

    return getFieldBit(myVisibleField, comp);
}

inline void
PRM_Instance::setVisibleState(bool f, int comp)
{
    if (getVectorSize())
	setFieldBit(myVisibleField, comp, f);
    else
	myVisibleField = f ? SYS_UINT32_MAX : 0;
}

#endif
