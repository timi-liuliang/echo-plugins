/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __RBD_Visualization_h__
#define __RBD_Visualization_h__

#include "RBD_API.h"
#include <SIM/SIM_Visualization.h>

/// Class for inspecting the state of a cloth simulation.
/// Using prim/point/vertex attributes on the cloth geometry, it generates
/// guide geometry showing internal forces, energy and impacts.
/// 
class RBD_API RBD_Visualization : public SIM_Visualization
{
protected:
    explicit		 RBD_Visualization(const SIM_DataFactory *factory);
    virtual		~RBD_Visualization();

    virtual bool	 getShowImpacts() const;
    virtual UT_Vector3	 getImpactColor() const;
    virtual fpreal	 getImpactScale() const;
    virtual bool	 getImpactShowAsVector() const { return true; }
    virtual bool	 getShowSubstepImpacts() const;

private:
    static const SIM_DopDescription	*getVisualizationDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(RBD_Visualization,
			SIM_Data,
			"RBD Visualization",
			getVisualizationDopDescription());
};
#endif
