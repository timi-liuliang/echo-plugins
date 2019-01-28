/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The SlideModifierPaint SOP, which is used to paint a per-point
 *	     sliding attribute for use in muscle sliding
 *
 */

#ifndef __SOP_SlideModifierPaint_h__
#define __SOP_SlideModifierPaint_h__

#include "SOP_API.h"
#include <GU/GU_Brush.h>
#include <GEO/GEO_Detail.h>

#include "SOP_GDT.h"
#include "SOP_BrushBase.h"

class PRM_MenuEntry;

enum {
    // Group parameters
    SOP_SLIDEPAINT_GRP_IDX = SOP_GDT_GRP_IDX,

    SOP_SLIDEPAINT_SWITCHER_IDX,

#define SOP_SLIDEPAINT_NUM_DEFORM_PARMS	(10)
    SOP_SLIDEPAINT_ATTRIB_IDX,
    SOP_SLIDEPAINT_MERGEMODE_IDX,
    SOP_SLIDEPAINT_STENCILACCUM_IDX,
    SOP_SLIDEPAINT_STENCILAPPLY_IDX,
    SOP_SLIDEPAINT_FLOODAPPLY_IDX,
    SOP_SLIDEPAINT_OP_IDX,
    SOP_SLIDEPAINT_CLEARALL_IDX,
    SOP_SLIDEPAINT_VISUALIZE_IDX,
    SOP_SLIDEPAINT_VISRANGE_IDX,
    SOP_SLIDEPAINT_VISMODE_IDX,

#define SOP_SLIDEPAINT_NUM_NIB_PARMS (18)
    SOP_SLIDEPAINT_SHAPE_IDX,
    SOP_SLIDEPAINT_BITMAP_IDX,
    SOP_SLIDEPAINT_BITMAPCHAN_IDX,
    SOP_SLIDEPAINT_RAD_IDX,
    SOP_SLIDEPAINT_UVRAD_IDX,
    SOP_SLIDEPAINT_RADPRESSURE_IDX,
    SOP_SLIDEPAINT_USEDEPTH_IDX,
    SOP_SLIDEPAINT_DEPTH_IDX,
    SOP_SLIDEPAINT_ANGLE_IDX,
    SOP_SLIDEPAINT_SQUASH_IDX,
    SOP_SLIDEPAINT_OPAC_IDX,
    SOP_SLIDEPAINT_OPACPRESSURE_IDX,
    SOP_SLIDEPAINT_SPLATTER_IDX,
    SOP_SLIDEPAINT_PAPER_IDX,
    SOP_SLIDEPAINT_SOFTEDGE_IDX,
    SOP_SLIDEPAINT_KERNEL_IDX,
    SOP_SLIDEPAINT_UPTYPE_IDX,
    SOP_SLIDEPAINT_UPVECTOR_IDX,

#define SOP_SLIDEPAINT_NUM_SYM_PARMS (9)
    SOP_SLIDEPAINT_DOREFLECT_IDX,
    SOP_SLIDEPAINT_DOROTATION_IDX,
    SOP_SLIDEPAINT_SYMAXIS_IDX,
    SOP_SLIDEPAINT_SYMORIG_IDX,
    SOP_SLIDEPAINT_SYMROT_IDX,
    SOP_SLIDEPAINT_SYMDIST_IDX,
    SOP_SLIDEPAINT_DOUVREFLECT_IDX,
    SOP_SLIDEPAINT_SYMUVORIG_IDX,
    SOP_SLIDEPAINT_SYMUVANGLE_IDX,

#define SOP_SLIDEPAINT_NUM_RAY_PARMS	(11)
    SOP_SLIDEPAINT_PROJTYPE_IDX,
    SOP_SLIDEPAINT_USECONNECTIVITY_IDX,
    SOP_SLIDEPAINT_USENORMALS_IDX,
    SOP_SLIDEPAINT_REALTIME_IDX,
    SOP_SLIDEPAINT_RAY_ORIENT_IDX,
    SOP_SLIDEPAINT_RAY_HITPOS_IDX,
    SOP_SLIDEPAINT_RAY_HITPRIM_IDX,
    SOP_SLIDEPAINT_RAY_HITUVW_IDX,
    SOP_SLIDEPAINT_RAY_HITPRESSURE_IDX,
    SOP_SLIDEPAINT_RAY_HITPT_IDX,
    SOP_SLIDEPAINT_RAY_EVENT_IDX
};

