/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The Comb SOP, which works in conjunction with the
 * 	     Comb State and the GDT classes.
 *
 */

#ifndef __SOP_Comb_h__
#define __SOP_Comb_h__

#include "SOP_API.h"
#include <GU/GU_Brush.h>

#include "SOP_GDT.h"
#include "SOP_BrushBase.h"

class PRM_MenuEntry;

enum {
    // Group parameters
    SOP_COMB_GRP_IDX = SOP_GDT_GRP_IDX,

    SOP_COMB_SWITCHER_IDX,

#define SOP_COMB_NUM_DEFORM_PARMS	(6)
    SOP_COMB_LIFT_IDX,
    SOP_COMB_PRESERVENML_IDX,
    SOP_COMB_OVRNML_IDX,
    SOP_COMB_NMLNAME_IDX,
    SOP_COMB_CLEARALL_IDX,
    SOP_COMB_OP_IDX,

#define SOP_COMB_NUM_NIB_PARMS (18)
    SOP_COMB_SHAPE_IDX,
    SOP_COMB_BITMAP_IDX,
    SOP_COMB_BITMAPCHAN_IDX,
    SOP_COMB_RAD_IDX,
    SOP_COMB_UVRAD_IDX,
    SOP_COMB_RADPRESSURE_IDX,
    SOP_COMB_USEDEPTH_IDX,
    SOP_COMB_DEPTH_IDX,
    SOP_COMB_ANGLE_IDX,
    SOP_COMB_SQUASH_IDX,
    SOP_COMB_OPAC_IDX,
    SOP_COMB_OPACPRESSURE_IDX,
    SOP_COMB_SPLATTER_IDX,
    SOP_COMB_PAPER_IDX,
    SOP_COMB_SOFTEDGE_IDX,
    SOP_COMB_KERNEL_IDX,
    SOP_COMB_UPTYPE_IDX,
    SOP_COMB_UPVECTOR_IDX,

#define SOP_COMB_NUM_SYM_PARMS (9)
    SOP_COMB_DOREFLECT_IDX,
    SOP_COMB_DOROTATION_IDX,
    SOP_COMB_SYMAXIS_IDX,
    SOP_COMB_SYMORIG_IDX,
    SOP_COMB_SYMROT_IDX,
    SOP_COMB_SYMDIST_IDX,
    SOP_COMB_DOUVREFLECT_IDX,
    SOP_COMB_SYMUVORIG_IDX,
    SOP_COMB_SYMUVANGLE_IDX,

#define SOP_COMB_NUM_RAY_PARMS	(11)
    SOP_COMB_PROJTYPE_IDX,
    SOP_COMB_USECONNECTIVITY_IDX,
    SOP_COMB_USENORMALS_IDX,
    SOP_COMB_REALTIME_IDX,
    SOP_COMB_RAY_ORIENT_IDX,
    SOP_COMB_RAY_HITPOS_IDX,
    SOP_COMB_RAY_HITPRIM_IDX,
    SOP_COMB_RAY_HITUVW_IDX,
    SOP_COMB_RAY_HITPRESSURE_IDX,
    SOP_COMB_RAY_HITPT_IDX,
    SOP_COMB_RAY_EVENT_IDX
};

class SOP_API SOP_Comb : public SOP_BrushBase
{
public:
	     SOP_Comb(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Comb();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];
    static PRM_Template		 myObsoleteList[];

