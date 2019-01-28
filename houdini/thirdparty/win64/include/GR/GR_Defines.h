/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_Defines.h
 *
 * COMMENTS:	Drawing definitions for the render library.
 *
 */


#ifndef __GR_Defines_h__
#define __GR_Defines_h__

#include <UT/UT_VectorTypes.h>

#define	GR_DRAW_WIREFRAME	0x01
#define	GR_DRAW_SHADED		0x02

enum GR_RenderVersion
{
    GR_RENDER_UNDEF = 0,
    GR_RENDER_GL3   = 3,
    GR_RENDER_GL4   = 4
};

// public shading modes the user can see
enum GR_ShadingMode
{
    GR_BOUNDING_BOX_WIRE,
    GR_BOUNDING_BOX_SHADED,
    GR_WIRE,
    GR_WIRE_GHOST,
    GR_HIDDEN_LINE_INVISIBLE,
    GR_HIDDEN_LINE_GHOST,
    GR_FLAT_SHADED,
    GR_FLAT_WIRE_SHADED,
    GR_SMOOTH_SHADED,
    GR_SMOOTH_WIRE_SHADED,
};

// private shading modes the renderer uses
enum GR_RenderMode
{
    // Beauty modes
    GR_RENDER_BEAUTY,		// Shaded, lit
    GR_RENDER_MATERIAL,		// Shaded, material components for HQ Lighting
    GR_RENDER_CONSTANT,		// Solid, constant color/alpha

    // Wireframe modes
    GR_RENDER_WIREFRAME,	// Line outline
    GR_RENDER_HIDDEN_LINE,	// Line outline, hidden lines removed
    GR_RENDER_XRAY_LINE,	// Line outline, hidden lines dimmed
    GR_RENDER_GHOST_LINE,	// Line outline, BG color fill w/ghost color
    GR_RENDER_MATERIAL_WIREFRAME, // Line outline pass following material pass

    // Utility modes
    GR_RENDER_DEPTH,		// Solid, depth only
    GR_RENDER_DEPTH_CUBE,	// Depth only, 6 faces of a cubemap
    GR_RENDER_DEPTH_LINEAR,	// Solid, depth only, non-projected depth
    GR_RENDER_MATTE,		// Solid, constant color/alpha, depth-check
    GR_RENDER_XRAY,		// XRay Render geometry
    GR_RENDER_OBJECT_PICK,	// Object selection
    GR_RENDER_SHADER_AS_IS,	// Render the geometry without setting a shader

    // Bounding box modes
    GR_RENDER_BBOX, 		// Bounding boxes only

    GR_RENDER_POST_PASS,	// A post-pass was requested by a primitive

    GR_RENDER_NUM_BEAUTY_MODES  = GR_RENDER_CONSTANT + 1,
    GR_RENDER_NUM_UTILITY_MODES = GR_RENDER_XRAY - GR_RENDER_DEPTH + 1,
    GR_RENDER_NUM_WIRE_MODES    = GR_RENDER_GHOST_LINE - GR_RENDER_WIREFRAME + 1
};

// bitfield for modifying the basic GR_RenderMode. A flag may not apply to all
// render modes (like a FLAT_SHADED and GR_RENDER_WIREFRAME combination).
enum GR_RenderFlags
{
    GR_RENDER_FLAG_NONE			= 0x00,
    
    GR_RENDER_FLAG_FLAT_SHADED		= 0x01,
    GR_RENDER_FLAG_UNLIT		= 0x02,
    GR_RENDER_FLAG_WIRE_OVER		= 0x04,
    GR_RENDER_FLAG_FADED		= 0x08,
    GR_RENDER_FLAG_COLOR_OVERRIDE	= 0x10,
    GR_RENDER_FLAG_ALPHA_OVERRIDE	= 0x20,
    GR_RENDER_FLAG_POINTS_ONLY		= 0x40,
    GR_RENDER_FLAG_SHADED_CURVES	= 0x80,
    GR_RENDER_FLAG_WIRE_PRIMS_ONLY	= 0x100,
    GR_RENDER_FLAG_USE_SUBDIVISION	= 0x200,

    GR_RENDER_FLAG_POINT_UV_POS		= 0x400,
    GR_RENDER_FLAG_VERTEX_UV_POS	= 0x800,
    GR_RENDER_FLAG_UV_POS		= 0xC00,

    GR_RENDER_FLAG_BONE_DEFORM		= 0x01000,
    GR_RENDER_FLAG_BONE_DEFORM_SINGLE	= 0x02000,
    GR_RENDER_FLAG_BONE_DEFORM_4	= 0x04000,
    GR_RENDER_FLAG_BONE_DEFORM_8	= 0x08000,
    GR_RENDER_FLAG_BONE_DEFORM_12	= 0x10000,
    GR_RENDER_FLAG_MASK_BONE_DEFORM	= 0x1F000,

