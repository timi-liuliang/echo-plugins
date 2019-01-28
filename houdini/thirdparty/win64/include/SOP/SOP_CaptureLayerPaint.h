/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The CaptureLayerPaint SOP, which works in conjunction with the
 * 	     CaptureLayerPaint State and the GDT classes.
 *
 */

#ifndef __SOP_CaptureLayerPaint_h__
#define __SOP_CaptureLayerPaint_h__

#include "SOP_API.h"
#include <UT/UT_String.h>
#include <PRM/PRM_ChoiceList.h>
#include <PRM/PRM_Parm.h>
#include <GU/GU_Brush.h>

#include "SOP_GDT.h"
#include "SOP_BrushBase.h"
#include "SOP_CaptureData.h"
#include "SOP_CaptureLayerData.h"

class PRM_MenuEntry;
class PRM_Name;
//class PRM_Parm;

enum {
    // Group parameters
    SOP_CAPTURELAYERPAINT_GRP_IDX = SOP_GDT_GRP_IDX,
    SOP_CAPTURELAYERPAINT_GRPTYPE_IDX,

    SOP_CAPTURELAYERPAINT_CAPTYPE_IDX,

    SOP_CAPTURELAYERPAINT_SWITCHER_IDX,

#define SOP_CAPTURELAYERPAINT_NUM_DEFORM_PARMS	(13)
    SOP_CAPTURELAYERPAINT_SKELROOT_IDX,
    SOP_CAPTURELAYERPAINT_CAPTUREREGION_IDX,
    SOP_CAPTURELAYERPAINT_MAXPAINTVALUE_IDX,
    SOP_CAPTURELAYERPAINT_MINREDUCEVALUE_IDX,
    SOP_CAPTURELAYERPAINT_FLOODAPPLY_IDX,
    SOP_CAPTURELAYERPAINT_OP_IDX,
    SOP_CAPTURELAYERPAINT_CLEARALL_IDX,
    SOP_CAPTURELAYERPAINT_FLATTEN_IDX,
    SOP_CAPTURELAYERPAINT_VISUALIZE_IDX,
    SOP_CAPTURELAYERPAINT_VISNORMALIZED_IDX,
    SOP_CAPTURELAYERPAINT_VISLAYER_IDX,
    SOP_CAPTURELAYERPAINT_VISTYPE_IDX,
    SOP_CAPTURELAYERPAINT_VISMODE_IDX,

#define SOP_CAPTURELAYERPAINT_NUM_NIB_PARMS (18)
    SOP_CAPTURELAYERPAINT_SHAPE_IDX,
    SOP_CAPTURELAYERPAINT_BITMAP_IDX,
    SOP_CAPTURELAYERPAINT_BITMAPCHAN_IDX,
    SOP_CAPTURELAYERPAINT_RAD_IDX,
    SOP_CAPTURELAYERPAINT_UVRAD_IDX,
    SOP_CAPTURELAYERPAINT_RADPRESSURE_IDX,
    SOP_CAPTURELAYERPAINT_USEDEPTH_IDX,
    SOP_CAPTURELAYERPAINT_DEPTH_IDX,
    SOP_CAPTURELAYERPAINT_ANGLE_IDX,
    SOP_CAPTURELAYERPAINT_SQUASH_IDX,
    SOP_CAPTURELAYERPAINT_OPAC_IDX,
    SOP_CAPTURELAYERPAINT_OPACPRESSURE_IDX,
    SOP_CAPTURELAYERPAINT_SPLATTER_IDX,
    SOP_CAPTURELAYERPAINT_PAPER_IDX,
    SOP_CAPTURELAYERPAINT_SOFTEDGE_IDX,
    SOP_CAPTURELAYERPAINT_KERNEL_IDX,
    SOP_CAPTURELAYERPAINT_UPTYPE_IDX,
    SOP_CAPTURELAYERPAINT_UPVECTOR_IDX,

#define SOP_CAPTURELAYERPAINT_NUM_SYM_PARMS (9)
    SOP_CAPTURELAYERPAINT_DOREFLECT_IDX,
    SOP_CAPTURELAYERPAINT_DOROTATION_IDX,
    SOP_CAPTURELAYERPAINT_SYMAXIS_IDX,
    SOP_CAPTURELAYERPAINT_SYMORIG_IDX,
    SOP_CAPTURELAYERPAINT_SYMROT_IDX,
    SOP_CAPTURELAYERPAINT_SYMDIST_IDX,
    SOP_CAPTURELAYERPAINT_DOUVREFLECT_IDX,
    SOP_CAPTURELAYERPAINT_SYMUVORIG_IDX,
    SOP_CAPTURELAYERPAINT_SYMUVANGLE_IDX,

#define SOP_CAPTURELAYERPAINT_NUM_RAY_PARMS	(11)
    SOP_CAPTURELAYERPAINT_PROJTYPE_IDX,
    SOP_CAPTURELAYERPAINT_USECONNECTIVITY_IDX,
    SOP_CAPTURELAYERPAINT_USENORMALS_IDX,
    SOP_CAPTURELAYERPAINT_REALTIME_IDX,
    SOP_CAPTURELAYERPAINT_RAY_ORIENT_IDX,
    SOP_CAPTURELAYERPAINT_RAY_HITPOS_IDX,
    SOP_CAPTURELAYERPAINT_RAY_HITPRIM_IDX,
    SOP_CAPTURELAYERPAINT_RAY_HITUVW_IDX,
    SOP_CAPTURELAYERPAINT_RAY_HITPRESSURE_IDX,
    SOP_CAPTURELAYERPAINT_RAY_HITPT_IDX,
    SOP_CAPTURELAYERPAINT_RAY_EVENT_IDX
};

