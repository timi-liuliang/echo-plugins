/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The Edit SOP, which works in conjunction with the
 * 	     Edit State and the GDT classes.
 *
 */

#ifndef __SOP_Edit_h__
#define __SOP_Edit_h__

#include "SOP_API.h"

#include "SOP_BrushBase.h"

#include <GDT/GDT_MirrorTransform.h>
#include <GU/GU_SurfaceDistance.h>

class PRM_MenuEntry;

enum {
    // Group parameters
    SOP_EDIT_GRP_IDX, // = SOP_GDT_GRP_IDX,
    SOP_EDIT_GRPTYPE_IDX,

    SOP_EDIT_MODESWITCHER_IDX,
#define SOP_EDIT_NUM_EDIT_PARMS			(3)
#define SOP_EDIT_NUM_SCULPT_PARMS		(1)

    // Main Transform/Peak Switcher
    SOP_EDIT_SWITCHER_IDX,
    
    // Transform parameters
#define SOP_EDIT_NUM_TRANSFORM_PARMS		(12)
    SOP_EDIT_TRS_IDX,
    SOP_EDIT_XYZ_IDX,
    SOP_EDIT_T_IDX,
    SOP_EDIT_R_IDX,
    SOP_EDIT_S_IDX,
    SOP_EDIT_SHEAR_IDX,
    SOP_EDIT_PIVOTXFORMGROUPER_IDX,
#define SOP_EDIT_NUM_PIVOTXFORM_PARMS		(2)
    SOP_EDIT_P_IDX,
    SOP_EDIT_PR_IDX,
    SOP_EDIT_PREXFORMGROUPER_IDX,
#define SOP_EDIT_NUM_PREXFORM_PARMS		(6)
    SOP_EDIT_PREXFORM_TRS_IDX,
    SOP_EDIT_PREXFORM_XYZ_IDX,
    SOP_EDIT_PREXFORM_T_IDX,
    SOP_EDIT_PREXFORM_R_IDX,
    SOP_EDIT_PREXFORM_S_IDX,
    SOP_EDIT_PREXFORM_SHEAR_IDX,
    SOP_EDIT_KEEP_IDX,
    SOP_EDIT_LOCALSPACE_IDX,
    SOP_EDIT_LOCALSPACEUPVECTOR_IDX,
    SOP_EDIT_SLIDEONSURFACE_IDX,
    
    // Peak Parameters
#define SOP_EDIT_NUM_PEAK_PARMS			(2)
    SOP_EDIT_DIST_IDX,
    SOP_EDIT_LEADNML_IDX,

    // Commit Button
    SOP_EDIT_APPLYBUTTON_IDX,

    // Soft Parameters
    SOP_EDIT_SOFTPARMSGROUPER_IDX,

#define SOP_EDIT_NUM_SOFT_PARMS			(9)
    SOP_EDIT_DIST_METRIC_IDX,
    SOP_EDIT_APPLY_ROLLOFF_IDX,
    SOP_EDIT_DIST_ATTR_IDX,
    SOP_EDIT_LEAD_POINT_ATTR_IDX,
    SOP_EDIT_SOFTRADIUS_IDX,
    SOP_EDIT_SOFTTYPE_IDX,
    SOP_EDIT_TANGENT_IDX,
    SOP_EDIT_KERNEL_IDX,
    SOP_EDIT_VISFALLOFF_IDX,

// SCULPT parameters

