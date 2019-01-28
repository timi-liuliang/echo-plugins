/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_Multi.h
 *
 * COMMENTS:
 *	Base class for multi-input COPs. Handles depth/rate/frame range
 *	issues.
 */
#ifndef _COP2_MULTI_BASE_H_
#define _COP2_MULTI_BASE_H_

#include "COP2_API.h"
#include "COP2_Node.h"


#define ARG_MULTIBASE_PLANE_MERGE	(myMultiParmBase +0)
#define ARG_MULTIBASE_DEPTH_MATCH	(myMultiParmBase +1)
#define ARG_MULTIBASE_RANGE_MATCH	(myMultiParmBase +2)
#define ARG_MULTIBASE_RATE_MATCH	(myMultiParmBase +3)
#define ARG_MULTIBASE_FRAME_MATCH	(myMultiParmBase +4)

class COP2_API COP2_MultiBase : public COP2_Node
{
public:
    virtual TIL_Sequence *cookSequenceInfo(OP_ERROR &err);
 
    static OP_TemplatePair	 myTemplatePair;
    static OP_VariablePair	 myVariablePair;
    static PRM_Template		 myTemplateList[];
    static CH_LocalVariable	 myVariableList[];

protected:
		COP2_MultiBase(OP_Network *parent, const char *name,
			       OP_Operator *entry);
    virtual	~COP2_MultiBase();

    virtual COP2_ThreadPref getThreadPreference() const
			    { return COP2_THREAD_MULTI; }
    
    virtual int	getPrimaryInput() const { return 0; }

    // We override this method because it is possible that we need
    // to be considered time dependent if our inputs are static
    // sequences and we don't match their range anymore.
    virtual void checkTimeDependencies( int do_parms=1,
					int do_inputs=1,
					int do_extras=1 );

private:

    int		PLANE_MERGE()
		{ return evalInt(ARG_MULTIBASE_PLANE_MERGE,0,0); }
    
    int		DEPTH_MATCH()
		{ return evalInt(ARG_MULTIBASE_DEPTH_MATCH,0,0); }
    
    int		RANGE_MATCH()
		{ return evalInt(ARG_MULTIBASE_RANGE_MATCH,0,0); }
    
    int		RATE_MATCH()
		{ return evalInt(ARG_MULTIBASE_RATE_MATCH,0,0); }

    int		FRAME_MATCH()
		{ return evalInt(ARG_MULTIBASE_FRAME_MATCH,0,0); }


    int		myMultiParmBase;
};

#endif
