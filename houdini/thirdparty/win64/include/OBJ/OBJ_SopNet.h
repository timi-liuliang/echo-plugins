/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_SopNet.h (Custom Library, C++)
 *
 * COMMENTS:    An object to describe a generic SOP Network.  It derives 
 *		from OBJ_Geometry as it contains geometry.
 *
 */

#ifndef __OBJ_SopNet__
#define __OBJ_SopNet__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"

class OBJ_API OBJ_SopNet : public OBJ_Geometry
{
public:
				 OBJ_SopNet(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_SopNet();

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    static PRM_Template		*getTemplateList();

    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				 { return sopnetIndirect; }

    ///Absolute width
    virtual fpreal	 	 getW() const;
    ///Absolute height
    virtual fpreal		 getH() const;

protected:
    virtual int			 isObjectRenderable(fpreal t) const;

private:
    static int			*sopnetIndirect;
};

#endif
