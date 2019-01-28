/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ library (C++)
 *
 * COMMENTS:	Ambient Light object definition (base class for lights too)
 *
 */

#ifndef __OBJ_Ambient_h__
#define __OBJ_Ambient_h__

#include "OBJ_API.h"
#include "OBJ_Camera.h"

class obj_Scene;
class IFD_LightParms;
class OBJ_ReLightProxy;
class RE_Light;

#define LIGHTBANK_PREFIX	"lb:"

enum OBJ_AmbIndex
{
    I_DIMMER = I_N_CAM_INDICES,

    I_N_AMB_INDICES		// should always be last in the list
};

// Yes, this is gross, however, because we assume that you can render
//	from any light source, we have to sub-class from OBJ_Camera.
class OBJ_API OBJ_Ambient : public OBJ_Camera
{
public:
	     OBJ_Ambient(OP_Network *, const char *, OP_Operator *);
    virtual ~OBJ_Ambient();

    static PRM_Template		*getObsolete();

    virtual RE_Light		*getLightValue();
    virtual void		 updateLightValue(fpreal t,
						  bool for_viewport = true);
    virtual int			 isAmbient();

    void	getLightColor(fpreal &r, fpreal &g, fpreal &b, fpreal t)
		{
		    fpreal	dim = DIMMER(t);
		    r = CR(t)*dim;
		    g = CG(t)*dim;
		    b = CB(t)*dim;
		}
    virtual OBJ_OBJECT_TYPE	 getObjectType() const;

    virtual void		 rmanBestShader(UT_String &str, fpreal t) = 0;
    virtual void		 vexBestShader(UT_String &str, fpreal t) = 0;
    virtual void		 vexBestShadow(UT_String &str, fpreal t) = 0;

    static PRM_Template		*getTemplateList();

    virtual OBJ_Ambient		*castToOBJAmbient() { return this; }

    /// Get the tag used for light linking
    bool	getLightLinkTag(UT_String &tag, bool create=true);

    // Remove an existing light linking tag. Returns false if the tag doesn't
    // exist.
    bool	removeLightLinkTag(const char *tag);

    // Create a new light linking tag for a given suffix.
    // and the new tag checked for uniqueness. The new tag name is returned.
    bool	createLightLinkTag(const char *tag_suffix, UT_String &tag);

    fpreal	DIMMER(fpreal t);
    void	SHADER(UT_String &str, fpreal t);
    void	RISHADER(UT_String &str, fpreal t);
    fpreal	CR(fpreal t);
    fpreal	CG(fpreal t);
    fpreal	CB(fpreal t);
    int		BIND_OGL_LIGHT(fpreal t);
    int		BIND_LIGHT(fpreal t);

    virtual int		 collectPropertiesFromChannels(
					PI_EditScriptedParms &editparms,
					UT_BitArray &selection,
					const CH_ChannelList &channels);
    virtual int		 collectPropertiesFromParms(
					PI_EditScriptedParms &editparms,
					UT_StringArray &names,
					PRM_ParmNameMap &nmap);

protected:

    // Used to get pointer to indirection indices for each object type
    virtual int			*getIndirect() const
				{ return ambientIndirect; }

    // The camera does not use all of its parameters during a cook.  Therefore
    // in order to get the time dependencies right, we must override this.
    virtual bool		 cookedDataUsesAllParameters() const
				 { return true; }

    OBJ_ReLightProxy		*myRELightProxy;
private:

    static int		*ambientIndirect;
};

#endif
