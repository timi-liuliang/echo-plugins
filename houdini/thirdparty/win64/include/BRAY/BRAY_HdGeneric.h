/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_HdGeneric.h (BRAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __BRAY_HdGeneric__
#define __BRAY_HdGeneric__

#include "BRAY_API.h"
#include <UT/UT_Map.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>
#include "BRAY_Generic.h"

class BRAY_Generic;

/// Provides an interface to bind Hydra USD primitives to Mantra primitives.
///
/// The constructor/destructor typically look like this: @code
/// Subclass::Subclass()
/// {
///     bind("tokenname");
/// }
/// Subclass::~Subclass()
/// {
///     unbind("tokenname");
/// }
/// @endcode
class BRAY_API BRAY_HdGeneric
{
public:
    BRAY_HdGeneric();
    virtual ~BRAY_HdGeneric();

    /// Create a generic
    virtual BRAY_Generic		*create() const = 0;

#if 0
    /// TODO:  Update the generic when parameters have changed
    virtual bool	update(BRAY_Generic *generic,
				HdSceneDelegate *sceneDelegate,
				HdRenderParam *renderParam,
				HdDirtyBits *dirtyBits,
				TfToken const &reprName) const = 0;
#endif

    /// The list of generics is stored in a map of {name, HdGeneric}
    using MapType = UT_StringMap<const BRAY_HdGeneric *>;
    using iterator = MapType::const_iterator;

    /// @{
    /// Access the list of all the registered generics
    static const MapType	&generics();
    static iterator		 begin() { return generics().begin(); }
    static iterator		 end() { return generics().end(); }
    /// @}

protected:
    /// Bind this HdGeneric handler to the Hydra primitive rprim type.
    /// For example, the token might be @c HdPrimTypeTokens->points to override
    /// rendering of the points primitive.
    bool	bind(const UT_StringHolder &token);

    /// Unbind this handler
    bool	unbind(const UT_StringHolder &token);
};

extern "C" {
    /// Register a new generic handler.  This method should allocate a new
    /// sub-class of the BRAY_HdGeneric.
    SYS_VISIBILITY_EXPORT extern void	BRAYregisterGeneric();
}

#endif
