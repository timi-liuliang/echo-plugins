/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:	OP_Parameters encapsulates the parameter lists used
 *		by the OP library.
 *
 */

#ifndef __OP_Parameters_h__
#define __OP_Parameters_h__

#include <iosfwd>

#include <CH/CH_ExprLanguage.h>
#include <EXPR/EX_Error.h>
#include <PRM/PRM_ChanState.h>
#include <PRM/PRM_ParmList.h>
#include <PRM/PRM_ParmOwner.h>
#include <SYS/SYS_Types.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Defines.h>
#include <UT/UT_LockedRawPtr.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringSet.h>
#include <UT/UT_Thread.h>

#include "OP_API.h"
#include "OP_Error.h"
#include "OP_Lock.h"
#include "OP_Stat.h"
#include "OP_Value.h"
#include "OP_Context.h"

class UT_BitArray;
class UT_Ramp;
class UT_JSONValueMap;
class PRM_Parm;
class PRM_ParmList;
class PRM_ScriptPage;
class PRM_ScriptImports;
class PRM_BatchList;
class CH_LocalVariable;
class CMD_Args;
class OP_Channels;
class OP_Operator;
class OP_ParmCache;
class OP_PostIt;
class OP_MultiparmInfo;
class OP_PropertyLookupList;
class OP_SpareParms;

enum OP_ScopeOp {
    OP_SCOPE_SET,
    OP_SCOPE_ADD,
    OP_SCOPE_REMOVE,
    OP_SCOPE_NUM_OPS	    // sentinel
};

enum OP_ScopeType {
    OP_SCOPE_TYPE_SCOPE,
    OP_SCOPE_TYPE_DISPLAY,
    OP_SCOPE_TYPE_PIN,
    OP_SCOPE_TYPE_NUM_OPS	    // sentinel
};

class OP_API OP_ScopeOptions
{
public:
    // allow_nonchannels - determines whether or not we should scope parameters
    //			   that don't have CH_Channel pointers associated with
    //			   them.
    //
    // exclude_animated  - determines whether or not to exclude channels that 
    //			   are time dependent.
    //
    // exclude_constant  - determines whether or not to exclude channels that
    //			   are not time dependent.
    //
    // include_only_nonint_keys - determines whether or not to include only
    //				  those channels that have non integer 
    //				  keyframes.
    // scoped_related_layers - scope the related animation layers
    // scope_type - modify the scope or display flags
    OP_ScopeOptions(bool allow_nonchannels, bool exclude_animated,
		    bool exclude_constant,  bool include_only_nonint_keys,
		    bool scoped_related_layers,
		    OP_ScopeType scope_type) :
	myAllowNonChannels(allow_nonchannels),
	myExcludeAnimated(exclude_animated),
	myExcludeConstant(exclude_constant),
	myIncludeOnlyNonIntKeys(include_only_nonint_keys),
	myScopedRelatedLayers(scoped_related_layers),
	myScopeType( scope_type )
	{}

    bool myAllowNonChannels;
    bool myExcludeAnimated;
    bool myExcludeConstant;
    bool myIncludeOnlyNonIntKeys;
    bool myScopedRelatedLayers;

    OP_ScopeType myScopeType;
};

class OP_API OP_FollowChanRefsOptions
{
public:
    OP_FollowChanRefsOptions( fpreal t )
    : myTime(t)
    , myFollowCHOPLayerOverrides(true)
    , myFollowReferences(true)
    , myStopAtEditabledHDA(false)
    {
    }

    fpreal myTime;
    bool myFollowCHOPLayerOverrides;
    bool myFollowReferences;
    bool myStopAtEditabledHDA;
};

class OP_API OP_Parameters : public PRM_ParmOwner
{
public:
    const UT_String	&getName()     const { return myName; }
    OP_Operator		*getOperator() const { return myTableEntry; }

    //  This function updates the hidden/disable flags on all parameters by
    //  asking both the conditionals and the overridden updateParmsFlags
    //  functions.
    //  Returns true if something was changed.
    bool		 updateParmsAndSpareParmsFlags();
    
    //  Clone a parameter by referencing all the parameters of the "from"
    //  OP_Parameters.
    virtual unsigned	referenceAllParameters(OP_Parameters *from,
				bool relative_references = true);
    /// Reference a single parameter from the @c from node.  This builds a
    /// channel reference for the given parameter index.
    /// If the @c relative_path isn't specified, it will be computed as @code
    ///    from->getFullPath(frompath);
    ///    this->getFullPath(thispath);
    ///    relative_path.getRelativePath(thispath, frompath);
    /// @endcode
    virtual bool	referenceSingleParameter(OP_Parameters *from,
				int parm_idx,
				const char *relative_path = NULL);

    //  Methods for dealing with channels & key frames
    void		   animate(unsigned idx);
    void		   animate(const char *name);
    void		 deAnimate(unsigned idx);
    void		 deAnimate(const char *name);
    CH_Channel		*addChannel(const char *name);
    CH_Channel		*getChannel(const char *name);
    const CH_Channel	*getChannel(const char *name) const;
    int			 destroyChannel(const char *name);

    bool		 isObsoleteChannel(const char *name) const;

    void		 takeActivateParm(uint idx, bool enable=true);
    void		 takeActivateParm(const char *name, bool enable=true);
    void		 takeActivateParm(PRM_Parm *parm, bool enable=true);
    void		 takeDeactivateParm(uint idx)
			     { takeActivateParm(idx, false); }
    void		 takeDeactivateParm(const char *name)
			     { takeActivateParm(name, false); }
    void		 takeDeactivateParm(PRM_Parm *parm)
			     { takeActivateParm(parm, false); }

    bool		 takeIsActiveParm(uint idx);
    bool		 takeIsActiveParm(const char *name);
    bool		 takeIsActiveParm(const PRM_Parm *parm);

    // Activates the parm for the take if it should be automatically
    // activated based on its current state and the current take state.
    void	 takeAutoActivateParm(PRM_Parm *) override;

    fpreal		 findKey(fpreal t, int direction) const;

    PRM_ChanState	 getChanState(fpreal t, bool scoped_only) const;
    PRM_ChanState	 isAtKey(fpreal t) const
			 { return getChanState(t, false); }
    void		 updatePending(fpreal t);
    bool		 isPending( fpreal t );

    OP_Channels		*getChannels(void) const { return myChannels; }

    // This gets the variable value AS as string.  Not the string corresponding
    // to the specified index! Note that you should call the base class if you
    // don't handle the given index.
    virtual bool	 evalVariableValue(UT_String &val, int index,
					   int thread);
    // Note that you should call the base class if you don't handle the given
    // index.
    virtual bool	 evalVariableValue(fpreal &val, int index, int thread);

    /// @deprecated Please override evalVariableValue() above instead
    SYS_DEPRECATED(10.0)
    virtual void	 getVariableString(int index, UT_String &value,
					   int thread);
    /// @deprecated Please override the evalVariableValue() above instead
    SYS_DEPRECATED(10.0)
    virtual float	 getVariableValue(int index, int thread);

    int			 getNumParms() const 
			 { return myParmList->getEntries(); }
    int			 getParmIndex(const char *name) const
			 { return myParmList->getParmIndex(name); }
    PRM_Parm		&getParm(int i)
			 {
                             PRM_Parm *p = myParmList->getParmPtr(i);
                             UT_ASSERT(p);
                             return *p;
                         }
    const PRM_Parm	&getParm(int i) const
			 {
                             const PRM_Parm *p = myParmList->getParmPtr(i);
                             UT_ASSERT(p);
                             return *p;
                         }
    PRM_Parm		&getParm(const char *name)     
			 {
                             PRM_Parm *p = myParmList->getParmPtr(name);
                             UT_ASSERT(p);
                             return *p;
                         }
    const PRM_Parm	&getParm(const char *name) const 
			 {
                             const PRM_Parm *p = myParmList->getParmPtr(name);
                             UT_ASSERT(p);
                             return *p;
                         }
    bool		 hasParm(const char *name) const
			 { return myParmList->getParmPtr(name) != 0; }
    PRM_Parm		*getParmPtr(const UT_StringRef &name)     
			 { return myParmList->getParmPtr(name); }
    const PRM_Parm	*getParmPtr(const UT_StringRef &name) const 
			 { return myParmList->getParmPtr(name); }
    PRM_Parm		*getParmPtrInst(const char *name, const int *inst,
					int nestlevel = 1);
    const PRM_Parm	*getParmPtrInst(const char *name, const int *inst,
					int nestlevel = 1) const;

