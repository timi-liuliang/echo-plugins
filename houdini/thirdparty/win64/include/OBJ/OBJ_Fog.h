/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ library (C++)
 *
 * COMMENTS:	Atmosphere object definition
 */

#ifndef __OBJ_Fog_h__
#define __OBJ_Fog_h__

#include "OBJ_API.h"
#include "OBJ_Node.h"

class obj_Scene;

enum OBJ_FogIndex
{
    I_FOG_MATERIAL = I_N_BASE_INDICES,
    I_FOG_MATERIAL_OPT,
    I_N_FOG_INDICES		// should always be last in the list
};

class OBJ_API OBJ_Fog : public OBJ_Node
{
public:
	     OBJ_Fog(OP_Network *, const char *, OP_Operator *);
    virtual ~OBJ_Fog();

    static PRM_Template		*getObsolete();
    static PRM_Template		*getTemplateList();
    static OP_Node		*myConstructor(OP_Network *net,
					const char *name, OP_Operator *entry);

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;
    virtual OBJ_Fog		*castToOBJFog()		{ return this; }

    virtual int		 collectPropertiesFromChannels(PI_EditScriptedParms &,
					UT_BitArray &selection,
					const CH_ChannelList &channels);
    virtual int		 collectPropertiesFromParms(PI_EditScriptedParms &,
					UT_StringArray &names,
					PRM_ParmNameMap &nmap);

    void			 RISHADER(UT_String &str, fpreal t);

protected:

    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return fogIndirect; }

private:

    static int		*fogIndirect;
};

#endif
