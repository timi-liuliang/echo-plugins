/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Parm.h (Parameter Library)
 *
 * COMMENTS:
 *		PRM_Parm is the class which contains all the information
 *		about a parameter. A parameter is any value or set of
 *		values that are used in the Houdini world. It may contain
 *		expressions, channels, or simple values.
 *
 *		It is divided into two part, the parameter's type
 *		definition, stored in the PRM_Template, and the
 *		parameter's actual data, stored in the PRM_Instance.
 *		Each PRM_Parm contains all the information it needs to
 *		perform its functions independently of other objects.
 */

#ifndef __PRM_Parm__
#define __PRM_Parm__

#include "PRM_API.h"
#include <iosfwd>
#include <SYS/SYS_Types.h>
#include <UT/UT_Defines.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_SmallObject.h>
#include <UT/UT_StringHolder.h>
#include <DEP/DEP_MicroNode.h>
#include <CH/CH_Types.h>

#include "PRM_Default.h"
#include "PRM_KeySetType.h"
#include "PRM_Type.h"
#include "PRM_Template.h"
#include "PRM_ParmOwner.h"
#include "PRM_ChanState.h"

class PRM_Instance;
class PRM_Multi;
class PRM_ParmList;
class PRM_RefId;
class PRM_Value;
class CH_Channel;
class CH_Collection;
class CH_FullKey;
class UT_JSONValueMap;
class UT_String;
class UT_TokenString;


/// An enumeration specifying how, if wanted, keys are set when values are
/// set.
enum PRM_AddKeyType
{
    PRM_AK_MARK_PENDING,    ///< Keys are not immediately added for the value  
			    ///  being but marked as pending.
    PRM_AK_SET_KEY,	    ///< Keys are added if the current value differs 
			    ///  from the value being set.
    PRM_AK_FORCE_KEY	    ///< Keys are added even if the current value 
			    ///  matches the value being set.
};


enum PRM_OverrideType 
{
    PRM_OVERRIDDEN,
    PRM_LAYERED
};

class PRM_API PRM_DataItem
{
public:
    virtual ~PRM_DataItem() {}

    virtual PRM_Type::PRM_DataType getDataType() const = 0;
    virtual const char *getDataTypeToken() const = 0;
    virtual bool saveAscii(std::ostream &os) const = 0;
    virtual bool saveBinary(std::ostream &os) const = 0;
    virtual int64 getMemoryUsage() const = 0;

    // Data accessors.
    virtual UT_JSONValueMap *getKeyValueDict() const { return nullptr; }
};
typedef UT_SharedPtr<const PRM_DataItem> PRM_DataItemHandle;

class PRM_API PRM_DataFactory
{
public:
    virtual ~PRM_DataFactory() {}

    virtual const char *getDataTypeToken() const = 0;
    virtual PRM_DataItemHandle loadBinary(UT_IStream &is) const = 0;
    virtual PRM_DataItemHandle loadAscii(UT_IStream &is) const = 0;

    static void install(const char *type, PRM_DataFactory *factory);
    static PRM_DataItemHandle parseAscii(const char *type, UT_IStream &is);
    static PRM_DataItemHandle parseBinary(const char *type, UT_IStream &is);
};

class PRM_API PRM_UndoData
{
public:
    virtual ~PRM_UndoData() {}

    virtual const char *getDataType() const = 0;
    virtual int64 getMemoryUsage() const = 0;
};

typedef UT_SharedPtr<PRM_UndoData> PRM_UndoDataHandle;
typedef UT_Array<std::pair<UT_StringHolder, PRM_UndoDataHandle> > PRM_UndoDataList;