    // This function is called when our spare parm definition is changing.
    // Parses the supplied dialog script and uses it to update the spare
    // parms for this node.
    virtual bool	 changeSpareParms(UT_IStream &ds, UT_String &errors);
    // Returns true if we are in the middle of chaning our spare parm
    // definition. Lets us avoid handling certain events (such as multiparm
    // change handling in OPUI_Dialog).
    bool		 getIsChangingSpareParms() const
			 { return myChangingSpareParms; }

    // This is a secondary parm list where all obsolete parameters are
    // placed (if any)
    PRM_ParmList    *createObsoleteParmList() override;
    void	     resolveAndDeleteObsoleteParmList(
					PRM_ParmList *&obsolete_parms) override;
    
    // After any call which might have loaded something into the ObsoleteList,
    // we should call this to resolve those parameters:
    virtual void	 resolveObsoleteParms(PRM_ParmList * /*obsolete_parms*/)			 { /*nothing at this level*/ }

    /// Sync the node assuming that it is currently at old_version to the
    /// current_version.
    virtual void	 syncNodeVersion(
				const char *old_version,
				const char *current_version,
                                bool *node_deleted)
			 { /*nothing at this level*/ }

    /// Track down the parameter associated with a given property
    /// @{
    virtual bool	 getParameterOrProperty(
				      const UT_StringRef &name,
				      fpreal now,
				      OP_Node *&op,
				      PRM_Parm *&parm,
				      bool create_missing_multiparms,
				      PRM_ParmList *obsolete=0);
    virtual bool	 getParameterOrPropertyByChannel(
				      const UT_StringRef &chname,
				      fpreal now,
				      OP_Node *&op,
				      PRM_Parm *&parm,
				      int &vector_index,
				      PRM_ParmList *obsolete=0);
    /// @}

    /// This just calls getParameterOrProperty() with
    /// create_missing_multiparms=false and allows for const correctness.
    bool getParameterOrPropertyConst(
	    const UT_StringRef &name,
	    fpreal now,
	    OP_Node *&op,
	    PRM_Parm *&parm,
	    PRM_ParmList *obsolete) const
    {
	return const_cast<OP_Parameters*>(this)->getParameterOrProperty(
		    name, now, op, parm, false, obsolete);
    }

    /// Fill out the property lookup list with parameters. 
    /// Returns the total of found and resolved parameters in the list 
    /// (whether they were resolved by previous calls or this particular call,
    /// ie, NOT the total of nodes found in this call)
    virtual int		findParametersOrProperties(fpreal now,
					OP_PropertyLookupList &list);

    // Take a PRM_Parm pointer, and look at its value. If it is a simple
    // channel reference, return the referenced node, parm, and vecidx.
    static bool		 followChannelReferencesOneStep(const OP_FollowChanRefsOptions& opt,
							OP_Node *&newop,
							PRM_Parm *&newparm,
							int &newvecidx)
			    {
				return followChannelReferencesOneStepT(
					opt, newop, newparm, newvecidx,
					SYSgetSTID());
			    }
    static bool		 followChannelReferencesOneStepT(const OP_FollowChanRefsOptions& opt,
							 OP_Node *&newop,
							 PRM_Parm *&newparm,
							 int &newvecidx,
							 int thread);
    static void		 followChannelReferences(const OP_FollowChanRefsOptions& opt,
						 OP_Node *&newop,
						 PRM_Parm *&newparm,
						 int &newvecidx)
			    {
				followChannelReferencesT(
					opt, newop, newparm, newvecidx,
					SYSgetSTID());
			    }
    static void		 followChannelReferencesT(const OP_FollowChanRefsOptions& opt,
						  OP_Node *&newop,
						  PRM_Parm *&newparm,
						  int &newvecidx,
						  int thread);
    static void		 followChannelReferences(const OP_FollowChanRefsOptions& opt, CH_ChannelRef &ref)
			    {
				followChannelReferencesT(opt, ref, SYSgetSTID());
			    }
    static void		 followChannelReferencesT(const OP_FollowChanRefsOptions& opt, CH_ChannelRef &ref,
						  int thread);
    
    static void		 getValueAsProcessedNodePaths(fpreal t, 
				       UT_WorkBuffer &resultbuf, 
				       PRM_Parm *parm, int index,
				       OP_Node *srcnode
                                       );
    
    const OP_Stat	&getStat() const	{ return myStats; }
    OP_Stat		&getStat()		{ return myStats; }

    //  "vi" refers to the vector index. "pi" is the parameter index.
    //  "pn" refers to the parmeter name. "v" refers to the vector component.
    //  if v == -1, all components are affected.
    bool		 enableParm(int   pi, int state, int v = -1);
    bool		 enableParm(const char *pn, int state, int v = -1);
    bool		 enableParmInst(const char *name, const int *inst,
					int state, int nestlevel = 1,
					int v = -1);
    unsigned		 getEnableState(int pi, int v = -1);
    unsigned		 getEnableState(const char *pn, int v = -1);
    unsigned		 getEnableStateInst(const char *name, const int *inst,
					    int nestlevel = 1, int v = -1);

    bool		 setVisibleState(int pi, bool state, int v = -1);
    bool		 setVisibleState(const char *pn, bool state, int v = -1);
    bool		 setVisibleStateInst(const char *name, const int *inst,
					int state, int nestlevel = 1,
					int v = -1);

    bool		 getVisibleState(int pi, int v = -1);
    bool		 getVisibleState(const char *pn, int v = -1);
    
    /// @anchor OP_Node_EvalParameters

    /// @name Parameter evaluation
    ///
    ///	- int pi	    @n Parameter index
    ///	- const char *pn    @n Parameter name token
    ///	- int *pi	    @n Parameter index passed by reference, looked up
    ///			       using 'pn'
    /// - PRM_Parm *parm    @n Parameter by reference
    ///	- int vi	    @n Component index of a parameter
    ///	- fpreal t	    @n Evaluation time
    ///	- int thread	    @n Thread id (ie. return value from SYSgetSTID())
    // @{
    exint		 evalInt(int pi, int vi, fpreal t) const
			    { return evalIntT(pi, vi, t, SYSgetSTID()); }
    exint		 evalIntT(int pi, int vi, fpreal t, int thread) const;
    exint		 evalInt(const UT_StringRef &pn, int vi, fpreal t) const
			    { return evalIntT(pn, vi, t, SYSgetSTID()); }
    exint		 evalIntT(const UT_StringRef &pn, int vi, fpreal t,
				  int thread) const;
    exint		 evalInt(const UT_StringRef &pn, int *pi, int vi,
				 fpreal t) const
			    { return evalIntT(pn, pi, vi, t, SYSgetSTID()); }
    exint		 evalIntT(const UT_StringRef &pn, int *pi, int vi,
				  fpreal t, int thread) const;
    exint		 evalInt(const PRM_Parm *parm, int vi, fpreal t) const
			    { return evalIntT(parm, vi, t, SYSgetSTID()); }
    exint		 evalIntT(const PRM_Parm *parm, int vi, fpreal t,
				  int thread) const;

    fpreal		 evalFloat(int pi, int vi, fpreal t) const
			    { return evalFloatT(pi, vi, t, SYSgetSTID()); }
    fpreal		 evalFloatT(int pi, int vi, fpreal t, int thread) const;
    fpreal		 evalFloat(const UT_StringRef &pn, int vi,
				   fpreal t) const
			    { return evalFloatT(pn, vi, t, SYSgetSTID()); }
    fpreal		 evalFloatT(const UT_StringRef &pn, int vi, fpreal t,
				    int thread) const;
    fpreal		 evalFloat(const UT_StringRef &pn, int *pi, int vi,
				   fpreal t) const
			    { return evalFloatT(pn, pi, vi, t, SYSgetSTID()); }
    fpreal		 evalFloatT(const UT_StringRef &pn, int *pi, int vi,
				    fpreal t, int thread) const;
    fpreal		 evalFloat(const PRM_Parm *parm, int vi, fpreal t) const
			    { return evalFloatT(parm, vi, t, SYSgetSTID()); }
    fpreal		 evalFloatT(const PRM_Parm *parm, int vi, fpreal t,
				    int thread) const;

