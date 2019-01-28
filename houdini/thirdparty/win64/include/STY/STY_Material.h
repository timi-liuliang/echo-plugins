/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_Material.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_Material__
#define __STY_Material__

#include "STY_API.h"
#include "STY_MaterialProperties.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class STY_LoadContext;
class UT_JSONValueMap;
class UT_JSONValue;

/// Represents a material defined within a style sheet.
class STY_API STY_Material :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_Material>
{
public:
			 STY_Material( const UT_JSONValue *json,
			    const STY_LoadContext *load_context );
			~STY_Material();

    const STY_MaterialProperties &
			getProperties() const
			{ return myProperties; }

    /// Save this material as JSON to a style sheet.
    void		saveStyleSheet(UT_JSONValueMap *map) const;

private:
    void		loadStyleSheetMaterial( const UT_JSONValue *json,
			    const STY_LoadContext *load_context );

    STY_MaterialProperties	 myProperties;
};

#endif

