/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RelationshipSource_h__
#define __SIM_RelationshipSource_h__

#include "SIM_API.h"
#include "SIM_RelationshipData.h"

/// This class defines a source relationship between objects. Source
/// relationships are useful for solvers that generate some physical
/// entity as part of the solve process. An example of this would be
/// the fluid solver which may use another DOP object as a fluid source
/// to generate new fluid. Another example is the HDK sample Snow
/// solver or Hair solver which generate snow and hair using a source
/// object.
class SIM_API SIM_RelationshipSource : public SIM_RelationshipData
{
protected:
    explicit		 SIM_RelationshipSource(const SIM_DataFactory *factory);
    virtual		~SIM_RelationshipSource();

private:
    static const SIM_DopDescription	*getSourceDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RelationshipSource,
			SIM_RelationshipData,
			"Source Relationship",
			getSourceDopDescription());
};

#endif