    // Evaluate an array of floats
    void		 evalFloats(int pi, fpreal64 *v, fpreal t) const
			    { return evalFloatsT(pi, v, t, SYSgetSTID()); }
    void		 evalFloatsT(int pi, fpreal64 *v,
				     fpreal t, int thread) const;
    void		 evalFloats(const UT_StringRef &pn, fpreal64 *v,
				    fpreal t) const
			    { return evalFloatsT(pn, v, t, SYSgetSTID()); }
    void		 evalFloatsT(const UT_StringRef &pn, fpreal64 *v,
				     fpreal t, int thread) const;
    void		 evalFloats(const UT_StringRef &pn, int *pi,
				    fpreal64 *v, fpreal t) const
			    { return evalFloatsT(pn, pi, v, t, SYSgetSTID()); }
    void		 evalFloatsT(const UT_StringRef &pn, int *pi,
				     fpreal64 *v, fpreal t, int thread) const;
    void		 evalFloats(const PRM_Parm *parm, fpreal64 *v, fpreal t) const
			    { return evalFloatsT(parm, v, t, SYSgetSTID()); }
    void		 evalFloatsT(const PRM_Parm *parm, fpreal64 *v,
				     fpreal t, int thread) const;

    // @deprecated Please use the other evalFloats() versions
    // @{
    void		 evalFloatsT(int pi, fpreal32 *v,
				     fpreal t, int thread) const;
    void		 evalFloats(int pi, fpreal32 *v, fpreal t) const
			    { return evalFloatsT(pi, v, t, SYSgetSTID()); }
    void		 evalFloatsT(const UT_StringRef &pn, fpreal32 *v,
				    fpreal t, int thread) const;
    void		 evalFloats(const UT_StringRef &pn, fpreal32 *v,
				    fpreal t) const
			    { return evalFloatsT(pn, v, t, SYSgetSTID()); }
    void		 evalFloatsT(const UT_StringRef &pn, int *pi,
				    fpreal32 *v, fpreal t, int thread) const;
    void		 evalFloats(const UT_StringRef &pn, int *pi,
				    fpreal32 *v, fpreal t) const
			    { return evalFloatsT(pn, pi, v, t, SYSgetSTID()); }
    void		 evalFloatsT(const PRM_Parm *parm, fpreal32 *v,
				    fpreal t, int thread) const;
    void		 evalFloats(const PRM_Parm *parm, fpreal32 *v,
				    fpreal t) const
			    { return evalFloatsT(parm, v, t, SYSgetSTID()); }
    void		 evalFloatsInst(const UT_StringRef &name,
					const int *inst,
					fpreal32 *v, fpreal t,
					int nestlevel = 1) const;
    // @}

    // String evaluation, expanding any inline expressions (backticks) and
    // variables
    void		 evalString(UT_String &val, int   pi, int vi,
				    fpreal t) const
			    { return evalStringT(val, pi, vi, t, SYSgetSTID()); }
    void		 evalStringT(UT_String &val, int   pi, int vi, fpreal t,
				     int thread) const;
    void		 evalString(UT_String &val,
				    const UT_StringRef &pn, int vi,
				    fpreal t) const
			    { return evalStringT(val, pn, vi, t, SYSgetSTID()); }
    void		 evalStringT(UT_String &val,
				     const UT_StringRef &pn, int vi,
				     fpreal t, int thread) const;
    void		 evalString(UT_String &val, const char *pn, int *pi,
				    int vi, fpreal t) const
			    {
				return evalStringT(val, pn, pi, vi, t,
						   SYSgetSTID());
			    }
    void		 evalStringT(UT_String &val,
				     const UT_StringRef &pn, int *pi,
				     int vi, fpreal t, int thread) const;
    void		 evalString(UT_String  &val, const PRM_Parm *parm,
				    int vi, fpreal t) const
			    {
				return evalStringT(val, parm, vi, t,
						   SYSgetSTID());
			    }
    void		 evalStringT(UT_String  &val, const PRM_Parm *parm,
				     int vi, fpreal t, int thread) const;
    
    // String evaluation, with no expression or variable expansion
    void		 evalStringRaw(UT_String &val, int pi, int vi,
				       fpreal t) const
			    {
				return evalStringRawT(val, pi, vi, t,
						      SYSgetSTID());
			    }
    void		 evalStringRawT(UT_String &val, int pi, int vi, fpreal t,
					int thread) const;
    void		 evalStringRaw(UT_String &val,
				       const UT_StringRef &pn, int vi,
				       fpreal t) const
			    {
				return evalStringRawT(val, pn, vi, t,
						      SYSgetSTID());
			    }
    void		 evalStringRawT(UT_String &val,
					const UT_StringRef &pn, int vi,
					fpreal t, int thread) const;
    void		 evalStringRaw(UT_String &val, const UT_StringRef &pn,
				       int *pi, int vi, fpreal t) const
			    {
				return evalStringRawT(val, pn, pi, vi, t,
						      SYSgetSTID());
			    }
    void		 evalStringRawT(UT_String &val,
				        const UT_StringRef &pn, int *pi,
					int vi, fpreal t, int thread) const;
    void		 evalStringRaw(UT_String &val, const PRM_Parm *parm,
				       int vi, fpreal t) const
			    {
				return evalStringRawT(val, parm, vi, t,
						      SYSgetSTID());
			    }
    void		 evalStringRawT(UT_String &val, const PRM_Parm *parm,
					int vi, fpreal t, int thread) const;

    PRM_DataItemHandle	 evalData(int pi, int vi, fpreal t) const
			    { return evalDataT(pi, vi, t, SYSgetSTID()); }
    PRM_DataItemHandle	 evalDataT(int pi, int vi, fpreal t, int thread) const;
    PRM_DataItemHandle	 evalData(const UT_StringRef &pn, int vi,
				  fpreal t) const
			    { return evalDataT(pn, vi, t, SYSgetSTID()); }
    PRM_DataItemHandle	 evalDataT(const UT_StringRef &pn, int vi,
				   fpreal t, int thread) const;
    PRM_DataItemHandle	 evalData(const UT_StringRef &pn, int *pi, int vi,
				  fpreal t) const
			    { return evalDataT(pn, pi, vi, t, SYSgetSTID()); }
    PRM_DataItemHandle	 evalDataT(const UT_StringRef &pn, int *pi, int vi,
				   fpreal t, int thread) const;
    PRM_DataItemHandle	 evalData(const PRM_Parm *parm, int vi, fpreal t) const
			    { return evalDataT(parm, vi, t, SYSgetSTID()); }
    PRM_DataItemHandle	 evalDataT(const PRM_Parm *parm, int vi, fpreal t,
				  int thread) const;

    // JSON map evaluation, expanding any inline expressions (backticks) and
    // variables
    void		 evalJSONMap(UT_JSONValueMap &val, int pi, int vi,
				     fpreal t) const
			    { return evalJSONMapT(
				val, pi, vi, t, SYSgetSTID()); }
    void		 evalJSONMapT(UT_JSONValueMap &val, int pi, int vi,
				      fpreal t, int thread) const;
    void		 evalJSONMap(UT_JSONValueMap &val,
				     const UT_StringRef &pn, 
				     int vi, fpreal t) const
			    { return evalJSONMapT(
				val, pn, vi, t, SYSgetSTID()); }
    void		 evalJSONMapT(UT_JSONValueMap &val,
				      const UT_StringRef &pn, 
				      int vi, fpreal t, int thread) const;
    void		 evalJSONMap(UT_JSONValueMap &val,
				     const UT_StringRef &pn, 
				     int *pi, int vi, fpreal t) const
			    {
				return evalJSONMapT(
				    val, pn, pi, vi, t, SYSgetSTID());
			    }
    void		 evalJSONMapT(UT_JSONValueMap &val,
				      const UT_StringRef &pn,
				      int *pi, int vi, fpreal t, int thread) 
				      const;
    void		 evalJSONMap(UT_JSONValueMap  &val, 
				     const PRM_Parm *parm, int vi, fpreal t)
				     const
			    {
				return evalJSONMapT(
				    val, parm, vi, t, SYSgetSTID());
			    }
    void		 evalJSONMapT(UT_JSONValueMap  &val, 
				      const PRM_Parm *parm, int vi, fpreal t,
				      int thread) const;
    
