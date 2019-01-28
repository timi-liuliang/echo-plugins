/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_SopGuide.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	Used to visualize a field of particles within DOPs
 *
 */

#ifndef __SIM_SopGuide_H__
#define __SIM_SopGuide_H__

#include "SIMZ_Utils.h"

#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Color.h>

#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_DataUtils.h>
#include <SIM/SIM_Data.h>

class SIM_Geometry;
class GEO_AttributeHandle;

#define SIM_SOPGEOMETRY_USESOPPATH		"usesoppath"
class SIMZ_API SIM_SopGuide : public SIM_Data,
			public SIM_OptionsUser
{
public:
    GET_GUIDE_FUNC_B(SIM_NAME_SHOWGUIDE, ShowGuide, true);
    GET_GUIDE_FUNC_V3(SIM_NAME_COLOR, Color, (1, 1, 1));
    
    GET_GUIDE_FUNC_B("wireframe", Wireframe, true);
    GET_GUIDE_FUNC_B(SIM_SOPGEOMETRY_USESOPPATH, UseSopPath, false);
    GET_GUIDE_FUNC_S(SIM_NAME_SOPPATH, SopPath, "");

protected:
    explicit		 SIM_SopGuide(const SIM_DataFactory *factory);
    virtual		~SIM_SopGuide();

    virtual SIM_Guide	*createGuideObjectSubclass() const;
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

private:
    static const SIM_DopDescription	*getSopGuideDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_SopGuide,
			SIM_Data,
			"SopGuide",
			getSopGuideDopDescription());
};

#endif
