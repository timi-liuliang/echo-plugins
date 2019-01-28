/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_DecorationRender.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Base class for decoration rendering
 */
#ifndef GR_DecorationRender_h
#define GR_DecorationRender_h

class RE_Render;
class RE_Geometry;
class RE_ShaderRegistry;
class GU_Detail;
class GR_DisplayOption;
class GT_Primitive;

#include <SYS/SYS_Types.h>
#include <UT/UT_Color.h>
#include <UT/UT_BitArray.h>
#include <RE/RE_Texture.h>

#include <GA/GA_Types.h>
#include <GT/GT_Types.h>

#include "GR_API.h"
#include "GR_Defines.h"
#include "GR_RenderInfo.h"
#include "GR_UpdateParms.h"

#define GR_DECOR_OPT_VECTOR_SCALE		"vector_scale"
#define GR_DECOR_OPT_VECTOR_ARROW_TIP		"vector_arrow_tip"
#define GR_DECOR_OPT_POINT_SCALE		"point_scale"
#define GR_DECOR_OPT_COLOR			"color"
#define GR_DECOR_OPT_SELECT_COLOR		"select_color"
#define GR_DECOR_OPT_FONT_SIZE			"font_size"
#define GR_DECOR_OPT_COLOR_ATTRIB		"color_attrib"
#define GR_DECOR_OPT_NORMALIZE			"normalize"
#define GR_DECOR_OPT_COLOR_XFORM		"color_xform"
#define GR_DECOR_OPT_COLOR_XFORM_REF_VEC	"color_xform_ref_vec"

class GR_VisualizerInfo;

class GR_API GR_DecorationParms
{
public:
    GR_DecorationParms(const GR_DisplayOption *opt,
		       bool over,
		       bool over_vis,
		       int inst_grp,
		       GR_DecorRenderFlags flags)
	: opts(opt),
	  overlay(over),
	  override_vis(over_vis),
	  instance_group(inst_grp),
	  render_flags(flags),
	  visualizer(NULL){ }

    const GR_DisplayOption	*opts;
    bool			 overlay;
    bool			 override_vis;
    int				 instance_group;
    GR_DecorRenderFlags		 render_flags;
    const GR_VisualizerInfo	*visualizer;
};




class GR_API GR_DecorationRender
{
public:
			 GR_DecorationRender(const GR_RenderInfo *info);
    virtual		~GR_DecorationRender();

    // Initializes all the decorations from the given display options.
    // 'select_dec'     is a bitfield of display option overrides (point marker,
    //			breakpoint marker, vertex marker)
    // 'parms.needed_attribs' is modified so that any attributes a supported
    //			decoration migght need is appended to the list.
    // 'supported'	is a NULL terminated list of decorations supported by
    //			the primitive. (NULL is 0 or GR_NO_DECORATION)
    void		 setupFromDisplayOptions(const GR_DisplayOption &opts,
				GR_DecorationOverride select_dec,
				GR_UpdateParms &parms,
				const GR_Decoration *supported,
				GR_AttribMask mask);

    // Update the geometry required to produce the active decorations. This may
    // add new buffers to geo (normals, UV, etc).
    bool		 updateDecoration(RE_Render *r,
				GR_Decoration dec,
				const GT_Primitive &prim,
				const GR_UpdateParms &p,
				RE_Geometry &geo);
    
    // Returns true if this decoration renderer supports mouse-motion visibility
    // modes, and one of these modes is active
    bool		 hasOverlayDecorations() const
			 { return myHasOverlayDecorations; }

    enum RenderTarget
    {
	TARGET_MAIN,
	TARGET_OVERLAY
    };

    enum PrimitiveType
    {
	PRIM_TRIANGLE,
	PRIM_MESH,
	PRIM_LINE,
	PRIM_POINT,
	PRIM_TET,
	PRIM_TYPE_COUNT
    };

