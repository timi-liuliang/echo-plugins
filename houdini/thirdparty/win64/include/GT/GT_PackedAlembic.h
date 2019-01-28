/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PackedAlembic.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PackedAlembic__
#define __GT_PackedAlembic__

#include "GT_API.h"
#include "GT_GEODetailList.h"
#include "GT_GEOPrimPacked.h"
#include "GT_PrimInstance.h"

class GU_PrimPacked;

/// Collection class for a single archive's worth of Alembic primitives.
/// This is generally only useful for the viewport.
class GT_API GT_PackedAlembicArchive : public GT_Primitive
{
public:
    GT_PackedAlembicArchive(const UT_StringHolder &archive_name,
			    const GT_GEODetailListHandle &source_list);

    const UT_StringHolder &archiveName() const { return myName; }

    const UT_StringArray &getAlembicObjects() const { return myAlembicObjects; }
    const GA_OffsetArray &getAlembicOffsets() const { return myAlembicOffsets; }

    virtual int		 getPrimitiveType() const
				    { return GT_PRIM_ALEMBIC_ARCHIVE; }

    void	appendAlembic(GA_Offset alembic_prim_offset)
			    { myAlembicOffsets.append(alembic_prim_offset); }

    virtual bool	bucketPrims(const GT_PackedAlembicArchive *prev_archive,
			    const GT_RefineParms *ref_parms,
			    bool force_update) = 0;

    int		getNumChildPrims() const
		    {  return (myConstShapes.entries() +
			       myTransformShapes.entries() +
			       myDeformShapes.entries() +
			       myCombinedShapes.entries()); }

    const UT_Array<GT_PrimitiveHandle> &constantShapes() const
		    { return myConstShapes; }
    const UT_Array<GT_PrimitiveHandle> &transformShapes() const
		    { return myTransformShapes; }
    const UT_Array<GT_PrimitiveHandle> &deformShapes() const
		    { return myDeformShapes; }
    const UT_Array<GT_PrimitiveHandle> &combinedShapes() const
		    { return myCombinedShapes; }
    
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				      int nsegments) const;
    virtual int		getMotionSegments() const  { return 1; }
    virtual int64	getMemoryUsage() const;

    void		setRefinedSubset(bool reduced_consts,
					 UT_IntArray &const_prims,
					 bool reduced_transforms,
					 UT_IntArray &trans_prims);
    bool		hasConstantSubset() const { return myHasConstSubset; }
    const UT_IntArray  &getConstantSubset() const { return myConstSubset; }
    bool		hasTransformSubset() const { return myHasTransSubset; }
    const UT_IntArray  &getTransformSubset() const { return myTransSubset; }

    int64		getAlembicVersion() const { return myAlembicVersion; }

    const GU_ConstDetailHandle &parentDetail() const
				{ return myDetailList->getGeometry(0); }
    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);
protected:
    virtual bool	archiveMatch(const GT_PackedAlembicArchive *archive) const = 0;
    
    UT_StringHolder		 myName;
    GT_GEODetailListHandle	 myDetailList;
    UT_StringArray		 myAlembicObjects;
    GA_OffsetArray		 myAlembicOffsets;
    
    UT_Array<GT_PrimitiveHandle> myConstShapes;
    UT_Array<GT_PrimitiveHandle> myTransformShapes;
    UT_Array<GT_PrimitiveHandle> myDeformShapes;
    UT_Array<GT_PrimitiveHandle> myCombinedShapes;
    UT_IntArray			 myConstSubset;
    UT_IntArray			 myTransSubset;
    unsigned			 myHasConstSubset : 1,
				 myHasTransSubset : 1;
    int64			 myAlembicVersion;
};


/// Single Alembic shape (non-instanced)
class GT_API GT_PackedAlembic : public GT_GEOPrimPacked
{
public:
	     GT_PackedAlembic(const GU_ConstDetailHandle &prim_gdh,
			      const GU_PrimPacked *prim,
			      const GT_DataArrayHandle &vp_mat,
			      const GT_DataArrayHandle &vp_remap,
			      bool build_packed_attribs);
    
	     GT_PackedAlembic(const GT_PackedAlembic &src);
    virtual ~GT_PackedAlembic();

    virtual void	 initVisAnim() = 0;
    
    virtual int		 getPrimitiveType() const
				    { return GT_PRIM_ALEMBIC_SHAPE; }
    
    bool		 hasChanged() const { return myHasChanged; }
    void		 changeProcessed() { myHasChanged = false; }
    bool		 needsInit() const { return myNeedsInit; }
    void	         initProcessed()  { myNeedsInit = false; }

    
    GT_TransformHandle		fullCachedTransform();
    virtual bool		isVisible() = 0;

    virtual bool		getUniqueID(int64 &id) const
				{ id = myID; return true; }

    void			setAnimationType(GEO_AnimationType t)
				{ myAnimType = t; }
    GEO_AnimationType		animationType() const { return myAnimType; }

