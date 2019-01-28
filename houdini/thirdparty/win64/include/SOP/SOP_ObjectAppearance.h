/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_ObjectAppearance.h (SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_ObjectAppearance__
#define __SOP_ObjectAppearance__

#include "SOP_API.h"
#include <SYS/SYS_AtomicInt.h>
#include <UT/UT_Set.h>
#include <UT/UT_IntrusivePtr.h>
#include <GA/GA_Types.h>
#include <GEO/GEO_PackedTypes.h>
#include "SOP_ObjectAppearanceFwd.h"

class UT_StringArray;
class UT_TagManager;

class SOP_API SOP_ObjectAppearance
    : public UT_IntrusiveRefCounter<SOP_ObjectAppearance>
{
public:
    typedef UT_Set<std::string>	PrimList;

    SOP_ObjectAppearance();
    virtual ~SOP_ObjectAppearance();
    
    /// Return \c true if the object is a geometric object, which supports 
    /// per-primitive settings. If \c false, then the first argument to the 
    /// setters and getters is ignored (const char *, and GA_Offset), and the
    /// PrimList setters will fail.
    virtual bool	isGeometricObject() const = 0;

    /// Edits are not guaranteed to be applied until this method is called
    virtual bool	applyEdits() = 0;

    /// Get list of all possible paths for the leaf nodes in the tree.
    virtual bool	getLeafPaths(UT_StringArray &paths) const;

    /// Return the list of primitives which match the given path
    virtual GA_Offset	getPrimitiveOffset(const char *path) const;

    /// @{
    /// Get/Set the viewport visibility for the given primitive.
    virtual bool	visibility(GA_Offset prim,
				bool &visible) const;
    virtual bool	setVisibility(const char *path,
				bool visible);
    virtual bool	setVisibility(const PrimList &prims,
				bool visible);
    /// @}


    /// @{
    /// Get/Set the viewport LOD for the given primitive.
    virtual bool	viewportLOD(GA_Offset prim,
				GEO_ViewportLOD &lod) const; 
    virtual bool	setViewportLOD(const char *path,
				GEO_ViewportLOD l);
    virtual bool	setViewportLOD(const PrimList &prims,
				GEO_ViewportLOD l);
    /// @}

    /// @{
    /// Get/Set the material for the given primitive.
    virtual bool	material(GA_Offset prim,
				std::string &material) const;
    virtual bool	setMaterial(const char *path,
				const char *material);
    virtual bool	setMaterial(const PrimList &prims,
				const char *material);
    /// @}

    /// @{
    /// Get/Set the light selection expression for the given primitive.
    virtual bool	lightSelection(GA_Offset prim,
				std::string &light_selection) const;
    virtual bool	lightSelection(const char *path,
				std::string &light_selection) const;
    virtual bool	setLightSelection(const char *path,
				const char *light_selection);
    virtual bool	setLightSelection(const PrimList &prims,
				const char *light_selection);
    /// @}

    /// Add all paths to the @c selected_paths.  Each path in the appearance
    /// which matches the given @c tag should be prefixed by the @c path_head
    /// before it's added to the set.
    virtual bool	getLightSelection(UT_TagManager &tags,
				const std::string &path_head,
				UT_Set<std::string> &selected_paths,
				const char *tag) const;

    /// Set light selection for a given tag.  This should remove the tag from
    /// all paths not referenced in the selection list.  The @c path_head
    /// should be ignored from each path in the selection.
    virtual bool	editLightSelection(UT_TagManager &tags,
				const std::string &path_head,
				const UT_Set<std::string> &selected_paths,
				const char *tag);

private:
};

#endif