    // Called before any calls to drawSingle.
    void		 setupDrawing(RE_Render *r);

    // Draw a single decoration. The visibility affects which framebuffer is
    // currently being drawn (main or 
    void		 drawSingle(RE_Render *r,
				RE_Geometry &geo,
				GR_Decoration dec,
				const GR_DisplayOption *opt,
				GR_DecorRenderFlags flags,
				GR_SelectMode select,
				RenderTarget render_mode,
				GR_DecorVisibility *vis_override,
				PrimitiveType glprim_type,
				int instance_group,
				int select_inst,
				RE_OverrideList *override_list);

    // custom decorations
    void		 drawSingleVisualizer(RE_Render *r,
					  RE_Geometry &geo,
                                          const GR_VisualizerInfo *dec_info,
                                          const GR_DisplayOption *opt,
					  GR_DecorRenderFlags flags,
                                          PrimitiveType prim_type);

    static RE_Texture	*getViewportFontTexture(RE_Render *r,
				GR_DecorFontSize s,
				GR_DecorFontType t,
				bool outlined,
				UT_Vector3 &font_parms);

    /// Returns a 512-entry 1D texture.
    static RE_Texture  *getRampLookupTexture(RE_Render *r,
				UT_ColorRamp rtype);

private:
    // Description of user decorations.
    class GR_UserDecorInfo
    {
    public:
		 GR_UserDecorInfo()
		     : myCustomShaderRegistry(NULL),
		       myUserOption(NULL),
		       myAttribSize(0),
		       myIsFP(false),
		       myIsString(false)
		 { }
		 GR_UserDecorInfo(const RE_ShaderRegistry *shader_registry,
				  const GR_UserOption *user_option,
				  int attrib_size,
				  bool is_fp,
				  bool is_string)
		     : myCustomShaderRegistry(shader_registry),
		       myUserOption(user_option),
		       myAttribSize(attrib_size),
		       myIsFP(is_fp),
		       myIsString(is_string)
		 { }

	const RE_ShaderRegistry	*myCustomShaderRegistry;
	const GR_UserOption	*myUserOption;
	int			 myAttribSize;
	bool			 myIsFP;
	bool			 myIsString;
    };

    // update a single decoration
    bool		 update(RE_Render *r,
				GR_Decoration dec,
				const GT_Primitive &prim,
				const GR_UpdateParms &p,
				RE_Geometry &geo);

    // draw a single decoration
    void		 draw(RE_Render *r,
				RE_Geometry &geo,
				GR_Decoration dec,
				GR_DecorVisibility visibility,
				const GR_DisplayOption *opt,
				GR_DecorRenderFlags flags,
				GR_SelectMode select,
				PrimitiveType prim_type,
				int instance_group,
				int select_inst,
				RE_OverrideList *override_list);

    static RE_Texture	*getViewportBoxTexture(RE_Render *r,
				GR_DecorFontSize s,
				bool pointed);

    static RE_Texture	*getViewportVertexTexture(RE_Render *r);

    bool		 showSelections() const
			 { return myRenderInfo->showSelections(); }
    bool 		 getDecorationEnabled(GR_Decoration dec)
			 { return myDecorEnabled[dec]; }

    const RE_ShaderRegistry	*getUserDecorShaderRegistry(int decor) const;
    const GR_UserOption		*getUserOption(int decor) const;
    int				 getUserDecorAttribSize(int decor) const;
    bool			 isFPUserDecor(int decor) const;
    bool			 isStringUserDecor(int decor) const;

    void		 getUserOptionColors(const GR_UserOption *user,
				const GR_DisplayOption *opt,
				UT_Color &col, UT_Color &select);

