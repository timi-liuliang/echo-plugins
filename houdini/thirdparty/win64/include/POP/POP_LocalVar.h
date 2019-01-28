/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_LocalVar_h__
#define __POP_LocalVar_h__

#include "POP_API.h"
#include "POP_Node.h"


class UT_String;


/// A subclass of POP_Node that handles many common local variables.  Many
/// modifier POPs will derive from this subclass instead of POP_Node.

class POP_API POP_LocalVar : public POP_Node
{
public:

    static CH_LocalVariable	myVariables[];
    static OP_VariablePair	myVariablePair;

    static OP_TemplatePair	myTemplatePair;

    GA_Offset curPtOff();
    GEO_PointP curPoint();
    GU_Detail		*curGdp() const;

protected:
	     POP_LocalVar (OP_Network* net, const char* name, 
		           OP_Operator* entry);
    virtual ~POP_LocalVar (void);

    virtual bool	evalVariableValue(fpreal &v, int index, int thread);
    virtual bool	evalVariableValue(UT_String &v, int i, int thr)
			{ return POP_Node::evalVariableValue(v, i, thr); }

    /// Call at the beginning of a cook (typically right after lockInputs())
    /// to initialize the use of user-defined local variables.  Make sure to
    /// call cleanupDynamicVars() at the end of the cook.
    void		setupDynamicVars (POP_ContextData* data);

    /// Call at the end of a cook to clean up state associated with the use
    /// of user-defined local variables.
    void		cleanupDynamicVars ();

    /// Call during cooking to initialize data necessary for the evaluation
    /// of our local variables.
    void		setupVars        (POP_ContextData* data, 
					  const GA_PointGroup* source = 0);

    virtual int		isVarDependent  (GU_Detail* gdp, POP_FParam param,
					 float t, int thread);
    virtual int		isVarDependent  (GU_Detail* gdp, POP_IParam param,
					 float t, int thread);
    virtual int		isVarDependent  (GU_Detail* gdp, POP_SParam param,
					 float t, int thread);
    virtual int		isVarDependent  (GU_Detail* gdp, EV_EXPRESSION* expr,
					 int thread);

    virtual void	allowVar (int val) { myAllowLocalVar = val; };

    virtual const CH_LocalVariable *resolveExtraVariableForSyntaxHighlight(
							    const char *name);

    /// Update a ramp from a specified multiparm.  A return value of true
    /// indicates that none of the parameters evaluated for this update is
    /// dependent on a varying (per particle) local variable.  A return 
    /// value of false indicates that the ramp will need to be updated for
    /// every particle using OP_Parameters::updateRampFromMultiParm().
    bool		updateVarIndependentRamp(GU_Detail *gdp,
						 const PRM_Parm &multi, float t,
						 UT_Ramp &ramp, int thread);

    /// The point currently being processed, used to retrieve attribute values
    /// when evaluating local variables.  The derived class is responsible for
    /// updating this member variable.
    GA_Offset		myCurrPtOff;

    /// An integer representing the current processing iteration, used to
    /// implement the $ITER local variable.  The derived class is responsible
    /// for updating this member variable.
    int 		myCurrIter;

    int			myVarTest;

    /// Retrieve a list of the local variables visible to this node.
    /// This list is unsorted and may contain duplicates.
    virtual void buildLocalVarNames(UT_StringArray &out_vars);
   
private:

    void		computeBBox     (void);

    int			myNumPoints;
    int			myNumGroupPoints;
    GU_Detail*		myBBGdp;
    UT_Vector3		myBBOrigin;
    UT_Vector3		myBBScale;
    float		myTimeInc;

    int			myAllowLocalVar;
    int			myVarDependent;

    POP_ContextData*	myContextData;

    POP_UserVarResolveInfo	 myUserVarResolveInfo;
};

#endif
