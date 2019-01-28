/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The UV Edit SOP
 *
 * 	     IMPORTANT: Any changes made to this SOP will probably
 * 	     need to made to SOP_UVTransform, and vice versa!!!
 *
 */

#ifndef __SOP_UVEdit_h__
#define __SOP_UVEdit_h__

#include "SOP_API.h"
#include "SOP_GDT.h"
#include <GEO/GEO_SoftTransformCache.h>

class PRM_MenuEntry;

enum {
    // Group parameters
    SOP_UVEDIT_GRP_IDX = SOP_GDT_GRP_IDX,
    SOP_UVEDIT_GRPTYPE_IDX,

    SOP_UVEDIT_UVATTRIB_IDX,

    // Transform parameters
    SOP_UVEDIT_TRS_IDX,
    SOP_UVEDIT_XYZ_IDX,
    SOP_UVEDIT_T_IDX,
    SOP_UVEDIT_R_IDX,
    SOP_UVEDIT_S_IDX,
    SOP_UVEDIT_SHEAR_IDX,
    SOP_UVEDIT_P_IDX,

    // Soft Parameters
    SOP_UVEDIT_SOFTPARMSGROUPER_IDX,

#define SOP_UVEDIT_NUM_SOFT_PARMS		(8)
    SOP_UVEDIT_RADIUS_IDX,
    SOP_UVEDIT_SOFTTYPE_IDX,
    SOP_UVEDIT_TANGENT_IDX,
    SOP_UVEDIT_KERNEL_IDX,
    SOP_UVEDIT_METRIC_IDX,
    SOP_UVEDIT_GLOBAL_IDX,
    SOP_UVEDIT_UVGLOBAL_IDX,
    SOP_UVEDIT_VISFALLOFF_IDX,

    // UV Sew Parameters
    SOP_UVEDIT_UVSEW_IDX

};

class SOP_API SOP_UVEdit : public SOP_GDT
{
friend class MSS_UVEditState;
public:
	     SOP_UVEdit(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_UVEdit();

    // Parameter indices (same as above, but different names):
    enum
    {
	I_GROUP = SOP_GDT_GRP_IDX,
	I_GROUPTYPE,
        I_ATTRIB,
	I_TRS,
	I_XYZ,
	I_T,
	I_R,
	I_S,
	I_SHEAR,
	I_P
    };

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];
    static PRM_Template		 myObsoleteList[];

    virtual OP_ERROR		 cookInputGroups(OP_Context &context,
						 int alone = 0);

    virtual void		 getXformOrder(UT_XformOrder &xord, 
					       const CH_Channel *chp) const;

    virtual void		 setVisualization(int val);

    void			 TOGGLE_UVSEW();

    // An enum by which functions can report actions taken regarding a
    // particular attribute.
    enum AttribAction
    {
	NO_CHANGE, CHANGED_EXISTING, REPLACED_EXISTING, ADDED_MISSING
    };

    virtual void getDescriptiveParmName(UT_String &name) const
    {
        name = "uvattrib";
    }

protected:
    virtual OP_ERROR		 cookMySop(OP_Context &context);
    virtual const char		*inputLabel(unsigned idx) const;

    virtual void		 applyAndClearCurrentDelta();

    virtual bool		 getGroupCentroid(OP_Context &context,
						  UT_Vector3 &centroid);

private:					  
#ifdef INTEL_COMPILER
	SOP_UVEdit(const SOP_UVEdit& ) : SOP_GDT(NULL, NULL, NULL) { }
#endif
	
private:
    // Selection
    int		 GRPTYPE() const	{ return evalInt(I_GROUPTYPE, 0, 0); }