    RE_Shader		*setupDecorationParms(RE_Render *r,
				RE_Geometry &geo,
				GR_Decoration dec,
				GR_DecorVisibility vis,
				GR_DecorRenderFlags flags,
				const GR_DisplayOption *opt,
				RE_PrimType &type,
				fpreal32 color[4],
				fpreal32 select_color[4],
				fpreal32 &scale,
				int &npasses,
				RE_OverrideList &attrib_override,
				PrimitiveType prim_type);
    void		 cleanupDecorationParms(RE_Render *r,
				GR_Decoration dec);

    void		 setupShaderDepthTest(RE_Render *r, RE_Shader *sh,
				bool zwrites);
    void		 setupFontParms(RE_Render *r,
				RE_Shader *sh,
				const GR_DisplayOption *opt,
				GR_DecorFontType type,
				bool pointed_box,
				GR_DecorFontSize size = GR_DECOR_FONT_NUM_SIZES);
    void		 setupVertexColors(RE_Render *r, RE_Shader *sh);
    void		 cleanupFontParms(RE_Render *r);

    // visualizer-based decorations
    void		 drawVisualizer(RE_Render *r,
					RE_Geometry &geo,
					const GR_VisualizerInfo *dec_info,
					const GR_DisplayOption *opt,
					GR_DecorRenderFlags flags,
					PrimitiveType prim_type);

    RE_Shader *		 setupVisualizerDraw(RE_Render *r,
                                         RE_Geometry &geo,
                                         const GR_VisualizerInfo *dec_info,
                                         const GR_DisplayOption *opt,
					 GR_DecorRenderFlags flags,
                                         PrimitiveType prim_type,
                                         RE_PrimType &re_prim,
                                         int &npasses,
                                         RE_OverrideList &overrides);

    void		 cleanupVisualizerDraw(RE_Render *r,
        		                   const GR_VisualizerInfo *dec_info);
    // cached option data
    bool				 myCanDrawDecor[GR_MAX_DECORATIONS];
    UT_Color				 myPointColor;
    UT_Color				 mySelectedPointColor;
    UT_Color				 myPrimColor;
    UT_Color				 mySelectedPrimColor;
    fpreal				 myNormalScale;
    fpreal				 myPointSize;
    fpreal				 myVectorScale;

    // Keep user option counts separate for primitive, point and vertex attribs.
    int					 myUserStringCount[3];
    int					 myUserStringIndex[3];
    
    bool				 myDecorEnabled[GR_MAX_DECORATIONS];
    bool				 myHasDecorations;
    bool				 myHasOverlayDecorations;
    const GR_RenderInfo			*myRenderInfo;

    UT_Map<int, GR_UserDecorInfo>	 myUserDecorMap;
    static UT_Map<int, GR_UserDecorInfo> theStandardDecorMap;
};


// Structure used for passing various specifications of a custom
// decoration rendering down the rendering chain

enum GR_VisualizerDecorStyle
{
    GR_VISUALIZER_DECOR_TEXT,
    GR_VISUALIZER_DECOR_INDEX,
    GR_VISUALIZER_DECOR_POINT,
    GR_VISUALIZER_DECOR_VECTOR,
    GR_VISUALIZER_DECOR_TRAIL
};

class GR_API GR_VisualizerInfo
{
public:
    GR_VisualizerInfo() :
	myDecorAttribOwner(GA_ATTRIB_POINT),
	myDecorAttribSize(1),
	myDecorAttribStorage(GT_STORE_INT32),
	myStyle(GR_VISUALIZER_DECOR_TEXT),
	mySelectMode(GR_SELECT_NONE),
	myVisibility(GR_DECOR_ALWAYS_VISIBLE),
	myOptions(0),
	myOwnerId(-1)
    {}

