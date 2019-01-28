/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_IndexFieldVisualization.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	This simple procedure allows for intersection with signed
 *	distance functions.
 */

#ifndef __SIM_IndexFieldVisualization_H__
#define __SIM_IndexFieldVisualization_H__

#include "SIM_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>

#include "SIM_Names.h"
#include "SIM_OptionsUser.h"
#include "SIM_DataUtils.h"

class UT_IStream;
class SIM_Geometry;
class SIM_IndexField;

/// This class holds a signed distance function representing a GU_Detail.
class SIM_API SIM_IndexFieldVisualization : public SIM_Data,
			public SIM_OptionsUser
{
public:
    GET_GUIDE_FUNC_B(SIM_NAME_SHOWGUIDE, ShowGuide, true);
    GET_GUIDE_FUNC_V3(SIM_NAME_COLOR, Color, (1, 1, 1));
    GET_GUIDE_FUNC_I("guideindex", GuideIndex, 0);
    GET_GUIDE_FUNC_B("guideinvert", GuideInvert, false);
    GET_GUIDE_FUNC_B("usebox", UseBox, false);
    GET_GUIDE_FUNC_B("useboxhash", UseBoxHash, false);
   
    GET_GUIDE_FUNC_B("guideuseplane", GuideUsePlane, false);
    GET_GUIDE_FUNC_I("guideplane", GuidePlane, 0);
    GET_GUIDE_FUNC_I("guidevismode", GuideVisMode, 0);
    GET_GUIDE_FUNC_V2("guiderange", GuideRange, (0, 1));
    GET_GUIDE_FUNC_F("guideplaneval", GuidePlaneVal, 0);
    
protected:
    explicit		 SIM_IndexFieldVisualization(const SIM_DataFactory *factory);
    virtual		~SIM_IndexFieldVisualization();

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
    static const SIM_DopDescription	*getIndexFieldVisualizationDopDescription();

    const SIM_IndexField		*myIndexField;

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_IndexFieldVisualization,
			SIM_Data,
			"IndexFieldVisualization",
			getIndexFieldVisualizationDopDescription());
};

#endif