    // JSON map evaluation, with no expression or variable expansion
    void		 evalJSONMapRaw(UT_JSONValueMap &val, int pi, int vi,
				        fpreal t) const
			    {
				return evalJSONMapRawT(
				    val, pi, vi, t, SYSgetSTID());
			    }
    void		 evalJSONMapRawT(UT_JSONValueMap &val, int pi, int vi,
					 fpreal t, int thread) const;
    void		 evalJSONMapRaw(UT_JSONValueMap &val,
					const UT_StringRef &pn,
					int vi, fpreal t) const
			    {
				return evalJSONMapRawT(
				    val, pn, vi, t, SYSgetSTID());
			    }
    void		 evalJSONMapRawT(UT_JSONValueMap &val,
					 const UT_StringRef &pn,
					 int vi, fpreal t, int thread) const;
    void		 evalJSONMapRaw(UT_JSONValueMap &val,
					const UT_StringRef &pn,
					int *pi, int vi, fpreal t) const
			    {
				return evalJSONMapRawT(
				    val, pn, pi, vi, t, SYSgetSTID());
			    }
    void		 evalJSONMapRawT(UT_JSONValueMap &val,
					 const UT_StringRef &pn,
					 int *pi, int vi, fpreal t, int thread)
					 const;
    void		 evalJSONMapRaw(UT_JSONValueMap &val, 
					const PRM_Parm *parm, int vi, fpreal t)
					const
			    {
				return evalJSONMapRawT(
				    val, parm, vi, t, SYSgetSTID());
			    }
    void		 evalJSONMapRawT(UT_JSONValueMap &val, 
					 const PRM_Parm *parm, int vi, 
					 fpreal t, int thread) const;
    // @}

    /// @name Multi-Parameter evaluation
    ///
    ///	- name	    @n Parameter name token
    /// - inst	    @n Array of instance indices to evaluate at
    ///	- vi	    @n Component index of a parameter
    ///	- t	    @n Evaluation time
    /// - nestlevel @n Number of entries in 'inst'
    ///	- thread    @n Thread id (ie. return value from SYSgetSTID())
    // @{
    exint		 evalIntInst(const UT_StringRef &name,
				     const int *inst, int vi,
				     fpreal t, int nestlevel = 1) const
			    {
				return evalIntInstT(name, inst, vi, t,
						    SYSgetSTID(), nestlevel);
			    }
    exint		 evalIntInstT(const UT_StringRef &name,
				      const int *inst, int vi,
				      fpreal t, int thread,
				      int nestlevel = 1) const;
    fpreal		 evalFloatInst(const UT_StringRef &name,
				       const int *inst, int vi,
				       fpreal t, int nestlevel = 1) const
			    {
				return evalFloatInstT(name, inst, vi, t,
					SYSgetSTID(), nestlevel);
			    }
    fpreal		 evalFloatInstT(const UT_StringRef &name,
					const int *inst, int vi,
					fpreal t, int thread,
					int nestlevel = 1) const;
    void		 evalFloatsInst(const UT_StringRef &name,
					const int *inst,
					fpreal64 *v, fpreal t,
					int nestlevel = 1) const
			    {
				return evalFloatsInstT(name, inst, v, t,
					SYSgetSTID(), nestlevel);
			    }
    void		 evalFloatsInstT(const UT_StringRef &name,
					 const int *inst,
					 fpreal64 *v, fpreal t, int thread,
					 int nestlevel = 1) const;
    void		 evalStringInst(const UT_StringRef &name,
					const int *inst,
					UT_String &val, int vi, fpreal t,
					int nestlevel = 1) const
			    {
				return evalStringInstT(name, inst, val, vi,
					t, SYSgetSTID(), nestlevel);
			    }
    void		 evalStringInstT(const UT_StringRef &name,
					 const int *inst,
					 UT_String &val, int vi, fpreal t,
					 int thread, int nestlevel = 1) const;
    void		 evalStringInstRaw(const UT_StringRef &name,
					   const int *inst,
					   UT_String &val, int vi, fpreal t,
					   int nestlevel = 1) const
			    {
				return evalStringInstRawT(name, inst, val, vi,
					t, SYSgetSTID(), nestlevel);
			    }
    void		 evalStringInstRawT(const UT_StringRef &name,
					    const int *inst,
					    UT_String &val, int vi, fpreal t,
					    int thread,
					    int nestlevel = 1) const;
    PRM_DataItemHandle	 evalDataInst(const UT_StringRef &name,
				      const int *inst, int vi,
				     fpreal t, int nestlevel = 1) const
			    {
				return evalDataInstT(name, inst, vi, t,
						    SYSgetSTID(), nestlevel);
			    }
    PRM_DataItemHandle	 evalDataInstT(const UT_StringRef &name,
				       const int *inst, int vi,
				      fpreal t, int thread,
				      int nestlevel = 1) const;
    // @}

    /// @name Property evaluation
    /// 
    /// Properties will be evaluated if the parameter is not found.
    ///
    ///	- parm_name	@n Parameter name token
    ///	- vec_index	@n Component index of a parameter
    ///	- now		@n Evaluation time
    ///	- thread	@n Thread id (ie. return value from SYSgetSTID())
    /// - parmptr	@n optional source parameter return value
    // @{
    bool		evalParameterOrProperty(const UT_StringRef &parm_name,
						int vec_index,
						fpreal now,
						int &value,
						PRM_Parm **parmptr = NULL) const
			    {
				return evalParameterOrPropertyT(parm_name,
					vec_index, now, value, SYSgetSTID(),
					parmptr);
			    }
    bool		evalParameterOrPropertyT(const UT_StringRef &parm_name,
						 int vec_index,
						 fpreal now,
						 int &value,
						 int thread,
						 PRM_Parm **parmptr=NULL) const;
    bool		evalParameterOrProperty(const UT_StringRef &parm_name,
						int vec_index,
						fpreal now,
						fpreal &values,
						PRM_Parm **parmptr=NULL) const
			    {
				return evalParameterOrPropertyT(parm_name,
					vec_index, now, values, SYSgetSTID(),
					parmptr);
			    }
    bool		evalParameterOrPropertyT(const UT_StringRef &parm_name,
						 int vec_index,
						 fpreal now,
						 fpreal &values, int thread,
						 PRM_Parm **parmptr=NULL) const;
    bool		evalParameterOrProperty(const UT_StringRef &parm_name,
						int vec_index,
						fpreal now,
						UT_String &value,
						PRM_Parm **parmptr=NULL) const
			    {
				return evalParameterOrPropertyT(parm_name,
					 vec_index, now, value, SYSgetSTID(),
					 parmptr);
			    }
    bool		evalParameterOrPropertyT(const UT_StringRef &parm_name,
						 int vec_index,
						 fpreal now,
						 UT_String &value,
						 int thread,
						 PRM_Parm **parmptr=NULL) const;
    bool		evalParameterOrPropertyRaw(
				const UT_StringRef &parm_name,
				int vec_index,
				fpreal now,
				UT_String &value,
				PRM_Parm **parmptr=NULL) const
			    {
				return evalParameterOrPropertyRawT(parm_name,
					  vec_index, now, value, SYSgetSTID(),
					  parmptr);
			    }
    bool		evalParameterOrPropertyRawT(
				const UT_StringRef &parm_name,
				int vec_index,
				fpreal now,
				UT_String &value,
				int thread,
				PRM_Parm **parmptr=NULL) const;
    bool		evalParameterOrPropertyExpr(
				const UT_StringRef &parm_name,
				int vec_index,
				fpreal now,
				UT_String &value,
				PRM_Parm **parmptr=NULL) const
			    {
				return evalParameterOrPropertyExprT(parm_name,
					 vec_index, now, value, SYSgetSTID(),
					 parmptr);
			    }
    bool		evalParameterOrPropertyExprT(
				const UT_StringRef &parm_name,
				int vec_index,
				fpreal now,
				UT_String &value,
				int thread,
				PRM_Parm **parmptr=NULL) const;

    bool		evalVectorProperty(const UT_StringRef &parm_name,
					   fpreal now,
					   int *values,
					   int vec_size,
					   PRM_Parm **parmptr = NULL) const
			    {
				return evalVectorPropertyT(parm_name,
					   now, values, vec_size, SYSgetSTID(),
					   parmptr);
			    }
    bool		evalVectorPropertyT(const UT_StringRef &parm_name,
					    fpreal now,
					    int *values,
					    int vec_size,
					    int thread,
					    PRM_Parm **parmptr = NULL) const;
    bool		evalVectorProperty(const UT_StringRef &parm_name,
					   fpreal now,
					   fpreal *values,
					   int vec_size,
					   PRM_Parm **parmptr = NULL) const
			    {
				return evalVectorPropertyT(parm_name,
					   now, values, vec_size, SYSgetSTID(),
					   parmptr);
			    }
    bool		evalVectorPropertyT(const UT_StringRef &parm_name,
					    fpreal now,
					    fpreal *values,
					    int vec_size,
					    int thread,
					    PRM_Parm **parmptr = NULL) const;
    // @}