    virtual void setAccumulateStencil(bool)
		 {
		 }
    virtual void setRayOrigin(const UT_Vector3 &orig, fpreal t)
		 {
		     setFloat(SOP_COMB_RAY_HITPOS_IDX, 0, t, orig.x());
		     setFloat(SOP_COMB_RAY_HITPOS_IDX, 1, t, orig.y());
		     setFloat(SOP_COMB_RAY_HITPOS_IDX, 2, t, orig.z());
		 }
    virtual void setRayOrientation(const UT_Vector3 &orient, fpreal t)
		 {
		     setFloat(SOP_COMB_RAY_ORIENT_IDX, 0, t, orient.x());
		     setFloat(SOP_COMB_RAY_ORIENT_IDX, 1, t, orient.y());
		     setFloat(SOP_COMB_RAY_ORIENT_IDX, 2, t, orient.z());
		 }
    virtual void setHitPrimitive(int primidx, fpreal t)
		 {
		     setInt(SOP_COMB_RAY_HITPRIM_IDX, 0, t, primidx);
		 }
    virtual void setRadius(fpreal r, fpreal t)
		 {
		     setFloat(SOP_COMB_RAD_IDX, 0, t, r);
		 }
    virtual void setUVRadius(fpreal r, fpreal t)
		 {
		     setFloat(SOP_COMB_UVRAD_IDX, 0, t, r);
		 }
    virtual void setHitPoint(int pt, fpreal t)
		 {
		     setInt(SOP_COMB_RAY_HITPT_IDX, 0, t, pt);
		 }
    virtual void setHitUVW(fpreal u, fpreal v, fpreal w, fpreal t)
	 	 {
		     setFloat(SOP_COMB_RAY_HITUVW_IDX, 0, t, u);
		     setFloat(SOP_COMB_RAY_HITUVW_IDX, 1, t, v);
		     setFloat(SOP_COMB_RAY_HITUVW_IDX, 2, t, w);
		 }
    virtual void setHitPressure(fpreal p, fpreal t)
		 {
		     setFloat(SOP_COMB_RAY_HITPRESSURE_IDX, 0, t, p);
		 }
    virtual void setBrushEvent(SOP_BrushEvent event)
		 {
		     setInt(SOP_COMB_RAY_EVENT_IDX, 0, 0, int(event));
		 }
    virtual void setBrushOp(SOP_BrushOp op)
		 {
		     int iop;
		     switch (op)
		     {
			 default:
			 case SOP_BRUSHOP_COMB:		iop = 0; break;
			 case SOP_BRUSHOP_SMOOTHNORMAL:	iop = 1; break;
			 case SOP_BRUSHOP_ERASE:	iop = 2; break;
			 case SOP_BRUSHOP_LIFT:		iop = 3; break;
			 case SOP_BRUSHOP_ROTATE:	iop = 4; break;
		     }
		     setInt(SOP_COMB_OP_IDX, 0, 0, iop);
		 }
    virtual void setBrushShape(SOP_BrushShape shape)
		 {
		     setInt(SOP_COMB_SHAPE_IDX, 0, 0, int(shape));
		 }
    virtual void setProjectionType(int projtype)
		 {
		     setInt(SOP_COMB_PROJTYPE_IDX, 0, 0, projtype);
		 }
    virtual void useForegroundColor(void)
		 {
		 }
    virtual void useBackgroundColor(void)
		 {
		 }

    virtual int		altersGeometry() const { return 0; }
    virtual int		altersColor() const { return 0; }
    virtual fpreal	RAWRADIUS(fpreal t)
			    { return evalFloat(SOP_COMB_RAD_IDX, 0, t); }
    virtual fpreal	RAWUVRADIUS(fpreal t)
			    { return evalFloat(SOP_COMB_UVRAD_IDX, 0, t); }
    virtual fpreal	RADIUSPRESSURE(fpreal t)
			{ return evalFloat(SOP_COMB_RADPRESSURE_IDX, 0, t); }
    virtual fpreal	RAWOPACITY(fpreal t)
			    {return evalFloat(SOP_COMB_OPAC_IDX, 0, t); }
    virtual fpreal	OPACITYPRESSURE(fpreal t)
			{return evalFloat(SOP_COMB_OPACPRESSURE_IDX, 0, t) ; }

    virtual SOP_BrushOp	OP(void)
			{ 
			    switch (evalInt(SOP_COMB_OP_IDX, 0, 0))
			    {
				default:
				case 0: return SOP_BRUSHOP_COMB;
				case 1: return SOP_BRUSHOP_SMOOTHNORMAL;
				case 2: return SOP_BRUSHOP_ERASE;
				case 3: return SOP_BRUSHOP_LIFT;
				case 4: return SOP_BRUSHOP_ROTATE;
			    }
			}
    virtual int		ACCUMSTENCIL(void) { return 0; }
    virtual int		PROJECTIONTYPE(void)
			    { return evalInt(SOP_COMB_PROJTYPE_IDX, 0, 0); }
    virtual int		REALTIME(void)
			    { return evalInt(SOP_COMB_REALTIME_IDX, 0, 0); }
    virtual int		SHAPE(fpreal t)
			    { return evalInt(SOP_COMB_SHAPE_IDX, 0, t); }
    virtual int		OVERRIDENML()	
			    { return evalInt(SOP_COMB_OVRNML_IDX, 0, 0); }
    virtual void	NMLNAME(UT_String &str, fpreal t)
			    { evalString(str, SOP_COMB_NMLNAME_IDX, 0, t); }

protected:
    virtual const char	*inputLabel(unsigned idx) const;
    virtual void	doErase();


