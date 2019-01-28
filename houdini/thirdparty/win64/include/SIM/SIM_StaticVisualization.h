/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_StaticVisualization_h__
#define __SIM_StaticVisualization_h__

#include "SIM_Names.h"
#include "SIM_Visualization.h"
#include <GEO/GEO_Face.h>
#include <GU/GU_Detail.h>
#include <GU/GU_PrimSphere.h>
#include <GU/GU_PrimTube.h>
#include <GU/GU_PrimPoly.h>

SIM_API extern void SIMdrawCollisionRadiusGuideGeometry(
    const GU_Detail& inGdp,
    GU_Detail& outGdp,
    fpreal collision_radius,
    UT_Vector3 thicknessColor,
    bool penetrationEnabled,
    UT_Vector3 penetrationColor,
    GA_RWHandleV3& myAttOutCd,
    const std::vector<UT_Vector3> valuesPointPositions,
    const std::vector<fpreal> scalePointThickness,
    const std::vector<int> isResolvedExternal,
    const std::vector<int> isResolvedCloth,
    const std::vector<int> isResolvedSelf
);

class SIM_StaticVisualization : public SIM_Visualization
{
protected:
    explicit		 SIM_StaticVisualization(const SIM_DataFactory *factory);
    virtual		~SIM_StaticVisualization();

    virtual void	 getPointVectorAttribs(
				UT_Array<simVisualizationItem<UT_Vector3> > &data) const;
    virtual void	 renderPrimAttribs() const;

private:
    fpreal               getVelocityScale() const;
    bool		 isEnabled(const char *option_name) const;
    void		 getPointVectorAttrib(
				UT_Array<simVisualizationItem<UT_Vector3> > &data,
				const char *option_name,
				const char *attrib_name,
				const char *color_option_name,
				const UT_Vector3 &default_color,
				fpreal scale) const;
    void		 drawCollisionRadiusGuideGeometry() const;

    static const SIM_DopDescription	*getVisualizationDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_StaticVisualization,
			SIM_Data,
			"Static Visualization",
			getVisualizationDopDescription());
};

#endif
