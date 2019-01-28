/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: The UV Transform SOP
 * 	  
 * 	     Note: This is the first example of a SOP which uses
 * 	     the GDT functionality as a means towards optimal
 * 	     interactive performance.  Notice how it does not
 * 	     use the "beginNewOperation" functionality
 * 	     of SOP_GDT.  However, SOP_GDT provides facilities
 * 	     for restoring the input geometry extremely quickly.
 *
 * 	     IMPORTANT: Any changes made to this SOP will probably
 * 	     need to be made to SOP_UVEdit, and vice versa!!!
 *
 */

#ifndef __SOP_UVTransform_h__
#define __SOP_UVTransform_h__

#include "SOP_API.h"
#include "SOP_GDT.h"
#include <GEO/GEO_SoftTransformCache.h>

class PRM_MenuEntry;

enum {
    // Group parameters
    SOP_UVTRANSFORM_GRP_IDX = SOP_GDT_GRP_IDX,
    SOP_UVTRANSFORM_GRPTYPE_IDX,

    SOP_UVTRANSFORM_UVATTRIB_IDX,

    // Transform parameters
    SOP_UVTRANSFORM_TRS_IDX,
    SOP_UVTRANSFORM_XYZ_IDX,
    SOP_UVTRANSFORM_T_IDX,
    SOP_UVTRANSFORM_R_IDX,
    SOP_UVTRANSFORM_S_IDX,
    SOP_UVTRANSFORM_SHEAR_IDX,
    SOP_UVTRANSFORM_P_IDX,

    // Soft Parameters
    SOP_UVTRANSFORM_SOFTPARMSGROUPER_IDX,

#define SOP_UVTRANSFORM_NUM_SOFT_PARMS		(8)
    SOP_UVTRANSFORM_RADIUS_IDX,
    SOP_UVTRANSFORM_SOFTTYPE_IDX,
    SOP_UVTRANSFORM_TANGENT_IDX,
    SOP_UVTRANSFORM_KERNEL_IDX,
    SOP_UVTRANSFORM_METRIC_IDX,
    SOP_UVTRANSFORM_GLOBAL_IDX,
    SOP_UVTRANSFORM_UVGLOBAL_IDX,
    SOP_UVTRANSFORM_VISFALLOFF_IDX
};

class SOP_API SOP_UVTransform : public SOP_GDT
{
public:
	     SOP_UVTransform(OP_Network *net, const char *name,
			     OP_Operator *entry);
    virtual ~SOP_UVTransform();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];
    static PRM_Template		 myObsoleteList[];
    static CH_LocalVariable	 myVariables[];

    virtual bool	 evalVariableValue(fpreal &v, int index, int thread);
    // create overload that delegates to super class to avoid shadow warnings
    virtual bool	 evalVariableValue(UT_String &v, int i, int thread)
			 { return SOP_Node::evalVariableValue(v, i, thread); }

    virtual OP_ERROR		 cookInputGroups(OP_Context &context,
						 int alone = 0);

    virtual void		 resolveObsoleteParms(
						PRM_ParmList *obsolete_parms);

    virtual void		 setVisualization(int val);

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

    virtual bool		 getGroupCentroid(OP_Context &context,
						  UT_Vector3 &centroid);

    virtual void		 getXformOrder(UT_XformOrder &xord, 
					       const CH_Channel *chp) const;


#ifdef INTEL_COMPILER
private:					
	SOP_UVTransform(const SOP_UVTransform& ) : SOP_GDT(NULL, NULL, NULL) { }
#endif					
private:
    // Selection
    int		 GRPTYPE() const
		    { return evalInt(SOP_UVTRANSFORM_GRPTYPE_IDX, 0, 0); }

    // Transform
    int		 TRS() const
		    { return evalInt(SOP_UVTRANSFORM_TRS_IDX, 0, 0); }
    int		 XYZ() const
		    { return evalInt(SOP_UVTRANSFORM_XYZ_IDX, 0, 0); }
    fpreal	 TX(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_T_IDX, 0, t); }
    fpreal	 TY(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_T_IDX, 1, t); }
    fpreal	 TZ(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_T_IDX, 2, t); }
    fpreal	 RX(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_R_IDX, 0, t); }
    fpreal	 RY(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_R_IDX, 1, t); }
    fpreal	 RZ(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_R_IDX,	2, t); }
    fpreal	 SX(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_S_IDX, 0, t); }
    fpreal	 SY(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_S_IDX, 1, t); }
    fpreal	 SZ(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_S_IDX, 2, t); }
    fpreal	 SHEAR_XY(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_SHEAR_IDX, 0, t); }
    fpreal	 SHEAR_XZ(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_SHEAR_IDX, 1, t); }
    fpreal	 SHEAR_YZ(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_SHEAR_IDX, 2, t); }
    fpreal	 PX(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_P_IDX, 0, t); }
    fpreal	 PY(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_P_IDX, 1, t); }
    fpreal	 PZ(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_P_IDX, 2, t); }
    
    // Soft
    fpreal	 RADIUS(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_RADIUS_IDX, 0, t); }
    int          SOFTTYPE() const
		    { return evalInt(SOP_UVTRANSFORM_SOFTTYPE_IDX, 0, 0); }
    fpreal	 TANDEG0(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_TANGENT_IDX,0, t); }
    fpreal	 TANDEG1(fpreal t) const
		    { return evalFloat(SOP_UVTRANSFORM_TANGENT_IDX,1, t); }
    void         KERNEL(UT_String &str, fpreal t) const
		    { evalString(str, SOP_UVTRANSFORM_KERNEL_IDX, 0, t); }
    int          METRIC() const
		    { return evalInt(SOP_UVTRANSFORM_METRIC_IDX, 0, 0); }
    int          GLOBAL() const
		    { return evalInt(SOP_UVTRANSFORM_GLOBAL_IDX, 0, 0); }
    int          UVGLOBAL() const
		    { return evalInt(SOP_UVTRANSFORM_UVGLOBAL_IDX,0,0); }
    int		 VISUALIZE_FALLOFF(fpreal t) const
		    { return evalInt(SOP_UVTRANSFORM_VISFALLOFF_IDX, 0, t); }
    bool	 falloffAttributeRequested(fpreal t) const;

    // Group Calculations
    void	 computeTransformGroups(GA_AttributeOwner owner, bool changed_input);
    void	 computeSelectionCentroid(GA_AttributeOwner owner);

    // Full Bounding Box Calculation
    void	 computeBBox(GA_AttributeOwner owner);

    // UV Transform
    void	 doTransform(GA_AttributeOwner owner, const GA_RWHandleV3 &uvattrib, fpreal t, AttribAction &falloff_action);

    GA_AttributeOwner myOwner;
    bool		 myCentroidComputed;
    UT_Vector3	 	 myInitialCentroid;
    bool		 myBBoxComputed;
    UT_BoundingBox	 myBBox;
    
    GA_VertexGroup	*myVtxGroup;
    GA_PointGroup	*myPtGroup;
    
    GEO_SoftTransformCache mySoftCache;
    bool		 mySoftCacheDirty;

    bool		 myVisualizationFlag;
    bool		 mySetFalloffOnLastCook;
    bool		 myOverwroteInputFalloff;
};

#endif
