/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_RelationshipEmpty.h ( SIM Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SIM_RelationshipEmpty__
#define __SIM_RelationshipEmpty__

#include "SIM_API.h"
#include "SIM_RelationshipData.h"

/// Defines a relationship between objects that has no specific meaning
/// This will affect solve order without creating any extra semantic
/// information.  This is most useful when an implied relationship is
/// present (through dop() expressions, perhaps) which can't be
/// detected by the engine.
class SIM_API SIM_RelationshipEmpty : public SIM_RelationshipData
{
protected:
    explicit		 SIM_RelationshipEmpty(const SIM_DataFactory *factory);
    virtual		~SIM_RelationshipEmpty();

private:
    static const SIM_DopDescription	*getEmptyDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_RelationshipEmpty,
			SIM_RelationshipData,
			"Empty Relationship",
			getEmptyDopDescription());
};

#endif