class SOP_API SOP_CaptureLayerPaint : public SOP_BrushBase
{
public:
	     SOP_CaptureLayerPaint(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_CaptureLayerPaint();

    virtual bool		 updateParmsFlags() override;

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];
    static PRM_Template		 myObsoleteList[];

    virtual void setAccumulateStencil(bool) override {}

    virtual void setRayOrigin(const UT_Vector3 &orig, fpreal t) override
    {
	setFloat(SOP_CAPTURELAYERPAINT_RAY_HITPOS_IDX, 0, t, orig.x());
	setFloat(SOP_CAPTURELAYERPAINT_RAY_HITPOS_IDX, 1, t, orig.y());
	setFloat(SOP_CAPTURELAYERPAINT_RAY_HITPOS_IDX, 2, t, orig.z());
    }
    virtual void setRayOrientation(const UT_Vector3 &orient,
			fpreal t) override
    {
	setFloat(SOP_CAPTURELAYERPAINT_RAY_ORIENT_IDX, 0, t,orient.x());
	setFloat(SOP_CAPTURELAYERPAINT_RAY_ORIENT_IDX, 1, t,orient.y());
	setFloat(SOP_CAPTURELAYERPAINT_RAY_ORIENT_IDX, 2, t,orient.z());
    }
    virtual void setHitPrimitive(int primidx, fpreal t) override
    {
	setInt(SOP_CAPTURELAYERPAINT_RAY_HITPRIM_IDX, 0, t, primidx);
    }
    virtual void setHitPoint(int pt, fpreal t) override
    {
	setInt(SOP_CAPTURELAYERPAINT_RAY_HITPT_IDX, 0, t, pt);
    }
    virtual void setHitUVW(fpreal u, fpreal v,
			fpreal w, fpreal t) override
    {
	setFloat(SOP_CAPTURELAYERPAINT_RAY_HITUVW_IDX, 0, t, u);
	setFloat(SOP_CAPTURELAYERPAINT_RAY_HITUVW_IDX, 1, t, v);
	setFloat(SOP_CAPTURELAYERPAINT_RAY_HITUVW_IDX, 2, t, w);
    }
    virtual void setHitPressure(fpreal p, fpreal t) override
    {
	setFloat(SOP_CAPTURELAYERPAINT_RAY_HITPRESSURE_IDX, 0, t, p);
    }
    virtual void setRadius(fpreal r, fpreal t) override
    {
	setFloat(SOP_CAPTURELAYERPAINT_RAD_IDX, 0, t, r);
    }
    virtual void setUVRadius(fpreal r, fpreal t) override
    {
	setFloat(SOP_CAPTURELAYERPAINT_UVRAD_IDX, 0, t, r);
    }
    virtual void setBrushEvent(SOP_BrushEvent event) override
    {
	setInt(SOP_CAPTURELAYERPAINT_RAY_EVENT_IDX, 0, 0, int(event));
    }
    virtual void setBrushOp(SOP_BrushOp op) override
    {
	int iop;
	switch (op)
	{
	    case SOP_BRUSHOP_SMOOTHATTRIB:	iop = 1; break;
	    case SOP_BRUSHOP_SMOOTHLAYER:	iop = 2; break;
	    case SOP_BRUSHOP_SMOOTHSINGLE:	iop = 3; break;
	    case SOP_BRUSHOP_REDUCE:		iop = 4; break;
	    case SOP_BRUSHOP_ERASE:		iop = 5; break;
	    case SOP_BRUSHOP_ERASESINGLE:	iop = 6; break;
	    case SOP_BRUSHOP_EYEDROP:		iop = 7; break;
	    case SOP_BRUSHOP_PAINT:
	    default:				iop = 0; break;
	}
	setInt(SOP_CAPTURELAYERPAINT_OP_IDX, 0, 0, iop);
    }
    virtual void setBrushShape(SOP_BrushShape shape) override
    {
	setInt(SOP_CAPTURELAYERPAINT_SHAPE_IDX, 0, 0, int(shape));
    }
    virtual void setProjectionType(int projtype) override
    {
	setInt(SOP_CAPTURELAYERPAINT_PROJTYPE_IDX, 0, 0, projtype);
    }
    virtual void useForegroundColor(void) override { }
    virtual void useBackgroundColor(void) override { }
    virtual void setVisualization(int state) override
    {
	 // only trigger recooks if different!
	if( myForceVisualize != state )
	{
	    myForceVisualize = state;
	    forceRecook();
	}
    }

