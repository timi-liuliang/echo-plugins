/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimCircleMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimCircleMesh__
#define __GT_PrimCircleMesh__

#include "GT_API.h"
#include "GT_PrimCircle.h"
#include "GT_PrimQuadricMesh.h"

class GT_API GT_PrimCircleMesh : public GT_PrimQuadricMesh
{
public:
    GT_PrimCircleMesh()
	: GT_PrimQuadricMesh()
    {}
    GT_PrimCircleMesh(const GT_PrimCircleMesh &src)
	: GT_PrimQuadricMesh(src)
    {
    }

    GT_PrimCircleMesh(const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &detail,
			const GT_TransformArrayHandle &xforms)
	: GT_PrimQuadricMesh(vertex, detail, xforms)
    {}
    GT_PrimCircleMesh(const GT_PrimCircle *circle)
	: GT_PrimQuadricMesh(circle)
    {}
    ~GT_PrimCircleMesh() {}

    virtual const char *className() const { return "GT_PrimCircleMesh"; }
    virtual bool	save(UT_JSONWriter &w) const
			    { return saveQuadricMesh(w, "CircleMesh"); }
    virtual GT_PrimitiveHandle	doSoftCopy() const
			    { return new GT_PrimCircleMesh(*this); }
    virtual GT_PrimitiveHandle	doHarden() const;
    
    /// @{
    /// Methods from GT_Primitive
    virtual int	getPrimitiveType() const;
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

