/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimSphereMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimSphereMesh__
#define __GT_PrimSphereMesh__

#include "GT_API.h"
#include "GT_PrimSphere.h"
#include "GT_PrimQuadricMesh.h"

class GT_API GT_PrimSphereMesh : public GT_PrimQuadricMesh
{
public:
    GT_PrimSphereMesh()
	: GT_PrimQuadricMesh()
    {}
    GT_PrimSphereMesh(const GT_PrimSphereMesh &src)
	: GT_PrimQuadricMesh(src)
    {
    }

    GT_PrimSphereMesh(const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &detail,
			const GT_TransformArrayHandle &xforms)
	: GT_PrimQuadricMesh(vertex, detail, xforms)
    {}
    GT_PrimSphereMesh(const GT_PrimSphere *sphere)
	: GT_PrimQuadricMesh(sphere)
    {}
    ~GT_PrimSphereMesh() {}

    virtual const char *className() const { return "GT_PrimSphereMesh"; }
    virtual bool	save(UT_JSONWriter &w) const
			    { return saveQuadricMesh(w, "SphereMesh"); }
    
    /// @{
    /// Methods from GT_Primitive
    virtual int	getPrimitiveType() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimSphereMesh(*this); }
    virtual GT_PrimitiveHandle	doHarden() const;
    /// @}

protected:
    /// @{
    /// Methods from GT_PrimQuadricMesh
    virtual void		getQBounds(UT_BoundingBox &box) const;
    virtual GT_PrimitiveHandle	makeQuadric(GT_Size index,
					const GT_AttributeListHandle &h,
					const GT_TransformHandle &x,
					const GT_RefineParms *parms) const;
    /// @}
private:
};

#endif