    bool	 getCurrentStencilColor(OP_Context &context, UT_Color &color);
    bool	 getAccumulatedModifiedPoints(UT_IntArray &changed);

protected:
    virtual OP_ERROR	 processBrushOp(OP_Context &context,
				bool changed_input,
				bool changed_group) override;

    virtual const char	*inputLabel(unsigned idx) const override;
    virtual int		 isRefInput(unsigned idx) const override;

    virtual void	 doErase() override;

    void		 updateCaptureWeight(int ptnum, const GU_Detail *pgdp,
					     float *alphas);

    virtual OP_ERROR		 cookInputGroups(OP_Context &context,
						 int alone = 0) override;
    void		 updateStencilMappings();
    OP_ERROR		 migrateStencils();
    virtual OP_ERROR	 cookMySop(OP_Context &context) override;
    void		 doVisualize(OP_Context &context, bool update_all);

    //
    //  I/O methods
    //
    virtual OP_ERROR    save(std::ostream &os, const OP_SaveFlags &flags,
			      const char *path_prefix,
			      const UT_String &name_override = UT_String()
			) override;
    virtual bool	 load(UT_IStream &is, const char *extension,
			      const char *path) override;

    virtual OP_Node	*getRestGeoInput() override
    {
	return getInput(1);
    }

public:
    virtual int		altersGeometry() const override { return 0; }
    virtual int		altersColor() const override { return 1; }
    virtual fpreal	RAWRADIUS(fpreal t) override
			{ return evalFloat(SOP_CAPTURELAYERPAINT_RAD_IDX, 0, t); }
    virtual fpreal	RAWUVRADIUS(fpreal t) override
			{ return evalFloat(SOP_CAPTURELAYERPAINT_UVRAD_IDX, 0, t); }
    virtual fpreal	RADIUSPRESSURE(fpreal t) override
			{ return evalFloat(SOP_CAPTURELAYERPAINT_RADPRESSURE_IDX, 0, t); }
    virtual fpreal	RAWOPACITY(fpreal t) override
			{return evalFloat(SOP_CAPTURELAYERPAINT_OPAC_IDX, 0, t); }
    virtual fpreal	OPACITYPRESSURE(fpreal t) override
			{return evalFloat(SOP_CAPTURELAYERPAINT_OPACPRESSURE_IDX, 0, t) ; }