    // Group parameters
    SOP_EDIT_SCULPT_SWITCHER_IDX,

#define SOP_EDIT_SCULPT_NUM_DEFORM_PARMS	(8)
    SOP_EDIT_SCULPT_OP_IDX,
    SOP_EDIT_SCULPT_AXIS_IDX,
    SOP_EDIT_SCULPT_USER_IDX,
    SOP_EDIT_SCULPT_STENCILACCUM_IDX,
    SOP_EDIT_SCULPT_STENCILAPPLY_IDX,
    SOP_EDIT_SCULPT_RESETNORMALS_IDX,
    SOP_EDIT_SCULPT_FLOODAPPLY_IDX,
    SOP_EDIT_SCULPT_COLOR_SWITCHER_IDX,
    SOP_EDIT_SCULPT_FG_IDX,
    SOP_EDIT_SCULPT_BG_IDX,

#define SOP_EDIT_SCULPT_NUM_NIB_PARMS		(18)
    SOP_EDIT_SCULPT_SHAPE_IDX,
    SOP_EDIT_SCULPT_BITMAP_IDX,
    SOP_EDIT_SCULPT_BITMAPCHAN_IDX,
    SOP_EDIT_SCULPT_RAD_IDX,
    SOP_EDIT_SCULPT_UVRAD_IDX,
    SOP_EDIT_SCULPT_RADPRESSURE_IDX,
    SOP_EDIT_SCULPT_USEDEPTH_IDX,
    SOP_EDIT_SCULPT_DEPTH_IDX,
    SOP_EDIT_SCULPT_ANGLE_IDX,
    SOP_EDIT_SCULPT_SQUASH_IDX,
    SOP_EDIT_SCULPT_OPAC_IDX,
    SOP_EDIT_SCULPT_OPACPRESSURE_IDX,
    SOP_EDIT_SCULPT_SPLATTER_IDX,
    SOP_EDIT_SCULPT_PAPER_IDX,
    SOP_EDIT_SCULPT_SOFTEDGE_IDX,
    SOP_EDIT_SCULPT_KERNEL_IDX,
    SOP_EDIT_SCULPT_UPTYPE_IDX,
    SOP_EDIT_SCULPT_UPVECTOR_IDX,

#define SOP_EDIT_SCULPT_NUM_RAY_PARMS		(11)
    SOP_EDIT_SCULPT_PROJTYPE_IDX,
    SOP_EDIT_SCULPT_USECONNECTIVITY_IDX,
    SOP_EDIT_SCULPT_USENORMALS_IDX,
    SOP_EDIT_SCULPT_REALTIME_IDX,
    SOP_EDIT_SCULPT_RAY_ORIENT_IDX,
    SOP_EDIT_SCULPT_RAY_HITPOS_IDX,
    SOP_EDIT_SCULPT_RAY_HITPRIM_IDX,
    SOP_EDIT_SCULPT_RAY_HITUVW_IDX,
    SOP_EDIT_SCULPT_RAY_HITPRESSURE_IDX,
    SOP_EDIT_SCULPT_RAY_HITPT_IDX,
    SOP_EDIT_SCULPT_RAY_EVENT_IDX,

    SOP_EDIT_USEORIENT_IDX,

    // Symmetry Parameters
    SOP_EDIT_USEMIRROR_IDX,
    SOP_EDIT_SYMMETRYAXIS_IDX,
    SOP_EDIT_SYMMETRYORIGIN_IDX,
    SOP_EDIT_SYMMETRYDISTANCE_IDX,
    SOP_EDIT_SYMMETRYTHRESHOLD_IDX,
    SOP_EDIT_SYMMETRYPLANETOL_IDX,

    SOP_EDIT_UPDATENMLS_IDX,

    // Reset Button
    SOP_EDIT_RESETBUTTON_IDX
};

class SOP_API SOP_Edit : public SOP_BrushBase
{
friend class MSS_EditState;
public:
	     SOP_Edit(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Edit();

    virtual int		isRefInput(unsigned int i) const override;
    virtual void	unloadDataHack() override;
    virtual bool	unloadData() override;

    // Allow toggling of operation type
    void	 toggleXformPeak();
    
    virtual bool		 updateParmsFlags() override;

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];
    static PRM_Template		 myObsoleteList[];

    // Get the selection group string:
    // We have to override this since the parameter's index has changed
    virtual void	 getSelString(UT_String &str) override
			    { evalString(str, SOP_EDIT_GRP_IDX, 0, 0.0); }

