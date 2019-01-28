/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimVolume.h ( GT Library, C++)
 *
 * COMMENTS:
 */
#ifndef GT_PrimVolume_h
#define GT_PrimVolume_h

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_GEOPrimCollect.h"
#include "GT_AttributeList.h"
#include <GEO/GEO_PrimVolume.h>
#include <GU/GU_DetailHandle.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_VoxelArray.h>


class GEO_Primitive;
class GU_PrimVolumeCache;
class GU_PrimVolumeCacheParms;

class GT_API GT_PrimVolume : public GT_Primitive
{
public:
    GT_PrimVolume(const GU_ConstDetailHandle &gdh,
		  const GEO_Primitive *prim,
		  const GT_DataArrayHandle &mat_id);
    GT_PrimVolume(const GT_PrimVolume &src);
    virtual ~GT_PrimVolume();

    virtual int		getPrimitiveType() const { return GT_PRIM_VOXEL_VOLUME; }

    virtual const char *className() const { return "GT_PrimVolume"; }
    
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				      int nsegments) const;
    virtual int		getMotionSegments() const { return 1; }
    virtual int64	getMemoryUsage() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimVolume(*this); }

    virtual bool	refine(GT_Refine &refiner,
			       const GT_RefineParms *parms) const;
    bool		refineHeightField(GT_Refine &refiner,
			       const GT_RefineParms *parms) const;

    // Isosurface
    bool		isIsosurface() const;
    GT_PrimitiveHandle	buildIsosurface(const GT_RefineParms &p,
					bool as_points) const;
    GU_ConstDetailHandle	getIsosurfaceDetail() const;
    UT_Vector3		getIsoCd() const;

    // Height fields
    bool		isHeightField() const;
    GT_PrimitiveHandle	buildHeightField(const GT_RefineParms &p) const;
    GT_PrimitiveHandle	buildHeightMesh(const GT_RefineParms &p,
					UT_Vector2i &map_res) const;
    enum gt_BlendMask
    {
	BlendNone,
	BlendMissingMask,
	BlendMask,
    };
    gt_BlendMask	blendMaskState(int index) const;

    // 2D/3D volume
    bool		isSmokeVolume() const;
    bool		is3DVolume() const;
    GEO_PrimVolumeXform getVolumeSpace() const;

    UT_Vector3I		getRes() const;

    void		get2DBounds(UT_Vector3F &p0, UT_Vector3F &p1,
				    UT_Vector3F &p2, UT_Vector3F &p3) const;
				    
    const UT_VoxelArray<UT_Vector4> *getVoxels(const GU_PrimVolumeCacheParms &p,
					       int64 version[2]) const;

    GEO_PrimVolumeXform getVolumeTransform() const;
    GEO_PrimVolumeXform getVolumeIndexTransform() const;
    GT_PrimitiveHandle	getPointPrimitive();
    const GEO_Primitive	*getGeoPrimitive() const
			    { return getPrimitive(); }

    exint		 getNumVolumeAttributes() const
				{ return myVolumeAttributes.entries(); }
    const GEO_Primitive	*getVolumeAttribute(exint i) const
				{ return myVolumeAttributes(i); }
    void		 appendVolumeAttribute(const GEO_Primitive *prim)
				{ myVolumeAttributes.append(prim); }

    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniformList; }
    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);

    bool		getFlip() const { return myFlip; }
    bool		isVolumeReduced() const { return myVolumeReduced; }
    
private:
    const GEO_Primitive *getPrimitive() const;
    
    GU_PrimVolumeCache		*myCache;
    GT_AttributeListHandle	 myUniformList;

    // Is either VDB or Volume.
    GU_ConstDetailHandle		myDetail;
    GA_Offset				myPrimOffset;
    UT_Array<const GEO_Primitive *>	myVolumeAttributes;
    mutable int64			myCacheVersion[2];
    mutable bool			myIsosurfaceFlag;
    mutable bool			mySmokeVolumeFlag;
    mutable bool			myHeightFieldFlag;
    mutable bool			myIs3DVolume;
    mutable bool			myFlip;
    mutable GT_PrimitiveHandle		myMesh;
    mutable UT_Vector2i			myMeshSize;
    mutable UT_Vector3F			myMeshP0;
    mutable UT_Vector3F			myMeshP1;
    mutable UT_Vector3F			myMeshP2;
    mutable UT_Vector3F			myMeshP3;
    mutable UT_Array<gt_BlendMask>	myBlendMaskState;
    mutable bool			myVolumeReduced;
    static  int64			theMeshDataID;
    static  int64			theMeshTopID;
};
 
/// Trivial collector class for Volume - 1 GT_PrimVolume per Volume primitive
/// with the visualization set to 'volume'.
class GT_API GT_GEOPrimCollectVolume: public GT_GEOPrimCollect
{
public:
		    GT_GEOPrimCollectVolume() {}
    virtual	   ~GT_GEOPrimCollectVolume() {}

    virtual GT_GEOPrimCollectData *
		beginCollecting(const GT_GEODetailListHandle &geometry,
				const GT_RefineParms *parms) const;

    virtual GT_PrimitiveHandle collect(const GT_GEODetailListHandle &,
				       const GEO_Primitive *const* prim_list,
				       int,
				       GT_GEOPrimCollectData *) const;

    virtual GT_PrimitiveHandle
		endCollecting(const GT_GEODetailListHandle &geometry,
				GT_GEOPrimCollectData *data) const;
};

#endif
