/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AIFJSON.h ( GA Library, C++)
 *
 * COMMENTS:	Attribute Interface for file I/O
 */

#ifndef __GA_AIFJSON__
#define __GA_AIFJSON__

#include "GA_API.h"

#include <SYS/SYS_Types.h>


class GA_Attribute;
class GA_LoadMap;
class GA_Range;
class GA_SaveMap;
class UT_JSONParser;
class UT_JSONWriter;


/// @brief Attribute Interface for file I/O
/// 
/// This class provides an interface used to save and load attributes to and
/// from JSON files.  This class is responsible for saving and loading the
/// private @b data object in an attribute's JSON description.
///
/// @section JSON-GA_Attribute-Data JSON Schema: GA_Attribute-Data
/// Each attribute can save/load its information in its own JSON attribute
/// block.  Any data needed by the attribute can be saved in this data block.
///
/// @see GA_Attribute, GA_AttributeType, @ref JSON-GA_ATINumeric,
///      @ref JSON-GA_ATIString
class GA_API GA_AIFJSON 
{
public:
	     GA_AIFJSON() {}
    virtual ~GA_AIFJSON() {}

    /// Save the attribute to JSON stream.
    /// @param w	The JSON writer
    /// @param attrib	The attribute to be saved
    /// @param save	The save map.
    virtual bool	 save(UT_JSONWriter &w,
				const GA_Attribute *attrib,
				const GA_SaveMap &save) const = 0;

    /// Load and create a new attribute from a JSON stream.
    /// @param p @n
    ///	  The JSON parser
    /// @param attrib @n
    ///   The attribute to load
    /// @param version @n
    ///	  The version specified in the attribute definition (the value returned
    ///	  by getSaveVersion() when the attribute was saved).  Default is 0.
    /// @param load @n
    ///   The load map
    /// The detail being loaded should be retrieved by from the
    /// GA_LoadMap::getDetail() method.
    virtual bool	 load(UT_JSONParser &p,
				GA_Attribute *attrib,
				int64 save_version,
				const GA_LoadMap &load) const = 0;

protected:
    /// During saving, this method can get an iterator for the specific
    /// attribute.  The iterator will process the items which need to be
    /// saved.
    const GA_Range	&getRange(const GA_Attribute *attrib,
				     const GA_SaveMap &map);
};

#endif
