/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimPolySoup.h (GU Library, C++)
 *
 * COMMENTS:	Declarations of utility functions of the polygon soup primitive
 */

#ifndef __GU_PrimPolySoup__
#define __GU_PrimPolySoup__

#include "GU_API.h"
#include <GEO/GEO_PrimPolySoup.h>

//#define SOUP_FILE_TESTING

class GA_Detail;
class GEO_PolyCounts;
class UT_MemoryCounter;
class GEO_ConvertParms;

class GU_API GU_PrimPolySoup : public GEO_PrimPolySoup
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimPolySoup() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimPolySoup(GA_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_PrimPolySoup(gdp, offset)
    {}

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    virtual const GA_PrimitiveDefinition &getTypeDef() const
    {
        UT_ASSERT(theDefinition);
        return *theDefinition;
    }

    // Conversion Methods
    virtual GEO_Primitive       *convert(GEO_ConvertParms &parms,
                                         GA_PointGroup *usedpts = 0);
    virtual GEO_Primitive       *convertNew(GEO_ConvertParms &parms);

    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimPolySoup::<functname>
    //        i.e.        partptr = GU_PrimPolySoup::build(params...);

    // Optional Build Methods

    /// Builds an empty polygon soup (i.e. contains no vertices or polygons)
    static GU_PrimPolySoup	*build(GA_Detail *detail);
    /// Builds npoints new points with the specified positions, and then creates
    /// a polygon soup that uses those points, as dictated by polygonsizelist
    /// and polygonpointnumbers, in parallel.  polygonpointnumbers lists the
    /// numbers of the points used by each polygon, relative to the first point
    /// created by this method.  The offset of the polysoup is returned.
    ///
    /// If sharedvertices is false, there will be
    /// polygonsizelist.getNumVertices() vertices, and they won't be shared
    /// by any polygons in the polygon soup.  If sharedvertices is true,
    /// there will be npoints vertices, and each will be shared by all
    /// polygons using the corresponding point.
    ///
    /// NOTE: If sharedvertices is true, it's necessary for the polysoup to
    /// use all points created.  If sharedvertices is false, this limitation
    /// is lifted, although doing so probably isn't useful.
    /// @{
    static GA_Offset build(GA_Detail *detail,
        const UT_Vector3 *points,
        const GA_Size npoints,
        const GEO_PolyCounts &polygonsizelist,
        const int *polygonpointnumbers,
        const bool sharedvertices = true);
    static GA_Offset build(GA_Detail *detail,
        const UT_Vector3 *points,
        const GA_Size npoints,
        const GA_PolyCounts &polygonsizelist,
        const int *polygonpointnumbers,
        const bool sharedvertices = true);
    /// @}

    /// Builds a polygon soup using the specified range of point offsets,
    /// as dictated by polygonsizelist and polygonpointnumbers, in parallel.
    /// polygonpointnumbers lists the *offsets* of the points used by
    /// each polygon *MINUS* startpt, i.e. they are offsets relative to startpt,
    /// *not* indices relative to startpt.  The offset of the polysoup is
    /// returned.  All polygonpointnumbers must be between 0 (inclusive) and
    /// npoints (exclusive).
    ///
    /// If sharedvertices is false, there will be
    /// polygonsizelist.getNumVertices() vertices, and they won't be shared
    /// by any polygons in the polygon soup.  If sharedvertices is true,
    /// there will be npoints vertices, and each will be shared by all
    /// polygons using the corresponding point.
    ///
    /// NOTE: If sharedvertices is true, *no primitives* can be using the points
    /// in the specified range already, and polygons in the polygon soup being
    /// created should use all of them, because the vertices will be wired as
    /// the only vertices for their corresponding points.  If sharedvertices is
    /// false, these limitations are lifted, allowing multiple primitives
    /// to use the same points, and allowing the polysoup being created to use
    /// a subset of the points in the range.
    /// @{
    static GA_Offset build(GA_Detail *detail,
        const GA_Offset startpt,
        const GA_Size npoints,
        const GEO_PolyCounts &polygonsizelist,
        const int *polygonpointnumbers,
        const bool sharedvertices = true);
    static GA_Offset build(GA_Detail *detail,
        const GA_Offset startpt,
        const GA_Size npoints,
        const GA_PolyCounts &polygonsizelist,
        const int *polygonpointnumbers,
        const bool sharedvertices = true);
    /// @}

#ifdef SOUP_FILE_TESTING
    bool                         saveCustom(const char *filename);
    static GA_Offset             loadCustom(GA_Detail *detail,
                                            const char *filename);
#endif

    void                removeInlinePoints(float tol, GA_PointGroup *&delgrp);

    virtual void	normal(NormalComp &output) const;

    /// Sorts the polygons in the polysoup spatially
    void                         sortSpatial();

    virtual void convex(GA_Size maxpts = 3, const GA_Detail *restgdp = nullptr,
        bool flipedges = false, bool avoiddegeneracy = false) final;

protected:
private:
    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;

    // This function calls the constructor for GU_PrimitiveFactory
    //template<typename>
    //friend GA_Primitive *gu_NewPrimitive(GA_Detail &gdp,GA_Offset offset,
    //                                     const GA_PrimitiveDefinition &);
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif

