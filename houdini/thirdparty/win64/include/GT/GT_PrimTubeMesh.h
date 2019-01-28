/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimTubeMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimTubeMesh__
#define __GT_PrimTubeMesh__

#include "GT_API.h"
#include "GT_PrimQuadricMesh.h"

class GT_PrimTube;

class GT_API GT_PrimTubeMesh : public GT_PrimQuadricMesh
{
public:
    GT_PrimTubeMesh(fpreal taper=1, bool caps=false)
	: myTaper(taper)
	, myCaps(caps)
	, GT_PrimQuadricMesh()
    {
    }
    GT_PrimTubeMesh(const GT_PrimTubeMesh &src)
	: GT_PrimQuadricMesh(src)
	, myTaper(src.myTaper)
	, myCaps(src.myCaps)
    {
    }

    GT_PrimTubeMesh(fpreal taper, bool caps,
		    const GT_AttributeListHandle &vertex,
		    const GT_AttributeListHandle &detail,
		    const GT_TransformArrayHandle &xforms)
	: myTaper(taper)
	, myCaps(caps)
	, GT_PrimQuadricMesh(vertex, detail, xforms)
    {
    }
    GT_PrimTubeMesh(const GT_PrimTube *tube);

    virtual ~GT_PrimTubeMesh();

    /// @{
    /// Methods defined on GT_PrimQuadricMesh
    virtual const char	*className() const { return "GT_PrimTubeMesh"; }
    virtual bool	 save(UT_JSONWriter &w) const;
    virtual int		 getPrimitiveType() const
			    { return GT_PRIM_TUBE_MESH; }
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimTubeMesh(*this); }
    virtual GT_PrimitiveHandle	doHarden() const;
    /// @}

    /// Initialize the mesh
    bool		init(fpreal taper, bool caps,
			     const GT_AttributeListHandle &vertex,
			     const GT_AttributeListHandle &detail,
			     const GT_TransformArrayHandle &xforms);

    /// @{
    /// Accessors
    /// See GT_PrimQuadricMesh for additional queries.
    bool		getCaps() const		{ return myCaps; }
    fpreal		getTaper() const	{ return myTaper; }
    GT_Size		getTubeCount() const
			    { return getQuadricCount(); }
    /// @}

    virtual bool	getBaseTransform(UT_Matrix4D &mat) const;
    
private:
    /// Compute the bounding box for a single (untransformed) tube
    virtual void	getQBounds(UT_BoundingBox &box) const;
    virtual GT_PrimitiveHandle	makeQuadric(GT_Size index,
					const GT_AttributeListHandle &h,
					const GT_TransformHandle &x,
					const GT_RefineParms *parms) const;

    fpreal	myTaper;
    bool	myCaps;
};

#endif
