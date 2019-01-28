/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_MaterialProperties.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_MaterialProperties__
#define __STY_MaterialProperties__

#include "STY_API.h"
#include "STY_Types.h"

class UT_JSONValue;
class UT_JSONValueMap;

/// Provides values for the properties of materials defined in a style sheet.
class STY_API STY_MaterialProperties
{
public:
		STY_MaterialProperties();

    /// Loads the material properties from json value.
    void	loadStyleSheetMaterialProperties(const UT_JSONValue *json);

    /// Checks if there are any properties for the material.
    bool	isEmpty() const
		{ return myProperties.empty(); }

    /// Represents the material properties object as options.
    const STY_ConstantMap &getProperties() const
		{ return myProperties; }

    /// Save these material properties as JSON to a style sheet.
    void	saveStyleSheet(UT_JSONValueMap *map) const;

private:
    /// Internal representation of properties.
    STY_ConstantMap	myProperties;
};

#endif