    virtual SOP_BrushOp	 OP(void) override
			{
			    return SOP_BRUSHOP_CALLBACK;
			}
    SOP_BrushOp	 getBrushOP()
			{
			    switch (evalInt(SOP_CAPTURELAYERPAINT_OP_IDX, 0, 0))
			    {
				case 1: return SOP_BRUSHOP_SMOOTHATTRIB;
				case 2: return SOP_BRUSHOP_SMOOTHLAYER;
				case 3: return SOP_BRUSHOP_SMOOTHSINGLE;
				case 4: return SOP_BRUSHOP_REDUCE;
				case 5: return SOP_BRUSHOP_ERASE;
				case 6: return SOP_BRUSHOP_ERASESINGLE;
				case 7: return SOP_BRUSHOP_EYEDROP;
				default: return SOP_BRUSHOP_PAINT;
			    }
			}

    virtual int		ACCUMSTENCIL(void) override { return 0; }

    virtual int		PROJECTIONTYPE(void) override
		    { return evalInt(SOP_CAPTURELAYERPAINT_PROJTYPE_IDX, 0, 0); }
    virtual int		REALTIME(void) override
		    { return evalInt(SOP_CAPTURELAYERPAINT_REALTIME_IDX, 0, 0); }
    virtual int		SHAPE(fpreal t) override
		    { return evalInt(SOP_CAPTURELAYERPAINT_SHAPE_IDX,  0, t); }


    virtual void	 brushOpCallback(
			    GA_Offset pt,
			    const UT_Array<GA_Offset> *ptneighbour,
			    GA_Offset vtx,
			    const UT_Array<GA_Offset> *vtxneighbour,
			    float alpha,
			    GEO_Delta *delta) override;

    virtual void 	flood(fpreal t) override;
    static int		flattenStatic(void *op, int, fpreal time,
				      const PRM_Template *);
    void 		flatten(fpreal t);
    virtual void 	clearAllDeltas(fpreal t) override;
    void		applyUndo(const SOP_CaptureLayerData &delta,
				  const UT_String &rootpath,
				  const UT_StringArray &mapping);
    void		applyRedo(const SOP_CaptureLayerData &delta,
				  const UT_String &rootpath,
				  const UT_StringArray &mapping);

    GEO_Detail::CaptureType getCaptureType() const
			{ return myCaptureType; }

protected:
    void		applyDelta(const SOP_CaptureLayerData &delta,
				   const UT_String &rootpath,
				   const UT_StringArray &mapping, bool is_add);
    int			getNumStencils();
    float		getStencilValue(int snum, int ptnum);
    void		setStencilValue(int snum, int ptnum, float value);

    void		createTempLayer();
    void		postOperation();

    void		setupForUndo();
    void		saveUndo();

    virtual const GA_PointGroup	*getPointGroup() override
    {
	return myPointGroup;
    }
    virtual bool isGroupParmDirty(OP_Context &context) override;

    int		GRPTYPE()
		{ return evalInt(SOP_CAPTURELAYERPAINT_GRPTYPE_IDX, 0, 0); }

    virtual GEO_Detail::CaptureType CAPTURETYPE() override
		{
		    auto type = evalInt(SOP_CAPTURELAYERPAINT_CAPTYPE_IDX, 0, 0);
			if ( type == 3 )
			    return GEO_Detail::CAPTURE_MUSCLE;
			else 
			    return static_cast<GEO_Detail::CaptureType>(type);
	       	}

    virtual int	MERGEMODE() override { return 0; }
    virtual void	SCRIPT(UT_String &, fpreal) override {}
    virtual int		AXIS(void) override	{ return   0 ;  }
    virtual fpreal	USERX(fpreal) override	{ return 0.0;  }
    virtual fpreal	USERY(fpreal) override	{ return 0.0;  }
    virtual fpreal	USERZ(fpreal) override	{ return 0.0;  }