    virtual int		MERGEMODE() { return 0; }
    virtual void	SCRIPT 	(UT_String &, fpreal  ) {}
    virtual int		AXIS(void)	{ return   0 ;  }
    virtual fpreal	USERX(fpreal)	{ return 0.0;  }
    virtual fpreal	USERY(fpreal)	{ return 0.0;  }
    virtual fpreal	USERZ(fpreal)	{ return 0.0;  }

    virtual fpreal	RAYORIENTX(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_ORIENT_IDX, 0, t); }
    virtual fpreal	RAYORIENTY(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_ORIENT_IDX, 1, t); }
    virtual fpreal	RAYORIENTZ(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_ORIENT_IDX, 2, t); }
    virtual fpreal	RAYHITX(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_HITPOS_IDX, 0, t); }
    virtual fpreal	RAYHITY(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_HITPOS_IDX, 1, t); }
    virtual fpreal	RAYHITZ(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_HITPOS_IDX, 2, t); }
    virtual fpreal	RAYHITU(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_HITUVW_IDX,  0, t); }
    virtual fpreal	RAYHITV(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_HITUVW_IDX,  1, t); }
    virtual fpreal	RAYHITW(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_HITUVW_IDX,  2, t); }
    virtual fpreal	RAYHITPRESSURE(fpreal t)
			    { return evalFloat(SOP_COMB_RAY_HITPRESSURE_IDX, 0, t); }
    virtual int		PRIMHIT(fpreal t)
			    { return   evalInt(SOP_COMB_RAY_HITPRIM_IDX,0, t); }
    virtual int		PTHIT(fpreal t)
			    { return evalInt(SOP_COMB_RAY_HITPT_IDX, 0, t); }
    virtual int		EVENT()
			    { return   evalInt(SOP_COMB_RAY_EVENT_IDX,  0, 0); }
    virtual bool	USE_FOREGROUND(void)
			    { return 1; }

    // We return the lift as our first entry of the foreground colour.
    virtual fpreal	FGR(fpreal t)
			{ return evalFloat(SOP_COMB_LIFT_IDX, 0, t); }
    virtual fpreal	FGG(fpreal)   { return 0.0; }
    virtual fpreal	FGB(fpreal)   { return 0.0; }
    virtual fpreal	BGR(fpreal)   { return 0.0; }
    virtual fpreal	BGG(fpreal)   { return 0.0; }
    virtual fpreal	BGB(fpreal)   { return 0.0; }

    virtual fpreal	SOFTEDGE(fpreal t)
			    { return evalFloat(SOP_COMB_SOFTEDGE_IDX, 0, t); }
    virtual void	KERNEL(UT_String &str, fpreal t) 
			    { evalString(str, SOP_COMB_KERNEL_IDX, 0, t); }
    virtual int		UPTYPE(fpreal t)
			    { return evalInt(SOP_COMB_UPTYPE_IDX, 0, t); }
    virtual fpreal	UPX(fpreal t)
			    { return evalFloat(SOP_COMB_UPVECTOR_IDX, 0, t); }
    virtual fpreal	UPY(fpreal t)
			    { return evalFloat(SOP_COMB_UPVECTOR_IDX, 1, t); }
    virtual fpreal	UPZ(fpreal t)
			    { return evalFloat(SOP_COMB_UPVECTOR_IDX, 2, t); }

    virtual fpreal	PAPERNOISE(fpreal t)
			    { return evalFloat(SOP_COMB_PAPER_IDX, 0, t); }
    virtual fpreal	SPLATTER(fpreal t)
			    { return evalFloat(SOP_COMB_SPLATTER_IDX, 0, t); }
    virtual void	BITMAP(UT_String &str, fpreal t)
			    { evalString(str, SOP_COMB_BITMAP_IDX, 0, t); }
    virtual int		BITMAPCHAN(fpreal t)
			    { return evalInt(SOP_COMB_BITMAPCHAN_IDX, 0, t); }
    virtual fpreal	ANGLE(fpreal t)
			    { return evalFloat(SOP_COMB_ANGLE_IDX,  0, t); }
    virtual fpreal	SQUASH(fpreal t)
			    { return evalFloat(SOP_COMB_SQUASH_IDX, 0, t); }

    virtual int		DOSTAMPING()			{ return 0; }
    virtual int		WRITEALPHA()			{ return 0; }
    virtual int		OVERRIDECD()			{ return 0; }
    virtual void	CDNAME(UT_String &, fpreal)	{ }
    virtual int		OVERRIDEALPHA()			{ return 0; }
    virtual void	ALPHANAME(UT_String &, fpreal)	{ }
    virtual int		ADDATTRIB()			{ return 1; }
    virtual int		PRESERVENML()
			    { return evalInt(SOP_COMB_PRESERVENML_IDX, 0, 0); }
    virtual fpreal SYMMETRYDIRX(fpreal t) 
			    { return evalFloat(SOP_COMB_SYMAXIS_IDX, 0, t); }
    virtual fpreal SYMMETRYDIRY(fpreal t) 
			    { return evalFloat(SOP_COMB_SYMAXIS_IDX, 1, t); }
    virtual fpreal SYMMETRYDIRZ(fpreal t) 
			    { return evalFloat(SOP_COMB_SYMAXIS_IDX, 2, t); }
    virtual fpreal SYMMETRYORIGX(fpreal t) 
			    { return evalFloat(SOP_COMB_SYMORIG_IDX, 0, t); }
    virtual fpreal SYMMETRYORIGY(fpreal t) 
			    { return evalFloat(SOP_COMB_SYMORIG_IDX, 1, t); }
    virtual fpreal SYMMETRYORIGZ(fpreal t) 
			    { return evalFloat(SOP_COMB_SYMORIG_IDX, 2, t); }
    virtual int   DOREFLECTION() 
			    { return evalInt(SOP_COMB_DOREFLECT_IDX,0, 0); }
    virtual int	  DOROTATION() 
			    { return evalInt(SOP_COMB_DOROTATION_IDX,0, 0); }
    virtual int	  SYMMETRYROT(fpreal t) 
			    { return evalInt(SOP_COMB_SYMROT_IDX,0, t); }
    virtual fpreal SYMMETRYDIST(fpreal t) 
			    { return evalFloat(SOP_COMB_SYMDIST_IDX,0, t); }

    virtual int	  DOUVREFLECTION()
		    { return evalInt(SOP_COMB_DOUVREFLECT_IDX, 0, 0); }
    virtual fpreal SYMMETRYUVORIGU(fpreal t)
		    { return evalFloat(SOP_COMB_SYMUVORIG_IDX, 0, t); }
    virtual fpreal SYMMETRYUVORIGV(fpreal t)
		    { return evalFloat(SOP_COMB_SYMUVORIG_IDX, 1, t); }
    virtual fpreal SYMMETRYUVANGLE(fpreal t)
		    { return evalFloat(SOP_COMB_SYMUVANGLE_IDX, 0, t);}

    virtual int		USECONNECTIVITY()
			    { return evalInt(SOP_COMB_USECONNECTIVITY_IDX, 0, 0); }
    virtual int		USENORMALS()
			    { return evalInt(SOP_COMB_USENORMALS_IDX, 0, 0); }
    virtual int		USEDEPTH()
			    { return evalInt(SOP_COMB_USEDEPTH_IDX, 0, 0); }
    virtual fpreal	DEPTH(fpreal t)
			    { return evalFloat(SOP_COMB_DEPTH_IDX, 0, t); }
    virtual fpreal	HEIGHT(fpreal t)
			    { return evalFloat(SOP_COMB_DEPTH_IDX, 1, t); }

    // Find out if the brush has moved or done anything to cause us to add its
    // effect to the current gdp.
    virtual bool	hasStrokeChanged(fpreal t);
    virtual bool	hasStyleChanged(fpreal t);

    // Find if the nib file has changed:
    virtual bool	hasNibFileChanged(fpreal t);
    virtual bool	hasAccumStencilChanged(fpreal t);
    virtual bool	hasNibLookChanged(fpreal t);

    // Do we want our cook selection to be updated and displayed?
    virtual bool	 wantsCookSelection(void) const { return true; }

};

#endif
