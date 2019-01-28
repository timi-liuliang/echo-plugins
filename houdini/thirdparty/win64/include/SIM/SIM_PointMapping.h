/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_PointMapping.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	This class builds a mapping from some point attribute value to
 *	the point number that has that attribute value.
 */

#ifndef __SIM_PointMapping_h__
#define __SIM_PointMapping_h__

#include "SIM_API.h"
#include <UT/UT_Map.h>
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"

/// This class holds a mapping from some point attribute to point numbers
/// on the parent geometry data. The attribute must be a single integer.
class SIM_API SIM_PointMapping : public SIM_Data,
				 public SIM_OptionsUser
{
public:
    /// The name of the attribute to use for our mapping.
    GETSET_DATA_FUNCS_S(SIM_NAME_ATTRIBUTENAME, AttributeName);

    /// The name of the attribute to map.
    GA_Index		 getPointWithAttributeValue(int attribvalue) const;

protected:
    explicit		 SIM_PointMapping(const SIM_DataFactory *factory);
    virtual		~SIM_PointMapping();

    virtual void	 initializeSubclass();
    virtual int64	 getMemorySizeSubclass() const;
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);

private:
    UT_Map<int, GA_Index> myMapping;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_PointMapping,
			SIM_Data,
			"Point Mapping",
			getEmptyDopDescription());
};

#endif
