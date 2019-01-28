/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RelationshipArray_h__
#define __SIM_RelationshipArray_h__

#include "SIM_API.h"
#include "SIM_PtrArraySorted.h"
#include "SIM_Data.h"

class UT_String;
class SIM_Relationship;

/// Holds pointers to a number of SIM_Relationships.
class SIM_API SIM_RelationshipArray :
      public SIM_PtrArraySorted<SIM_Relationship *>
{
public:
			 SIM_RelationshipArray();
    virtual		~SIM_RelationshipArray();

    /// Change the supplied name to make it different from any other
    /// relationship in this array.
    void		 forceUniqueRelationshipName(UT_String &name) const;

    /// Find the relationship with the specified name.
    SIM_Relationship	*findRelationshipByName(const char *name) const;

    /// Find the position of the relationship with the specified name.
    int			 findRelationshipPosByName(const char *name) const;

    /// Get all relationships of a particular type. Note that the filter
    /// is applied to the SIM_RelationshipData subdata of the
    /// SIM_Relationship, not to the SIM_Relationship itself.
    void		 filter(const SIM_DataFilter &filter,
				SIM_DataArray &rels) const;

    /// Get const pointers to all relationships of a particular type.
    /// As with filter(), the SIM_DataFilter is applied to the
    /// SIM_RelationshipData subdata, not the SIM_Relationship itself.
    void		 filterConst(const SIM_DataFilter &filter,
				     SIM_ConstDataArray &rels) const;

    /// Function for sorting pointers according to the relationship name.
    static int		 compareRelationshipNames(SIM_Relationship * const*,
						  SIM_Relationship * const*);
};

#endif

