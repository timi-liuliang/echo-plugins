/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Each.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_Each__
#define __SOP_Each__

#include "SOP_API.h"
#include "SOP_Node.h"

class SOP_API SOP_Each : public SOP_Node
{
public:
	     SOP_Each(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Each();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);

    virtual const char	*inputLabel(unsigned idx) const;

    bool		 FEEDBACK()
			 { return evalInt("feedback", 0, 0); }
    void		 FORPATH(UT_String &str, fpreal t)
			 { evalString(str, "forpath", 0, t); }
    void		 GROUP(UT_String &str, fpreal t)
			 { evalString(str, "group", 0, t); }
    void		 ATTRIB(UT_String &str, fpreal t)
			 { evalString(str, "attrib", 0, t); }
    void		 ATTRIBVAL(UT_Vector3D &v, fpreal t)
			 {
			     v.x() = evalFloat("attribval", 0, t);
			     v.y() = evalFloat("attribval", 1, t);
			     v.z() = evalFloat("attribval", 2, t);
			 }
    fpreal		 ATTRIBTOL(fpreal t)
			 { return evalFloat("attribtol", 0, t); }
    bool		 EACHPOINT(fpreal t)
			 { return evalInt("eachpoint", 0, t); }

    // Id of the forsop we were last registered to so we can
    // unregister.
    int			 myLastForSOP;

private:
};


#endif