class SOP_API SOP_SlideModifierPaint : public SOP_BrushBase
{
public:
	     SOP_SlideModifierPaint(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_SlideModifierPaint();

    virtual bool		 updateParmsFlags() override;

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];
    static PRM_Template		 myObsoleteList[];

    virtual void setAccumulateStencil(bool yesno) override
		 {
		     setInt(SOP_SLIDEPAINT_STENCILACCUM_IDX, 0, 0, yesno);
		 }
    virtual void setRayOrigin(const UT_Vector3 &orig, fpreal t) override
		 {
		     setFloat(SOP_SLIDEPAINT_RAY_HITPOS_IDX, 0, t, orig.x());
		     setFloat(SOP_SLIDEPAINT_RAY_HITPOS_IDX, 1, t, orig.y());
		     setFloat(SOP_SLIDEPAINT_RAY_HITPOS_IDX, 2, t, orig.z());
		 }
    virtual void setRayOrientation(const UT_Vector3 &orient, fpreal t) override
		 {
		     setFloat(SOP_SLIDEPAINT_RAY_ORIENT_IDX, 0, t, orient.x());
		     setFloat(SOP_SLIDEPAINT_RAY_ORIENT_IDX, 1, t, orient.y());
		     setFloat(SOP_SLIDEPAINT_RAY_ORIENT_IDX, 2, t, orient.z());
		 }
    virtual void setHitPrimitive(int primidx, fpreal t) override
		 {
		     setInt(SOP_SLIDEPAINT_RAY_HITPRIM_IDX, 0, t, primidx);
		 }
    virtual void setHitPoint(int pt, fpreal t) override
		 {
		     setInt(SOP_SLIDEPAINT_RAY_HITPT_IDX, 0, t, pt);
		 }
    virtual void setHitUVW(fpreal u, fpreal v, fpreal w, fpreal t) override
	 	 {
		     setFloat(SOP_SLIDEPAINT_RAY_HITUVW_IDX, 0, t, u);
		     setFloat(SOP_SLIDEPAINT_RAY_HITUVW_IDX, 1, t, v);
		     setFloat(SOP_SLIDEPAINT_RAY_HITUVW_IDX, 2, t, w);
		 }
    virtual void setHitPressure(fpreal p, fpreal t) override
		 {
		     setFloat(SOP_SLIDEPAINT_RAY_HITPRESSURE_IDX, 0, t, p);
		 }   
    virtual void setRadius(fpreal r, fpreal t) override
		 {
		     setFloat(SOP_SLIDEPAINT_RAD_IDX, 0, t, r);
		 }
    virtual void setUVRadius(fpreal r, fpreal t) override
		 {
		     setFloat(SOP_SLIDEPAINT_UVRAD_IDX, 0, t, r);
		 }
    virtual void setBrushEvent(SOP_BrushEvent event) override
		 {
		     setInt(SOP_SLIDEPAINT_RAY_EVENT_IDX, 0, 0, int(event));
		 }
    virtual void setBrushOp(SOP_BrushOp op) override
		 {
		     int iop;
		     switch (op)
		     {
			 case SOP_BRUSHOP_SMOOTHATTRIB:	iop = 1; break;
			 case SOP_BRUSHOP_ERASE:	iop = 2; break;
			 case SOP_BRUSHOP_PAINT:
			 default:			iop = 0; break;
		     }
		     setInt(SOP_SLIDEPAINT_OP_IDX, 0, 0, iop);
		 }
    virtual void setBrushShape(SOP_BrushShape shape) override
		 {
		     setInt(SOP_SLIDEPAINT_SHAPE_IDX, 0, 0, int(shape));
		 }
    virtual void setProjectionType(int projtype) override
		 {
		     setInt(SOP_SLIDEPAINT_PROJTYPE_IDX, 0, 0, projtype);
		 }
    virtual void useForegroundColor(void) override
		 { myUseFG = true; }
    virtual void useBackgroundColor(void) override
		 { myUseFG = false; }