    /// @anchor OP_Node_SetParameters

    /// @name Setting parameter values
    ///
    /// - parmi     @n Parameter index
    /// - parmname  @n Parameter name string
    /// - vectori   @n Component index of a parameter
    /// - t         @n Evaluation time
    /// - value     @n Value to set
    /// - h         @n If true, it will set a keyframe if the parameter
    ///                has a channel. If false, then the value will
    ///                need to committed first.
    // @{
    void		 setInt(int parmi, int vectori, fpreal t, exint value);
    void		 setInt(const char *parmname, int vectori, fpreal t, exint value);
    void		 setInt(const char *parmname, int &parmi, int vectori, fpreal t,
				exint value);
    void		 setFloat(int parmi, int vectori, fpreal t,
				  fpreal value,
				  PRM_AddKeyType add_key=PRM_AK_MARK_PENDING);
    void		 setFloat(const char *parmname, int vectori,  fpreal t,
				  fpreal value,
				  PRM_AddKeyType add_key=PRM_AK_MARK_PENDING);
    void		 setFloat(const char *parmname, int &parmi, int vectori,
				  fpreal t, fpreal value,
				  PRM_AddKeyType add_key=PRM_AK_MARK_PENDING);
    void		 setString(const UT_StringRef &val,
				   CH_StringMeaning meaning,
				   int parmi, int vectori,
				   fpreal t);
    void		 setString(const UT_StringRef &val,
				   CH_StringMeaning meaning,
				   const char *parmname, int vectori, fpreal t);
    void		 setString(const UT_StringRef &val,
				   CH_StringMeaning meaning,
				   const char *parmname,
				   int &parmi, int vectori, fpreal t);
    void		 setData(int parmi, int vectori, fpreal t,
				 const PRM_DataItemHandle &val);
    void		 setData(const char *parmname, int vectori, fpreal t,
				 const PRM_DataItemHandle &val);
    void		 setData(const char *parmname, int &parmi, int vectori, fpreal t,
				 const PRM_DataItemHandle &val);
    // @}

    /// @name Setting parameter values, following channel references
    ///
    /// These functions will follow simple channel references first, and then 
    /// set the value on the destination parameter.
    ///
    ///	- pi	    @n Parameter index
    ///	- pn	    @n Parameter name string
    ///	- vi	    @n Component index of a parameter
    ///	- t	    @n Time at which to set the value
    /// - val	    @n Value to set
    /// - add_key   @n If PRM_AK_SET_KEY, it will set a keyframe if the 
    ///		       parameter has a channel and the current value differs.
    ///		       If PRM_AK_FORCE_KEY, it will set a keyframe if the
    ///		       parameter has a channel regardless of the current value.
    ///		       If PRM_AK_MARK_PENDING, then the value will need to be
    ///		       committed first.
    /// - propagate @n If true, it notify observers
    ///	- thread    @n Thread id (ie. return value from SYSgetSTID())
    // @{
    void		 setChRefInt(int pi, int vi, fpreal t, exint val,
				     PRM_AddKeyType add_key=PRM_AK_MARK_PENDING,
				     bool propagate=true);
    void		 setChRefInt(const char *pn, int vi, fpreal t, exint val,
        		             PRM_AddKeyType add_key=PRM_AK_MARK_PENDING,
        		             bool propagate=true);
    void		 setChRefFloat(int pi, int vi, fpreal t, fpreal val, 
        		               PRM_AddKeyType add_key=
					    PRM_AK_MARK_PENDING,
				       bool propagate=true);
    void		 setChRefFloat(const char *pn, int vi, fpreal t,
				       fpreal val,
				       PRM_AddKeyType add_key=
					    PRM_AK_MARK_PENDING,
				       bool propagate=true);
    void		 setChRefString(const UT_StringRef &val,
					CH_StringMeaning meaning,
					int pi, int vi, 
					fpreal t,
					PRM_AddKeyType add_key=
					    PRM_AK_MARK_PENDING,
					bool propagate=true);
    void		 setChRefString(int pi, int vi, 
					fpreal t,
					const UT_StringRef &val,
					CH_StringMeaning meaning,
					PRM_AddKeyType add_key=
					    PRM_AK_MARK_PENDING,
					bool propagate=true)
			    {
				setChRefString(val, meaning, pi, vi, t,
				               add_key, propagate);
			    }
    void		 setChRefString(const UT_StringRef &val,
					CH_StringMeaning meaning,
					const char *pn,
					int vi, fpreal t,
					PRM_AddKeyType add_key=
					    PRM_AK_MARK_PENDING,
					bool propagate=true);
    void		 setChRefString(const char *pn,
					int vi, fpreal t,
					const UT_StringRef &val,
					CH_StringMeaning meaning,
					PRM_AddKeyType add_key=
					    PRM_AK_MARK_PENDING,
					bool propagate=true)
			    {
				setChRefString(val, meaning, pn, vi, t,
				               add_key, propagate);
			    }
    // @}

    /// @name Setting property values
    ///
    /// The setParameterOrProperty methods will track down, not only channel
    /// references, but also property parameter references.
    /// The bool result indicates whether the parameter was actually set.
    /// Some possible causes of failure are:
    ///	- Parameter or property not found
    ///	- Permissions error
    ///	- Locked parameter
    ///	- Out of bounds vector index
    ///
    /// Parameters:
    ///	- parm_name @n Parameter name string
    ///	- vec_index @n Component index of a parameter
    ///	- now	    @n Time at which to set the value
    /// - val	    @n Value to set
    /// - set_key   @n If true, it will set a keyframe if the parameter
    ///		    has a channel. If false, then the value will
    ///		    need to committed first.
    /// - propagate @n If true, it notify observers
    // @{
    bool		 setParameterOrProperty(const char *parm_name,
						int vec_index,
						fpreal now,
						int32 value,
						bool set_key=false,
						bool propagate=true);
    bool		 setParameterOrProperty(const char *parm_name,
						int vec_index,
						fpreal now,
						int64 value,
						bool set_key=false,
						bool propagate=true);
    bool		 setParameterOrProperty(const char *parm_name,
						int vec_index,
						fpreal now,
						fpreal value,
						bool set_key=false,
						bool propagate=true);
    bool		 setParameterOrProperty(const char *parm_name,
						int vec_index,
						fpreal now,
						const char *value,
						CH_StringMeaning meaning,
						bool set_key=false,
						bool propagate=true);
    // @}

    /// @name Multi-Parameter evaluation
    ///
    /// - name      @n Parameter name token
    /// - inst      @n Array of instance indices to evaluate at
    /// - vectori   @n Component index of a parameter
    /// - t         @n Evaluation time
    /// - nestlevel @n Number of entries in 'inst'
    /// - value     @n Value to set
    // @{
    void		 setIntInst(exint value, const char *parmname,
				    const int *inst, int vectori, fpreal t,
				    int nestlevel = 1);
    void		 setFloatInst(fpreal value, const char *parmname,
				      const int *inst, int vectori, fpreal t,
				      int nestlevel = 1);
    void		 setStringInst(const UT_StringRef &value,
				       CH_StringMeaning meaning,
				       const char *parmname,
				       const int *inst, int vectori, fpreal t,
				       int nestlevel = 1);
    void		 setDataInst(const PRM_DataItemHandle &value,
				     const char *parmname, const int *inst,
				     int vectori, fpreal t, int nestlevel = 1);
    // @}

    /// @name Setting parameter values silently
    ///
    /// These methods do not trigger additional events to notify observers
    ///
    /// - int pi            @n Parameter index
    /// - const char *pn    @n Parameter name string
    /// - int &pi           @n Parameter index, returned from 'pn' lookup
    /// - int vi            @n Component index of a parameter
    /// - fpreal t          @n Time at which to set the value
    /// - value             @n Value to set
    // @{
    void		 setSilentInt(int parmi, int vectori, fpreal t,
				exint value);
    void		 setSilentInt(const char *parmname, int vectori,
				fpreal t, exint value);
    void		 setSilentInt(const char *parmname, int &parmi,
				int vectori, fpreal t,exint value);

    void		 setSilentFloat(int parmi, int vectori, fpreal t,
				fpreal value,
				PRM_AddKeyType add_key=PRM_AK_MARK_PENDING);
    void		 setSilentFloat(const char *parmname, int vectori,
				fpreal t, fpreal value,
				PRM_AddKeyType add_key=PRM_AK_MARK_PENDING);
    void		 setSilentFloat(const char *parmname,
				int &parmi, int vectori,
				fpreal t, fpreal value,
				PRM_AddKeyType add_key=PRM_AK_MARK_PENDING);

