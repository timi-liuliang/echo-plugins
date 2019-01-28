/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DM_GeoDetail.h ( DM Library, C++)
 *
 * COMMENTS:
 *	Thin wrapper around a GUI_DetailLook to make it a simpler, safer
 *	interface.
 */
#ifndef DM_GeoDetail_h
#define DM_GeoDetail_h

#include "DM_API.h"
#include "DM_ViewportType.h"

#include <UT/UT_BoundingBox.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <UT/UT_Vector4.h>
#include <GU/GU_DetailHandle.h>

class RE_Light;
class OP_Node;
class GR_DisplayOption;
class GUI_DisplayOption;
class GUI_DetailLook;

class DM_API DM_GeoDetail
{
public:
    bool	isValid() const { return myLook != NULL; }

    // Node queries ---
		 
    /// The object this detail represents. May be NULL for guides.
    OP_Node	*getObject() const;

    /// The SOP this detail represents. May be NULL.
    OP_Node	*getSop() const;

   
    // Per-detail queries --------
    
    /// Returns the number of details this geometry has. Generally 1, but
    /// DOPs and guides can have more than 1.
    int			 getNumDetails() const;

    /// Returns a detail handle for one of the details contained in this
    /// geometry, where index is from 0 to getNumDetails()-1. The detail handle
    /// may be NULL.
    GU_ConstDetailHandle getDetailHandle(int detail_index) const;

    /// Get the transform of the detail at 'detail_index'.
    UT_Matrix4D		 getDetailTransform(int detail_index) const;

    /// Get the world root position of the detail at 'detail_index'
    UT_Vector4D		 getWorldPosition(int detail_index) const;

    /// Query if the detail is selected. This only applies to DOPs.
    bool		 isDetailSelected(int detail_index) const;

    /// Returns a descriptive (printable) name of the detail at 'detail_index'.
    UT_String		 getDescriptiveName(int detail_index) const;

    /// Returns a descriptive node path name of the detail at 'detail_index'.
    UT_String		 getDescriptivePath(int detail_index) const;


    
    // Lighting queries ----
    
    /// Returns true if any of the contained details are lit.
    bool			  areAnyLit() const;

    /// Returns true if any of the contained details are unlit.
    bool			  areAnyUnlit() const;

    /// Returns an array of RE Light objects that are illuminating this
    /// geometry. The RE_Light object contains a subset of lighting parameters
    /// for GL display.
    const UT_ValArray<RE_Light *> &getLights() const;
    
    /// Returns true if any of the contained details have transparency.
    bool			  hasTransparency() const;



    
    // Global queries ------------
    
    /// Check if this geometry is enabled for display. This is not the same
    /// thing as the display flag.
    bool	isEnabled() const;

    /// Is this object currently selected at the scene level.
    bool	isSelectedObject() const;

    /// Does this object belong to the secondary selection. 
    bool	isSecondarySelected() const;

    /// Is this object the current object at the scene level. This implies that
    /// it is also selected, but is the primary selected object for cases where
    /// only one object can be chosen (such as when diving into an object with
    /// multiple objects selected).
    bool	isCurrentObject() const;

    /// Does the object have a hint on it. Normally used to indicate a previous
    /// selection or group.
    bool	isHintedObject() const;
    
    /// Is this object ghosted. When in SOPs, other objects are ghosted by
    /// default.
    bool	isGhostedObject() const;

    /// Is this object doing point instancing of another geometry on its points.
    bool	isPointInstanced() const;

    /// Does this represent guide geometry (usually wireframe or unlit)
    bool	isGuide() const;

    /// Does this object have the xray-flag set, or the X-ray display option
    /// set for its display set.
    bool	isXray() const;

    /// Does this geometry represent user-editable geometry (ie, not a light)
    bool	isGeometry() const;

    /// Is this geometry representing a templated SOP.
    bool	isTemplate() const;

    /// Is this geometry representing a selectable template SOP.
    bool	isSelectableTemplate() const;

    /// Set if the SOP is displayed, but not current.
    bool	isTargetOutput() const;

    /// Set if this SOP is the current SOP (primarily selected)
    bool	isCurrentOutput() const;

    /// Does this geometry vary over time.
    bool	isTimeDependent() const;

    /// Should this geometry highlight partial selections (points, prims, etc)
    bool	showPartialSelections(GUI_DisplayOption &disp_opt) const;

    /// Bitfield of the viewport types this geometry can appear in.
    DM_ViewportType getViewportMask() const;


    // Utility methods --------

    /// Return the bounding box of objects in the 3D viewport. May return false
    /// if the detail is not enabled, or contains no geometry. 'rot' is an
    /// option rotation matrix.
    bool	getBoundingBox3D(UT_BoundingBox &bbox,
				 const UT_Matrix3R *rot = NULL) const;

    
    DM_GeoDetail();
    DM_GeoDetail(const GUI_DetailLook *look);
    DM_GeoDetail(const DM_GeoDetail &d);
private:
    const GUI_DetailLook *myLook;
};






#endif

