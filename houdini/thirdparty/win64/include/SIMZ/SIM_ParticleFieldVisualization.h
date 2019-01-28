/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ParticleFieldVisualization.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	Used to visualize a field of particles within DOPs
 *
 */

#ifndef __SIM_ParticleFieldVisualization_H__
#define __SIM_ParticleFieldVisualization_H__

#include "SIMZ_Utils.h"

#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_DataUtils.h>
#include <SIM/SIM_Data.h>

#include <GA/GA_Handle.h>

#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Color.h>

class SIM_Geometry;

enum SIM_VisType
{
    SIM_VISTYPE_NONE,
    SIM_VISTYPE_SPEED,
    SIM_VISTYPE_DIR,
    SIM_VISTYPE_VALUE,
};

/// This class holds a signed distance function representing a GU_Detail.
class SIMZ_API SIM_ParticleFieldVisualization : public SIM_Data,
			public SIM_OptionsUser
{
public:
    GET_GUIDE_FUNC_B(SIM_NAME_SHOWGUIDE, ShowGuide, true);
    GET_GUIDE_FUNC_V3(SIM_NAME_COLOR, Color, (1, 1, 1));
    
    GET_GUIDE_FUNC_I("visprim", VisPrim, 0); 

    GET_GUIDE_FUNC_F("scale", Scale, 1.0);
    GET_GUIDE_FUNC_S("spriteshop", SpriteShop, "");
    GET_GUIDE_FUNC_S("grainsoppath", GrainSopPath, "");
   
    GET_GUIDE_FUNC_S("guideattrib", GuideVisAttrib, "");
    GET_GUIDE_FUNC_I("guidevistype", GuideVisType, 0);
    GET_GUIDE_FUNC_I("guidevismode", GuideVisMode, 0);
    GET_GUIDE_FUNC_F("guidevisscale", GuideVisScale, 1.0);
    GET_GUIDE_FUNC_B("guidedetectrange", GuideDetectRange, true);
    GET_GUIDE_FUNC_V2("guiderange", GuideRange, (0, 1));

    GET_GUIDE_FUNC_RAMP("guidevisramp", GuideVisRamp);

protected:
    explicit		 SIM_ParticleFieldVisualization(const SIM_DataFactory *factory);
    virtual		~SIM_ParticleFieldVisualization();

    /// Overrides to properly implement this class as a SIM_Data.
    virtual void	 initializeSubclass();

    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);
    virtual SIM_Guide	*createGuideObjectSubclass() const;
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

private:

    static UT_Vector2 detectRange(
        const GU_Detail *igdp,
        const GA_ROHandleF &vish,
        bool forcescalar,
        SIM_VisType vistype);

    static const SIM_DopDescription *getParticleFieldVisualizationDopDescription();

    const SIM_Geometry *myGeometry;

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_ParticleFieldVisualization,
			SIM_Data,
			"ParticleFieldVisualization",
			getParticleFieldVisualizationDopDescription());
};

#endif