    void		 setSilentString(const UT_StringRef &value,
				CH_StringMeaning meaning,
				int parmi, int vectori, fpreal t);
    void		 setSilentString(const UT_StringRef &value,
				CH_StringMeaning meaning,
				const char *parmname, int vectori, fpreal t);
    void		 setSilentString(const UT_StringRef &value,
				CH_StringMeaning meaning,
				const char *parmname,
				int &parmi, int vectori, fpreal t);
    void		 setSilentData(int parmi, int vectori, fpreal t,
				const PRM_DataItemHandle &value);
    void		 setSilentData(const char *parmname, int vectori,
				fpreal t, const PRM_DataItemHandle &value);
    void		 setSilentData(const char *parmname,
				int &parmi, int vectori, fpreal t,
				const PRM_DataItemHandle &value);

    void		 setSilentIntInst(exint value, const char *parmname,
				const int *inst, int vectori,
				fpreal t, int nestlevel = 1);
    void		 setSilentFloatInst(fpreal value, const char *parmname,
				const int *inst, int vectori,
				fpreal t, int nestlevel = 1);
    void		 setSilentStringInst(const UT_StringRef &value,
				CH_StringMeaning meaning,
				const char *parmname,
				const int *inst, int vectori,
				fpreal t, int nestlevel = 1);
    void		 setSilentDataInst(const PRM_DataItemHandle &value,
				const char *parmname, const int *inst,
				int vectori, fpreal t, int nestlevel = 1);
    // @}

    CH_StringMeaning	 getStringMeaning(int pi, int vi, fpreal t) const;
    CH_StringMeaning	 getStringMeaning(const char *pn, int vi,
				fpreal t) const;
    CH_StringMeaning	 getStringMeaningInst(const char *name, const int *inst,
				int vi, fpreal t, int nest_level = 1) const;

    // Batch setting of parameters.  The value stored in the batch list will be
    // assigned to the operator/parameter which the parameter was last
    // evaluated from.  This assumes that the parameter list has actually been
    // evaluated.  Modifications will be done in block mode.
    static void		 setBatchParms(PRM_BatchList &list, fpreal now,
					bool set_key=false,
					bool create_channels=false);
    static bool		 canSetBatchParms(PRM_BatchList &list, fpreal now);

    // When a new expression is created it will use the node's expression
    // language.
    CH_ExprLanguage	 getExprLanguage() const override;
    void		 setExprLanguage(CH_ExprLanguage language);

    // multiparm manipulation
    void		 insertMultiParmItem(const char *pn, int i);
    void		 removeMultiParmItem(const char *pn, int i);

    /// updateMultiParmFromRamp() will follow simple channel references as
    /// necessary
    void		 updateMultiParmFromRamp(fpreal t, const UT_Ramp &ramp,
				PRM_Parm &m, bool add_wrap,
				PRM_AddKeyType add_key=PRM_AK_MARK_PENDING);
    void		 updateRampFromMultiParm(fpreal t, const PRM_Parm &m,
				UT_Ramp &ramp, bool *time_dep = NULL,
				DEP_MicroNode *depnode = NULL) const;

    // This method converts an obsolete animated parm to a string parm.  It 
    // will create the necessary backticks for expressions.  It does not, 
    // however, support keyframed channel data or multiple-segment channels,
    // in which case the evaluated value at the given time is returned.
    // It also does not support CHOP overrides.
    // When reset_obs is true, the obsolete parameter is reset to factory
    // defaults after the expression has been extracted.
    // Returns true if successful, and false otherwise.  False is returned
    // if the obsolete is at the factory default.
    bool		 convertObsAnimatedToStringExpression(
				PRM_ParmList *obsolete_parms,
				const char *pn, int vi, fpreal t,
				UT_String &result, bool reset_obs);

    /// Expand expressions within string value.
    ///
    /// This method will expand backtick expressions and local/global
    /// variables by modifying the input string. Always use this method instead
    /// of calling CH_Manager::expandString() directly within node evaluation
    /// code paths in order to establish proper node dependencies.
    ///
    /// @param channel_name	Channel name of the parameter from where val
    ///				was obtained from. This is important for
    ///				establishing data and time dependencies.
    ///
    bool		 expandString(UT_String &val, fpreal t, int thread,
				      const char *channel_name = 0);

    virtual int		 findString(const char *str, bool fullword,
				    bool usewildcards) const;
    virtual int		 changeString(const char *from, const char *to,
				      bool fullword);

    // These methods check to see if the current value differs from
    // the cached ones.  The time is the time to evaluate the parm at,
    // this does NOT support independent dirty flags for multiple times
    // (and likely you wouldn't want that anyways)
    // cacheParmValues ONLY saves those who had been tested for dirtiness
    //
    // NOTE: This is deprecated, please use the override the
    //       OP_Node::parmListMicroNode() method to detect parm value changes.
    //
    //       This legacy mechanism will cause your parameter to be evaluated
    //       multiple times per cook.
    bool		 isParmDirty(int idx, fpreal t);
    bool		 isParmDirty(const char *n, fpreal t);
    bool		 isParmDirtyInst(const char *name, const int *inst,
					 fpreal t, int nestlevel=1);
    void		 cacheParmValues(const OP_Context &context);
    void		 clearParmCache();

    /// Query if a parameter is time dependent, by parm index. If comp == -1,
    /// any vector component that is time dependent will return true for the
    /// parm.
    bool		 isParmTimeDependent(int idx, int comp = -1) const;

    /// Query if a parameter is time dependent, by parm name. If comp == -1,
    /// any vector component that is time dependent will return true for the
    /// parm.
    bool		 isParmTimeDependent(const char *name,
				int comp = -1) const;

    // error stuff
    int			 getErrorMessages(UT_String &messages, 
					  UT_ErrorSeverity severity
					  = UT_ERROR_NONE)
    			 {
			     ensureErrorsAreUpdatedSubclass();

			     OP_Lock::Scope lock(myErrorManagerLock);
			     stealGlobalErrors();
			     return myErrorManager.getErrorMessages(messages,
								    severity);
			 }

    virtual void	 getRawErrors(UT_Array<UT_Error> &errors, bool update);

    int			 findError(const char *type, int code) const
    			 {
			     OP_Lock::Scope lock(myErrorManagerLock);
			     return myErrorManager.findError(type, code);
			 }

    int			 removeError(int idx)
    			 {
			     OP_Lock::Scope lock(myErrorManagerLock);
			     return myErrorManager.removeError(idx);
			 }

    void		 clearDuplicateErrors()
			 {
			     OP_Lock::Scope lock(myErrorManagerLock);
			     myErrorManager.pruneDuplicates();
			 }

    void		 opMessage(int code, const char *m=0) const
			 { UTaddMessage("OP",code,m);}
    void		 opWarning(int code, const char *m=0) const
			 { UTaddWarning("OP",code,m);}
    void		 opError(int code, const char *m=0) const
			 { UTaddError("OP", code, m);}
    void		 opFatal(int code, const char *m=0) const
			 { UTaddFatal("OP", code, m);}

    void		 opLocalError(int code, const char *m=0)
			 {
			     OP_Lock::Scope lock(myErrorManagerLock);
			     myErrorManager.addError("OP", code, m);
			 }
    void		 opLocalWarning(int code, const char *m=0)
			 {
			     OP_Lock::Scope lock(myErrorManagerLock);
			     myErrorManager.addWarning("OP",code,m);
			 }
    void		 opLocalDeadlockError()
			 {
			     OP_Lock::Scope lock(myErrorManagerLock);
			     myErrorManager.addDeadlockError();
			 }

    /// appendError() adds a generic error *directly* to this node's error
    /// manager.
    void		 appendError(const char *type,
				     int code, const char *m,
				     UT_ErrorSeverity sev)
			 {
			     if (!UTgetErrorManager()->isDisabled())
			     {
				 OP_Lock::Scope lock(myErrorManagerLock);
				 myErrorManager.addGeneric(type, code, m, sev);
			     }
			 }

    void		 clearErrors() // use error(OP_Context&) to avoid querying the current thread id
			 {
			     UTgetErrorManager()->clearAndDestroyErrors();
			     OP_Lock::Scope lock(myErrorManagerLock);
			     myErrorManager.clearAndDestroyErrors(); 
			 }

    void		 clearErrors(OP_Context &context)
			 {
			     UTgetErrorManager(context.getThread())->clearAndDestroyErrors();
			     OP_Lock::Scope lock(myErrorManagerLock);
			     myErrorManager.clearAndDestroyErrors(); 
			 }