    // Transform
    int		 TRS() const		{ return evalInt(I_TRS, 0, 0); }
    int		 XYZ() const		{ return evalInt(I_XYZ, 0, 0); }
    fpreal	 TX() const		{ return evalFloat(I_T, 0, 0); }
    fpreal	 TY() const		{ return evalFloat(I_T, 1, 0); }
    fpreal	 TZ() const		{ return evalFloat(I_T, 2, 0); }
    fpreal	 RX() const		{ return evalFloat(I_R, 0, 0); }
    fpreal	 RY() const		{ return evalFloat(I_R, 1, 0); }
    fpreal	 RZ() const		{ return evalFloat(I_R, 2, 0); }
    fpreal	 SX() const		{ return evalFloat(I_S, 0, 0); }
    fpreal	 SY() const		{ return evalFloat(I_S, 1, 0); }
    fpreal	 SZ() const		{ return evalFloat(I_S, 2, 0); }
    fpreal	 SHEAR_XY() const	{ return evalFloat(I_SHEAR, 0, 0); }
    fpreal	 SHEAR_XZ() const	{ return evalFloat(I_SHEAR, 1, 0); }
    fpreal	 SHEAR_YZ() const	{ return evalFloat(I_SHEAR, 2, 0); }
    fpreal	 PX() const		{ return evalFloat(I_P, 0, 0); }
    void	 SET_PX(float f)	{ setFloat(I_P, 0, 0, f); }
    fpreal	 PY() const		{ return evalFloat(I_P, 1, 0); }
    void	 SET_PY(float f)	{ setFloat(I_P, 1, 0, f); }
    fpreal	 PZ() const		{ return evalFloat(I_P, 2, 0); }
    void	 SET_PZ(float f)	{ setFloat(I_P, 2, 0, f); }

    // Soft
    fpreal	 RADIUS() const		{ return evalFloat(
					    SOP_UVEDIT_RADIUS_IDX, 0, 0); }
    int          SOFTTYPE() const	{ return evalInt(
					    SOP_UVEDIT_SOFTTYPE_IDX, 0, 0); }
    fpreal	 TANDEG0() const	{ return evalFloat(
					    SOP_UVEDIT_TANGENT_IDX,0, 0); }
    fpreal	 TANDEG1() const	{ return evalFloat(
					    SOP_UVEDIT_TANGENT_IDX,1, 0); }
    void         KERNEL(UT_String &str) const
					{ evalString(str,
					    SOP_UVEDIT_KERNEL_IDX, 0, 0); }
    int          METRIC() const		{ return evalInt(
					    SOP_UVEDIT_METRIC_IDX, 0, 0); }
    int          GLOBAL() const		{ return evalInt(
					    SOP_UVEDIT_GLOBAL_IDX, 0, 0); }
    int          UVGLOBAL() const	{ return evalInt(
					    SOP_UVEDIT_UVGLOBAL_IDX,0,0); }
    int		 VISUALIZE_FALLOFF(fpreal t) const
			{ return evalInt(SOP_UVEDIT_VISFALLOFF_IDX, 0, t); }
    bool	 falloffAttributeRequested(fpreal t) const;

    // UVSew
    int		 UVSEW() const		{ return evalInt(
					    SOP_UVEDIT_UVSEW_IDX, 0, 0); }


    // Group Calculations
    void	 computeTransformGroups(GA_AttributeOwner owner, bool changed_input);
    void	 computeSelectionCentroid(GA_AttributeOwner owner, UT_Vector3 &centroid);

    // UV Transform
    void	 doTransform(GA_AttributeOwner owner, const GA_RWHandleV3 &uvattrib, fpreal t, AttribAction &falloff_action);
    // Soft-related methods
    bool	 isSoft() const { return RADIUS() != 0.0; }
	
    UT_Vector3	 	 myInitialCentroid;
    
    GA_VertexGroup	*myVtxGroup;
    GA_PointGroup	*myPtGroup;
    
    GEO_SoftTransformCache mySoftCache;
    bool		 mySoftCacheDirty;

    bool		 myVisualizationFlag;
    bool		 mySetFalloffOnLastCook;
    bool		 myOverwroteInputFalloff;
};

#endif