    virtual fpreal	RAYORIENTX(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_ORIENT_IDX,0,t); }
    virtual fpreal	RAYORIENTY(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_ORIENT_IDX,1,t); }
    virtual fpreal	RAYORIENTZ(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_ORIENT_IDX,2,t); }
    virtual fpreal	RAYHITX(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_HITPOS_IDX,0,t); }
    virtual fpreal	RAYHITY(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_HITPOS_IDX,1,t); }
    virtual fpreal	RAYHITZ(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_HITPOS_IDX,2,t); }
    virtual fpreal	RAYHITU(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_HITUVW_IDX, 0,t); }
    virtual fpreal	RAYHITV(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_HITUVW_IDX, 1,t); }
    virtual fpreal	RAYHITW(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_HITUVW_IDX, 2,t); }
    virtual fpreal	RAYHITPRESSURE(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_RAY_HITPRESSURE_IDX, 0, t); }
    virtual int		PRIMHIT(fpreal t) override
		    { return evalInt(SOP_CAPTURELAYERPAINT_RAY_HITPRIM_IDX, 0,t); }
    virtual int		PTHIT(fpreal t) override
		    { return evalInt(SOP_CAPTURELAYERPAINT_RAY_HITPT_IDX, 0, t); }
    virtual int		EVENT() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_RAY_EVENT_IDX,   0,0); }
    virtual bool USE_FOREGROUND(void) override	{ return true; }
    virtual fpreal FGR(fpreal) override		{ return 0.0; }
    virtual fpreal FGG(fpreal) override		{ return 0.0; }
    virtual fpreal FGB(fpreal) override		{ return 0.0; }
    virtual fpreal BGR(fpreal) override		{ return 0.0; }
    virtual fpreal BGG(fpreal) override		{ return 0.0; }
    virtual fpreal BGB(fpreal) override		{ return 0.0; }
    virtual fpreal	SOFTEDGE(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SOFTEDGE_IDX, 0, t); }
    virtual void	KERNEL(UT_String &str, fpreal t) override
		    { evalString(str, SOP_CAPTURELAYERPAINT_KERNEL_IDX, 0, t); }
    virtual int		UPTYPE(fpreal t) override
			{ return evalInt(SOP_CAPTURELAYERPAINT_UPTYPE_IDX, 0, t); }
    virtual fpreal	UPX(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_UPVECTOR_IDX, 0, t); }
    virtual fpreal	UPY(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_UPVECTOR_IDX, 1, t); }
    virtual fpreal	UPZ(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_UPVECTOR_IDX, 2, t); }

    virtual fpreal	PAPERNOISE(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_PAPER_IDX, 0, t); }
    virtual fpreal	SPLATTER(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SPLATTER_IDX, 0, t); }
    virtual void	BITMAP(UT_String &str, fpreal t) override
		    { evalString(str, SOP_CAPTURELAYERPAINT_BITMAP_IDX, 0, t); }
    virtual int		BITMAPCHAN(fpreal t) override
		    { return evalInt(SOP_CAPTURELAYERPAINT_BITMAPCHAN_IDX, 0, t); }
    virtual fpreal	ANGLE(fpreal t) override
			{ return evalFloat(SOP_CAPTURELAYERPAINT_ANGLE_IDX,  0, t); }
    virtual fpreal	SQUASH(fpreal t) override
			{ return evalFloat(SOP_CAPTURELAYERPAINT_SQUASH_IDX, 0, t); }
    virtual int		DOSTAMPING() override { return 0; }
    virtual int		WRITEALPHA() override { return 0; }

    virtual int		OVERRIDECD() override { return 1; }
    virtual void	CDNAME(UT_String & /*str*/,
				fpreal /*t*/)  override{ }
    virtual int		OVERRIDEALPHA() override { return 0; }
    virtual void	ALPHANAME(UT_String & /*str*/,
				fpreal /*t*/) override {}
    virtual int		OVERRIDENML() override { return 0; }
    virtual void	NMLNAME(UT_String & /*str*/,
				fpreal /*t*/) override {}

    virtual fpreal SYMMETRYDIRX(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMAXIS_IDX, 0, t); }
    virtual fpreal SYMMETRYDIRY(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMAXIS_IDX, 1, t); }
    virtual fpreal SYMMETRYDIRZ(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMAXIS_IDX, 2, t); }
    virtual fpreal SYMMETRYORIGX(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMORIG_IDX, 0, t); }
    virtual fpreal SYMMETRYORIGY(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMORIG_IDX, 1, t); }
    virtual fpreal SYMMETRYORIGZ(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMORIG_IDX, 2, t); }
    virtual int   DOREFLECTION() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_DOREFLECT_IDX,0, 0); }
    virtual int	  DOROTATION() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_DOROTATION_IDX,0, 0); }
    virtual int	  SYMMETRYROT(fpreal t) override
		    { return evalInt(SOP_CAPTURELAYERPAINT_SYMROT_IDX,0, t); }
    virtual fpreal SYMMETRYDIST(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMDIST_IDX,0, t); }

    virtual int	  DOUVREFLECTION() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_DOUVREFLECT_IDX, 0, 0); }
    virtual fpreal SYMMETRYUVORIGU(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMUVORIG_IDX, 0, t); }
    virtual fpreal SYMMETRYUVORIGV(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMUVORIG_IDX, 1, t); }
    virtual fpreal SYMMETRYUVANGLE(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_SYMUVANGLE_IDX, 0, t);}

    virtual int		ADDATTRIB() override { return 0; }
    bool	SHOWVISUALIZE()
		    { return myForceVisualize || evalInt(SOP_CAPTURELAYERPAINT_VISUALIZE_IDX, 0, 0); }
    int		VISLAYER()
		    { return evalInt(SOP_CAPTURELAYERPAINT_VISLAYER_IDX, 0, 0); }
    virtual int		VISTYPE() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_VISTYPE_IDX, 0, 0); }
    virtual int		VISMODE() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_VISMODE_IDX, 0, 0); }
    virtual int		USECAPTURE() override { return 1; }
    virtual int		CAPTUREIDX(fpreal /*t*/) override
		    {
			UT_String cregion_name;
			CAPTUREREGION(cregion_name);
			return gdp->getCaptureRegionIndex(cregion_name,
							  myCaptureType);
		    }
    void	SKELROOT(UT_String &str)
		    { evalString(str, SOP_CAPTURELAYERPAINT_SKELROOT_IDX, 0, 0); }
