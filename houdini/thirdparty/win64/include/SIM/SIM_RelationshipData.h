/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RelationshipData_h__
#define __SIM_RelationshipData_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"

/// This is the base class for all data types that can be attached as
/// subdata to a SIM_Relationship. It is this subdata of the SIM_Relationship
/// that defines the nature of the relationship.
class SIM_API SIM_RelationshipData : public SIM_Data
{
protected:
    explicit		 SIM_RelationshipData(const SIM_DataFactory *factory);
    virtual		~SIM_RelationshipData();

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_RelationshipData, SIM_Data);
};

#endif