class PRM_API PRM_Parm : public UT_SmallObject<PRM_Parm,
                                       UT_SMALLOBJECT_CLEANPAGES_DEFAULT,
                                       UT_SMALLOBJECT_PAGESIZE_DEFAULT,
				       UT_SMALLOBJECT_THREADSAFE_OFF>
{
public:
		 // for these constructors, a nil CH_Collection is invalid
		 // only PRM_ParmList handles a nil correctly
		 PRM_Parm(PRM_Template	*thetemplate,
			  PRM_ParmList	*owner);
		 PRM_Parm(PRM_Parm	*theprm,
			  PRM_ParmList	*owner);
		~PRM_Parm();

    void	 adopt(PRM_Parm &thesrcparm);
    void	 adoptOverrides(PRM_Parm &srcparm);

    void	 revertToDefaults(fpreal time);
    void	 revertToDefault(fpreal time, int index, bool propagate=true);
    void	 restoreFactoryDefaults();
    void	 restoreFactoryDefault(int index);

    static bool  getDestructiveRevertToDefaultFlag() 
		    { return myDestructiveRevertToDefaultFlag; }
    static void  setDestructiveRevertToDefaultFlag(bool v)
		    { myDestructiveRevertToDefaultFlag = v;    }

    static bool  getRevertInvisibleToDefaultsFlag() 
		    { return myRevertInvisibleToDefaultsFlag; }
    static void  setRevertInvisibleToDefaultsFlag(bool v)
		    { myRevertInvisibleToDefaultsFlag = v;    }

    static bool  getMakeSpareParmsForUnknownChannelsFlag() 
		    { return myMakeSpareParmsForUnknownChannelsFlag; }
    static void  setMakeSpareParmsForUnknownChannelsFlag(bool v)
		    { myMakeSpareParmsForUnknownChannelsFlag = v;    }

    // Static functions to access PRM_Instance subclass static functions.
    static bool	 isStringThatCanExpand(const char *str);
    static bool	 isOrdinalExpression(const char *str,
				     const PRM_Template *tplate = nullptr);
    static bool	 isOrdinalExpression(const PRM_Default &dflt,
				     const PRM_Template *tplate = nullptr);

    // Static functions to convert string parm values to and from
    // expressions.
    static void	 convertStringToExprLanguage(const char *str,
					     UT_String &expr,
					     PRM_Template *tplate = nullptr);
    static void	 convertStringFromExprLanguage(const char *str,
					       UT_String &expr);

    // Static functions to access PRM_Multi static functions.
    static bool	 getIsAddingOrRemovingMultiParms();
    // Replace a number in the source string and return the result in the
    // destination (work buffer)
    static bool	 replaceMultiNumber(UT_String &str, const char *replace);
    // If there are any numeric placeholders left in the specified string,
    // this function replaces them with underscores.
    static bool	 replaceRemainingMultiNumbers(UT_String &str);
    // Instance a string, replacing all '#' marks with integers from the
    // index list.
    static void	 instanceMultiString(UT_String &token,
				const UT_IntArray &indexlist,
				bool fill_remaining=true);
    static void	 instanceMultiString(UT_String &token,
				const int *indexlist, int num,
				bool fill_remaining=true);

    static bool	 getAlwaysHardenFlag() { return myAlwaysHardenFlag; }
    static void	 setAlwaysHardenFlag(bool o) { myAlwaysHardenFlag = o; }

    int		 isDefault() const;
    int		 isDefault(int index) const;
    int		 isFactoryDefault() const;
    int		 isFactoryDefault(int index) const;

    // isTrueFactoryDefault differs from isFactoryDefault in that it 
    // will do a string compare on the expression if there is one, thus
    // an expression that evaluates to the default value will still not be a
    // "true" factory default.
    int		 isTrueFactoryDefault(int index) const;
    int		 isTrueFactoryDefault() const;
    
    // These are exactly like isTrueFactoryDefault() except that it excludes
    // the checking of the lock flag. This is because we don't want locked
    // parms to show up bolded in the parameters pane.
    int		 isFactoryDefaultUI(int index) const;
    int		 isFactoryDefaultUI() const;

    void	 overwriteDefaults(fpreal time);
    void	 overwriteDefault(fpreal time, int index);

    void	 buildOpDependencies(const PRM_RefId &ref_id, int thread);
    void	 changeOpRef(const char *new_fullpath, const char *old_fullpath,
			     const char *old_cwd, const char *chan_name,
			     const char *old_chan_name,
			     void (*undo_callback)(void *), void *undo_data,
			     int thread);

    void	 getValue(fpreal time, int32 &value, int index,
			  int thread) const;
    void	 getValue(fpreal time, int64 &value, int index,
			  int thread) const;
    void	 getValue(fpreal time, fpreal &value, int index,
			  int thread) const;
    void	 getValue(fpreal time, UT_String &value, int index,
			  bool expand, int thread) const;
    void	 getValue(fpreal time, UT_StringHolder &value, int index,
			  bool expand, int thread) const;
    void	 getValue(fpreal time, PRM_DataItemHandle &value, int index,
			  int thread) const;

    // Specialized variant for Key-Value Dictionary data parameters.
    // Return either an expanded or unexpanded JSON map.
    void	 getValue(fpreal time, UT_JSONValueMap &value, int index,
			  bool expand, int thread) const;

    void	 getValues(fpreal time, fpreal32 *value, int thread) const;
    void	 getValues(fpreal time, fpreal64 *value, int thread) const;
    void	 getValues(fpreal time, int32 *value, int thread) const;
    void	 getValues(fpreal time, int64 *value, int thread) const;
    void	 getDefaultValue(fpreal &value, int index) const;
    void	 getDefaultValue(UT_String &value, int index) const;

    // The following method is very similar to getExpressionOrValue() except
    // that it returns a custom string when the parameter is overridden by a
    // CHOP.
    void	 getExpressionStringForUI(fpreal time, UT_String &value,
					  int index, int thread) const;

    // The following method, unlike getExpressionOnly(), will convert the
    // value to a string and return that if there is no channel.
    void	 getExpressionOrValue(fpreal time, UT_String &value,
				      int index, int thread) const;
    void	 getExpressionOrValue(fpreal time, UT_StringHolder &value,
				      int index, int thread) const;

    // The following method returns the expression string, returning an empty
    // string in the case of a parameter with no channel/expression.
    void	 getExpressionOnly(fpreal time, UT_String &value,
				   int index, int thread) const;

    bool	 setExpression(fpreal time, const char *value,
			       CH_ExprLanguage language,
			       int index, bool evaluate = true,
			       bool rmchannel = false, bool propagate = true);

    // returns true if the value was set, of false otherwise (eg, parm locked)
    bool	 setValue(fpreal time, const char *value,
			  CH_StringMeaning meaning,
			  bool kill_expr = false, int index = 0,
			  PRM_AddKeyType add_key = PRM_AK_MARK_PENDING, 
			  bool propagate = true);

    bool	 setValue(fpreal time, fpreal value,
			  bool kill_expr = false, int index = 0,
			  PRM_AddKeyType add_key = PRM_AK_MARK_PENDING, 
			  bool propagate = true);

    bool	 setValue(fpreal time, int32 value,
			  bool kill_expr = false, int index = 0,
			  PRM_AddKeyType add_key = PRM_AK_MARK_PENDING, 
			  bool propagate = true);

    bool	 setValue(fpreal time, int64 value,
			  bool kill_expr = false, int index = 0,
			  PRM_AddKeyType add_key = PRM_AK_MARK_PENDING, 
			  bool propagate = true);

    bool	 setValue(fpreal time, const PRM_DataItemHandle &value,
			  bool kill_expr = false, int index = 0,
			  PRM_AddKeyType add_key = PRM_AK_MARK_PENDING, 
			  bool propagate = true);

    //
    // The setValues methods set all entries in the vector at once...
    // returns true if the values were set, of false otherwise (eg, parm locked)
    //
    bool	  setValues(fpreal time, const fpreal *values,
        	            bool kill_expr = false, 
        	            PRM_AddKeyType add_key = PRM_AK_MARK_PENDING, 
        	            bool propagate = true);

    bool	  setValues(fpreal time, const int32 *values,
        	            bool kill_expr = false, 
        	            PRM_AddKeyType add_key = PRM_AK_MARK_PENDING, 
        	            bool propagate = true);

    bool	  setValues(fpreal time, const int64 *values,
        	            bool kill_expr = false, 
        	            PRM_AddKeyType add_key = PRM_AK_MARK_PENDING, 
        	            bool propagate = true);

    //
    // Multi Parms methods.
    //

    // Returns true if the parm is a multi parm
    bool		 isMultiParm() const;

    // Insert a new Multi Parm Instance before index.
    // If index equals zero, the parm instance can be added at the start of the list.
    // If index equals MultiParmNumItems(), the parm instance is added at the end of the list.
    void		 insertMultiParmItem(int index, bool propagate = 1);

    // Remove Multi Parm Instance at index.
    void		 removeMultiParmItem(int index, bool propagate = 1);

    // Revert Multi Parm Instance at index.
    void		 revertMultiParmItem(int index);

    // Revert all the multi parm instances. This is has different behaviour for
    // ramps in that it will revert to the initialized state.
    void		 revertAllMultiParmItems();

    // Copy all the Parm Values under the Multi Parm Instance from index 'from' to index 'to'
    void		 assignMultiParmItem(int from, int to);

    // Swap 2 Multi Param instances.
    // This can be used to move up and down param instances in the list.
    void		 swapMultiParmItem(int from, int to);

    // Returns the number of instance parameters.
    int			 getMultiParmNumItems() const;

    // Returns the number of parameters within an instance parameter.
    // It returns 1 for array of simple types ( int, float, string ).
    // It returns the number of parameters within the struct for arrray of structs.
    int			 getMultiParmInstancesPerItem() const;

    // Returns the param token for an instance parm at 'index' and child parm at 'p'
    const char*		 getMultiParmToken(int p, int index) const;

    // Returns the number of UI Parms Entries.
    // A float3 or color4 will account for a single parameter.
    // An int, string or float will account for one parm as well.
    int			 getMultiParmUIEntries() const;

    // Returns the PRM_Template ar index 'idx'
    const PRM_Template	*getMultiParmTemplate(int idx) const;
    PRM_Template	*getMultiParmTemplate(int idx);

    // These functions allow a linear traversal of all multiparm instances
    // parameters.
    PRM_Parm		*getMultiParm(int idx) const;
    int			 getMultiParmCount() const;

    // The only difference between the next two functions is that the
    // second send the change notification without question. The first
    // calls the second, but only if the parm isn't a PRM_CALLBACK. In
    // those cases we usually don't want to actually send out change
    // notifications. The notable exception is the opparm -c command.
    void	 valueChanged(int vec_idx, bool propagate)
		    { privateValueChanged(vec_idx, propagate,
					  /*update_multi*/true); }
    void	 sendValueChanged(int vec_idx, bool propagate);
    void	 indirectChange(int vec_idx, bool expr_changed,
				bool update_multi = true);

    PRM_Callback getCallback() { return getTemplatePtr()->getCallback(); }
    int		 setOverride(int index, int data_idx, const char *source, PRM_OverrideType type);
    const char	*getOverride(int index) const;
    int		 getOverrideDataIndex(int index) const;
    bool	 getOverrideNodePath(int index, UT_String &path) const;
    bool	 getOverrideTrackName(int index, UT_String &trackname) const;
    bool	 getOverrideType(int index, PRM_OverrideType &type) const;
    int		 setOverrideDisable(int index, int onoff);
    int          getIsOverrideActive(int index) const;

    /// copyParm() copies the value of an entire parm, including channels. If
    /// it is a multi-parm, then all instance parameters will be also be
    /// copied. If the they are a mismatch of vector sizes, the smaller of the
    /// two sizes will be used.
    void	 copyParm(const PRM_Parm &src);

    /// copyValue copies the value for a particular parm component. If
    /// current_value_only is true, then only the value evaluated at time t
    /// will be copied. Otherwise, any channels in the src_parm will be copied
    /// wholesale.
    void	 copyValue(fpreal t, int dest_vi,
			   const PRM_Parm &src_parm, int src_vi,
			   bool set_name_alias,
			   bool current_value_only,
			   bool set_value_to_default = false,
			   PRM_AddKeyType add_key = PRM_AK_MARK_PENDING,
			   PRM_KeySetType key_set_type = PRM_USE_PREFERENCES);

    void	 addChannels();
    void         addChannel(int index, bool propagate = true);
    void         addChannel(int index, const char *theexpr,
			    CH_ExprLanguage language, bool propagate = true);
    int		 addChannel(const char *name);
    int		 addChannel(const char *name, const char *expr,
			    CH_ExprLanguage language);

    void         clearAndDestroyChannels(bool force = false);
    void         removeChannel(int index, bool propagage = true);

    // Destroy and clear references to those channels not referenced by the
    // supplied replacement parameter.
    void	 clearAndDestroyUnrefChannels(const PRM_Parm &repl_parm);

    CH_Channel	*getChannel(int subindex) const;
    int		 getChannelCount() const;

    void	 reloadChannelPtrs();
    int		 channelPtrModified(const char *name);

    int		 isTimeDependent() const;
    bool	 isTimeDependent(int subindex) const;
    int		 isDataDependent(fpreal gtime) const;
    bool	 isDataDependent(fpreal gtime, int subindex) const;
    void	 forceTimeDependent(int subindex);

    bool	 hasContextOptionDeps(int subindex) const;
    const DEP_ContextOptionDeps &getContextOptionDeps(int subindex) const;
    void	 forceContextOptionDep(int subindex,
			const UT_StringHolder &opt);
    
    const PRM_Type	&getType() const;
    PRM_TypeExtended	getTypeExtended() const;
    PRM_MultiType	getMultiType() const;
    fpreal		 getMultiSize() const;
    int			 getMultiStartOffset() const;

    bool	 isRampType() const;
    bool	 isRampTypeColor() const;

    int		 getVectorSize() const
		    { return getTemplatePtr()->getVectorSize(); }
    void	 getChannelToken(UT_String &thestrref, int index = 0) const;
    void	 getChannelLabel(UT_String &thestrref, int index = 0) const;
    void	 getToken(UT_String &thestrref) const;
    void	 getLabel(UT_String &thestrref) const;
    void	 getHelpText(UT_String &helptext) const;
    void	 getHelpUrl(UT_String &helptext) const;
    void	 getHelpTitle(UT_String &helptext) const;

    const char	*getToken() const;
    const char	*getLabel() const;

    const UT_StringRef &
		 getTokenRef() const
		    { return getTemplatePtr()->getTokenRef(); }

    const UT_StringHolder &
		 getChannelToken(int vi = 0) const
		    { return getTemplatePtr()->getChannelToken(vi); }

    void	 getChannelLabelForUI(UT_String &thestrref, int vec_idx) const;

    const PRM_SpareData	*getSparePtr() const;

    unsigned     getTokenHash() const;

    bool	 hasChannelAlias(int subindex) const ;
    void	 getChannelAlias(UT_String &stringref, int subindex) const;
    const UT_StringHolder &
		 getChannelAlias(int subindex) const;

    // This performs all the checks to verify that a parameter can be accessed
    bool	 canAccess(uint mask) const;

    // Sets a channel alias.
    // Do not use this call -- use the OP_Node version to ensure that all
    // dependencies and symbol tables are updated correctly.
    bool	 setChannelAlias(const char *token_alias, int subindex);

    PRM_ChoiceList *getChoiceListPtr();
    const PRM_ChoiceList *getChoiceListPtr() const;

    // Return the group input notify value.  It is nil by default, but
    // can be created by calling createInputNotifyValue.
    PRM_Value	*inputNotifyValue();
    void	 createInputNotifyValue();

    DEP_MicroNode & microNode(int vi);
    bool	    hasMicroNodes() const;

    // A parameter becomes dirty whenever its value is set.
    int		 isDirty(int vector_idx=0) const;
    int		 areAllFlagsClean() const;
    void	 clearAllDirtyFlags();

    // mySendExtraFlags is a per-component bitfield that has its bit set
    // if it needs to be dirty propagated.
    bool	 isSendExtra(int vi) const;
    bool	 hasAnySendExtraFlags() const;

    int		 findSubIndex(const char *thechannelname, bool allow_alias) const;
    
    const PRM_Template	*getTemplatePtr() const;
    PRM_Template	*getTemplatePtr();

    PRM_Instance	*getInstancePtr();

    CH_Collection	*getChannelGroupPtr() const;

    unsigned	 getEnableState(int comp = -1) const;
    unsigned	 getInstanceEnableState(int comp = -1) const;
    int		 setEnableState(int thestate, int comp = -1);

    bool	 getActiveTakeFlag() const;
    bool	 getAlwaysTakeFlag() const;
    void	 setActiveTakeFlag(int onoff);
    bool	 getBypassFlag() const;
    void	 setBypassFlag(bool v);

    bool	 getVisibleState(int comp = -1) const;
    bool	 setVisibleState(bool f, int comp = -1);
    
    int		 getExpressionState();
    void	 setExpressionState(int state);
    int		 getValueState();
    void	 setValueState(int state);

    // Query whether a parameter dialog should call setValue() to record any
    // changes made by the user.  Note that this does not control whether or
    // not the field should be disabled.
    bool	 isValueEditableByUI(fpreal time, int index) const;

    // Query whether a parameter dialog should call setExpression() to record
    // any changes made by the user.  Note that this does not control whether
    // or not the field should be disabled.
    bool	 isExpressionEditableByUI(fpreal time, int index) const;
    
    // hardenChanges() will create keys if the parm already has channels
    void	 hardenChanges(fpreal time, bool forceflag = 0,
	    		       const char *patt = 0,
			       CH_ChannelRefList *list = 0);
    void	 hardenChanges(fpreal time, int index, bool forceflag = 0,
			       const char *patt = 0,
			       CH_ChannelRefList *list = 0);
    // setKey() will create keys even if the parm has no channels
    void	 setKey(fpreal time, int index);
    void	 setKey(fpreal time, int index, const char *exp,
			CH_ExprLanguage language,
			const char *patt = 0, CH_ChannelRefList *list = 0);
    
    void setFullKey(fpreal gtime, int index, CH_FullKey const& key, 
		    bool accel_ratios = true);
    
    CH_StringMeaning getStringMeaning(fpreal time, int index) const;

    // If this parm contains an expression this function returns that
    // expression's language.  Otherwise, it returns the language that it
    // would be if it was turned into an expression.
    CH_ExprLanguage getExprLanguageIfMadeAnExpression(fpreal time, int index) const;

    // If there is an expression, this function returns whether the
    // expression's language for the segment at the given time is the same as
    // the node's langauge.  If there is no expression, this function returns
    // true.
    bool	  languageIsNodesLanguage(fpreal time, int index);

    int		  changePending(int index);
    PRM_ChanState getChanState(fpreal time) const;
    PRM_ChanState getChanState(fpreal time, int index) const;

    void	  save(std::ostream &os, int binary, bool compiled) const;
    bool	  load(UT_IStream &is);

    void	  saveUndoData(PRM_UndoDataHandle &data) const;
    bool	  loadUndoData(const PRM_UndoDataHandle &data);

    void	  saveCommand(std::ostream &os, int values=0, int index=-1) const;
    int		  loadCommand(int argc, const char *const argv[], int index,
       		              bool values_only, PRM_KeySetType set_type);

    fpreal	 findNextKey(fpreal theoldtime, int index = -1);
    fpreal	 findPrevKey(fpreal theoldtime, int index = -1);

    int		 findString(const char *str, bool fullword,
			    bool usewildcards) const;
    int		 changeString(const char *from, const char *to, bool fullword);

    bool	 notifyVarChange(const char *varname);

    bool	 getUndoSavedFlag();

    // WARNING: the setUndoSavedFlag method should only be called from
    // OP_Parameters.  Calling it from anywhere else can cause undo errors.
    void	 setUndoSavedFlag(bool f);

    bool	 getAutoTakeUndoSavedFlag();

    // WARNING: the setAutoTakeUndoSavedFlag method should only be called from
    // OP_Parameters.  Calling it from anywhere else can cause undo errors.
    void	 setAutoTakeUndoSavedFlag(bool f);

    bool	 getLockedFlag(int vec_idx) const;
    bool	 areAllComponentsLocked() const;
    void	 setLockedFlag(int vec_idx, bool f);
    unsigned int getLockedField() const;
    bool	 getAutoScopedFlag(int vec_idx) const;
    void	 setAutoScopedFlag(int vec_idx, bool f);
    unsigned int getAutoScopedField() const;


    bool	 getMagicString(UT_TokenString &string,	fpreal t,
				int parm_group_mask /*=1*/,
				bool add_if_default /*= true*/,
				bool ignore_group /*= false*/,
				int thread);

    void	 destroyChildParms();

    PRM_ParmList	*getOwner() const
			    { return myOwner; }

    PRM_ParmOwner	*getParmOwner() const;

    // Returns true if this parm is compatible with other_parm.  Being 
    // compatible means that the two parms have the same token string,
    // the same size and are of the same type.
    bool		 isCompatible(const PRM_Parm &other_parm) const;

    // Returns true if the parm is of the basic type check_type, as defined
    // in PRM_Type.h as basic types.
    bool		 isOfBasicType(PRM_Type check_type) const;

    // Returns true if this parm and other_parm are of the same basic type,
    // and have equal values (including evaluated expressions), and false
    // otherwise.
    bool		 isParmValueEqual(PRM_Parm &other_parm, int vec_index,
					  fpreal time, int thread) const;
    bool		 isParmValueEqual(int vec_index, PRM_Parm &other_parm,
					  int other_index, fpreal time,
					  int thread) const;

    // Returns true if the channame is the token (or alias) of the channel at 
    // vec_index, and returns false otherwise.
    // The length of channame can be given in channamelen to spped up rejections
    // if the caller caches the string length.
    bool		 hasChannelName(const UT_StringRef &channame,
					int vec_index) const;

    // Returns the aliases of all the channels in this parm in one string
    // separated by the character sep.
    void		 getAllChannelAliases(UT_String &aliasstring, 
					      char sep ='/') const;

    // Methods for accessing and setting the active handle binding flag
    bool		 isActive(const int index) const;
    void		 setActive(const int index, const bool active);

    int64		 getMemoryUsage(bool inclusive) const;

    bool		 isSpareParm() const;
    void		 setSpareParm(bool sp) const;

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

    void		 rebuildParmDependency();

    bool		 isRotationParm() const;

    // For explanations of path vs ch_name, see comment for 
    // constructChannelReference
    void		setChannelReference(fpreal time,
					    int index, 
					    const char *path, 
					    const char *ch_name=nullptr,
					    bool evaluate=true,
					    bool rmchannel=false);

    // Construct the string representing reference to the channel given by 
    // path/name from this parameter. 
    // If name == nullptr, path is assumed to be (node path + channel name)
    // If name not nullptr, path is assumed to be a path without channel name, 
    // name is assumed to be channel name without the path.
    void		 constructChannelReference(UT_String &reference,
						   CH_ExprLanguage language,
						   const char *path, 
						   const char *ch_name=nullptr);

    static bool		 isChanRefString(const char *str, bool is_expr,
					 UT_String *chref = 0);

    bool getHaveCompiledExpressions() const;


    // Set a flag to have the label persisted with the parm.
    // This is used to set a label on multi params
    void setSaveLabelFlag(bool f);

    // Custom Channel Color that persists when set.
    bool		 hasChannelCustomColor(int index) const;
    const UT_Color	&getChannelCustomColor(int index) const;
    void		 setChannelCustomColor(int index, const UT_Color &c);
    void		 resetChannelCustomColor(int index);

    bool	 getAutoSelectFlag(int vec_idx) const;
    void	 setAutoSelectFlag(int vec_idx, bool f);
    unsigned int getAutoSelectField() const;
private:
    void		 privateValueChanged(
			    int vec_idx,
			    bool propagate,
			    bool update_multi);

    static PRM_AddKeyType getAddKeyValue(PRM_AddKeyType add_key);
    
private:
    PRM_Instance	*myInstancePtr;
    PRM_ParmList	*myOwner;

    static bool	 	 myAlwaysHardenFlag;
    static bool		 myDestructiveRevertToDefaultFlag;
    static bool		 myRevertInvisibleToDefaultsFlag;
    static bool		 myMakeSpareParmsForUnknownChannelsFlag;

    friend class	 PRM_ParmList;
};

#endif
