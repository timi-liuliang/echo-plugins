/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_RelationshipTarget.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_RelationshipTarget__
#define __SIM_RelationshipTarget__

#include "SIM_API.h"
#include "SIM_RelationshipData.h"

/// Defines a relationship between objects that the affectee objects
/// should target the affector objects.
class SIM_API SIM_RelationshipTarget : public SIM_RelationshipData
{
protected:
    explicit		 SIM_RelationshipTarget(const SIM_DataFactory *factory);
    virtual		~SIM_RelationshipTarget();

private:
    static const SIM_DopDescription	*getTargetDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RelationshipTarget,
			SIM_RelationshipData,
			"Target Relationship",
			getTargetDopDescription());
};

#endif