    virtual OP_ERROR	 getErrorSeverity();

    bool		 hasDeadlockError() const
			 {
			     // We intentionally do NOT lock in this case
			     return myErrorManager.hasDeadlockError();
			 }

    virtual OP_ERROR	 error(OP_Context &context)
			 {
			    ensureErrorsAreUpdatedSubclass();

			    UT_ErrorManager &mgr = *UTgetErrorManager(context.getThread());
			    if (mgr.getNumErrors() > 0)
			    {
				stealGlobalErrors();
			    }
			    else
			    {
				// Even if we have no errors, we still need to
				// update the severity because error logging
				// might be disabled.
				myErrorManager.updateSeverity(
					mgr.getSeverity());
			    }
			    return myErrorManager.getSeverity();
			 }
    virtual OP_ERROR	 error() // use error(OP_Context&) to avoid querying the current thread id
			 {
			    ensureErrorsAreUpdatedSubclass();

			    UT_ErrorManager &mgr = *UTgetErrorManager();
			    if (mgr.getNumErrors() > 0)
			    {
				stealGlobalErrors();
			    }
			    else
			    {
				// Even if we have no errors, we still need to
				// update the severity because error logging
				// might be disabled.
				myErrorManager.updateSeverity(
					mgr.getSeverity());
			    }
			    return myErrorManager.getSeverity();
			 }
    void		 stealErrors(UT_ErrorManager &src,
				     bool borrow_only = false);
    OP_ERROR		 getSaveError();
    bool		 isGettingSaveError() const
			 { return myIsGettingSaveError; }

    // This method should only be called from OP_Channels.
    void		 addChannelError(const char *type, int code,
					 const char *msg = 0);

    void		 chChanged(CH_Channel *ch, CH_CHANGE_TYPE reason);

    virtual void	 saveParmsForUndo(bool scopedOnly = false,
					  CH_Collection *collection = NULL);
    void		 saveSingleParmForUndo(PRM_Parm *parm);
    void		 saveParmForUndo(PRM_Parm *);
    virtual void	 saveKeyFrameForUndo(CH_Channel *chp, fpreal gtime);

    // Clear any undo flags for the node in question
    virtual void	 clearUndoFlags();
    virtual void	 setAnyUndoFlag();
    int			 getAnyUndoPending() const { return myAnyUndoFlagSet; }

    int			 canAccess(unsigned mask) const;
    bool		 canAccess(unsigned mask, const PRM_Parm *parm) const;
    bool		 canAccessChannel(uint mask,
					  const CH_Channel *chp) const;
    void		 updateUserMask();

    bool		 canModifyFromUI(const PRM_Parm *parm, int vi) const;

    // If an OP references external files, this method should be implemented to
    // reload new data which is out of date with the external file.  If data
    // was re-loaded, then the method should return 1 (otherwise 0).
    virtual int		 reloadNewFiles(fpreal time_now);

    void		 triggerOPChanged(OP_EventType reason, int index);

    void		 parmChanged( int parm_index );
    void		 parmExprChanged(int parm_index);

    // This is called when our OP_Operator is deleted before we are.
    void		 operatorDeleted();

    // These functions deal with the opparm command.
    void		 saveParmCommand(std::ostream &os, int values = 0,
					 int defaultstoo = 1,
					 bool spareparms = true) const;
    int			 loadParmCommand(int argc, const char *const argv[],
					 int docallbacks, void *data,
					 fpreal now, bool followchrefs,
					 CMD_Args *args);
    void		 commandParmCallback(void *data, fpreal now,
					     int argc, char *argv[],
					     CMD_Args *args);
    void		 saveMultiParmInstanceCommand(
					 std::ostream &os,
					 const char *opname,
					 bool spareparms) const;

    // Run the callback function for a particular parm, and the callbacks
    // of any parms linked to this parm. This function is implemented in
    // OP_Node.
    virtual bool	 triggerParmCallback(PRM_Parm *parmptr, fpreal now,
					     int value, void *data,
					     const UT_Options *options = 0) = 0;

    int64	 getMemoryUsage(bool inclusive) const override;

    void		 bumpTakeReference(int dir)
			     { myTakeReferences += dir; }
    int			 getTakeReferences() const
			     { return myTakeReferences; }


    /// This method will create channel groups and populate them with
    /// channels from this operator.  The channel groups created will
    /// correspond to the folders in this operator's parameters.
    /// Unless only_channels is specified, all auto-scoped parms will get 
    /// included.
    void		 createChannelGroupsFromFolders(CH_Group &parent_group,
					    bool only_channels,
					    bool leaf_channels_only = false,
					    bool only_unlocked = true);

    /// Returns a list of the autoscopeable channels for this node.
    ///  if follow_layer_overrides is true, the channel references are followed but only for animation layers.
    ///  if include_animated is true, channels without the auto scope flag but with animation will also be returned.
    void		 getAutoScopedChannels(CH_ChannelRefList &chlist,
					    bool follow_layer_overrides=true,
					    bool include_animated=false,
					    bool follow_channel_refs=true,
					    bool include_autoscope=true
                                            );

    // Returns command to chopscope CHOPed channels for this node.
    void		 getChopScopeChannels(UT_WorkBuffer &chopscope_cmd);

    // Returns true if the specified parameter is visible in the interface
    // templates. This accounts for visiblity of containing parm pages
    // as well as the parm itself.
    bool		 getParmIsVisible(const char *parmname);

    // Returns a pointer to the templates used in constructing dialogs for
    // this node.
    const PRM_Template	    *getInterfaceParmTemplates() const;

    const PRM_Template	    *getSpareParmTemplates() const;
    const PRM_ScriptImports *getSpareParmTemplateImports() const;
    const PRM_Template	    *getSpareParmLayoutTemplates() const;
    const PRM_ScriptImports *getSpareParmLayoutTemplateImports() const;
    int			     getSpareParmTemplatesId() const;

    /// The number of potential spare inputs created by spare 
    /// parameters.  These are spare_input0 and onwards.  There may be holes.
    int			     numSpareInputs() const;

    // Get our multiparm instance info used by initMultiparmInstance.
    bool		     hasMultiparmInfo() const;
    OP_MultiparmInfo	    &getMultiparmInfo();

    // This function is caled when any multiparm has a new instance added.
    // It gives the parm owner a chance to initialize the multiparm values.
    void	 initMultiparmInstance(UT_ValArray<PRM_Parm *> &parms) override;
    // This function sends an opChanged notification of our multiparm change.
    void	 addOrRemoveMultiparmInstance() override;

    bool	 savePresetContents(std::ostream &os) override;
    bool	 loadPresetContents(const char *tok, UT_IStream &is) override;

    static void		 buildMParmInst(const PRM_Template *templates,
				UT_StringArray &mparm_inst,
				UT_IntArray &mparm_lvl,
				int level);
    static bool		 isMParmInstance(const char *chname,
				const UT_StringArray &mparm_inst,
				const UT_IntArray &mparm_lvl);

    // Retrieve Animation Layers from a Scoped Channel List
    static void		 getLayersFromScopedChannels(
				const CH_ChannelRefList &ori_refs,
				CH_ChannelRefList &layer_refs );

protected:
    virtual void	 doGetFullPath(UT_WorkBuffer &str) const override;

    //  Uses the specified conditional stored on the PRM_Template for the
    //  specified parameter to check whether the specified parm should
    //  be updated based on the conditional. Returns false if the parm state
    //  doesn't change.
    bool		 updateParmDisableConditional(
				PRM_Parm &parm,
				bool	 force,
				const UT_StringArray *skip = 0);

    bool		 updateParmHideConditional(
				PRM_Parm &parm,
				const UT_StringArray *skip = 0);


    //  Returns zero if no change was made. This function is overridden
    //  by subclasses to disable parms specific to those nodes.
    //  NOTE: Superseded by the updateParmsFlags() function.
    SYS_DEPRECATED(12.0)
    virtual unsigned	 disableParms();

    //  This function is overridden by subclasses to update parm flags specific
    //  to those nodes.
    //  Returns false if no change was made.
    virtual bool	 updateParmsFlags() { return false; }
    
    
    void		 saveMultiParmForUndo(int index);

    void		 stealGlobalErrors();
    void		 stealErrors(OP_Parameters &src);

    // NOTE: Do not hold onto the ptr object longer than necessary!
    UT_LockedRawPtr<UT_ErrorManager,OP_Lock>
    getLockedErrorManager()
    {
	return UT_LockedRawPtr<UT_ErrorManager,OP_Lock>(
		    myErrorManager, myErrorManagerLock);
    }