    virtual void setAccumulateStencil(bool yesno) override
		 {
		     setInt(SOP_EDIT_SCULPT_STENCILACCUM_IDX, 0, 0, yesno);
		 }
    virtual void setRayOrigin(const UT_Vector3 &orig, fpreal t) override
		 {
		     setFloat(SOP_EDIT_SCULPT_RAY_HITPOS_IDX, 0, t, orig.x());
		     setFloat(SOP_EDIT_SCULPT_RAY_HITPOS_IDX, 1, t, orig.y());
		     setFloat(SOP_EDIT_SCULPT_RAY_HITPOS_IDX, 2, t, orig.z());
		 }
    virtual void setRayOrientation(const UT_Vector3 &orient, fpreal t) override
		 {
		     setFloat(SOP_EDIT_SCULPT_RAY_ORIENT_IDX, 0, t, orient.x());
		     setFloat(SOP_EDIT_SCULPT_RAY_ORIENT_IDX, 1, t, orient.y());
		     setFloat(SOP_EDIT_SCULPT_RAY_ORIENT_IDX, 2, t, orient.z());
		 }
    virtual void setHitPrimitive(int primidx, fpreal t) override
		 {
		     setInt(SOP_EDIT_SCULPT_RAY_HITPRIM_IDX, 0, t, primidx);
		 }
    virtual void setHitPoint(int pt, fpreal t) override
		 {
		     setInt(SOP_EDIT_SCULPT_RAY_HITPT_IDX, 0, t, pt);
		 }
    virtual void setHitUVW(fpreal u, fpreal v, fpreal w, fpreal t) override
	 	 {
		     setFloat(SOP_EDIT_SCULPT_RAY_HITUVW_IDX, 0, t, u);
		     setFloat(SOP_EDIT_SCULPT_RAY_HITUVW_IDX, 1, t, v);
		     setFloat(SOP_EDIT_SCULPT_RAY_HITUVW_IDX, 2, t, w);
		 }
    virtual void setHitPressure(fpreal p, fpreal t) override
		 {
		     setFloat(SOP_EDIT_SCULPT_RAY_HITPRESSURE_IDX, 0, t, p);
		 }   
    virtual void setRadius(fpreal r, fpreal t) override
		 {
		     setFloat(SOP_EDIT_SCULPT_RAD_IDX, 0, t, r);
		 }
    virtual void setUVRadius(fpreal r, fpreal t) override
		 {
		     setFloat(SOP_EDIT_SCULPT_UVRAD_IDX, 0, t, r);
		 }
    virtual void setBrushEvent(SOP_BrushEvent event) override
		 {
		     setInt(SOP_EDIT_SCULPT_RAY_EVENT_IDX, 0, 0, int(event));
		 }
    virtual void setBrushOp(SOP_BrushOp op) override
		 {
		     int iop;
		     switch (op)
		     {
			 case SOP_BRUSHOP_SMOOTH:	iop = 1; break;
			 case SOP_BRUSHOP_SMOOTHDEFORM: iop = 2; break;
			 case SOP_BRUSHOP_ERASE:	iop = 3; break;
			 case SOP_BRUSHOP_DEFORM:
			 default:			iop = 0; break;
		     }
		     setInt(SOP_EDIT_SCULPT_OP_IDX, 0, 0, iop);
		 }
    virtual void setBrushShape(SOP_BrushShape shape) override
		 {
		     setInt(SOP_EDIT_SCULPT_SHAPE_IDX, 0, 0, int(shape));
		 }
    virtual void setProjectionType(int projtype) override
		 {
		     setInt(SOP_EDIT_SCULPT_PROJTYPE_IDX, 0, 0, projtype);
		 }
    virtual void useForegroundColor() override
		 {
		     setInt(SOP_EDIT_SCULPT_COLOR_SWITCHER_IDX, 0, 0, 0);
		 }
    virtual void useBackgroundColor() override
		 {
		     setInt(SOP_EDIT_SCULPT_COLOR_SWITCHER_IDX, 0, 0, 1);
		 }

    virtual int		altersGeometry() const override { return 1; }
    virtual int		altersColor() const override { return 0; }

