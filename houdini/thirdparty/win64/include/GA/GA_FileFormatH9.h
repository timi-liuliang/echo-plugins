/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_FileFormatH9.h ( GA Library, C++)
 *
 * COMMENTS:	Collection of classes to aid in loading ./geo/.bgeo files
 */

#ifndef __GA_FileFormatH9__
#define __GA_FileFormatH9__

#include "GA_API.h"

class GA_PrimitiveDefinition;

namespace GA_FileFormatH9
{

/// An abstract class for mapping primitive definitions to an appropriate H9
/// primitive type or vice versa.
class GA_API PrimTypeXlate
{
public:
    virtual ~PrimTypeXlate() {}

    /// Map the given GA_PrimitiveDefinition to the appropriate H9 primitive
    /// type, if any.  Returns 0 when no appropriate H9 type exists.
    virtual unsigned getTypeH9(const GA_PrimitiveDefinition &) const = 0;

    /// Map the given GA_PrimitiveDefinition to the appropriate H9 primitive
    /// type token, if any.  Returns 0 when no appropriate H9 type exists.
    virtual const char *getTokenH9(const GA_PrimitiveDefinition &) const = 0;

    /// Get the primitive definition to use when loading the specified H9
    /// primitive type, specified either by type id or type token.
    virtual const GA_PrimitiveDefinition *getFactoryDefinition(
					unsigned h9_primtype) const = 0;
    virtual const GA_PrimitiveDefinition *getFactoryDefinition(
				const char *h9_primtype_token) const = 0;
};

}

#endif
