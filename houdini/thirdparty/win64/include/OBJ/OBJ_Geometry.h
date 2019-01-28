/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ library (C++)
 *
 * COMMENTS:	Definition for geometry object
 *
 */

#ifndef __OBJ_Geometry_h__
#define __OBJ_Geometry_h__

#include "OBJ_API.h"
#include "OBJ_Node.h"
#include "OBJ_Shared.h"

#include <DEP/DEP_MicroNode.h>

class CHOP_Node;
class OP_Bundle;

typedef enum {
    OBJ_PHOTON_OFF	= 0,
    OBJ_PHOTON_CAST	= 1,
    OBJ_PHOTON_RECEIVE	= 2,
    OBJ_PHOTON_BOTH	= 3
} OBJ_PHOTON_MODE;

enum OBJ_GeoIndex
{
    I_SHOP_MATERIAL = I_N_BASE_INDICES,
    I_SHOP_MATERIALOPT,
    // Misc
    I_VPORT_SHADEOPEN,
    I_VPORT_DISPLAYASSUBDIV,
    I_VPORT_ONIONSKIN,
    I_N_GEO_INDICES	// should always be last in the list
};

#define INT_OBJ_PARM(name, idx, vi, t)	\
		return evalInt(name, &getIndirect()[idx], vi, t);

class OBJ_API OBJ_Geometry : public OBJ_Node
{
public:
	     OBJ_Geometry(OP_Network *, const char *, OP_Operator *);
    virtual ~OBJ_Geometry();

    static PRM_Template		*getObsolete();
    static OP_Node		*myConstructor(OP_Network *net,
					const char *name, OP_Operator *entry);

    virtual void	 resolveObsoleteParms(PRM_ParmList *obsolete_parms);

    void		 getShaderSpace(OP_Context &context,
					UT_DMatrix4 &xform);
    void		 getDisplacementSpace(UT_DMatrix4 &xform, fpreal t);
    void		 getShaderSpace(OP_Context &context, UT_Matrix4 &xform);
    void		 getDisplacementSpace(UT_Matrix4 &xform, fpreal t);

    OP_Bundle		*getReflectBundle(fpreal t);

    // If you need to get the light mask bundle, check to make sure you don't
    // actually want to get the list of light mask objects.
    OP_Bundle		*getLightMaskBundle(fpreal t);

    // getLightMaskObjects differs from getLightMaskBundle method in that it
    // will expand light subnets and return the list of objects that will light
    // this geo.
    void		 getLightMaskObjects(OP_NodeList &lights, fpreal t);

    virtual int		 isObjectLitBy(OBJ_Node *light, fpreal t);
    virtual bool	 isLightInLightMask(const OBJ_Node *light, fpreal now);
    virtual int		 isObjectLitBySubnet(OBJ_Node *subnet, fpreal t);

    virtual void		 getActingLightSubset(
				    const UT_ValArray<OBJ_Ambient *> &lights,
				    UT_ValArray<OBJ_Ambient *> &active,
				    fpreal t);

    /// Interface to manipulate object level parameters
    virtual SOP_ObjectAppearancePtr	getObjectAppearance();
    
    virtual int			 doDrawLit() const { return 1; }

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    virtual CHOP_Node	*getSoundMaterial(fpreal t);
    static PRM_Template	*getTemplateList(OBJ_ParmsStyle style);

    virtual OBJ_Geometry*castToOBJGeometry()	{ return this; }

    virtual int		 collectPropertiesFromChannels(PI_EditScriptedParms &,
					UT_BitArray &selection,
					const CH_ChannelList &channels);
    virtual int		 collectPropertiesFromParms(PI_EditScriptedParms &,
					UT_StringArray &names,
					PRM_ParmNameMap &nmap);

    virtual obj_OnionSkin	 getOnionSkin();

protected:
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return geoIndirect; }

    GU_DetailHandle	getBoundingBoxGeometryHandle(OP_Context &context);
    GU_DetailHandle	getBoundingSphereGeometryHandle(OP_Context &context);

    virtual void	 buildParmDependency( int parm_index );

    // Flags for Use Parent Parameter
public:
    int		SMOOTH(fpreal now);
    int		NOBACK(fpreal now);
    void	DETAIL(UT_String &str, fpreal t);
    void	DETAIL_FILE(UT_String &str, fpreal t);
    void	DETAIL_AUTOARCHIVE(UT_String &str, fpreal t);
    void	DETAIL_RIBPROC(UT_String &str, fpreal t);
    void	DETAIL_MANTRAPROC(UT_String &str, fpreal t);
    int		DETAIL_BLUR(fpreal t);
    void	DETAIL_MIN(fpreal *v, fpreal t);
    void	DETAIL_MAX(fpreal *v, fpreal t);
    int		AUTOREFLECT(fpreal t);
    void	REFLMAP(UT_String &str, fpreal t);
    int		REFLRES(fpreal t);
    void	LIGHTMASK(UT_String &str,fpreal t);
    void	REFLMASK(UT_String &str, fpreal t);
    void	RISHADER(UT_String &str, fpreal t);
    void	RIDISPLACE(UT_String &str, fpreal t);
    void	RIINTERIOR(UT_String &str, fpreal t);
    bool	INSTANCEGROUP(UT_String &str, fpreal now);
    OBJ_PHOTON_MODE	CAUSTICSMODE(fpreal t);
    OBJ_PHOTON_MODE	GLOBILLUMMODE(fpreal t);
    int		RI_COLOR(fpreal now);
    void	RI_CS(fpreal v[3], fpreal t);
    void	RI_OS(fpreal v[3], fpreal t);
    void	RI_PHOTON(UT_String &str, fpreal t);
    bool	SHADESPACE(UT_String &str, fpreal t);
    bool	DISPSPACE(UT_String &str, fpreal t);

    // This parameter in the Misc tab only exists for geometry objects.
    virtual int VPORT_SHADEOPEN()
		{ INT_OBJ_PARM("vport_shadeopen",
			   I_VPORT_SHADEOPEN, 0, 0) }
    virtual int VPORT_DISPLAYASSUBDIV()
		{ INT_OBJ_PARM("vport_displayassubdiv",
			   I_VPORT_DISPLAYASSUBDIV, 0, 0) }

    // method for building a menu containing primitive groups
    virtual void	 buildPrimitiveGroupMenu(PRM_Name *menu,
						 int size, 
						 const PRM_SpareData *spare, 
						 const PRM_Parm *parm);

private:
    GU_DetailHandle	 myPopStandInGdpHandle;
    int			 myShaderHint;

    DEP_MicroNode	 myOnionSkinDepNode;
    int			 myOnionSkinState;

    static int		*geoIndirect;
};

#undef INT_OBJ_PARM

#endif
