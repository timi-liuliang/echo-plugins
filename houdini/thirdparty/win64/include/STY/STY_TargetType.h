/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_TargetType.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_TargetType__
#define __STY_TargetType__

#include "STY_API.h"
#include <UT/UT_StringHolder.h>

class UT_JSONValueMap;

STY_API extern const UT_StringHolder	     STY_TARGETTYPE_PRIMITIVE;
STY_API extern const UT_StringHolder	     STY_TARGETTYPE_POINT;
STY_API extern const UT_StringHolder	     STY_TARGETTYPE_GEOOBJECT;
STY_API extern const UT_StringHolder	     STY_TARGETTYPE_SELF;

//  =========================================================================== 
/// The target type used for identification general type of the targets.
/// Specified by the style target and used for matching by styled objects.
/// Uses shared string for both efficient storage an comparison (ptr comp).
class STY_API STY_TargetType
{
public:
    /// The style target is really a string wrapped by this class.
				 STY_TargetType();
    explicit			 STY_TargetType(const char *target_type);
    
    /// Assignment operator.
    STY_TargetType		&operator=(const STY_TargetType &other);

    /// Target type comparison (since member points into the string table,
    /// the pointer comparison is sufficient).
    bool			 operator==(const STY_TargetType &other) const
				 { return myTargetType == other.myTargetType; }
    bool			 operator!=(const STY_TargetType &other) const
				 { return !(*this == other); }
    
    /// Checks if this is a primitive target (the default).
    bool			 isPrimitive() const;
    /// Checks if this is a point target.
    bool			 isPoint() const;
    /// Checks if this is an object target.
    bool			 isGeoObject() const;
    /// Checks if this is a self target.
    bool			 isSelf() const;

    /// Saves the target type to a JSON map for writing out a style sheet.
    void			 saveStyleSheet(UT_JSONValueMap *map) const;

private:
    /// Target type name (pointer into shared table).
    UT_StringHolder		 myTargetType;
};


#endif

