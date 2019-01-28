/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_PythonScript.h (Custom Library, C++)
 *
 * COMMENTS:    An object to allow contained geometries to be altered via python script
 *
 */

#ifndef __OBJ_PythonScript__
#define __OBJ_PythonScript__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"


class OBJ_API OBJ_PythonScript : public OBJ_Geometry
{
public:

    virtual OBJ_OBJECT_TYPE	 getObjectType() const override;


				 OBJ_PythonScript(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    
    virtual			~OBJ_PythonScript();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		*getTemplateList();

    // These methods are used by HOMF_ObjNode::setCookTransform()
    // to set the parm transform from within the Python cook code.
    bool 			 isCooking() const { return myIsCooking; }
    
    void 			 setCookParmTransform(const UT_DMatrix4 &xform)
				{ myCookParmTransform = xform; }

    virtual bool 		 getParmTransform(OP_Context &context, UT_DMatrix4 &xform) override;
    virtual bool		 canCreateNewOpType() const override 
				{ return true; }

protected:
    
    virtual int			*getIndirect() const override
				{ return pythonIndirect; }

private:

    UT_DMatrix4 		 myCookParmTransform;
    bool 			 myIsCooking;
    static int   		*pythonIndirect;
  
};

#endif
