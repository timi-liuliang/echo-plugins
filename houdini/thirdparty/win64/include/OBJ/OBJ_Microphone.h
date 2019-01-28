/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Microphone.h (Custom Library, C++)
 *
 * COMMENTS:    An object to describe a motion capture handle
 *
 */

#ifndef __OBJ_Microphone__
#define __OBJ_Microphone__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"

enum OBJ_MicrophoneIndex
{
    I_MIC_ACTIVE = I_N_GEO_INDICES,
    I_MIC_SENSITIVITY,
    I_MIC_TYPE,
    I_MIC_CONE,
    I_MIC_OUTER_CONE,
    I_MIC_DROPOFF,
    I_MIC_DROPOFF_RATE,
    I_MIC_OUTER_SENSITIVITY,
    I_MIC_CHOP_FILTERPATH,
    I_N_MIC_INDICES     // should always be last in the list
};


#define FLOAT_OBJ_PARM(name, idx, vi, t)    \
	return evalFloat(name, &handleIndirect[idx], vi, t);
#define INT_OBJ_PARM(name, idx, vi, t)      \
	return evalInt(name, &handleIndirect[idx], vi, t);
#define STR_OBJ_PARM(name, idx, vi, t)      \
	evalString(str, name, &handleIndirect[idx], vi, t);

class OBJ_API OBJ_Microphone : public OBJ_Geometry
{
public:
				 OBJ_Microphone(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_Microphone();

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    void			 getMicFilter(UT_String &choppath, fpreal now);
    
    int				 isDirectional();
    int				 getRolloffType();
    void			 getMicParms(int &active,
					     fpreal &volume, fpreal &volume2,
					     fpreal &angle1, fpreal &angle2,
					     fpreal &rolloff, fpreal t);

    virtual void		 resolveObsoleteParms(PRM_ParmList *obsolete);
    static PRM_Template		*getObsolete();
    static PRM_Template		*getTemplateList();
protected:
    virtual bool	 	 updateParmsFlags();

    virtual OP_ERROR		 cookMyObj(OP_Context &context);
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return handleIndirect; }

private:
    int		ACTIVE(fpreal t)
		{ INT_OBJ_PARM("micactive", I_MIC_ACTIVE, 0, t); }

    fpreal	SENSITIVITY(fpreal t)
		{ FLOAT_OBJ_PARM("sensitive", I_MIC_SENSITIVITY, 0, t); }
    
    int		MIC_TYPE()
		{ INT_OBJ_PARM("direct", I_MIC_TYPE, 0, 0); }
    
    fpreal	RECORD_CONE(fpreal t)
		{ FLOAT_OBJ_PARM("reccone", I_MIC_CONE, 0, t); }

    fpreal	OUTER_CONE(fpreal t)
		{ FLOAT_OBJ_PARM("outcone", I_MIC_OUTER_CONE, 0, t); }
	
    int		DROPOFF()
		{ INT_OBJ_PARM("dropoff", I_MIC_DROPOFF, 0, 0); }

    fpreal	DROPOFF_RATE(fpreal t)
		{ FLOAT_OBJ_PARM("droprate", I_MIC_DROPOFF_RATE, 0, t); }
    
    fpreal	OUTER_SENSITIVITY(fpreal t)
		{ FLOAT_OBJ_PARM("outsensitive", I_MIC_OUTER_SENSITIVITY, 0, t); }
private:

   virtual int	 isObjectRenderable(fpreal t) const;

   static int	*handleIndirect;
};

#undef FLOAT_OBJ_PARM
#undef STR_OBJ_PARM
#undef INT_OBJ_PARM

#endif