    int		MODE()
			    { return evalInt(SOP_EDIT_MODESWITCHER_IDX,	0, 0); }
    virtual fpreal	RAWRADIUS(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAD_IDX,	0, t); }
    virtual fpreal	RAWUVRADIUS(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_UVRAD_IDX,	0, t); }
    virtual fpreal	RADIUSPRESSURE(fpreal t) override
			{ return evalFloat(SOP_EDIT_SCULPT_RADPRESSURE_IDX, 0, t); }
    virtual fpreal	RAWOPACITY(fpreal t) override
			    {return evalFloat(SOP_EDIT_SCULPT_OPAC_IDX,	0, t); }
    virtual fpreal	OPACITYPRESSURE(fpreal t) override
			{return evalFloat(SOP_EDIT_SCULPT_OPACPRESSURE_IDX, 0, t) ; }

    bool		inBrushMode(fpreal t)
			{ return evalInt("modeswitcher", 0, t); }

    virtual SOP_BrushOp	OP() override	
    { 
	switch (evalInt(SOP_EDIT_SCULPT_OP_IDX, 0, 0))
	{
	    case 0:	return SOP_BRUSHOP_DEFORM;
	    case 1:	return SOP_BRUSHOP_SMOOTH;
	    case 2:	return SOP_BRUSHOP_SMOOTHDEFORM;
	    case 3:	return SOP_BRUSHOP_ERASE;
	    default:	return SOP_BRUSHOP_DEFORM;
	}
    }
    virtual int		ACCUMSTENCIL() override 
			    { return evalInt(SOP_EDIT_SCULPT_STENCILACCUM_IDX, 0,0);}
    virtual int		PROJECTIONTYPE() override
			    { return evalInt(SOP_EDIT_SCULPT_PROJTYPE_IDX, 0, 0); }
    virtual int		REALTIME() override
			    { return evalInt(SOP_EDIT_SCULPT_REALTIME_IDX, 0, 0); }
    virtual int		SHAPE(fpreal t) override
			    { return evalInt(SOP_EDIT_SCULPT_SHAPE_IDX, 0, t); }

    virtual OP_ERROR	cookInputGroups(OP_Context &context,
				int alone = 0) override;

    virtual void	getXformOrder(UT_XformOrder &xord,
				const CH_Channel *chp) const override;

    virtual void	resolveObsoleteParms(
				PRM_ParmList *obsolete_parms) override;

    // This will force myRayIntersect to be recomputed
    void		clearRayIntersect();

    virtual void	beginNewOperation() override;
    virtual void	clearAllDeltas(fpreal t) override;

    // Functions for the BlendShapes SOP hack to only apply scaled deltas
    // without cooking
    void			 applyScaledDeltas(
				    GEO_Detail &apply_gdp,
				    const GDT_CoordinateFrames *coord_frames,
				    float scale,
				    const GA_PointGroup *ptgroup);

    bool			 getCookedFlag() const { return myCookedFlag; }

    void			 setUseOrient(bool f)
			    { setInt(SOP_EDIT_USEORIENT_IDX, 0, 0, f ? 1 : 0); }

    bool		 hasChange(fpreal t);

    virtual const GA_PrimitiveGroup	*getPrimitiveSelection() const override
			    { return myPrimitiveGroup; }

    virtual void		 setVisualization(int val) override;

    // An enum by which functions can report actions taken regarding a
    // particular attribute.
    enum AttribAction
    {
	NO_CHANGE, CHANGED_EXISTING, REPLACED_EXISTING, ADDED_MISSING
    };


