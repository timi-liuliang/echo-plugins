/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimAgentShape.h (GT Library, C++)
 *
 * COMMENTS:
 */
#ifndef GT_PrimAgentShape_h
#define GT_PrimAgentShape_h

#include "GT_API.h"
#include "GT_GEOPackedAgent.h"
#include "GT_AgentSupport.h"

#include <GU/GU_DetailHandle.h>

#include <UT/UT_IntArray.h>
#include <UT/UT_Map.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_NonCopyable.h>

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Types.h>

class RE_VertexArray;

/// A shape belonging to agents, instanced with material assignments.
class GT_API GT_PrimAgentShape : public GT_Primitive
{
public:
    enum ShapeClass
    {
	STATIC,
	DEFORMING
    };
	     GT_PrimAgentShape();
	     GT_PrimAgentShape(const GT_PrimAgentShape &shape);
	     GT_PrimAgentShape(const GT_PrimitiveHandle &geometry,
			       const GU_ConstDetailHandle &dh,
			       const GT_ShapeLODGroupHandle &agent_group,
			       ShapeClass shape_class,
			       int lod_level,
			       GT_DataArray *mat_remap);
    virtual ~GT_PrimAgentShape();

    /// Whether the shape's geometry is static or deformed
    ShapeClass		      getShapeClass() const;

    /// shape geometry.
    const GT_PrimitiveHandle &geometry() const;
    GU_ConstDetailHandle     &detail() const;

    /// Access to the shared data between LOD levels of the same agent shape
    const GT_ShapeLODGroupHandle &shapeGroup() const;
    
    /// Which LOD level this shape represents: 0=primary, 1+ lower resolutions
    int			      lodLevel() const;

    /// one set of transforms per material.
    exint		     getNumMatGroups() const;
    exint		     getMatGroupNumInstances(exint grp_idx) const;
    int			     getMatGroupMaterialID(exint grp_idx) const;
    const UT_IntArray	    &getMatGroupAgentPrimIDs(exint grp_idx) const;
    const UT_IntArray	    &getMatGroupAgentPointIDs(exint grp_idx) const;

    GT_DataArrayHandle	     getMatRemapAttrib() const { return myMatRemap; }

    int			    getNumTransformsPerInstance() const;

    /// Import computed transforms into 'xforms'. If packed_xform_only is true,
    /// only the agent transforms are returned. If false, the rig * agent
    /// transforms are returned.
    void  getMatGroupTransforms(exint grp_idx, UT_Matrix4FArray &xforms,
				bool packed_xform_only) const;

    /// Copy transforms into a fp32 mat buffer. Must be num_instances in size.
    void  fetchMatGroupTransforms(int idx,
				  UT_Matrix4F *buf,
				  bool packed_xform_only) const;

    /// Copy rig transforms into a fp32 4x3 matrix buffer.
    /// Must be num_instances * getNumTransformsPerInstance() in size.
    void  fetchMatGroupRigTransforms(int idx, fpreal32 *buf) const;

    /// Copy colors into a 4 x uint8 buffer.
    void fetchMatGroupColors(int idx, uint8 *buf) const;

    /// Return a list of visible instances in this material group, based on the
    /// visibility info computed by the GT_GEOPackedAgent. returns -1 if
    /// only some are visible, 0 if none are visible, and 1 if all are visible.
    /// The 'lod_bias' is a factor that will favour lower LOD levels if > 1,
    /// and higher LOD levels if < 1. A bias of zero will disable LOD. 
    int			    getVisibleInstances(int grp_idx,
						UT_IntArray &inst_idx,
						int lod_base = 0.0,
						fpreal lod_bias = 1.0) const;

    
    // non-const methods for GT_GEOPackedAgent to modify the shapes
    
    /// Add a instance 
    void		addShapeInstance(int xform_index,
					 int material_id,
					 GA_Index agent_prim_idx,
					 GA_Index agent_pnt_idx,
					 int shape_binding);

