/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_VectorFieldVisualization.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	This simple procedure allows for intersection with signed
 *	distance functions.
 */

#ifndef __SIM_VectorFieldVisualization_H__
#define __SIM_VectorFieldVisualization_H__

#include "SIM_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>

#include "SIM_Names.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

class UT_IStream;
class SIM_Geometry;
class SIM_VectorField;

/// This class holds a signed distance function representing a GU_Detail.
class SIM_API SIM_VectorFieldVisualization : public SIM_Data,
			public SIM_OptionsUser
{
public:
    GET_GUIDE_FUNC_B(SIM_NAME_SHOWGUIDE, ShowGuide, true);
    GET_GUIDE_FUNC_V3(SIM_NAME_COLOR, Color, (1, 1, 1));
    
    GET_GUIDE_FUNC_B("guideusesmoke", GuideUseSmoke, false);
    GET_GUIDE_FUNC_B("guideusebox", GuideUseBox, false);
    GET_GUIDE_FUNC_B("guideuseboxhash", GuideUseBoxHash, false);
    GET_GUIDE_FUNC_F("guidescale", GuideScale, 1.0);
    GET_GUIDE_FUNC_I("guidevistype", GuideVisType, 0);
    GET_GUIDE_FUNC_I("guidevismode", GuideVisMode, 0);
    GET_GUIDE_FUNC_F("guidevisscale", GuideVisScale, 1.0);
    GET_GUIDE_FUNC_V2("guiderange", GuideRange, (0, 1));
    
    GET_GUIDE_FUNC_B("guideoverridediv", GuideOverrideDiv, false);
    GET_GUIDE_FUNC_V3("guidediv", GuideDiv, (10, 10, 10));

    GET_GUIDE_FUNC_B("guideusestreamers", GuideUseStreamers, false);
    GET_GUIDE_FUNC_B("guideuseplane", GuideUsePlane, false);
    GET_GUIDE_FUNC_I("guideplane", GuidePlane, 0);
    GET_GUIDE_FUNC_F("guideplaneval", GuidePlaneVal, 0);
    GET_GUIDE_FUNC_F("guidestreamerlen", GuideStreamerLen, 1);
    GET_GUIDE_FUNC_F("guidestreamerminspeed", GuideStreamerMinSpeed, 0);

    GET_GUIDE_FUNC_B("guidebarbs", GuideBarbs, true);
    GET_GUIDE_FUNC_B("guidepercomp", GuidePerComp, false);

    /// Adds to the gdp a simple vector starting at pos and in direction
    /// vel.
    static void		 createVectorGuide(GU_Detail *gdp,
				const UT_Vector3 &pos,
				const UT_Vector3 &vel,
				const GA_RWHandleV3 &clrh,
				const UT_Vector3 &clr,
				bool usebarbs);
protected:
    explicit		 SIM_VectorFieldVisualization(const SIM_DataFactory *factory);
    virtual		~SIM_VectorFieldVisualization();

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
    UT_Vector3		 getColorFromDir(UT_Vector3 dir) const;

    static const SIM_DopDescription	*getVectorFieldVisualizationDopDescription();

    const SIM_VectorField		*myVectorField;

    // Used to build an isosurface:
    static float		 pointDensity(const UT_Vector3 &pos);
    static const SIM_VectorField*theCurrentField;

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_VectorFieldVisualization,
			SIM_Data,
			"VectorFieldVisualization",
			getVectorFieldVisualizationDopDescription());
};

#endif
