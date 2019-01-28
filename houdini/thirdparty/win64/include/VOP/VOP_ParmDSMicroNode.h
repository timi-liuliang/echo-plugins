/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_ParmDSMicroNode.h (VOP Library, C++)
 *
 * COMMENTS:	DEP_MicroNode subclass representing the parameter manager's
 *		owner's parameter dialog script.
 */

#ifndef __VOP_PARMDSMICRONODE_H_INCLUDED__
#define __VOP_PARMDSMICRONODE_H_INCLUDED__

#include "VOP_API.h"
#include <DEP/DEP_MicroNode.h>

class VOP_ExportedParmsManager;

class VOP_API VOP_ParmDSMicroNode : public DEP_MicroNode
{
public:
    explicit		    VOP_ParmDSMicroNode(
				VOP_ExportedParmsManager &parms_mgr);
    virtual		    ~VOP_ParmDSMicroNode();

    virtual const char *    className() const
				{ return "VOP_ParmDSMicroNode"; }

    virtual void	    getInputs(DEP_MicroNodeList &inputs) const;
    virtual void	    getOutputs(DEP_MicroNodeList &outputs) const;

private:
    void		    getCodeParmInputs_(
				DEP_MicroNodeList &inputs) const;
    void		    getSubnetParmInputs_(
				DEP_MicroNodeList &inputs) const;

    VOP_ExportedParmsManager &myParmsManager;
};

#endif // __VOP_PARMDSMICRONODE_H_INCLUDED__