    /// Transform arrays assigned by GT_GEOPackedAgent
    void		setDeformTransformMap(UT_IntArray *map);
    void		setInvRestTransforms(UT_Matrix4FArray *rest);

    
    // GT_Primitive pure virtual overrides
    virtual const char *className() const {  return "GT_PrimAgentShape"; }
    virtual int		getPrimitiveType() const { return GT_PRIM_AGENT_SHAPE; }
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
                                     int nsegments) const
                                { doEnlargeBounds(boxes, nsegments, false); }
    virtual void	enlargeRenderBounds(UT_BoundingBox boxes[],
                                            int nsegments) const
                                { doEnlargeBounds(boxes, nsegments, true); }
    virtual int		getMotionSegments() const { return 1; }
    virtual int64	getMemoryUsage() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimAgentShape(*this); }
    virtual GT_PrimitiveHandle	doHarden() const { return doSoftCopy(); }

    virtual bool        refine(GT_Refine &refiner,
                               const GT_RefineParms *parms = NULL) const;
 
    virtual bool	getTopologyVersion(int64 &version) const;

    class ShapeInstance
    {
    public:
	int		 myMaterialID;
	UT_IntArray	 myTransforms;
	UT_IntArray 	 myPrimIDs;
	UT_IntArray 	 myPointIDs;
	UT_IntArray	 myBindings;
    };

    // Stylesheet agent shape support. Unstyled groups are those with just
    // primitive agent material assignments, whereas styled groups have the
    // shape assignments as well.
    int			 getNumUnstyledGroups() const;
    ShapeInstance	&getShapeInstancesForUnstyledGroup(int mat_group);
    void		 resetStyledMatGroups();
    ShapeInstance	*addStyledMatGroup(int mat_id);
    
private:
    void doEnlargeBounds(UT_BoundingBox boxes[], int nsegments,
                         bool render) const;

    UT_SortedMap<int,int>	myMatGroupMap;

private:

    UT_Array<ShapeInstance>     myMatGroup;
    UT_Array<ShapeInstance *>	myStyledMatGroup;
    bool			myUseStyledMatGroups; 

    GT_ShapeLODGroupHandle	myLODGroup;
    GT_PrimitiveHandle		myGeometry;
    GU_ConstDetailHandle	myDetail;
    ShapeClass			myShapeClass;
    mutable UT_Vector2iArray	myVisibleInstances;
    int				myLODLevel;
    GT_DataArrayHandle		myMatRemap;
};


inline const GT_PrimitiveHandle &
GT_PrimAgentShape::geometry() const
{ return myGeometry; }

inline exint
GT_PrimAgentShape::getNumMatGroups() const
{
    return myUseStyledMatGroups ? myStyledMatGroup.entries()
				: myMatGroup.entries();
}

inline exint
GT_PrimAgentShape::getMatGroupNumInstances(exint i) const
{
    const ShapeInstance &inst = myUseStyledMatGroups ? *myStyledMatGroup(i)
						     : myMatGroup(i);
    return inst.myPrimIDs.entries();
}

inline int
GT_PrimAgentShape::getMatGroupMaterialID(exint i) const
{
    const ShapeInstance &inst = myUseStyledMatGroups ? *myStyledMatGroup(i)
						     : myMatGroup(i);
    return inst.myMaterialID;
}
    
inline const UT_IntArray &
GT_PrimAgentShape::getMatGroupAgentPrimIDs(exint i) const
{
    const ShapeInstance &inst = myUseStyledMatGroups ? *myStyledMatGroup(i)
						     : myMatGroup(i);
    return inst.myPrimIDs;
}
    
inline const UT_IntArray &
GT_PrimAgentShape::getMatGroupAgentPointIDs(exint i) const
{
    const ShapeInstance &inst = myUseStyledMatGroups ? *myStyledMatGroup(i)
						     : myMatGroup(i);
    return inst.myPointIDs;
}

inline int
GT_PrimAgentShape::getNumUnstyledGroups() const
{
    return myMatGroup.entries();
}

inline GT_PrimAgentShape::ShapeInstance	&
GT_PrimAgentShape::getShapeInstancesForUnstyledGroup(int mat_group)
{
    return myMatGroup(mat_group);
}

inline void
GT_PrimAgentShape::resetStyledMatGroups()
{
    for(auto grp : myStyledMatGroup)
	delete grp;
    myStyledMatGroup.entries(0);
    myUseStyledMatGroups = false;
}

inline GT_PrimAgentShape::ShapeInstance *
GT_PrimAgentShape::addStyledMatGroup(int mat_id)
{
    myUseStyledMatGroups = true;
    myStyledMatGroup.append(new ShapeInstance);
    myStyledMatGroup.last()->myMaterialID = mat_id;
    return myStyledMatGroup.last();
}

inline GU_ConstDetailHandle &
GT_PrimAgentShape::detail() const
{ return const_cast<GT_PrimAgentShape *>(this)->myDetail; }

inline GT_PrimAgentShape::ShapeClass
GT_PrimAgentShape::getShapeClass() const
{ return myShapeClass; }

inline int
GT_PrimAgentShape::lodLevel() const
{ return myLODLevel; }

inline const GT_ShapeLODGroupHandle &
GT_PrimAgentShape::shapeGroup() const
{ return myLODGroup; }

#endif


