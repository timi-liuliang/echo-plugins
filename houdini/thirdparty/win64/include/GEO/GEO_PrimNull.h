/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimNull.h (GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_PrimNull__
#define __GEO_PrimNull__

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include <GA/GA_Types.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Vector3.h>

class GA_Detail;
class GEO_ConvertParms;

/// Null primitive is a stand-in primitive to handle loading of unknown
/// primitives
class GEO_API GEO_PrimNull : public GEO_Primitive
{
protected:
    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimNull() {}

public:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimNull(GA_Detail *d, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_Primitive(d, offset)
    {}

    /// @{
    /// Required interface for GEO
    virtual bool	isDegenerate() const { return true; }
    virtual int		getBBox(UT_BoundingBox *bbox) const;
    virtual void	reverse() {}
    virtual UT_Vector3	computeNormal() const;
    virtual void	copyPrimitive(const GEO_Primitive *s);
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	copyUnwiredForMerge(
        const GA_Primitive *src, const GA_MergeMap &map) {}
    virtual GA_Size	getVertexCount() const { return 0; }
    virtual GA_Offset	getVertexOffset(GA_Size) const
    {
        UT_ASSERT(0);
        return GA_INVALID_OFFSET;
    }
#endif
    virtual int detachPoints(GA_PointGroup &g);
    virtual GA_DereferenceStatus dereferencePoint(GA_Offset, bool);
    virtual GA_DereferenceStatus dereferencePoints(
        const GA_RangeMemberQuery &, bool);
    virtual const GA_PrimitiveJSON *getJSON() const;
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void swapVertexOffsets(const GA_Defragment &) {}
#endif
    virtual bool evaluatePointRefMap(GA_Offset,
        GA_AttributeRefMap &,
        fpreal, fpreal, uint, uint) const;
    /// @}

    /// @{
    /// Special for factory primitives
    static GA_PrimitiveFamilyMask buildFamilyMask()
    { return GA_FAMILY_NONE; }
    /// @}

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const
    {
        return sizeof(*this);
    }

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    /// @{
    /// Minimal interface required by GU
    virtual const GA_PrimitiveDefinition &getTypeDef() const
    {
        UT_ASSERT(theDefinition);
        return *theDefinition;
    }

    virtual GEO_Primitive *convert(GEO_ConvertParms &, GA_PointGroup *)
    { return NULL; }
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &)
    { return NULL; }
    virtual void normal(NormalComp &) const {}
    virtual int intersectRay(const UT_Vector3 &,
        const UT_Vector3 &,
        float, float,
        float *, UT_Vector3 *,
        UT_Vector3 *, int,
        float *, float *,
        int) const
    { return 0; }
    /// @}

private:
    static GA_PrimitiveDefinition *theDefinition;
    friend class GU_PrimitiveFactory;
};

#endif