    GR_VisualizerInfo(GA_AttributeOwner dec_attrib_owner,
                 int dec_attrib_size,
                 const char *dec_attrib,
                 GT_Storage dec_attrib_store,
                 bool overlay = false,
                 GR_VisualizerDecorStyle style = GR_VISUALIZER_DECOR_TEXT,
                 GR_SelectMode select_mode = GR_SELECT_NONE,
                 GR_DecorVisibility visibility = GR_DECOR_ALWAYS_VISIBLE,
		 int owner_id = -1) :
        myDecorAttribOwner(dec_attrib_owner),
        myDecorAttribSize(dec_attrib_size),
        myDecorAttribStorage( dec_attrib_store),
        myStyle(style),
        mySelectMode(select_mode),
        myVisibility(visibility),
        myOptions(0),
	myOwnerId(owner_id)
    {
	if (dec_attrib)
	    myDecorAttrib.harden(dec_attrib);
    }

    GR_VisualizerInfo &
    operator=(const GR_VisualizerInfo &src)
    {
	myDecorAttribOwner = src.myDecorAttribOwner;
	myDecorAttribSize = src.myDecorAttribSize;

	myDecorAttrib.harden(src.myDecorAttrib);
	myMaskAttrib.harden(src.myMaskAttrib);

	myStyle = src.myStyle;
	mySelectMode = src.mySelectMode;
	myVisibility = src.myVisibility;
	myDecorAttribStorage = src.myDecorAttribStorage;

	myOptions = src.myOptions;
	return *this;
    }

    GA_AttributeOwner 	 getDecorAttribOwner() const
			    { return myDecorAttribOwner; }
    void		 setDecorAttribOwner(GA_AttributeOwner owner)
			    { myDecorAttribOwner = owner; }
    const char 		*getDecorAttrib() const
			    { return myDecorAttrib.length() ? myDecorAttrib : 0; }
    void		 setDecorAttrib(const char *attrib)
			    { myDecorAttrib.harden(attrib); }
    int			 getDecorAttribSize() const
			    { return myDecorAttribSize; }
    void		 setDecorAttribSize(int size)
			    { myDecorAttribSize = size; }
    GT_Storage		 getDecorAttribStorage() const
			    { return myDecorAttribStorage; }
    void		 setDecorAttribStorage(GT_Storage store)
			    { myDecorAttribStorage = store; }
    const char 		*getMaskAttrib() const
			    { return myMaskAttrib.length() ? myMaskAttrib : 0; }
    void		 setMaskAttrib(const char *attrib)
			    { myMaskAttrib.harden(attrib); }
    GR_VisualizerDecorStyle  getStyle() const
			    { return myStyle; }
    void		 setStyle(GR_VisualizerDecorStyle style)
			    { myStyle = style; }
    GR_SelectMode 	 getSelectMode() const
			    { return mySelectMode; }
    GR_DecorVisibility   getVisibility() const
			    { return myVisibility; }
    void		 setVisibility(GR_DecorVisibility vis)
			    { myVisibility = vis; }

    void		 attachOptions(UT_Options *opts) { myOptions = opts; }
    void		 destroyOptions() { if (myOptions) delete myOptions; }
    const UT_Options	*getOptions()
			    const { return myOptions; }


    /// If an owner id other than -1 is set, then passing this object to
    /// GUI_DecorationRender::customDecorationRender() will only cause
    /// decoration rendering on VIS_Primitives created by the visualizer
    /// with the given unique id. This way, a visualizer can trigger rendering
    /// of decorations only on primitives it itself has created.

    void		 setOwnerId(int owner_id) { myOwnerId = owner_id; }
    int			 getOwnerId() const { return myOwnerId; }
    
private:
    GA_AttributeOwner 	 myDecorAttribOwner;	// attribute class
    int			 myDecorAttribSize;	// num attrib components

    UT_String		 myDecorAttrib;		// name attribute itself
    UT_String		 myMaskAttrib;		// boolean (uint8) group mask

    GR_VisualizerDecorStyle myStyle;
    GR_SelectMode 	 mySelectMode;
    GR_DecorVisibility 	 myVisibility;
    GT_Storage		 myDecorAttribStorage;

    UT_Options		*myOptions;
    int 		 myOwnerId;		// See above!
};

#endif
