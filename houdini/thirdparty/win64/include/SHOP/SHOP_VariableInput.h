/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_VariableInput.h ( SHOP Library, C++)
 *
 * COMMENTS:	A node which takes variable numbers of inputs
 */

#ifndef __SHOP_VariableInput__
#define __SHOP_VariableInput__

#include "SHOP_API.h"
#include "SHOP_Node.h"

class SHOP_API SHOP_VariableInput : public SHOP_Node {
public:
    SHOP_VariableInput(OP_Network *dad, const char *name,
		       OP_Operator *entry, SHOP_TYPE shader_type=SHOP_SURFACE)
	: SHOP_Node(dad, name, entry, shader_type)
    {
    }
    virtual ~SHOP_VariableInput();

    virtual unsigned	 getNumVisibleInputs() const;
    virtual void	 opChanged(OP_EventType reason, void *data=0);
    virtual void	 ensureParametersAreValid();

protected:
    virtual void	 preOpChanged(OP_EventType, void *)
			 { }

    int			 myLatestOpChangedId;
    static SHOP_Node	*theOriginalOpChanged;
    static int		 theLatestOpChangedId;
};

#endif

