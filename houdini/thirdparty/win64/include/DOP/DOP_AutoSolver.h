/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_AutoSolver.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DOP_AutoSolver_h__
#define __DOP_AutoSolver_h__

#include "DOP_API.h"
#include "DOP_Auto.h"

class DOP_API DOP_AutoSolver : public DOP_Auto
{
public:
	     DOP_AutoSolver(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~DOP_AutoSolver();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 theCommonTemplates[];

protected:
    virtual bool	 updateParmsFlags();
    
    virtual void	 processObjectsSubclass(fpreal time,
					int foroutputidx,
					const SIM_ObjectArray &objects,
					DOP_Engine &engine);

    virtual void	 getInputInfoSubclass(int inputidx,
					      DOP_InOutInfo &info) const;
    virtual void	 getOutputInfoSubclass(int outputidx,
					       DOP_InOutInfo &info) const;

    virtual PRM_Template*getCommonTemplates() const;

    virtual bool	 ASSUMEDATACONST();

    virtual int		 SHAREDATA();
    virtual int		 UNIQUEDATANAME();
    virtual void	 DATANAME(UT_String &str, fpreal t);

    int			 MUTUALAFFECTOR();
};

#endif
