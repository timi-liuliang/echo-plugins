/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The UVBrush SOP, which works in conjunction with the
 * 	     UVBrush State and the GDT classes.
 *
 */

#ifndef __SOP_UVBrush_h__
#define __SOP_UVBrush_h__

#include "SOP_API.h"
#include <GU/GU_Brush.h>

#include "SOP_GDT.h"
#include "SOP_BrushBase.h"

class PRM_MenuEntry;

enum {
    // Group parameters
    SOP_UVBRUSH_GRP_IDX = SOP_GDT_GRP_IDX,

    SOP_UVBRUSH_UVATTRIB_IDX,

    SOP_UVBRUSH_SWITCHER_IDX,

#define SOP_UVBRUSH_NUM_DEFORM_PARMS	(4)
    SOP_UVBRUSH_FOREBACK_SWITCHER_IDX,
    SOP_UVBRUSH_FS_IDX,
    SOP_UVBRUSH_BS_IDX,
    SOP_UVBRUSH_FLOODAPPLY_IDX,
    SOP_UVBRUSH_OP_IDX,
    SOP_UVBRUSH_CLEARALL_IDX,

#define SOP_UVBRUSH_NUM_NIB_PARMS (18)
    SOP_UVBRUSH_SHAPE_IDX,
    SOP_UVBRUSH_BITMAP_IDX,
    SOP_UVBRUSH_BITMAPCHAN_IDX,
    SOP_UVBRUSH_RAD_IDX,
    SOP_UVBRUSH_UVRAD_IDX,
    SOP_UVBRUSH_RADPRESSURE_IDX,
    SOP_UVBRUSH_USEDEPTH_IDX,
    SOP_UVBRUSH_DEPTH_IDX,
    SOP_UVBRUSH_ANGLE_IDX,
    SOP_UVBRUSH_SQUASH_IDX,
    SOP_UVBRUSH_OPAC_IDX,
    SOP_UVBRUSH_OPACPRESSURE_IDX,
    SOP_UVBRUSH_SPLATTER_IDX,
    SOP_UVBRUSH_PAPER_IDX,
    SOP_UVBRUSH_SOFTEDGE_IDX,
    SOP_UVBRUSH_KERNEL_IDX,
    SOP_UVBRUSH_UPTYPE_IDX,
    SOP_UVBRUSH_UPVECTOR_IDX,

#define SOP_UVBRUSH_NUM_SYM_PARMS (9)
    SOP_UVBRUSH_DOREFLECT_IDX,
    SOP_UVBRUSH_DOROTATION_IDX,
    SOP_UVBRUSH_SYMAXIS_IDX,
    SOP_UVBRUSH_SYMORIG_IDX,
    SOP_UVBRUSH_SYMROT_IDX,
    SOP_UVBRUSH_SYMDIST_IDX,
    SOP_UVBRUSH_DOUVREFLECT_IDX,
    SOP_UVBRUSH_SYMUVORIG_IDX,
    SOP_UVBRUSH_SYMUVANGLE_IDX,

#define SOP_UVBRUSH_NUM_RAY_PARMS	(11)
    SOP_UVBRUSH_PROJTYPE_IDX,
    SOP_UVBRUSH_USECONNECTIVITY_IDX,
    SOP_UVBRUSH_USENORMALS_IDX,
    SOP_UVBRUSH_REALTIME_IDX,
    SOP_UVBRUSH_RAY_ORIENT_IDX,
    SOP_UVBRUSH_RAY_HITPOS_IDX,
    SOP_UVBRUSH_RAY_HITPRIM_IDX,
    SOP_UVBRUSH_RAY_HITUVW_IDX,
    SOP_UVBRUSH_RAY_HITPRESSURE_IDX,
    SOP_UVBRUSH_RAY_HITPT_IDX,
    SOP_UVBRUSH_RAY_EVENT_IDX
};

class SOP_API SOP_UVBrush : public SOP_BrushBase
{
public:
	     SOP_UVBrush(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_UVBrush();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];
    static PRM_Template		 myObsoleteList[];

