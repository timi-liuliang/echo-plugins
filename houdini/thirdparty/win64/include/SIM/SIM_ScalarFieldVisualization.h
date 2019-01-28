/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_ScalarFieldVisualization.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	This simple procedure allows for intersection with signed
 *	distance functions.
 */

#ifndef __SIM_ScalarFieldVisualization_H__
#define __SIM_ScalarFieldVisualization_H__

#include "SIM_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_ThreadedAlgorithm.h>

#include "SIM_Names.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

class UT_IStream;
class SIM_Geometry;
class SIM_ScalarField;

struct simFindSurfaceCellsArgs;

/// This class holds a signed distance function representing a GU_Detail.
class SIM_API SIM_ScalarFieldVisualization : public SIM_Data,
			public SIM_OptionsUser
{
public:
    GET_GUIDE_FUNC_B(SIM_NAME_SHOWGUIDE, ShowGuide, true);
    GET_GUIDE_FUNC_V3(SIM_NAME_COLOR, Color, (1, 1, 1));
    GET_GUIDE_FUNC_F("guideiso", GuideIso, 0.0);
    GET_GUIDE_FUNC_B("guideinvert", GuideInvert, false);
    GET_GUIDE_FUNC_B("usesmoke", UseSmoke, false);
    GET_GUIDE_FUNC_B("guideremapsmoke", GuideRemapSmoke, false);
    GET_GUIDE_FUNC_F("guidesmokedensity", GuideSmokeDensity, 10.0);
    GET_GUIDE_FUNC_B("usebox", UseBox, false);
    GET_GUIDE_FUNC_B("useboxhash", UseBoxHash, false);
   
    GET_GUIDE_FUNC_B("guideuseplane", GuideUsePlane, false);
    GET_GUIDE_FUNC_I("guideplane", GuidePlane, 0);
    GET_GUIDE_FUNC_I("guidevismode", GuideVisMode, 0);
    GET_GUIDE_FUNC_I("guidevisdensitymode", GuideVisDensityMode, 0);
    GET_GUIDE_FUNC_V2("guiderange", GuideRange, (0, 1));
    GET_GUIDE_FUNC_F("guideplaneval", GuidePlaneVal, 0);

    GET_GUIDE_FUNC_B("guiderangemode", GuideRangeMode, false);
    GET_GUIDE_FUNC_F("guiderangecenter", GuideRangeCenter, 0.5);
    GET_GUIDE_FUNC_F("guiderangesize", GuideRangeSize, 1);
    
    /// Adds a wire frame bounding box of the specified dimensions and
    /// with colour set to the given offset.
    /// If xdiv..zdiv are non-zero, the box will be hatched accordingly.
    static void			 createBoundingBoxGuide(GU_Detail *gdp,
				    const UT_BoundingBox &bbox,
				    int xdiv, int ydiv, int zdiv,
				    const UT_Vector3 &color,
				    const GA_RWHandleV3 &primclrh,
				    const GA_RWHandleV3 &ptclrh);

protected:
    explicit		 SIM_ScalarFieldVisualization(const SIM_DataFactory *factory);
    virtual		~SIM_ScalarFieldVisualization();

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
    bool		 shouldMultiThread() const { return true; }
    THREADED_METHOD1_CONST(SIM_ScalarFieldVisualization, shouldMultiThread(), findSurfaceCells, simFindSurfaceCellsArgs *, args);
    void findSurfaceCellsPartial(simFindSurfaceCellsArgs *args, const UT_JobInfo &info) const;
    static const SIM_DopDescription	*getScalarFieldVisualizationDopDescription();

    const SIM_ScalarField		*myScalarField;

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_ScalarFieldVisualization,
			SIM_Data,
			"ScalarFieldVisualization",
			getScalarFieldVisualizationDopDescription());
};

#endif
