/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ library (C++)
 *
 * COMMENTS:	Light object definition (base class for lights too)
 *
 */

#ifndef __OBJ_Light_h__
#define __OBJ_Light_h__

#include "OBJ_API.h"
#include "OBJ_Ambient.h"

class obj_Scene;
class RE_Light;
class OP_Bundle;

enum OBJ_LightIndex
{
    // Light
    I_N_LIGHT_INDICES = I_N_AMB_INDICES
};

// THIS ENUM MUST BE IN-SYNC WITH RE_LightAreaShape IN RE_Light.h.
enum OBJ_LightAreaShape
{
    OBJ_LIGHT_AREA_SHAPE_LINE,
    OBJ_LIGHT_AREA_SHAPE_GRID,
    OBJ_LIGHT_AREA_SHAPE_CUBE,
    OBJ_LIGHT_AREA_SHAPE_DISK,
    OBJ_LIGHT_AREA_SHAPE_SPHERE
};

// Autogenerate depth map return types
enum OBJ_LightAutogen {
    OBJ_LIGHT_AUTO_NONE = 0,
    OBJ_LIGHT_AUTO_MID,
    OBJ_LIGHT_AUTO_NEAR,
    OBJ_LIGHT_AUTO_FAR,
    OBJ_LIGHT_AUTO_DEEP,
    OBJ_LIGHT_AUTO_DEEP_INTERP,
    OBJ_LIGHT_AUTO_MINMAX
};

class OBJ_API OBJ_Light : public OBJ_Ambient
{
public:
	     OBJ_Light(OP_Network *, const char *, OP_Operator *);
    virtual ~OBJ_Light();

    static PRM_Template		*getObsolete();
    static OP_Node		*myConstructor(OP_Network *net,
					const char *name, OP_Operator *entry);

    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    virtual int			  isObjectShadower(OBJ_Node *obj, fpreal t);
    virtual int			  isSubnetShadower(OBJ_Node *subnet, fpreal t);
    virtual void		  updateLightValue(fpreal t,
						   bool for_viewport = true);

    // The following method will create the "best" renderman shader possible
    //	for a light object.
    virtual void		  rmanBestShader(UT_String &str, fpreal t);
    virtual void		  vexBestShader(UT_String &str, fpreal t);
    virtual void		  vexBestShadow(UT_String &str, fpreal t);

    virtual void		  resolveObsoleteParms(
					    PRM_ParmList *obsolete_parms);

    OP_Bundle			 *getShadowMaskBundle(fpreal t);

    void	SHADOWTYPE(UT_String &str, fpreal t);
    void	SHADOWMASK(UT_String &str, fpreal t);
    fpreal	SHADOWINTENSITY(fpreal t);
    fpreal	SHADOWBIAS(fpreal t);
    fpreal	SHADOWBLUR(fpreal t);
    fpreal	SHADOWQUALITY(fpreal t);
    void	AUTOGEN(UT_String &str, fpreal t);
    void	DEPTHMAP(UT_String &str, fpreal t);
    int		DEPTHSAMPLEX(fpreal t);
    int		DEPTHSAMPLEY(fpreal t);
    int		DEPTHBLUR(fpreal t);
    int		DEEP(fpreal t);
    void	DEPTHQUANT(UT_String &str, fpreal t);
    int		MINMAX(fpreal t);
    void	DEEPSTYLE(UT_String &str, fpreal t);
    void	OFSTORAGE(UT_String &str, fpreal t);
    void	PZSTORAGE(UT_String &str, fpreal t);
    int		DEPTHCOMPRESSION(fpreal t);
    fpreal	DEPTHZBIAS(fpreal t);
    int		DEPTHOFSIZE(fpreal t);
    void	SPECMAP(UT_String &str, fpreal t);
    void	DIFFMAP(UT_String &str, fpreal t);
    void	PROJMAP(UT_String &str, fpreal t);
    int		CLAMP_PROJMAP(fpreal t);
    int		USEACTIVERADIUS(fpreal t);
    int		CONELIGHT(fpreal now);
    fpreal	CONEANGLE(fpreal t);
    fpreal	CONEDELTA(fpreal t);
    fpreal	CONEROLL(fpreal t);
    void	TATTEN(UT_String &str, fpreal t);
    fpreal	ATTEN(fpreal t);
    fpreal	SPREAD(fpreal t);
    fpreal	REFLECTX(fpreal t);
    fpreal	REFLECTY(fpreal t);
    int		DIFFUSE(fpreal t);
    int		SPECULAR(fpreal t);
    int		PHOTONDIRECT(fpreal t);
    fpreal	ENERGYR(fpreal t);
    fpreal	ENERGYG(fpreal t);
    fpreal	ENERGYB(fpreal t);
    fpreal	EXPONENT(fpreal t);
    int		CPHOTONSSTORE(fpreal t);
    int		CPHOTONSEMIT(fpreal t);
    int		GIPHOTONSSTORE(fpreal t);
    int		GIPHOTONSEMIT(fpreal t);
    void	AREASHAPE(UT_String &str, fpreal t);
    fpreal	AREASAMPLES(fpreal t);
    fpreal	AREASIZE(fpreal t);

    static PRM_Template		*getTemplateList(OBJ_ParmsStyle style);

    virtual void		saveDialogScriptExtraInfo(std::ostream &os);

    virtual OBJ_Light		*castToOBJLight()    { return this; }
    virtual int		 collectPropertiesFromChannels(PI_EditScriptedParms &,
					UT_BitArray &selection,
					const CH_ChannelList &channels);
    virtual int		 collectPropertiesFromParms(PI_EditScriptedParms &,
					UT_StringArray &names,
					PRM_ParmNameMap &nmap);

protected:
    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return lightIndirect; }
    const GU_Detail		*getPortalGeometry(OP_Context &context,
						   const char *path,
						   UT_DMatrix4 &xform);

private:
    static int		*lightIndirect;
    char 		*mySavedProjectMap;
};

#endif