    virtual void	 permissionError(const char *chname = 0);

    // checkExpressionError() is split up into a checking part and an actual
    // error handling part, evaluationError() so that the checking part can
    // be inlined.
    SYS_FORCE_INLINE bool checkExpressionError(
                                    const PRM_Parm *parm, int thread) const
			    {
				int err = ev_GetErrNo(thread);
				if (err == EV_NOERROR)
				    return false;
				evaluationError(parm, err, thread);
				return true;
			    }

    void		 resetParmEventHandler();

    int			*allocIndirect(int size=64);

	     OP_Parameters(const char *name, OP_Operator *entry);
	     OP_Parameters(const OP_Parameters &source, const char *name);
    virtual ~OP_Parameters();

    //
    // The "initializeParms" method must be called to provide the channel
    // collection and build the parameter list.
    //
    void		 initializeParms(OP_Channels *channels);

    // op dependencies
    virtual void	 buildOpDependencies()
			 { }
    virtual void	 checkChannelDependencies(CH_Channel * /*ch*/,
						  CH_CHANGE_TYPE /*reason*/)
			 { }

    static bool		 isReservedOpName(const char *name);

    virtual int		 rename(const char *newname);
    virtual bool	 createSpareParameterFromChannel(const char *chname);
    virtual bool	 createSpareParametersFromChannels(
				UT_BitArray &selection,
				const CH_ChannelList &channels);

    CH_LocalVariable	*getVariable(int i) const;
    CH_LocalVariable	*getVariable(const char *name) const;

    //
    //
    OP_ERROR		 saveChannels(std::ostream &os, int binary, 
            		              bool compiled, int selected);
    OP_ERROR		 saveParmChannels(int parm_index, std::ostream &os,
				      int binary, bool compiled);
    OP_ERROR		 saveParameters(std::ostream &os, int binary, 
            		                bool compiled);
    OP_ERROR		 saveChannelAliases(std::ostream &os, int binary);

    bool		 loadChannels(UT_IStream &is, const char *path,
				      CH_Collection *obsolete_channels);
    bool		 loadParameters(UT_IStream &is, const char *path,
					PRM_ParmList *obsolete_parms);
    bool		 loadChannelAliases(UT_IStream &is, const char *path=0);
    bool		 loadOldSpareParms(UT_IStream &is);

    //  Allow our sub-class to be notified when something gets changed
    //	Since the parameter only holds the name & the parameters, there
    //	are only two types of events generated here...
    virtual void	 opChanged(OP_EventType type, void *dataptr = 0) = 0;

    /// This is a helper method to the setChannelScope method.  It traverses
    /// the node's parameters and sets the scope on them according to the
    /// given pattern.
    virtual void	 traverseScope(const char *pattern, OP_ScopeOp scope_op,
				       const OP_ScopeOptions &scope_options);

    // A virtual method to get access to the associated node flag.  This would
    // probably be unnecessary if myTakeReferences were used.
    virtual int		 getHasTakeData() const { return 0 ;}

    /// Ensure that the node errors are up-to-date.
    /// This method gives our subclass a chance to insert any errors on-demand.
    virtual void	 ensureErrorsAreUpdatedSubclass() {}

    /// Ensure that the spare parameter templates are up-to-date.
    /// This method gives our subclass a chance to update the spare parameters
    /// on-the-fly before returning them.
    virtual void	 ensureSpareParmsAreUpdatedSubclass() {}

    OP_Channels		*myChannels;
    OP_Stat		 myStats;

private:

    template<class T>
    void	 traverseScopeT(const char *pattern, OP_ScopeOp scope_op,
				       const OP_ScopeOptions &scope_options);

    // evaluationError() generates the appropriate call to opError() and then
    // clears the expression error state.
    void		 evaluationError(const PRM_Parm *parm, int err,
					 int thread) const;

    // these methods are the actual instantiation of the set... and
    // setSilent... methods.
    void		 setFullInt(int parmi, int vectori, fpreal t,
				exint value, bool p);
    void		 setFullInt(const char *parmname, int vectori, fpreal t,
				exint value, bool p);
    void		 setFullInt(const char *parmname, int &parmi,
				int vectori, fpreal t, exint value, bool p);


    void		 setFullFloat(int pi, int vi, fpreal t, fpreal val,
				      PRM_AddKeyType add_key, bool propagate);
    void		 setFullFloat(const char *pn, int vi,  fpreal t,
				      fpreal val, PRM_AddKeyType add_key,
				      bool propagate);
    void		 setFullFloat(const char *pn, int &pi, int vi,
				      fpreal t, fpreal val, 
				      PRM_AddKeyType add_key, bool propagate);

    void		 setFullString(const UT_StringRef &val,
				       CH_StringMeaning meaning, int pi, int vi,
				       fpreal t, bool ppropagate);
    void		 setFullString(const UT_StringRef &val,
				       CH_StringMeaning meaning,
				       const char *pn,
				       int vi, fpreal t, bool propagate);
    void		 setFullString(const UT_StringRef &val,
				       CH_StringMeaning meaning,
				       const char *pn,
				       int &pi, int vi, fpreal t, 
				       bool propagate);
    
    void		 setFullData(int pi, int vi, fpreal t,
				     const PRM_DataItemHandle &val, bool p);
    void		 setFullData(const char *pn, int vi, fpreal t,
				     const PRM_DataItemHandle &val, bool p);
    void		 setFullData(const char *pn, int &pi, int vi, fpreal t,
				    const PRM_DataItemHandle &val, bool p);

    void		 setFullIntInst(exint value, const char *parmname,
					const int *inst, int vectori, fpreal t,
					int nestlevel, bool propagate);
    void		 setFullFloatInst(fpreal val, const char *name,
					  const int *inst, int vi, fpreal t,
					  int nestlevel, bool propagate);
    void		 setFullStringInst(const UT_StringRef &val,
					   CH_StringMeaning meaning,
					   const char *name,
					   const int *inst, int vi, fpreal t,
					   int nestlevel, bool propagate);
    void		 setFullDataInst(const PRM_DataItemHandle &val,
					 const char *name, const int *inst,
					 int vi, fpreal t, int nestlevel,
					 bool propagate);

    // Set values given a parameter pointer (rather than a lookup index).
    // These all take add_key and propagate parameters.
    bool		 setFull(PRM_Parm &parm, int vectori, fpreal now,
				exint value, PRM_AddKeyType add_key,
				bool propgate);
    bool		 setFull(PRM_Parm &parm, int vectori, fpreal now,
				fpreal val, PRM_AddKeyType add_key,
				bool propgate);
    bool		 setFull(PRM_Parm &parm, int vectori, fpreal now,
				const char *value,
				CH_StringMeaning meaning,
				PRM_AddKeyType add_key, bool propgate);
    bool		 setFull(PRM_Parm &parm, int vectori, fpreal now,
				 const PRM_DataItemHandle &val,
				 PRM_AddKeyType add_key, bool propgate);

    bool		 determineParmVisibility(const char *parmname,
						 const PRM_Template *prm,
						 bool &visible);
    
    // Event handler
    static void		 changeEvent(void *, CH_CHANGE_TYPE, int);

    static void		 addLayerFromScopedChannels(
				const UT_StringRef &path,
				CH_ChannelRefList &chanrefs,
				UT_StringSet &scoped_layers);

    mutable OP_Lock	 myErrorManagerLock;
    UT_ErrorManager	 myErrorManager;

    UT_String		 myName;
    OP_Operator		*myTableEntry;
    uint		 myTakeReferences;

    unsigned		 myUndoAllParmSavedFlag:1,
			 myUndoScopedSavedFlag:1,
			 myAnyUndoFlagSet:1,
			 myIsGettingSaveError:1;

    OP_ParmCache	*myParmCache;

    OP_SpareParms	*mySpareParms;
    bool		 myChangingSpareParms;

    OP_MultiparmInfo	*myMultiparmInfo;

    friend class	 OP_TakeParm;
    friend class	 OP_UndoParm;
    friend class	 OP_UndoSpareParm;
};

// helper functions

OP_API PRM_Parm *OPgetParameterFromChannel( CH_Channel *chp, int *vecidx );
OP_API void OPsaveChannelForUndo( CH_Channel *chp );
OP_API void OPsaveChannelsForUndo( const CH_ChannelList &channels );
OP_API void OPtakeAutoActivateChannel( CH_Channel *chp );

#endif