    void			setVisibilityAnimated(bool anim)
				    { myAnimVis = anim; }
    bool			visibilityAnimated() const { return myAnimVis; }

    virtual bool 		getCachedGeometry(GT_PrimitiveHandle &ph) const = 0;
    
    virtual void 		getCachedTransform(GT_TransformHandle &ph) const = 0;
    virtual void		getCachedVisibility(bool &visible) const = 0;

    GT_TransformHandle		applyPrimTransform(const GT_TransformHandle &th)
					const;
    virtual GT_TransformHandle	getLocalTransform() const = 0;

    int64	      alembicVersion() const { return myAlembicVersion; }
    void	      setAlembicVersion(int64 v) { myAlembicVersion = v; }

    virtual const GT_AttributeListHandle &getDetailAttributes() const
				  { return myDetailAttribs; }
protected:
    int64	      myID;
    GEO_AnimationType myAnimType;
    GEO_ViewportLOD   myLOD;
    GT_AttributeListHandle myDetailAttribs;
    int64	      myAlembicVersion;
    GA_Offset	      myOffset;
    float	      myFrame;
    unsigned	      myHasChanged : 1,
		      myAnimVis : 1,
		      myVisibleConst : 1, // only valid when myAnimVis is false.
		      myNeedsInit : 1;
};

/// Alembic mesh which contains multiple alembic primitives merged together.
class GT_API GT_PackedAlembicMesh : public GT_Primitive
{
public:
    GT_PackedAlembicMesh(const GT_PrimitiveHandle &packed_geo, int64 id);
    GT_PackedAlembicMesh(const GT_PrimitiveHandle &packed_geo, int64 id,
			   UT_Array<GT_PrimitiveHandle> &individual_meshes);
    GT_PackedAlembicMesh(const GT_PackedAlembicMesh &mesh);
    
    virtual int		 getPrimitiveType() const
				    { return GT_PRIM_ALEMBIC_SHAPE_MESH; }
    virtual const char	*className() const
				    { return "GT_PackedAlembicMesh"; }
    virtual GT_PrimitiveHandle	doSoftCopy() const
				  { return new GT_PackedAlembicMesh(*this); }
    virtual bool	refine(GT_Refine &refiner,
			       const GT_RefineParms *parms=NULL) const;
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				      int nsegments) const;
    virtual int		getMotionSegments() const  { return 1; }
    virtual int64	getMemoryUsage() const;
    virtual bool	getUniqueID(int64 &id) const
			{ id = myID; return true; }

    void		update(bool initial_update);
    bool		hasAnimatedTransforms() const
			    { return myTransformArray.get() != NULL; }
    bool		hasAnimatedVisibility() const
			    { return myTransformArray.get() != NULL; }
    
    int64	      alembicVersion() const { return myAlembicVersion; }
    void	      setAlembicVersion(int64 v) { myAlembicVersion = v; }
    
private:
    GT_PrimitiveHandle myMeshGeo;
    GT_DataArrayHandle myTransformArray;
    GT_DataArrayHandle myVisibilityArray;
    UT_Array<GT_PrimitiveHandle> myPrims;
    int64	       myID;
    int64	       myTransID;
    int64	       myVisID;
    int64	       myAlembicVersion;
};

/// Instanced alembic primitive
class GT_API GT_AlembicInstance : public GT_PrimInstance
{
public:
    GT_AlembicInstance();
    GT_AlembicInstance(const GT_AlembicInstance &src);
    GT_AlembicInstance(
	    const GT_PrimitiveHandle &geometry,
	    const GT_TransformArrayHandle &transforms,
	    GEO_AnimationType animation,
	    const GT_GEOOffsetList &packed_prim_offsets=GT_GEOOffsetList(),
	    const GT_AttributeListHandle &uniform=GT_AttributeListHandle(),
	    const GT_AttributeListHandle &detail=GT_AttributeListHandle(),
	    const GT_GEODetailListHandle &source=GT_GEODetailListHandle());
    virtual ~GT_AlembicInstance();
    
    virtual int		 getPrimitiveType() const
				    { return GT_PRIM_ALEMBIC_INSTANCE; }

    virtual const char	*className() const { return "GT_AlembicInstance"; }
    
    GEO_AnimationType animationType() const { return myAnimType; }
    
    int64	      alembicVersion() const     { return myAlembicVersion; }
    void	      setAlembicVersion(int64 v) { myAlembicVersion = v; }
    
    bool	      hasChanged() const { return myHasChanged; }
    void	      changeProcessed()  { myHasChanged = false; }
    
    bool	      needsInit() const { return myNeedsInit; }
    void	      initProcessed()  { myNeedsInit = false; }

protected:
    GEO_AnimationType myAnimType;
    int64	      myAlembicVersion;
    unsigned	      myHasChanged : 1,
		      myNeedsInit : 1;
};

#endif
