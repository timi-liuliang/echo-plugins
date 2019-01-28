/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GUI_GroupData.h (GUI Library)
 *
 * COMMENTS:
 */
#ifndef __GUI_GroupData_h__
#define __GUI_GroupData_h__

#include "GUI_API.h"
#include <GR/GR_Defines.h>
#include <GU/GU_Selection.h>
#include <GU/GU_SelectType.h>
#include <GA/GA_Types.h>
#include <RE/RE_Texture.h>
#include <UT/UT_IntrusivePtr.h>

class RE_Render;
class RE_Geometry;
class RE_ShaderHandle;
class GUI_GeoRender;
class GR_CommonDispOption;
class GU_Detail;
class UT_Color;

/// Holds information sbout groups for display in the viewport.
class GUI_API GUI_GroupData : public UT_IntrusiveRefCounter<GUI_GroupData>
{
public:
				 GUI_GroupData(
				     GUI_GeoRender &owner,
				     const GR_CommonDispOption &options,
				     RE_Render *r,
				     GU_SelectionHandle *selection = nullptr,
				     UT_StringHolder *name = nullptr);
				~GUI_GroupData();

    const UT_StringHolder	&groupMask() const;
    GA_GroupType		 groupType() const;
    RE_Texture			*colorTexture() const;

    // These values are used for drawing primitive groups.
    RE_Texture			*idTexture() const;
    int				 minId() const;
    int				 maxId() const;

    // These values are used for drawing point and edge groups.
    RE_ShaderHandle		*shader(bool uv_viewport) const;
    RE_Geometry			*geometry() const;

    // Returns true if the provided mask specifies an adhoc group
    // (basically anything that isn't recognized as a connectivity
    // specifier, attribute range, or group name mask.
    static bool			 isAdhocGroupMask(const char *mask);

    // Returns a magic group name that is used to trigger the use of the
    // cook selection group from the detail, rather than a named group or  
    // an ad-hoc group.
    static UT_StringRef		 cookSelectionGroupMask();
    
private:
    void		 createIdTexture(RE_Render *r,
				const int *id_data,
				int numids);
    void		 createColorTexture(RE_Render *r,
				const UT_ValArray<uint8> &color_data);

    void		 initializePackedPrimsFromPoints(RE_Render *r,
				const GU_Detail &gdp,
				const UT_Array<int32> &id_data);
    void		 initializeFromConnectivity(RE_Render *r);
    void		 initializeFromAttrib(RE_Render *r);
    void		 initializeFromAdhocGroup(RE_Render *r);

    bool		 initializeFromGroup(
				RE_Render *r,
				GA_GroupType type = GA_GROUP_INVALID,
				const GA_Group *group = nullptr,
				const UT_StringRef *name = nullptr,
				const UT_Color *override_color = nullptr);
    void		 initializeFromPrimGroups(RE_Render *r,
				const GA_PrimitiveGroup *group = nullptr,
				const UT_StringRef *name = nullptr,
				const UT_Color *override_color = nullptr);
    void		 initializeFromPointGroups(RE_Render *r,
				const GA_PointGroup *group = nullptr,
				const UT_StringRef *name = nullptr,
				const UT_Color *override_color = nullptr);
    void		 initializeFromEdgeGroups(RE_Render *r,
				const GA_EdgeGroup *group = nullptr,
				const UT_StringRef *name = nullptr,
				const UT_Color *override_color = nullptr);
    void		 initializeFromVertexGroups(RE_Render *r,
				const GA_VertexGroup *group = nullptr,
				const UT_StringRef *name = nullptr,
				const UT_Color *override_color = nullptr);

    GUI_GeoRender	&myOwnerGeoRender;
    UT_StringHolder	 myGroupMask;
    GA_GroupType	 myGroupType;
    UT_StringHolder	 myAttribName;
    UT_StringHolder	 myAttribValueMask;
    GA_AttributeOwner	 myAttribOwner;
    GU_Connectivity	 myConnectivity;
    RE_Texture		*myColorTexture;

    RE_Texture		*myIdTexture;
    int			 myMinId;
    int			 myMaxId;

    RE_Geometry		*myGeometry;
};

typedef UT_IntrusivePtr<const GUI_GroupData> GUI_GroupDataHandle;

#endif