protected:
    // Helper class for avoiding evaluating mirror parameters multiple times
    // during a cook.
    class SOP_API MirrorParmCache
    {
    public:
	UT_Vector3	 myAxis;
	UT_Vector3	 myOrigin;
	float		 myDistance;
	float		 myPlaneTol;
	bool		 myUseMirror;
	bool		 myValidFlag;
    };

    void		 clearTransformParms();
    virtual OP_ERROR	 cookMySop(OP_Context &context) override;
    virtual const char	*inputLabel(unsigned idx) const override;

    virtual void	 rollbackCurrentDelta() override;
    void		 mirrorDelta(MirrorParmCache &cache);
    virtual void	 applyAndClearCurrentDelta() override;
    virtual void	 applyPermanentDelta() override;
    virtual void	 initializeInput(OP_Context &context);

    virtual bool	 getGroupCentroid(OP_Context &context,
					  UT_Vector3 &centroid) override;

    virtual void	doErase() override;

    // This function takes the transform parameters and modifies the gdp
    // pointer, while creating the appropriate deltas in myCurrentDelta.
    // It is virtual so that HDK classes may hook into this.
    virtual void	doTransform(MirrorParmCache &mirror_cache,
					     AttribAction &falloff_action);

    virtual int		MERGEMODE() override { return 0; }
    virtual void	SCRIPT(UT_String &, fpreal) override {}

    virtual int		AXIS() override
			    { return   evalInt(SOP_EDIT_SCULPT_AXIS_IDX, 0, 0);  }
    virtual fpreal	USERX(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_USER_IDX, 0, t);  }
    virtual fpreal	USERY(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_USER_IDX, 1, t);  }
    virtual fpreal	USERZ(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_USER_IDX, 2, t);  }

    virtual fpreal	RAYORIENTX(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_ORIENT_IDX,0,t); }
    virtual fpreal	RAYORIENTY(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_ORIENT_IDX,1,t); }
    virtual fpreal	RAYORIENTZ(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_ORIENT_IDX,2,t); }
    virtual fpreal	RAYHITX(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_HITPOS_IDX,0,t); }
    virtual fpreal	RAYHITY(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_HITPOS_IDX,1,t); }
    virtual fpreal	RAYHITZ(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_HITPOS_IDX,2,t); }
    virtual fpreal	RAYHITU(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_HITUVW_IDX, 0,t); }
    virtual fpreal	RAYHITV(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_HITUVW_IDX, 1,t); }
    virtual fpreal	RAYHITW(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_HITUVW_IDX, 2,t); }
    virtual fpreal	RAYHITPRESSURE(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_RAY_HITPRESSURE_IDX, 0, t); }
    virtual int		PRIMHIT(fpreal t) override
			    { return evalInt(SOP_EDIT_SCULPT_RAY_HITPRIM_IDX, 0,t); }
    virtual int		PTHIT(fpreal t) override
			    { return evalInt(SOP_EDIT_SCULPT_RAY_HITPT_IDX, 0, t); }
    virtual int		EVENT() override
			    { return evalInt(SOP_EDIT_SCULPT_RAY_EVENT_IDX,   0,0); }
    virtual bool	USE_FOREGROUND() override
			    { return evalInt(SOP_EDIT_SCULPT_COLOR_SWITCHER_IDX,
								   0, 0) == 0; }
    virtual fpreal FGR(fpreal t) override   { return evalFloat(SOP_EDIT_SCULPT_FG_IDX,	0, t); }
    virtual fpreal FGG(fpreal  ) override   { return 0.0; }
    virtual fpreal FGB(fpreal  ) override   { return 0.0; }
    virtual fpreal BGR(fpreal t) override   { return evalFloat(SOP_EDIT_SCULPT_BG_IDX,	0, t); }
    virtual fpreal BGG(fpreal  ) override   { return 0.0; }
    virtual fpreal BGB(fpreal  ) override   { return 0.0; }

    virtual fpreal	SOFTEDGE(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_SOFTEDGE_IDX, 0, t); }
    virtual void	KERNEL(UT_String &str, fpreal t) override 
			    { evalString(str, SOP_EDIT_SCULPT_KERNEL_IDX, 0, t); }
    virtual int		UPTYPE(fpreal t) override
			    { return evalInt(SOP_EDIT_SCULPT_UPTYPE_IDX, 0, t);}
    virtual fpreal	UPX(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_UPVECTOR_IDX, 0, t);}
    virtual fpreal	UPY(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_UPVECTOR_IDX, 1, t);}
    virtual fpreal	UPZ(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_UPVECTOR_IDX, 2, t);}

    virtual fpreal	PAPERNOISE(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_PAPER_IDX, 0, t);}
    virtual fpreal	SPLATTER(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_SPLATTER_IDX, 0, t); }
    virtual void	BITMAP(UT_String &str, fpreal t) override
			    { evalString(str, SOP_EDIT_SCULPT_BITMAP_IDX, 0, t); }
    virtual int		BITMAPCHAN(fpreal t) override
			    { return evalInt(SOP_EDIT_SCULPT_BITMAPCHAN_IDX, 0, t); }
    virtual fpreal	ANGLE(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_ANGLE_IDX, 0, t); }
    virtual fpreal	SQUASH(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_SQUASH_IDX, 0, t); }

    virtual int		DOSTAMPING() override			{ return 0; }
    virtual int		WRITEALPHA() override			{ return 0; }
    virtual int		OVERRIDECD() override			{ return 0; }
    virtual void	CDNAME(UT_String &, fpreal) override	{ }
    virtual int		OVERRIDEALPHA() override		{ return 0; }
    virtual void	ALPHANAME(UT_String &, fpreal) override	{ }
    virtual int		OVERRIDENML() override { return 0; }
    virtual void	NMLNAME(UT_String & /*str*/, fpreal /*t*/) override {}
    virtual int		ADDATTRIB() override		{ return 0; }

    virtual fpreal SYMMETRYDIRX(fpreal /*t*/) override { return 0.0; }
    virtual fpreal SYMMETRYDIRY(fpreal /*t*/) override { return 0.0; }
    virtual fpreal SYMMETRYDIRZ(fpreal /*t*/) override { return 0.0; }
    virtual fpreal SYMMETRYORIGX(fpreal /*t*/) override { return 0.0; }
    virtual fpreal SYMMETRYORIGY(fpreal /*t*/) override { return 0.0; }
    virtual fpreal SYMMETRYORIGZ(fpreal /*t*/) override { return 0.0; }
    virtual int   DOREFLECTION() override { return 0; }
    virtual int   DOPOINTREFLECTION() override { return 0; }
    virtual int	  DOROTATION() override { return 0; }
    virtual int	  SYMMETRYROT(fpreal /*t*/) override  { return 0; }
    virtual fpreal SYMMETRYDIST(fpreal /*t*/) override { return 0.0; }
    virtual int	  DOUVREFLECTION() override { return 0; }
    virtual fpreal SYMMETRYUVORIGU(fpreal /*t*/) override { return 0.0; }
    virtual fpreal SYMMETRYUVORIGV(fpreal /*t*/) override { return 0.0; }
    virtual fpreal SYMMETRYUVANGLE(fpreal /*t*/) override { return 0.0;}

    virtual int		USECONNECTIVITY() override
			    { return evalInt(SOP_EDIT_SCULPT_USECONNECTIVITY_IDX, 0, 0); }
    virtual int		USENORMALS() override
			    { return evalInt(SOP_EDIT_SCULPT_USENORMALS_IDX, 0, 0); }
    virtual int		USEDEPTH() override
			    { return evalInt(SOP_EDIT_SCULPT_USEDEPTH_IDX, 0, 0); }
    virtual fpreal	DEPTH(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_DEPTH_IDX, 0, t); }
    virtual fpreal	HEIGHT(fpreal t) override
			    { return evalFloat(SOP_EDIT_SCULPT_DEPTH_IDX, 1, t); }


    // Find out if the brush has moved or done anything to cause us to add its
    // effect to the current gdp.
    virtual bool	hasStrokeChanged(fpreal t) override;
    virtual bool	hasStyleChanged(fpreal t) override;

    // Find if the nib file has changed:
    virtual bool	hasNibFileChanged(fpreal t) override;
    virtual bool	hasAccumStencilChanged(fpreal t) override;
    virtual bool	hasNibLookChanged(fpreal t) override;

    // Do we want our cook selection to be updated and displayed?
    virtual bool	 wantsCookSelection() const override { return true; }

    GDT_CoordinateFrames *getCoordinateFrames()
				 { return &myCoordinateFrames; }

    virtual const GA_PointGroup	*getPointGroup() override { return myPointGroup; }
    virtual const GA_Group	*getGroup() { return myGroup; }

    virtual void        syncNodeVersion(const char *old_version,
                                        const char *cur_version,
                                        bool *node_deleted) override;

