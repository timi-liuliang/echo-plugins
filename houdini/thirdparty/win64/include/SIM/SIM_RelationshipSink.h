/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RelationshipSink_h__
#define __SIM_RelationshipSink_h__

#include "SIM_API.h"
#include "SIM_RelationshipData.h"

/// This class defines a sink relationship between objects. Sink
/// relationships are useful for solvers that destroy some physical
/// entity as part of the solve process. An example of this would be
/// the fluid solver which may use another DOP object as a fluid sink
/// to destroy fluid. 
class SIM_API SIM_RelationshipSink : public SIM_RelationshipData
{
protected:
    explicit		 SIM_RelationshipSink(const SIM_DataFactory *factory);
    virtual		~SIM_RelationshipSink();

private:
    static const SIM_DopDescription	*getSinkDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RelationshipSink,
			SIM_RelationshipData,
			"Sink Relationship",
			getSinkDopDescription());
};

#endif

