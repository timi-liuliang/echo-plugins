/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Handle.h (Custom Library, C++)
 *
 * COMMENTS:    An object to describe a motion capture handle
 *
 */

#ifndef __OBJ_Handle__
#define __OBJ_Handle__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"

class OBJ_Bone;

enum OBJ_HandleIndex
{
    I_TARGET = I_N_GEO_INDICES,
    I_WEIGHT,
    I_TWISTONLY,
    I_FALLOFF,

    I_DORXLIMIT,
    I_LIMRX,
    I_DORYLIMIT,
    I_LIMRY,
    I_DORZLIMIT,
    I_LIMRZ,
    I_N_HANDLE_INDICES     // should always be last in the list
};


#define FLOAT_OBJ_PARM(name, idx, vi, t)    \
	return evalFloat(name, &getIndirect()[idx], vi, t);
#define INT_OBJ_PARM(name, idx, vi, t)      \
	return evalInt(name, &getIndirect()[idx], vi, t);
#define STR_OBJ_PARM(name, idx, vi, t)      \
	evalString(str, name, &getIndirect()[idx], vi, t);

class OBJ_API OBJ_Handle : public OBJ_Geometry
{
public:
				 OBJ_Handle(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_Handle();

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		*getObsolete();
    void			 resolveObsoleteParms(PRM_ParmList *obsolete);

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;


    OBJ_Node		*getTarget();

    fpreal		 WEIGHT(fpreal t)
			 { FLOAT_OBJ_PARM("weight", I_WEIGHT, 0, t); }
    fpreal		 FALLOFF(fpreal t)
			 { FLOAT_OBJ_PARM("falloff", I_FALLOFF, 0, t); }

    int			 TWIST_ONLY()
			 { INT_OBJ_PARM("twistonly", I_TWISTONLY, 0, 0); }

    int			 DORXLIMIT()
			 { INT_OBJ_PARM("dorxlimit", I_DORXLIMIT, 0, 0); }

    int			 DORYLIMIT()
			 { INT_OBJ_PARM("dorylimit", I_DORYLIMIT, 0, 0); }

    int			 DORZLIMIT()
			 { INT_OBJ_PARM("dorzlimit", I_DORZLIMIT, 0, 0); }

    fpreal		 MINRX(fpreal t)
			 { FLOAT_OBJ_PARM("lrx", I_LIMRX, 0, t); }
    fpreal		 MAXRX(fpreal t)
			 { FLOAT_OBJ_PARM("lrx", I_LIMRX, 1, t); }

    fpreal		 MINRY(fpreal t)
			 { FLOAT_OBJ_PARM("lry", I_LIMRY, 0, t); }
    fpreal		 MAXRY(fpreal t)
			 { FLOAT_OBJ_PARM("lry", I_LIMRY, 1, t); }

    fpreal		 MINRZ(fpreal t)
			 { FLOAT_OBJ_PARM("lrz", I_LIMRZ, 0, t); }
    fpreal		 MAXRZ(fpreal t)
			 { FLOAT_OBJ_PARM("lrz", I_LIMRZ, 1, t); }

    void		 TARGET(UT_String &str)
			 { STR_OBJ_PARM  ("obj_targetpath", I_TARGET, 0, 0); }

    static PRM_Template		*getTemplateList();

    virtual OBJ_Handle		*castToOBJHandle()	{ return this; }

protected:
    virtual bool	 	 updateParmsFlags();
    
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return handleIndirect; }

    virtual int			 isObjectRenderable(fpreal t) const;

private:
    static int			*handleIndirect;
};

#undef FLOAT_OBJ_PARM
#undef STR_OBJ_PARM
#undef INT_OBJ_PARM

#endif