    virtual void setAccumulateStencil(bool )
		 {
		 }
    virtual void setRayOrigin(const UT_Vector3 &orig, fpreal t)
		 {
		     setFloat(SOP_UVBRUSH_RAY_HITPOS_IDX, 0, t, orig.x());
		     setFloat(SOP_UVBRUSH_RAY_HITPOS_IDX, 1, t, orig.y());
		     setFloat(SOP_UVBRUSH_RAY_HITPOS_IDX, 2, t, orig.z());
		 }
    virtual void setRayOrientation(const UT_Vector3 &orient, fpreal t)
		 {
		     setFloat(SOP_UVBRUSH_RAY_ORIENT_IDX, 0, t, orient.x());
		     setFloat(SOP_UVBRUSH_RAY_ORIENT_IDX, 1, t, orient.y());
		     setFloat(SOP_UVBRUSH_RAY_ORIENT_IDX, 2, t, orient.z());
		 }
    virtual void setHitPrimitive(int primidx, fpreal t)
		 {
		     setInt(SOP_UVBRUSH_RAY_HITPRIM_IDX, 0, t, primidx);
		 }
    virtual void setHitPoint(int pt, fpreal t)
		 {
		     setInt(SOP_UVBRUSH_RAY_HITPT_IDX, 0, t, pt);
		 }
    virtual void setHitUVW(fpreal u, fpreal v, fpreal w, fpreal t)
	 	 {
		     setFloat(SOP_UVBRUSH_RAY_HITUVW_IDX, 0, t, u);
		     setFloat(SOP_UVBRUSH_RAY_HITUVW_IDX, 1, t, v);
		     setFloat(SOP_UVBRUSH_RAY_HITUVW_IDX, 2, t, w);
		 }
    virtual void setHitPressure(fpreal p, fpreal t)
		 {
		     setFloat(SOP_UVBRUSH_RAY_HITPRESSURE_IDX, 0, t, p);
		 }   
    virtual void setRadius(fpreal r, fpreal t)
		 {
		     setFloat(SOP_UVBRUSH_RAD_IDX, 0, t, r);
		 }
    virtual void setUVRadius(fpreal r, fpreal t)
		 {
		     setFloat(SOP_UVBRUSH_UVRAD_IDX, 0, t, r);
		 }
    virtual void setBrushEvent(SOP_BrushEvent event)
		 {
		     setInt(SOP_UVBRUSH_RAY_EVENT_IDX, 0, 0, int(event));
		 }
    virtual void setBrushOp(SOP_BrushOp op)
		 {
		     int iop;
		     switch (op)
		     {
			 case SOP_BRUSHOP_SCALETEXTURE:	iop = 1; break;
			 case SOP_BRUSHOP_SMOOTHTEXTURE:	iop = 2; break;
			 case SOP_BRUSHOP_ERASE:	iop = 3; break;
			 case SOP_BRUSHOP_DRAGTEXTURE:
			 default:			iop = 0; break;
		     }
		     setInt(SOP_UVBRUSH_OP_IDX, 0, 0, iop);
		 }
    virtual void setBrushShape(SOP_BrushShape shape)
		 {
		     setInt(SOP_UVBRUSH_SHAPE_IDX, 0, 0, int(shape));
		 }
    virtual void setProjectionType(int projtype)
		 {
		     setInt(SOP_UVBRUSH_PROJTYPE_IDX, 0, 0, projtype);
		 }
    virtual void useForegroundColor(void)
		 {
		     setInt(SOP_UVBRUSH_FOREBACK_SWITCHER_IDX, 0, 0, 0);
		 }
    virtual void useBackgroundColor(void)
		 {
		     setInt(SOP_UVBRUSH_FOREBACK_SWITCHER_IDX, 0, 0, 1);
		 }

    virtual void setCurrentColor(const UT_Vector3 &cd)
		{
		    int		idx;
		    idx = USE_FOREGROUND() ?SOP_UVBRUSH_FS_IDX :SOP_UVBRUSH_BS_IDX;
		    setFloat(idx, 0, 0, cd.x());
		}
    virtual void setVisualization(int state)
		{
		    // only trigger recooks if different!
		    if( myForceVisualize != state )
		    {
			myForceVisualize = state;
			forceRecook();
		    }
		}

    virtual void getDescriptiveParmName(UT_String &name) const
    {
        name = "uvattrib";
    }

protected:
    virtual const char	*inputLabel(unsigned idx) const;

protected:
    virtual void	doErase();
    virtual void	addEraseAttributes(GU_Brush &brush);

public:
    virtual int		altersGeometry() const { return 0; }
    virtual int		altersColor() const { return 0; }
    virtual fpreal	RAWRADIUS(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAD_IDX, 0, t); }
    virtual fpreal	RAWUVRADIUS(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_UVRAD_IDX, 0, t); }
    virtual fpreal	RADIUSPRESSURE(fpreal t)
			{ return evalFloat(SOP_UVBRUSH_RADPRESSURE_IDX, 0, t); }
    virtual fpreal	RAWOPACITY(fpreal t)
			    {return evalFloat(SOP_UVBRUSH_OPAC_IDX, 0, t) ; }
    virtual fpreal	OPACITYPRESSURE(fpreal t)
			{return evalFloat(SOP_UVBRUSH_OPACPRESSURE_IDX, 0, t) ; }

    virtual SOP_BrushOp	 OP(void) 
			{ 
			    switch (evalInt(SOP_UVBRUSH_OP_IDX, 0, 0))
			    {
				case 0: return SOP_BRUSHOP_DRAGTEXTURE;
				case 1: return SOP_BRUSHOP_SCALETEXTURE;
				case 2: return SOP_BRUSHOP_SMOOTHTEXTURE;
				case 3: default: return SOP_BRUSHOP_ERASE;
			    }
			}

    virtual int		ACCUMSTENCIL(void) 
			    { return 0;}
    virtual int		PROJECTIONTYPE(void)
			    { return evalInt(SOP_UVBRUSH_PROJTYPE_IDX, 0, 0); }
    virtual int		REALTIME(void)
			    { return evalInt(SOP_UVBRUSH_REALTIME_IDX, 0, 0); }
    virtual int		SHAPE(fpreal t)
			    { return evalInt(SOP_UVBRUSH_SHAPE_IDX,  0, t); }