    GR_RENDER_FLAG_HULL_CLOSURE_SELECT	= 0x20000,
    GR_RENDER_FLAG_HIDE_MATERIALS	= 0x40000,
};

enum GR_AlphaPass
{
    GR_ALPHA_PASS_OPAQUE,	 // Only opaque objects
    GR_ALPHA_PASS_NORMAL,	 // All transparent objects

    GR_ALPHA_PASS_ALL		 // Ignore alpha and render all objects.
};

enum GR_ShadingPass
{
    GR_SHADING_SOLID,		 // only draw solid objects
    GR_SHADING_WIREFRAME,	 // only draw wireframe objects
    GR_SHADING_ANY		 // ignore object shading mode; draw everything.
};

enum GR_LightingPass
{
    GR_LIGHTING_UNLIT,
    GR_LIGHTING_LIT,
    GR_LIGHTING_ANY
};

enum GR_ParticleType
{
    GR_PARTICLE_POINTS,
    GR_PARTICLE_LINES,
    GR_PARTICLE_DISCS,
    GR_PARTICLE_PIXELS
};

enum GR_Decoration
{
    GR_NO_DECORATION = -1,
    
    GR_POINT_MARKER,
    GR_POINT_NUMBER,
    GR_POINT_NORMAL,
    GR_POINT_UV,
    GR_POINT_POSITION,
    GR_POINT_VELOCITY,
    GR_POINT_COINCIDENT,
    
    GR_PRIM_NORMAL,
    GR_PRIM_NUMBER,
    GR_PRIM_HULL,
    GR_PRIM_BREAKPOINT,
    GR_PRIM_PROFILE,
    GR_PRIM_PROFILE_NUMBER,

    GR_VERTEX_MARKER,
    GR_VERTEX_NORMAL,
    GR_VERTEX_NUMBER,
    GR_VERTEX_UV,
    
    GR_MAX_DECORATIONS, // max number of builtin decorations
    
    GR_USER_DECORATION = GR_MAX_DECORATIONS,   // deprecated
    GR_VISUALIZER_MARKER = GR_USER_DECORATION+1
};

enum GR_DecorVisibility
{
    GR_DECOR_ALWAYS_VISIBLE,
    GR_DECOR_SELECTED_ONLY,
    GR_DECOR_LOCAL_AREA,
    GR_DECOR_HOVER
};

enum GR_DecorFontSize
{
    GR_DECOR_FONT_TINY = 0,
    GR_DECOR_FONT_SMALL,
    GR_DECOR_FONT_MEDIUM,
    GR_DECOR_FONT_LARGE,

    GR_DECOR_FONT_NUM_SIZES
};

enum GR_DecorFontType
{
    GR_DECOR_FONT_FULL      =0,
    GR_DECOR_FONT_FP_DIGITS,
    GR_DECOR_FONT_INT_DIGITS,

    GR_DECOR_FONT_NUM_TYPES
};

enum GR_DecorationOverride
{
    GR_DECOR_OVERRIDE_NONE	 = 0x0,
    
    GR_DECOR_OVERRIDE_POINT	 = 0x1,
    GR_DECOR_OVERRIDE_VERTEX	 = 0x2,
    GR_DECOR_OVERRIDE_BREAKPOINT = 0x4,
    GR_DECOR_OVERRIDE_EDGE	 = 0x8,
    GR_DECOR_OVERRIDE_VELOCITY	 = 0x10
};

enum GR_DecorRenderFlags
{
    GR_DECOR_RENDER_FLAG_NONE			= 0x0,
    GR_DECOR_RENDER_FLAG_DIMMED			= 0x1,
    GR_DECOR_RENDER_FLAG_ACTIVE_INSTANCE_ONLY	= 0x2,
    GR_DECOR_RENDER_FLAG_POINT_UV_POS		= 0x10,
    GR_DECOR_RENDER_FLAG_VERTEX_UV_POS		= 0x20,
    GR_DECOR_RENDER_FLAG_UV_POS			= 0x30
};

enum GR_SelectMode
{
    GR_SELECT_NONE		= 0,
    GR_SELECT_PRIM_PARTIAL	= 1,
    GR_SELECT_PRIM_FULL		= 2,
    GR_SELECT_POINT_PARTIAL	= 3,
    GR_SELECT_POINT_FULL	= 4,
    GR_SELECT_VERTEX_PARTIAL	= 5,
    GR_SELECT_VERTEX_FULL	= 6
};
    
enum GR_PickStyle
{
    GR_PICK_NONE			= 0x0000,

    // For single picking from a buffer.
    GR_PICK_SINGLE			= 0x0001,

    // For multi-component picking.
    GR_PICK_MULTI_VISIBLE		= 0x0010,
    GR_PICK_MULTI_FRUSTUM		= 0x0020,
    GR_PICK_MULTI_FLAG			= 0x0030,

    // For picking in UV viewports.
    GR_PICK_POINT_UV_FLAG		= 0x0100,
    GR_PICK_VERTEX_UV_FLAG		= 0x0200,
    GR_PICK_UV_FLAG			= 0x0300,

