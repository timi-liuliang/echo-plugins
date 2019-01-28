/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_Sound.h (Custom Library, C++)
 *
 * COMMENTS:    An object to describe a motion capture handle
 *
 */

#ifndef __OBJ_Sound__
#define __OBJ_Sound__

#include "OBJ_API.h"
#include "OBJ_Geometry.h"

enum OBJ_SoundIndex
{
    I_SOUND_ACTIVE = I_N_GEO_INDICES,
    I_SOUND_VOLUME,
    I_SOUND_TYPE,
    I_SOUND_CONE,
    I_SOUND_OUTER_CONE,
    I_SOUND_DROPOFF,
    I_SOUND_DROPOFF_RATE,
    I_SOUND_OUTER_VOLUME,
    I_CHOP_SOURCEPATH,
    I_N_SOUND_INDICES     // should always be last in the list
};


#define FLOAT_OBJ_PARM(name, idx, vi, t)    \
	return evalFloat(name, &handleIndirect[idx], vi, t);
#define INT_OBJ_PARM(name, idx, vi, t)      \
	return evalInt(name, &handleIndirect[idx], vi, t);
#define STR_OBJ_PARM(name, idx, vi, t)      \
	evalString(str, name, &handleIndirect[idx], vi, t);

class OBJ_API OBJ_Sound : public OBJ_Geometry
{
public:
				 OBJ_Sound(OP_Network *net,
					   const char *name,
					   OP_Operator *op);
    virtual			~OBJ_Sound();

    static OP_Node              *myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    void			 getSoundSource(UT_String &choppath, fpreal t);
    int				 isDirectional() { return EMISSION_TYPE(); }
    int				 getRolloffType() { return DROPOFF(); }
    void			 getSoundParms(int &active,
					       fpreal &volume,  fpreal &volume2,
					       fpreal &angle1,  fpreal &angle2,
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
		{ INT_OBJ_PARM("soundactive", I_SOUND_ACTIVE, 0, t); }
    
    fpreal	VOLUME(fpreal t)
		{ FLOAT_OBJ_PARM("volume", I_SOUND_VOLUME, 0, t); }
    
    int		EMISSION_TYPE()
		{ INT_OBJ_PARM("direct", I_SOUND_TYPE, 0, 0); }
    
    fpreal	EMISSION_CONE(fpreal t)
		{ FLOAT_OBJ_PARM("emitcone", I_SOUND_CONE, 0, t); }

    fpreal	OUTER_CONE(fpreal t)
		{ FLOAT_OBJ_PARM("outcone", I_SOUND_OUTER_CONE, 0, t); }
	
    int		DROPOFF()
		{ INT_OBJ_PARM("dropoff", I_SOUND_DROPOFF, 0, 0); }

    fpreal	DROPOFF_RATE(fpreal t)
		{ FLOAT_OBJ_PARM("droprate", I_SOUND_DROPOFF_RATE, 0, t); }
    
    fpreal	OUTER_VOLUME(fpreal t)
		{ FLOAT_OBJ_PARM("outvolume", I_SOUND_OUTER_VOLUME, 0, t); }

public:
    virtual int	isObjectRenderable(fpreal t) const;
     
private:     
   static int	*handleIndirect;
};

#undef FLOAT_OBJ_PARM
#undef STR_OBJ_PARM
#undef INT_OBJ_PARM

#endif