private: // methods

    // Selection
    int		 GRPTYPE(fpreal t) const
		    { return evalInt(SOP_EDIT_GRPTYPE_IDX, 0, t); }

    // Access to switcher parm
    int		 SWITCHER(fpreal t) const
		    { return evalInt(SOP_EDIT_SWITCHER_IDX, 0, t); }
    void	 SET_SWITCHER(int i)
		    { setInt(SOP_EDIT_SWITCHER_IDX, 0, 0, i); }
    
    // Transform
    int		 TRS(fpreal t) const
		    { return evalInt(SOP_EDIT_TRS_IDX, 0, t); }
    int		 XYZ(fpreal t) const
		    { return evalInt(SOP_EDIT_XYZ_IDX, 0, t); }
    fpreal	 TX(fpreal t) const { return evalFloat(SOP_EDIT_T_IDX, 0, t); }
    fpreal	 TY(fpreal t) const { return evalFloat(SOP_EDIT_T_IDX, 1, t); }
    fpreal	 TZ(fpreal t) const { return evalFloat(SOP_EDIT_T_IDX, 2, t); }
    fpreal	 RX(fpreal t) const { return evalFloat(SOP_EDIT_R_IDX, 0, t); }
    fpreal	 RY(fpreal t) const { return evalFloat(SOP_EDIT_R_IDX, 1, t); }
    fpreal	 RZ(fpreal t) const { return evalFloat(SOP_EDIT_R_IDX, 2, t); }
    fpreal	 SX(fpreal t) const { return evalFloat(SOP_EDIT_S_IDX, 0, t); }
    fpreal	 SY(fpreal t) const { return evalFloat(SOP_EDIT_S_IDX, 1, t); }
    fpreal	 SZ(fpreal t) const { return evalFloat(SOP_EDIT_S_IDX, 2, t); }
    fpreal	 SHEAR_XY(fpreal t) const
		    { return evalFloat(SOP_EDIT_SHEAR_IDX, 0, t); }
    fpreal	 SHEAR_XZ(fpreal t) const
		    { return evalFloat(SOP_EDIT_SHEAR_IDX, 1, t); }
    fpreal	 SHEAR_YZ(fpreal t) const
		    { return evalFloat(SOP_EDIT_SHEAR_IDX, 2, t); }
    fpreal	 PX(fpreal t) const { return evalFloat(SOP_EDIT_P_IDX, 0, t); }
    void	 SET_PX(fpreal f)   { setFloat(SOP_EDIT_P_IDX, 0, 0, f); }
    fpreal	 PY(fpreal t) const { return evalFloat(SOP_EDIT_P_IDX, 1, t); }
    void	 SET_PY(fpreal f)   { setFloat(SOP_EDIT_P_IDX, 1, 0, f); }
    fpreal	 PZ(fpreal t) const { return evalFloat(SOP_EDIT_P_IDX, 2, t); }
    void	 SET_PZ(fpreal f)   { setFloat(SOP_EDIT_P_IDX, 2, 0, f); }   
    fpreal	 PRX(fpreal t) const { return evalFloat(SOP_EDIT_PR_IDX, 0,t); }
    fpreal	 PRY(fpreal t) const { return evalFloat(SOP_EDIT_PR_IDX, 1,t); }
    fpreal	 PRZ(fpreal t) const { return evalFloat(SOP_EDIT_PR_IDX, 2,t); }
    int		 PREXFORM_TRS(fpreal t) const
		    { return evalInt(SOP_EDIT_PREXFORM_TRS_IDX, 0, t); }
    int		 PREXFORM_XYZ(fpreal t) const
		    { return evalInt(SOP_EDIT_PREXFORM_XYZ_IDX, 0, t); }
    fpreal	 PREXFORM_TX(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_T_IDX, 0, t); }
    fpreal	 PREXFORM_TY(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_T_IDX, 1, t); }
    fpreal	 PREXFORM_TZ(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_T_IDX, 2, t); }
    fpreal	 PREXFORM_RX(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_R_IDX, 0, t); }
    fpreal	 PREXFORM_RY(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_R_IDX, 1, t); }
    fpreal	 PREXFORM_RZ(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_R_IDX, 2, t); }
    fpreal	 PREXFORM_SX(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_S_IDX, 0, t); }
    fpreal	 PREXFORM_SY(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_S_IDX, 1, t); }
    fpreal	 PREXFORM_SZ(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_S_IDX, 2, t); }
    fpreal	 PREXFORM_SHEAR_XY(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_SHEAR_IDX, 0, t); }
    fpreal	 PREXFORM_SHEAR_XZ(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_SHEAR_IDX, 1, t); }
    fpreal	 PREXFORM_SHEAR_YZ(fpreal t) const
		    { return evalFloat(SOP_EDIT_PREXFORM_SHEAR_IDX, 2, t); }
    int		 KEEP(fpreal t) const
		    { return evalInt(SOP_EDIT_KEEP_IDX, 0, t); }
    bool	 LOCALSPACE(fpreal t) const
		    { return evalInt( SOP_EDIT_LOCALSPACE_IDX, 0, t) == 1; }	

    fpreal	 LOCAL_UPX(fpreal t) const
		    { return evalFloat(SOP_EDIT_LOCALSPACEUPVECTOR_IDX, 0, t); }
    fpreal	 LOCAL_UPY(fpreal t) const
		    { return evalFloat(SOP_EDIT_LOCALSPACEUPVECTOR_IDX, 1, t); }
    fpreal	 LOCAL_UPZ(fpreal t) const
		    { return evalFloat(SOP_EDIT_LOCALSPACEUPVECTOR_IDX, 2, t); }
    bool	 SLIDEONSURFACE(fpreal t) const
		    { return evalInt( SOP_EDIT_SLIDEONSURFACE_IDX, 0, t) == 1; }

    // Peak
    fpreal	 DIST(fpreal t) const
			{ return evalFloat(SOP_EDIT_DIST_IDX, 0, t); }
    int          LEADNML(fpreal t) const
			{ return evalInt(SOP_EDIT_LEADNML_IDX, 0, t); }
    
    // Soft
    fpreal       SOFTRADIUS(fpreal t) const
			{ return evalFloat(SOP_EDIT_SOFTRADIUS_IDX, 0, t); }
    int          SOFTTYPE(fpreal t) const
			{ return evalInt(SOP_EDIT_SOFTTYPE_IDX, 0, t); }
    fpreal       TANDEG0(fpreal t) const
			{ return evalFloat(SOP_EDIT_TANGENT_IDX, 0, t); }
    fpreal       TANDEG1(fpreal t) const
			{ return evalFloat(SOP_EDIT_TANGENT_IDX, 1, t); }
    void         EDIT_KERNEL(UT_String &str, fpreal t)
			{ evalString(str, SOP_EDIT_KERNEL_IDX, 0, t); }
    int          DIST_METRIC(fpreal t) const
			{ return evalInt(SOP_EDIT_DIST_METRIC_IDX, 0, t); }
    bool         APPLY_ROLLOFF(fpreal t) const
                        { return evalInt(SOP_EDIT_APPLY_ROLLOFF_IDX, 0, t) != 0; }
    void         DIST_ATTR(UT_String& str, fpreal t) const
                        { evalString(str, SOP_EDIT_DIST_ATTR_IDX, 0, t); }
    void         LEAD_ATTR(UT_String& str, fpreal t) const
                        { evalString(str, SOP_EDIT_LEAD_POINT_ATTR_IDX, 0, t); }
    int          VISUALIZE_FALLOFF(fpreal t) const
			{ return evalInt(SOP_EDIT_VISFALLOFF_IDX, 0, t); }
    bool         falloffAttributeRequested(fpreal t) const;

    bool         USEORIENT(fpreal t) const
			{ return evalInt(SOP_EDIT_USEORIENT_IDX, 0, t) != 0; }

    // Symmetry
    bool         USEMIRROR(fpreal t) const
			{ return evalInt(SOP_EDIT_USEMIRROR_IDX, 0, t) != 0; }
    fpreal	 SYMMETRYAXISX(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYAXIS_IDX, 0, t); }
    fpreal	 SYMMETRYAXISY(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYAXIS_IDX, 1, t); }
    fpreal	 SYMMETRYAXISZ(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYAXIS_IDX, 2, t); }
    fpreal	 SYMMETRYORIGINX(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYORIGIN_IDX, 0, t); }
    fpreal	 SYMMETRYORIGINY(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYORIGIN_IDX, 1, t); }
    fpreal	 SYMMETRYORIGINZ(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYORIGIN_IDX, 2, t); }
    fpreal	 SYMMETRYDISTANCE(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYDISTANCE_IDX, 0, t); }
    fpreal	 SYMMETRYTHRESHOLD(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYTHRESHOLD_IDX, 0, t); }
    fpreal	 SYMMETRYPLANETOL(fpreal t) const
			{ return evalFloat(SOP_EDIT_SYMMETRYPLANETOL_IDX, 0, t); }

    int          UPDATENMLS(fpreal t) const
			{ return evalInt(SOP_EDIT_UPDATENMLS_IDX, 0, t); }

    void	 updateMirrorParmCache(MirrorParmCache &cache, fpreal t);
    bool	 getSoftSymmetryParms(GEO_Detail::SoftSymmetryParms &symmetry,
				      MirrorParmCache &cache, fpreal t);

    // Transform / Peak
    bool	 isTransform(fpreal t) const	{ return SWITCHER(t) == 0; }
    bool	 isPeak(fpreal t) const		{ return SWITCHER(t) != 0; }
    void	 doPeak(fpreal t, MirrorParmCache &mirror_cache,
			AttribAction &falloff_action);

    // Soft-related methods
    bool	 isSoft(fpreal t) const		{ return SOFTRADIUS(t) != 0.0; }
    
private: // data

    UT_Vector3			 myInitialCentroid;

    GDT_CoordinateFrames	 myCoordinateFrames;
    bool			 myUsedOrient;
    int				 myCurrentInputId;
    int				 myCurrentGeoCookCount;
    int				 myReferenceInputId;
    int				 myReferenceGeoCookCount;
    const GA_PrimitiveGroup	*myPrimitiveGroup;
    const GA_PointGroup		*myPointGroup;
    UT_String			 myPrimitiveClosureName;
    UT_String			 myPointClosureName;

    bool			 myVisualizationFlag;
    bool			 mySetFalloffOnLastCook;
    bool			 myOverwroteInputFalloff;

    // cached data for soft transforms
    GU_SurfaceDistanceCache	mySurfaceDistanceCache;

    // syncNodeVersion is called after resolveObsoleteParms, so we save what the
    // value of IgnorePointConnectivity is in resolveObsoleteParms, then if we
    // need to we use it in syncNodeVersion
    bool                         myPreSoftDistanceMetric;
};

#endif
