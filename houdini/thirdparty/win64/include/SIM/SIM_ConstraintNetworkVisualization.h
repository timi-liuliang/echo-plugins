/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 */

#ifndef __SIM_ConstraintNetworkVisualization_h__
#define __SIM_ConstraintNetworkVisualization_h__

#include "SIM_API.h"
#include "SIM_Data.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"

class SIM_Geometry;

/// Draws guide geometry for constraint networks.
class SIM_API SIM_ConstraintNetworkVisualization : public SIM_Data,
                                                   public SIM_OptionsUser
{
protected:
    explicit SIM_ConstraintNetworkVisualization(const SIM_DataFactory *factory);

    virtual void initializeSubclass();

    virtual bool getIsAlternateRepresentationSubclass() const;
    virtual void initAlternateRepresentationSubclass(const SIM_Data &parent);

    virtual SIM_Guide *createGuideObjectSubclass() const;
    virtual void
    buildGuideGeometrySubclass(const SIM_RootData &root,
                               const SIM_Options &options,
                               const GU_DetailHandle &gdh, UT_DMatrix4 *xform,
                               const SIM_Time &t) const;

private:
    static const SIM_DopDescription *getDopDescription();

    const SIM_Geometry *myConstraintGeometry;
    const SIM_Data *myRootData;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConstraintNetworkVisualization, SIM_Data,
                        "Constraint Network Visualization",
                        getDopDescription());
};

#endif