public:
    void	CAPTUREREGION(UT_String &str)
		    { evalString(str, SOP_CAPTURELAYERPAINT_CAPTUREREGION_IDX, 0, 0); }
protected:
    virtual int		USECONNECTIVITY() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_USECONNECTIVITY_IDX, 0, 0); }
    virtual int		USENORMALS() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_USENORMALS_IDX, 0, 0); }
    virtual int		USEDEPTH() override
		    { return evalInt(SOP_CAPTURELAYERPAINT_USEDEPTH_IDX, 0, 0); }
    virtual fpreal	DEPTH(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_DEPTH_IDX, 0, t); }
    virtual fpreal	HEIGHT(fpreal t) override
		    { return evalFloat(SOP_CAPTURELAYERPAINT_DEPTH_IDX, 1, t); }
    fpreal	MAXPAINTVALUE()
		    { return evalFloat(SOP_CAPTURELAYERPAINT_MAXPAINTVALUE_IDX, 0, 0); }
    fpreal	MINREDUCEVALUE()
		    { return evalFloat(SOP_CAPTURELAYERPAINT_MINREDUCEVALUE_IDX, 0, 0); }
    bool	VISNORMALIZED()
		    { return evalInt(SOP_CAPTURELAYERPAINT_VISNORMALIZED_IDX, 0, 0) != 0; }


    // Find out if the brush has moved or done anything to cause us to add its
    // effect to the current gdp.
    virtual bool	hasStrokeChanged(fpreal t) override;
    virtual bool	hasStyleChanged(fpreal) override { return false; }

    // Find if the nib file has changed:
    virtual bool	hasNibFileChanged(fpreal t) override;
    virtual bool	hasAccumStencilChanged(fpreal) override { return false; }
    virtual bool	hasNibLookChanged(fpreal t) override;
    bool		hasVisChanged(fpreal t);

    virtual void	buildOpDependencies() override;
    virtual void	buildParmDependency( int parm_index ) override;
    virtual void	handleOpDependency( int referenced_op_id,
					    const OP_Dependency &op_dep,
					    OP_InterestType interest_type,
					    bool &need_cook,
					    const char *old_fullpath,
					    const char *old_cwd,
					    const char *old_chan_name ) override;
    // CaptureLayerPaint doesn't use it's GDT... This will suppress SOP_UndoGDT
    // objects from being put on the undo stack
    virtual void	 applyAndClearCurrentDelta() override {}