    virtual void setCurrentColor(const UT_Vector3 &cd) override
		{ }
    virtual void setVisualization(int state) override
		{
		    // only trigger recooks if different!
		    if( myForceVisualize != state )
		    {
			myForceVisualize = state;
			forceRecook();
		    }
		}

protected:
    virtual const char	*inputLabel(unsigned idx) const override;

    // Override this function, so that we can recreate the
    // slideModifier attribute every time the source is
    // duplicated
    virtual void	 duplicateChangedSourceWrapper(
						unsigned idx,
						OP_Context &context,
						int *changed,
						bool force=false) override;

protected:
    virtual void	doErase() override;

public:
    virtual int		altersGeometry() const override { return 0; }
    virtual int		altersColor() const override { return 1; }
    virtual fpreal	RAWRADIUS(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAD_IDX, 0, t); }
    virtual fpreal	RAWUVRADIUS(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_UVRAD_IDX, 0, t); }
    virtual fpreal	RADIUSPRESSURE(fpreal t) override
			{ return evalFloat(SOP_SLIDEPAINT_RADPRESSURE_IDX, 0, t); }
    virtual fpreal	RAWOPACITY(fpreal t) override
			    {return evalFloat(SOP_SLIDEPAINT_OPAC_IDX, 0, t) ; }
    virtual fpreal	OPACITYPRESSURE(fpreal t) override
			{return evalFloat(SOP_SLIDEPAINT_OPACPRESSURE_IDX, 0, t) ; }

    virtual SOP_BrushOp	 OP(void) override 
			{ 
			    switch (evalInt(SOP_SLIDEPAINT_OP_IDX, 0, 0))
			    {
				case 0:
				    return SOP_BRUSHOP_PAINT;
				case 1: 
				    return SOP_BRUSHOP_SMOOTHATTRIB;
				case 2: 
				default:
				    return SOP_BRUSHOP_ERASE;
			    }
			}

    virtual int		ACCUMSTENCIL(void) override 
			    { return evalInt(SOP_SLIDEPAINT_STENCILACCUM_IDX, 0, 0);}
    virtual int		PROJECTIONTYPE(void) override
			    { return evalInt(SOP_SLIDEPAINT_PROJTYPE_IDX, 0, 0); }
    virtual int		REALTIME(void) override
			    { return evalInt(SOP_SLIDEPAINT_REALTIME_IDX, 0, 0); }
    virtual int		SHAPE(fpreal t) override
			    { return evalInt(SOP_SLIDEPAINT_SHAPE_IDX,  0, t); }

