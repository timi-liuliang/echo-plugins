/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ParmOwner.h (Parameter Library)
 *
 * COMMENTS:
 *		This class is the base class of OP_Parameters. It provides
 *		a method for a PRM_Instance to communicate with its owner
 *		OP_Node through virtual functions.
 */

#ifndef __PRM_ParmOwner__
#define __PRM_ParmOwner__

#include "PRM_API.h"
#include <CH/CH_ExprLanguage.h>
#include <PY/PY_Result.h>
#include <UT/UT_Error.h>
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_WorkBuffer.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class UT_StringArray;
class PY_CompiledCode;
class PY_EvaluationContext;
class PRM_Conditional;
class PRM_Instance;
class PRM_Parm;
class PRM_ParmList;
class PRM_ParmMicroNode;
class PRM_ParmNameMap;

// Forward declare of OP_Node so that we can provide a mechanism to cast to it
// safely.
class OP_Node;

class PRM_API PRM_ParmOwner
{
public:
    virtual OP_Node *		 castToOPNode()		{ return 0; }
    virtual const OP_Node *	 castToOPNode() const	{ return 0; }

    const char		*getFullPath(UT_String &str) const
			{ 
			    UT_WorkBuffer	buf;
			    doGetFullPath(buf);
			    buf.copyIntoString(str);
			    return str.buffer();
			}
    const char		*getFullPath(UT_WorkBuffer &str) const
			{
			    doGetFullPath(str);
			    return str.buffer();
			}
    UT_StringHolder 	getFullPath() const
			{
			    UT_WorkBuffer buf;
			    doGetFullPath(buf);
			    return UT_StringHolder(buf);
			}
    SYS_FORCE_INLINE
    PRM_ParmList	*getParmList()
				 { return myParmList; }
    SYS_FORCE_INLINE
    const PRM_ParmList	*getParmList() const
				 { return myParmList; }

    virtual bool		 createSpareParametersForLoad(
					    UT_StringArray &,
					    PRM_ParmNameMap &,
					    UT_String &);

    virtual PRM_ParmList	*createObsoleteParmList() = 0;
    virtual void		 resolveAndDeleteObsoleteParmList(
					    PRM_ParmList *&obsolete_parms) = 0;

    // Runs a command using the OPgetDirector()->getCommandManager()
    // at the OP level. The cwd is set to the current node before
    // executing the command.
    virtual void		 executeCommand(const char *command,
                		                std::ostream *out,
                		                std::ostream *err) const = 0;

    // Run some Python code, but set the cwd before running it and restore it
    // after running it.
    virtual void	 pushAsPwdAndRunPython(
			    PY_CompiledCode &compiled_code,
			    PY_Result::Type desired_result_type,
			    PY_Result &result,
                            PY_EvaluationContext *context=NULL) const = 0;

    // Pending overrides are strings giving a list of op nodes which
    // will be overridden.  These are used higher up at the OP_Node level.
    bool		 isPendingOverride() const
			 {
			    return myIsPendingOverride;
			 }
    virtual bool	 isParmPendingOverride(const char * /*parm_name*/,
					   int /*vec_idx*/) const
			 {
			     // default implementation that is too coarse
			     return isPendingOverride();
			 }
    const char 		*getPendingOverrideString() const
			 {
			    return (const char *) myOverridePending;
			 }
    void		 setPendingOverrideString(const char *s);
    virtual UT_ErrorSeverity
			 prmCookOverrides(fpreal /*t*/, int /*thread*/,
					  const char * /*parm_name*/,
					  int /*vec_idx*/)
			    { return UT_ERROR_NONE; }

    // Activates the parm for the take if it should be automatically
    // activated based on its current state and the current take state.
    virtual void	 takeAutoActivateParm(PRM_Parm *) = 0;

    // Rebuild the node dependency information for a particular parm on
    // a node. This allows the PRM library to force nodes to update their
    // dependency information.
    virtual void	 rebuildParmDependency(int parmidx) = 0;

    // Clear all parm dependency information
    virtual void	 clearParmDependency(int parmidx) = 0;

    // Converts opdef and oplib format paths from opdef:nodepath to
    // opdef:tablename/optype.
    virtual void	 convertOpdefToAbsolutePath(UT_String &str) const = 0;

    // This function is called when a spare parameter is being removed
    // from our parm list. This is called from PRM_ParmList::updateSpareParms,
    // and provides a way for the OP library to respond to individual parm
    // removals without understanding the internals of the whole operation.
    virtual void	 spareParmRemoved(const char *parmname) = 0;

    // This function is caled when any multiparm has a new instance added.
    // It gives the parm owner a chance to initialize the multiparm values.
    virtual void	 initMultiparmInstance(UT_ValArray<PRM_Parm *> &p) = 0;

    // This function is called when a multiparm instance is added or removed.
    virtual void	 addOrRemoveMultiparmInstance() = 0;

    virtual bool	 savePresetContents(std::ostream &os) = 0;
    virtual bool	 loadPresetContents(const char *token,
					    UT_IStream &is) = 0;

    virtual CH_ExprLanguage	 getExprLanguage() const = 0;

    // Creates an array of parm micronodes, one per component for the given
    // parm.
    virtual PRM_ParmMicroNode	*createParmMicroNodes(PRM_Parm &parm) const;


    // There are special bespoke functions for the disablewhen and hidewhen
    // conditionals that are parsed through this callback.  They have to
    // move upstream to OP to get access to nInputs(), etc.
    virtual UT_StringHolder	evaluateDisableExpression(const PRM_Parm &prm, const UT_StringRef &function) const;

    // Traverse channel references building dependencies.  Vector index of
    // final parameter is returned in 'sub_idx'.
    virtual const PRM_Parm *traverseRef(int *sub_idx, fpreal time,
					int parm_idx, int vec_idx) const = 0;

protected:
	     PRM_ParmOwner()
		     : myIsPendingOverride(false)
		     , myParmList(0)
		 { }
    virtual	~PRM_ParmOwner()
		 { }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myOverridePending.getMemoryUsage(false);
        return mem;
    }

    /// Compute the full path of the node
    virtual void	doGetFullPath(UT_WorkBuffer &buf) const = 0;

    PRM_ParmList			*myParmList;

private:
    UT_String		 myOverridePending;
    bool		 myIsPendingOverride;
};

#endif