public:
    // MSS_CaptureLayerPaintState uses this to build its capture region menu
    // too
    static void		    buildCaptureRegionMenu(
				void *data, PRM_Name *menu_entries,
				int num_entries, const PRM_SpareData *,
				const PRM_Parm *);

private:
    void		updateStencilIndex();
    int			getStencilIndex();
    void		checkCaptureRegions();

    int			addCaptureRegion( const char *cregion_name );

    void		clearStencils();
    int			findStencilForCaptureRegion(int cregion);
    int			getStencilForCaptureRegion(int cregion);
    int			addNewStencilForCaptureRegion(int cregion);
    void		addedStencilForCaptureRegion(int snum, int cregion);
    static bool		getRootPathCandidates(OP_Node &node, void *data);

    bool		boneCapture() const
			{ return myCaptureType == GEO_Detail::CAPTURE_BONE; }
    bool		metaCapture() const
			{ return myCaptureType == GEO_Detail::CAPTURE_META; }
    bool		wireCapture() const
			{ return myCaptureType == GEO_Detail::CAPTURE_WIRE; }
    bool		muscleCapture() const
			{ return myCaptureType == GEO_Detail::CAPTURE_MUSCLE; }

    bool		filterForCurrentStencilIndex(UT_IntArray &regions,
						     UT_FloatArray &weights);

private:
    UT_String		    myRootPath;

    static PRM_ChoiceList   theCaptureRegionMenu;

    // stencil data and mapping to capture regions
    SOP_CaptureLayerData	 myStencils;
    UT_StringArray		 myStencilMapping;
    UT_IntArray			 myCaptureRegionMapping;
    UT_IntArray			 myRegionToStencilMapping;
    int				 myNullStencilIndex;

    // temporary structures needed for smooth operation
    SOP_CaptureLayerData	*myTempSmoothLayer;
    float			*myTempSmoothAlpha;

    // current brush data
    int				 myStencilIndex;

    // data for undo
    SOP_CaptureLayerData	 myDelta;
    UT_String			 myOldRootPath;
    UT_StringArray		 myOldStencilMapping;

    // list of what has been modified so we can do incremental cooks
    UT_IntArray			 myModifiedPoints;
    bool			 myLastVisualize;

    // cache of changes for downstream SOPs. it is an accumulation of
    // myModifiedPoints.
    UT_IntArray			 myAccumulatedModifiedPoints;
    bool			 myHaveAccumulatedModifiedPoints;

    const GA_PointGroup		*myPointGroup;
    UT_String			 myPrimitiveClosureName;
    UT_String			 myPointClosureName;
    bool			 myAddWarningFlag;
    bool			 myFlattenFlag;

    // temporary data for migrating from houdini 6.6
    bool			 myMigrationNeeded;
    UT_String			 myTestCregionName;
    UT_IntArray			 myTestCregionCandidates;

    UT_String			 myLoadedRootPath;
    UT_String			 myLoadedPathChanged;
    UT_StringArray		 myLoadedStencilMapping;

    // The current type of capture attributes we're modifying.
    GEO_Detail::CaptureType	 myCaptureType;
};

#endif