    // Indicates we are picking to do snapping.
    GR_PICK_SNAP_FLAG			= 0x1000,
    // Don't allow picking of selected components (mostly to avoid snapping
    // to geometry that is being moved).
    GR_PICK_HIDE_SELECTED		= 0x2000,
    // Invert the sense of the box/lasso/paint pick (outside is selected)
    GR_PICK_OUTSIDE			= 0x4000,
    // As if the surface was wireframe
    GR_PICK_AS_WIRE			= 0x8000,

    // As if the surface was one solid shape
    GR_PICK_PACKED			= 0x10000,

    // OpenGL 4.2 (GL_ARB_image_load_store) based picking.
    GR_PICK_GL42			= 0x1000000
};

class GR_ObjectPickFlags
{
public:
		 GR_ObjectPickFlags()
		     : myUseObjectNodes(true),
		       myPickNonSelectable(false),
		       myPickTemplates(false),
		       myPickGhosted(true),
		       myPickXray(false)
		 { }

    // Controls whether details must be associated with object nodes.  If false,
    // any displayed detail is considered an "object" for picking purposes.
    bool	 myUseObjectNodes;

    bool	 myPickNonSelectable;
    bool	 myPickTemplates;
    bool	 myPickGhosted;
    bool	 myPickXray;
};

// bitfield for why GR_Primitive::update() was called.
// be sure to update GUI_RenderCommon.h:GUI_SetupResult if the bitfield gets
// larger than 0x8000.
enum GR_UpdateReason
{
    GR_UNCHANGED			= 0x0,		
    
    GR_GEO_CHANGED			= 0x001,
    GR_GEO_TOPOLOGY_CHANGED		= 0x002,
    GR_GEO_ATTRIB_LIST_CHANGED		= 0x004,
    GR_GEO_SELECTION_CHANGED		= 0x008,
    GR_GEO_DETAIL_SWITCHED		= 0x010,
    GR_GEO_PRIMITIVE_CHANGED		= 0x020,
    GR_DISPLAY_OPTIONS_CHANGED		= 0x040,
    GR_OBJECT_MODE_CHANGED		= 0x080,
    GR_INSTANCE_PARMS_CHANGED		= 0x100,
    GR_INSTANCE_SELECTION_CHANGED	= 0x200,
    GR_GL_STATE_CHANGED			= 0x400,
    GR_GL_VIEW_CHANGED			= 0x800,
    GR_LOD_CHANGED			= 0x1000,
    GR_MATERIALS_CHANGED		= 0x2000,
    GR_MATERIAL_ASSIGNMENT_CHANGED	= 0x4000,
    GR_GEO_VISIBILITY_RESTORED		= 0x8000,
    GR_VISUALIZER_NEEDS_REFINE		= 0x10000,
    GR_VISUALIZER_NEEDS_UPDATE		= 0x20000,
    GR_VIEWPORT_CONFIG_CHANGED		= 0x40000
};

enum GR_AttribMask
{
    GR_NO_ATTRIBS	 = 0x0,
	
    GR_VERTEX_ATTRIB     = 0x1,
    GR_POINT_ATTRIB      = 0x2,
    GR_PRIMITIVE_ATTRIB  = 0x4,
    GR_DETAIL_ATTRIB	 = 0x8,

    // These don't strictly exist as attribute types,
    // but are useful for selections.
    GR_EDGE_ATTRIB	 = 0x10,
    GR_BREAKPOINT_ATTRIB = 0x20,

    GR_ALL_ATTRIBS = 0xFFFFFFFF
};

enum GR_PrimAcceptResult
{
    GR_NOT_PROCESSED,
    GR_PROCESSED,
    GR_PROCESSED_NON_EXCLUSIVE
};

enum GR_BasePrimType
{
    GR_BASE_PRIM_NORMAL,	// most primitives are normal prim types.
    
    GR_BASE_PRIM_PACKED,	// container primitives
    GR_BASE_PRIM_PACKED_MESH,
    GR_BASE_PRIM_INSTANCE,
    GR_BASE_PRIM_FRAGMENT,
    GR_BASE_PRIM_AGENT_SHAPE,
    
    GR_BASE_PRIM_VISUALIZE	// visualizer output
};

enum GR_BoundaryMode
{
    GR_BOUNDARY_NONE = 0,
    GR_BOUNDARY_VIEW_3D = 1,
    GR_BOUNDARY_VIEW_UV = 2,
    GR_BOUNDARY_VIEW_ALL = 3
};

enum GR_AgentWireMode
{
    GR_AGENT_WIRE_LINE = 0,
    GR_AGENT_WIRE_BONE = 1
};

#include <UT/UT_Array.h>
#include <UT/UT_Pair.h>
typedef UT_Array<UT_Pair<UT_Matrix4D, UT_Vector2i> > GR_FrustumList;

#endif
