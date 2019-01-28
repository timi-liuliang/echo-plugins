/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_ExportedParmLayoutMicroNode.h (VOP Library, C++)
 *
 * COMMENTS:	DEP_MicroNode subclass representing the parameter manager's
 *		owner's parameter dialog script.
 */

#ifndef __VOP_EXPORTEDPARMLAYOUTMICRONODE_H_INCLUDED__
#define __VOP_EXPORTEDPARMLAYOUTMICRONODE_H_INCLUDED__

#include "VOP_API.h"
#include <DEP/DEP_MicroNode.h>

class VOP_ExportedParmsManager;

class VOP_API VOP_ExportedParmLayoutMicroNode : public DEP_MicroNode
{
public:
    explicit		    VOP_ExportedParmLayoutMicroNode(
				VOP_ExportedParmsManager &parms_mgr);
    virtual		    ~VOP_ExportedParmLayoutMicroNode();

    virtual const char *    className() const
				{ return "VOP_ExportedParmLayoutMicroNode"; }

    virtual void	    getInputs(DEP_MicroNodeList &inputs) const;

private:

    VOP_ExportedParmsManager &myParmsManager;
};

#endif // __VOP_EXPORTEDPARMLAYOUTMICRONODE_H_INCLUDED__