protected:
    virtual int		MERGEMODE()
			    { return 0; }
    virtual void	SCRIPT 	(UT_String &, fpreal  ) {}
    virtual int		AXIS(void)	{ return   0 ;  }
    virtual fpreal	USERX(fpreal)	{ return 0.0;  }
    virtual fpreal	USERY(fpreal)	{ return 0.0;  }
    virtual fpreal	USERZ(fpreal)	{ return 0.0;  }

    virtual fpreal	RAYORIENTX(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_ORIENT_IDX,0,t); }
    virtual fpreal	RAYORIENTY(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_ORIENT_IDX,1,t); }
    virtual fpreal	RAYORIENTZ(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_ORIENT_IDX,2,t); }
    virtual fpreal	RAYHITX(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_HITPOS_IDX,0,t); }
    virtual fpreal	RAYHITY(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_HITPOS_IDX,1,t); }
    virtual fpreal	RAYHITZ(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_HITPOS_IDX,2,t); }
    virtual fpreal	RAYHITU(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_HITUVW_IDX, 0,t); }
    virtual fpreal	RAYHITV(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_HITUVW_IDX, 1,t); }
    virtual fpreal	RAYHITW(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_HITUVW_IDX, 2,t); }
    virtual fpreal	RAYHITPRESSURE(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_RAY_HITPRESSURE_IDX, 0, t); }
    virtual int		PRIMHIT(fpreal t)
			    { return evalInt(SOP_UVBRUSH_RAY_HITPRIM_IDX, 0,t); }
    virtual int		PTHIT(fpreal t)
			    { return evalInt(SOP_UVBRUSH_RAY_HITPT_IDX, 0, t); }
    virtual int		EVENT()
			    { return evalInt(SOP_UVBRUSH_RAY_EVENT_IDX,   0,0); }
    virtual bool	USE_FOREGROUND(void)
			    { return evalInt(SOP_UVBRUSH_FOREBACK_SWITCHER_IDX,
								  0, 0) == 0; }

    virtual fpreal FGR(fpreal t)   { return evalFloat(SOP_UVBRUSH_FS_IDX,	0, t); }
    virtual fpreal FGG(fpreal  )   { return 0; }
    virtual fpreal FGB(fpreal  )   { return 0; }
    virtual fpreal BGR(fpreal t)   { return evalFloat(SOP_UVBRUSH_BS_IDX,	0, t); }
    virtual fpreal BGG(fpreal  )   { return 0; }
    virtual fpreal BGB(fpreal  )   { return 0; }

    virtual fpreal	SOFTEDGE(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_SOFTEDGE_IDX, 0, t); }
    virtual void	KERNEL(UT_String &str, fpreal t) 
			    { evalString(str, SOP_UVBRUSH_KERNEL_IDX, 0, t); }
    virtual int		UPTYPE(fpreal t)
			    { return evalInt(SOP_UVBRUSH_UPTYPE_IDX, 0, t); }
    virtual fpreal	UPX(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_UPVECTOR_IDX, 0, t); }
    virtual fpreal	UPY(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_UPVECTOR_IDX, 1, t); }
    virtual fpreal	UPZ(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_UPVECTOR_IDX, 2, t); }

    virtual fpreal	PAPERNOISE(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_PAPER_IDX, 0, t); }
    virtual fpreal	SPLATTER(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_SPLATTER_IDX, 0, t); }
    virtual void	BITMAP(UT_String &str, fpreal t)
			    { evalString(str, SOP_UVBRUSH_BITMAP_IDX, 0, t); }
    virtual int		BITMAPCHAN(fpreal t)
			    { return evalInt(SOP_UVBRUSH_BITMAPCHAN_IDX, 0, t); }
    virtual fpreal	ANGLE(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_ANGLE_IDX,  0, t); }
    virtual fpreal	SQUASH(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_SQUASH_IDX, 0, t); }
    virtual int		DOSTAMPING() { return 0; }
    virtual int		WRITEALPHA() { return 0; }

    virtual int		OVERRIDECD() { return 0; }
    virtual void	CDNAME(UT_String &, fpreal ) { }
    virtual int		OVERRIDEALPHA() { return 0; }
    virtual void	ALPHANAME(UT_String &, fpreal ) { }
    virtual int		OVERRIDENML() { return 0; }
    virtual void	NMLNAME(UT_String & /*str*/, fpreal /*t*/) {}

    virtual fpreal SYMMETRYDIRX(fpreal t) 
			    { return evalFloat(SOP_UVBRUSH_SYMAXIS_IDX, 0, t); }
    virtual fpreal SYMMETRYDIRY(fpreal t) 
			    { return evalFloat(SOP_UVBRUSH_SYMAXIS_IDX, 1, t); }
    virtual fpreal SYMMETRYDIRZ(fpreal t) 
			    { return evalFloat(SOP_UVBRUSH_SYMAXIS_IDX, 2, t); }
    virtual fpreal SYMMETRYORIGX(fpreal t) 
			    { return evalFloat(SOP_UVBRUSH_SYMORIG_IDX, 0, t); }
    virtual fpreal SYMMETRYORIGY(fpreal t) 
			    { return evalFloat(SOP_UVBRUSH_SYMORIG_IDX, 1, t); }
    virtual fpreal SYMMETRYORIGZ(fpreal t) 
			    { return evalFloat(SOP_UVBRUSH_SYMORIG_IDX, 2, t); }
    virtual int   DOREFLECTION() 
			    { return evalInt(SOP_UVBRUSH_DOREFLECT_IDX,0, 0); }
    virtual int	  DOROTATION() 
			    { return evalInt(SOP_UVBRUSH_DOROTATION_IDX,0, 0); }
    virtual int	  SYMMETRYROT(fpreal t) 
			    { return evalInt(SOP_UVBRUSH_SYMROT_IDX,0, t); }
    virtual fpreal SYMMETRYDIST(fpreal t) 
			    { return evalFloat(SOP_UVBRUSH_SYMDIST_IDX,0, t); }

    virtual int	  DOUVREFLECTION()
		    { return evalInt(SOP_UVBRUSH_DOUVREFLECT_IDX, 0, 0); }
    virtual fpreal SYMMETRYUVORIGU(fpreal t)
		    { return evalFloat(SOP_UVBRUSH_SYMUVORIG_IDX, 0, t); }
    virtual fpreal SYMMETRYUVORIGV(fpreal t)
		    { return evalFloat(SOP_UVBRUSH_SYMUVORIG_IDX, 1, t); }
    virtual fpreal SYMMETRYUVANGLE(fpreal t)
		    { return evalFloat(SOP_UVBRUSH_SYMUVANGLE_IDX, 0, t);}

    virtual int		ADDATTRIB() { return 0; }
    virtual void        UVATTRIB(UT_String &uvattribname, fpreal t)
                            { evalString(uvattribname, "uvattrib", 0, t); }
    virtual int		VISUALIZE() { return 0; }
    virtual fpreal	VISLOW(fpreal) { return 0; }
    virtual fpreal	VISHIGH(fpreal) { return 1; }
    virtual int		VISMODE() { return 0; }

    virtual int		USECONNECTIVITY()
			    { return evalInt(SOP_UVBRUSH_USECONNECTIVITY_IDX, 0, 0); }
    virtual int		USENORMALS()
			    { return evalInt(SOP_UVBRUSH_USENORMALS_IDX, 0, 0); }
    virtual int		USEDEPTH()
			    { return evalInt(SOP_UVBRUSH_USEDEPTH_IDX, 0, 0); }
    virtual fpreal	DEPTH(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_DEPTH_IDX, 0, t); }
    virtual fpreal	HEIGHT(fpreal t)
			    { return evalFloat(SOP_UVBRUSH_DEPTH_IDX, 1, t); }


    // Find out if the brush has moved or done anything to cause us to add its
    // effect to the current gdp.
    virtual bool	hasStrokeChanged(fpreal t);
    virtual bool	hasStyleChanged(fpreal t);

    // Find if the nib file has changed:
    virtual bool	hasNibFileChanged(fpreal t);
    virtual bool	hasAccumStencilChanged(fpreal t);
    virtual bool	hasNibLookChanged(fpreal t);

    virtual bool	hasVisrangeChanged(fpreal t);
};

#endif
