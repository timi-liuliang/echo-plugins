/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_TriangulatedGeometry_H__
#define __SIM_TriangulatedGeometry_H__

#include "SIM_API.h"
#include "SIM_Data.h"
#include "SIM_DataUtils.h"

#include <UT/UT_FloatArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Vector4Array.h>

/// This class holds a polyized geometry for which people can query.
/// This is an alternative representation.
/// This caches the point positions in the SIM_Geometry's space,
/// so are independent of SIM_Geometry's transform.
class SIM_API SIM_TriangulatedGeometry : public SIM_Data
{
public:
    /// initializes alternate geometry for lite functions
    void		init() const;
    
    /// Gets the position of a point in SIM_Geometry space, given point ID
    UT_Vector3 		getPointPos(int ptId) const;
    /// Gets the position of a point in SIM_Geometry space, 
    ///given primitive ID and vertex number
    UT_Vector3		getPointPos(int primId, int vtNum) const;
    /// Returns the total number of points
    int			getPointCount() const;
    /// Gets the corresponding point id given a vertex number and a
    /// primitive id
    int			getPointId(int primId, int vtNum) const;
    /// Gets the number of primitives
    int 		getPrimitiveCount() const;
    /// Gets the number of vertices of a given primitive
    int 		getVertexCount(int primNum = 0) const;
    /// Returns the baryCenter of a given primitive
    /// Gets the point density, given point ID
    float 		getPointDensity(int ptId) const;
    /// Gets the point density, given primitive ID and vertex number
    float		getPointDensity(int primId, int vtNum) const;

    /// Returns the barycenter of the triangle.
    /// It is estimated from point density (if any)
    UT_Vector3		getBaryCenter(int primNum) const;

protected:
    explicit	     SIM_TriangulatedGeometry(const SIM_DataFactory *factory);
    virtual	    ~SIM_TriangulatedGeometry();

    /// Overrides to properly implement this class as a SIM_Data.
    /// This object is meant to be updated from the object's geometry at
    /// each frame, so we only implement skeleton save, and load methods.
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual int64	 getMemorySizeSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);

private:
    UT_Vector3Array	 myPointPos; 
    UT_IntArray		 myPointNum;
    UT_FloatArray	 myDensity;    
    
    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_TriangulatedGeometry,
			SIM_Data,
			"TriangulatedGeometry",
			getEmptyDopDescription());
};

#endif
