/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 */

#ifndef __SIM_ConstraintNetworkRelationship_h__
#define __SIM_ConstraintNetworkRelationship_h__

#include "SIM_API.h"
#include "SIM_RelationshipData.h"
#include "SIM_OptionsUser.h"

class SIM_Geometry;
class SIM_GeometryCopy;

class SIM_API SIM_ConstraintNetworkRelationship : public SIM_RelationshipData,
                                                  public SIM_OptionsUser
{
public:
    const SIM_Geometry *getGeometry() const;
    SIM_GeometryCopy *getGeometry();

protected:
    explicit SIM_ConstraintNetworkRelationship(const SIM_DataFactory *factory);
    virtual ~SIM_ConstraintNetworkRelationship();

private:
    static const SIM_DopDescription *getConstraintNetworkDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_ConstraintNetworkRelationship,
			SIM_RelationshipData,
			"Constraint Network Relationship",
			getConstraintNetworkDopDescription());
};

#endif
