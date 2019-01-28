/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DOP_AutoSolver.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DOP_AutoRelType_h__
#define __DOP_AutoRelType_h__

#include "DOP_API.h"
#include "DOP_Auto.h"

class DOP_API DOP_AutoRelType : public DOP_Auto
{
public:
	     DOP_AutoRelType(OP_Network *net, const char *name,
			     OP_Operator *op);
    virtual ~DOP_AutoRelType();

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
    virtual void	 getOutputInfoSubclass(int outputidx,
					       DOP_InOutInfo &info) const;

    virtual PRM_Template*getCommonTemplates() const;

    virtual int		 UNIQUEDATANAME();

private:
    /// Display a warning if the user is creating a mutual affector
    /// relationship between objects from different solvers, since that can
    /// often be done unintentionally and cause feedback iterations.
    void                 checkMutualAffectorSolvers(
                            const SIM_ObjectArray &affected_list,
                            const SIM_ObjectArray &affectors_list);

    void		 AFFECTED(UT_String &str, fpreal t);
    void		 AFFECTORS(UT_String &str, fpreal t);
    void		 RELNAME(UT_String &str, fpreal t);
    int			 UNIQUERELNAME();
    bool		 MAKEMUTUAL();
};

#endif
