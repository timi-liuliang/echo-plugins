/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimVDB.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef GT_PrimVDB_h
#define GT_PrimVDB_h

#include "GT_API.h"
#include "GT_DataArray.h"
#include "GT_Primitive.h"
#include "GT_GEOPrimCollect.h"
#include <GEO/GEO_PrimVDB.h>
#include <UT/UT_VectorTypes.h>

// For quick testing of codepaths with no VDB support.
//#undef OPENVDB_ENABLED

#ifdef OPENVDB_ENABLED
#include <openvdb/Grid.h>
#endif

class GT_API GT_PrimVDB : public GT_Primitive
{
public:
    GT_PrimVDB(const GU_ConstDetailHandle &gdh, const GEO_Primitive *prim);
    virtual ~GT_PrimVDB();

    virtual int		getPrimitiveType() const { return GT_PRIM_VDB_VOLUME; }
    
    virtual const char *className() const { return "GT_PrimVDB"; }

    bool		isLevelSet() const { return myLevelSetFlag; }
    bool		isVolume() const   { return myVolumeFlag; }

    // Compute list of voxel coords with corresponding leaf node edge masks.
    // Returns coords.entries().
    exint		getLeafCoords(
			    UT_Vector3FArray &coords,
			    UT_Int16Array &masks) const;

    /// Returns a GT_PrimPointMesh with 1 point containing the VDB's 1 point.
    /// Contains all of the VDBs attributes as point or uniform attributes.
    GT_PrimitiveHandle  getPointPrimitive() const;

    GEO_PrimVolumeXform getVolumeTransform() const;
    GEO_PrimVolumeXform getVolumeIndexTransform() const;

    UT_Vector3I		getRes() const;

    /// Viewport refinement options -- changes on LOD
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;

    virtual bool	refine(GT_Refine &refiner,
			       const GT_RefineParms *parms=NULL) const;

    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				      int nsegments) const;
    virtual int		getMotionSegments() const { return 1; }
    virtual int64	getMemoryUsage() const { return sizeof(GEO_Primitive *);}
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimVDB(*this); }

    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);
    
    GA_Offset		getPrimitiveOffset() const { return myPrimID; }
    GA_Offset		getPointOffset() const     { return myPointID; }
    UT_Vector3F		getPrimColor() const   { return myPrimColor; }

    GT_TransformHandle	getGridTransform();
    
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniformList; }
#ifdef OPENVDB_ENABLED
    const openvdb::GridBase *	getGrid();
    const GEO_PrimVDB		*getGeoPrimitive() const { return myVDB; }
#endif

private:
#ifdef OPENVDB_ENABLED
    static GT_TransformHandle	getPrimitiveTransformFromGrid(
				    const openvdb::GridBase &grid);

    const GEO_PrimVDB *myVDB;
#endif

    GU_ConstDetailHandle	myDetail;
    GT_AttributeListHandle	myUniformList;
    GA_Offset			myPrimID;
    GA_Offset			myPointID;
    UT_Vector3F			myPrimColor;
    bool			myVolumeFlag;
    bool			myLevelSetFlag;
};

/// Trivial collector class for VDBs - 1 GT_PrimVDB per VDB primitive
class GT_API GT_GEOPrimCollectVDB : public GT_GEOPrimCollect
{
public:
		    GT_GEOPrimCollectVDB() {}
    virtual	   ~GT_GEOPrimCollectVDB() {}

    virtual GT_PrimitiveHandle collect(const GT_GEODetailListHandle &g,
				       const GEO_Primitive *const* prim_list,
				       int,
				       GT_GEOPrimCollectData *) const
    {
	GT_PrimitiveHandle vdbh;
#ifdef OPENVDB_ENABLED
	if(static_cast<const GEO_PrimVDB *>(prim_list[0])->getVisualization()
		!= GEO_VOLUMEVIS_INVISIBLE)
	    vdbh = new GT_PrimVDB(g->getGeometry(0), prim_list[0]);
#endif
	return vdbh;
    }
};

#endif