protected:
    enum AttribToPaint {
	SOP_SLIDE_MODIFIER,
	SOP_INFLATE_MODIFIER
    };

    virtual int		MERGEMODE() override
			    { return evalInt(SOP_SLIDEPAINT_MERGEMODE_IDX, 0, 0); }
    virtual void	SCRIPT 	(UT_String &, fpreal  )  override{}
    virtual int		AXIS(void) override	{ return   0 ;  }
    virtual fpreal	USERX(fpreal) override	{ return 0.0;  }
    virtual fpreal	USERY(fpreal) override	{ return 0.0;  }
    virtual fpreal	USERZ(fpreal) override	{ return 0.0;  }

    virtual fpreal	RAYORIENTX(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_ORIENT_IDX,0,t); }
    virtual fpreal	RAYORIENTY(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_ORIENT_IDX,1,t); }
    virtual fpreal	RAYORIENTZ(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_ORIENT_IDX,2,t); }
    virtual fpreal	RAYHITX(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_HITPOS_IDX,0,t); }
    virtual fpreal	RAYHITY(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_HITPOS_IDX,1,t); }
    virtual fpreal	RAYHITZ(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_HITPOS_IDX,2,t); }
    virtual fpreal	RAYHITU(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_HITUVW_IDX, 0,t); }
    virtual fpreal	RAYHITV(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_HITUVW_IDX, 1,t); }
    virtual fpreal	RAYHITW(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_HITUVW_IDX, 2,t); }
    virtual fpreal	RAYHITPRESSURE(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_RAY_HITPRESSURE_IDX, 0, t); }
    virtual int		PRIMHIT(fpreal t) override
			    { return evalInt(SOP_SLIDEPAINT_RAY_HITPRIM_IDX, 0,t); }
    virtual int		PTHIT(fpreal t) override
			    { return evalInt(SOP_SLIDEPAINT_RAY_HITPT_IDX, 0, t); }
    virtual int		EVENT() override
			    { return evalInt(SOP_SLIDEPAINT_RAY_EVENT_IDX,   0,0); }
    virtual bool	USE_FOREGROUND(void) override
			    { return myUseFG; }

    virtual fpreal FGR(fpreal t) override   { return 1.0; }
    virtual fpreal FGG(fpreal t) override   { return 1.0; }
    virtual fpreal FGB(fpreal t) override   { return 1.0; }
    virtual fpreal BGR(fpreal t) override   { return 0.0; }
    virtual fpreal BGG(fpreal t) override   { return 0.0; }
    virtual fpreal BGB(fpreal t) override   { return 0.0; }

    virtual fpreal	SOFTEDGE(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_SOFTEDGE_IDX, 0, t); }
    virtual void	KERNEL(UT_String &str, fpreal t) override 
			    { evalString(str, SOP_SLIDEPAINT_KERNEL_IDX, 0, t); }
    virtual int		UPTYPE(fpreal t) override
			    { return evalInt(SOP_SLIDEPAINT_UPTYPE_IDX, 0, t); }
    virtual fpreal	UPX(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_UPVECTOR_IDX, 0, t); }
    virtual fpreal	UPY(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_UPVECTOR_IDX, 1, t); }
    virtual fpreal	UPZ(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_UPVECTOR_IDX, 2, t); }

    virtual fpreal	PAPERNOISE(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_PAPER_IDX, 0, t); }
    virtual fpreal	SPLATTER(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_SPLATTER_IDX, 0, t); }
    virtual void	BITMAP(UT_String &str, fpreal t) override
			    { evalString(str, SOP_SLIDEPAINT_BITMAP_IDX, 0, t); }
    virtual int		BITMAPCHAN(fpreal t) override
			    { return evalInt(SOP_SLIDEPAINT_BITMAPCHAN_IDX, 0, t); }
    virtual fpreal	ANGLE(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_ANGLE_IDX,  0, t); }
    virtual fpreal	SQUASH(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_SQUASH_IDX, 0, t); }
    virtual int		DOSTAMPING() override
			    { return 0; }
    virtual int		WRITEALPHA() override
			    { return 0; }

    virtual int		OVERRIDECD() override
			    { return 1; }   // Always override an attribute

    // Chose the attribute name to override.  We base this on the current
    // value in the 'Attribute to Paint' menu.  Ideally, it should be
    // straightforward to add additional muscle-related attributes to
    // paint by just adding to the menu and setting the attribute to
    // paint accordingly.
    virtual void	CDNAME(UT_String &str, fpreal t) override
			    {
				int attrib_to_paint;

				attrib_to_paint
				    = evalInt(SOP_SLIDEPAINT_ATTRIB_IDX, 0, 0);

				switch ( attrib_to_paint )
				{
				    case SOP_SLIDE_MODIFIER:
				    {
					str = GEO_STD_ATTRIB_META_SLIDE;
					break;
				    }
				    case SOP_INFLATE_MODIFIER:
				    {
					str = GEO_STD_ATTRIB_INFLATE_MODIFIER;
					break;
				    }
				    default:
				    {
					str = GEO_STD_ATTRIB_META_SLIDE;
					break;
				    }
				}
			    }

    virtual int		OVERRIDEALPHA() override
			    { return 0; }
    virtual void	ALPHANAME(UT_String & /*str*/, fpreal /*t*/) override
			    {}
    virtual int		OVERRIDENML() override { return 0; }
    virtual void	NMLNAME(UT_String & /*str*/, fpreal /*t*/) override {}

    virtual fpreal SYMMETRYDIRX(fpreal t) override 
			    { return evalFloat(SOP_SLIDEPAINT_SYMAXIS_IDX, 0, t); }
    virtual fpreal SYMMETRYDIRY(fpreal t) override 
			    { return evalFloat(SOP_SLIDEPAINT_SYMAXIS_IDX, 1, t); }
    virtual fpreal SYMMETRYDIRZ(fpreal t) override 
			    { return evalFloat(SOP_SLIDEPAINT_SYMAXIS_IDX, 2, t); }
    virtual fpreal SYMMETRYORIGX(fpreal t) override 
			    { return evalFloat(SOP_SLIDEPAINT_SYMORIG_IDX, 0, t); }
    virtual fpreal SYMMETRYORIGY(fpreal t) override 
			    { return evalFloat(SOP_SLIDEPAINT_SYMORIG_IDX, 1, t); }
    virtual fpreal SYMMETRYORIGZ(fpreal t) override 
			    { return evalFloat(SOP_SLIDEPAINT_SYMORIG_IDX, 2, t); }
    virtual int   DOREFLECTION() override 
			    { return evalInt(SOP_SLIDEPAINT_DOREFLECT_IDX,0, 0); }
    virtual int	  DOROTATION() override 
			    { return evalInt(SOP_SLIDEPAINT_DOROTATION_IDX,0, 0); }
    virtual int	  SYMMETRYROT(fpreal t) override 
			    { return evalInt(SOP_SLIDEPAINT_SYMROT_IDX,0, t); }
    virtual fpreal SYMMETRYDIST(fpreal t) override 
			    { return evalFloat(SOP_SLIDEPAINT_SYMDIST_IDX,0, t); }

    virtual int	  DOUVREFLECTION() override
		    { return evalInt(SOP_SLIDEPAINT_DOUVREFLECT_IDX, 0, 0); }
    virtual fpreal SYMMETRYUVORIGU(fpreal t) override
		    { return evalFloat(SOP_SLIDEPAINT_SYMUVORIG_IDX, 0, t); }
    virtual fpreal SYMMETRYUVORIGV(fpreal t) override
		    { return evalFloat(SOP_SLIDEPAINT_SYMUVORIG_IDX, 1, t); }
    virtual fpreal SYMMETRYUVANGLE(fpreal t) override
		    { return evalFloat(SOP_SLIDEPAINT_SYMUVANGLE_IDX, 0, t);}

    virtual int		ADDATTRIB() override
			    { return 0; }
    virtual int		VISUALIZE() override
			    { return myForceVisualize ||
				    evalInt(SOP_SLIDEPAINT_VISUALIZE_IDX, 0, 0); }
    virtual fpreal	VISLOW(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_VISRANGE_IDX, 0, t); }
    virtual fpreal	VISHIGH(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_VISRANGE_IDX, 1, t); }
    virtual int		VISMODE() override
			    { return evalInt(SOP_SLIDEPAINT_VISMODE_IDX, 0, 0); }

    virtual int		USECONNECTIVITY() override
			    { return evalInt(SOP_SLIDEPAINT_USECONNECTIVITY_IDX, 0, 0); }
    virtual int		USENORMALS() override
			    { return evalInt(SOP_SLIDEPAINT_USENORMALS_IDX, 0, 0); }
    virtual int		USEDEPTH() override
			    { return evalInt(SOP_SLIDEPAINT_USEDEPTH_IDX, 0, 0); }
    virtual fpreal	DEPTH(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_DEPTH_IDX, 0, t); }
    virtual fpreal	HEIGHT(fpreal t) override
			    { return evalFloat(SOP_SLIDEPAINT_DEPTH_IDX, 1, t); }


    // Find out if the brush has moved or done anything to cause us to add its
    // effect to the current gdp.
    virtual bool	hasStrokeChanged(fpreal t) override;
    virtual bool	hasStyleChanged(fpreal t) override;

    // Find if the nib file has changed:
    virtual bool	hasNibFileChanged(fpreal t) override;
    virtual bool	hasAccumStencilChanged(fpreal t) override;
    virtual bool	hasNibLookChanged(fpreal t) override;

    virtual bool	hasVisrangeChanged(fpreal t) override;

private:
    // Store attribute offsets for the various attributes we can affect
    GA_ROAttributeRef	mySlideOffset;
    GA_ROAttributeRef	myInflateOffset;

    // Inidicates whether or not we should use the
    // foreground or background colour when painting.
    bool		myUseFG;

};

#endif
