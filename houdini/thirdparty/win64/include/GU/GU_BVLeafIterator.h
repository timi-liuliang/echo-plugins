/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GU_BVLeafIterator_h__
#define __GU_BVLeafIterator_h__

#include "GU_API.h"
#include "GU_DetailHandle.h"
#include <BV/BV_LeafIterator.h>
#include <UT/UT_DMatrix4.h>
#include <GA/GA_Iterator.h>

class GA_PrimitiveGroup;
class GEO_Primitive;
class GU_Detail;

/// Iterator for leaves to be enclosed by bounding volume. This iterator
/// fills each leaf with a single primitive.
/// Iterator retrieves info about each leaf:
/// - a set of vertices forming a bounding volume for the leaf
/// - barycenter (centroid) of primitive
class GU_API GU_BVLeafIterator : public BV_LeafIterator
{
public:
    typedef BV_LeafIterator BaseClass;

    explicit		 GU_BVLeafIterator(const GU_ConstDetailHandle &gdh,
					   const GA_PrimitiveGroup *group = 0);
    explicit		 GU_BVLeafIterator(const GU_ConstDetailHandle &gdh,
					   const UT_DMatrix4 &transform,
					   const GA_PrimitiveGroup *group = 0);
    explicit		 GU_BVLeafIterator(const GU_Detail &gdp,
					   const GA_PrimitiveGroup *group = 0);
    explicit		 GU_BVLeafIterator(const GU_Detail &gdp,
					   const UT_DMatrix4 &transform,
					   const GA_PrimitiveGroup *group = 0);
    virtual		~GU_BVLeafIterator();

protected:
    virtual void	 advanceSubclass();
    virtual void	 jumpSubclass(int leafId);
    virtual bool	 atEndSubclass() const;
    virtual void	 rewindSubclass();
    virtual int		 getLeafIdSubclass() const;

    virtual UT_Vector3	 calcBarycenterSubclass() const;

    virtual void	 advanceVertexSubclass();
    virtual bool	 atEndVertexSubclass() const;
    virtual void	 rewindVertexSubclass();
    virtual UT_Vector4	 getVertexSubclass() const;

    virtual void	 setSkipDupVertsSubclass(bool flag);
private:
    const GU_Detail	&getGdp() const;

    const GEO_Primitive *getOurPrimitive() const;
    const UT_DMatrix4	&getOurTransform() const;

    const GEO_Primitive *getPrimitiveByOffset(GA_Offset offset) const;

    /// Disallowed.
    /// @{
			 GU_BVLeafIterator(const GU_BVLeafIterator &);
    GU_BVLeafIterator	&operator=(const GU_BVLeafIterator &);
    /// @}

    GU_DetailHandleAutoReadLock	*myGdh;
    const GU_Detail	&myGdp;
    bool		 myHasTransform;
    const UT_DMatrix4	 myTransform;
    const GA_PrimitiveGroup
			*myGroup;
    GA_Iterator		 myPrimIterator;
    const GEO_Primitive	*myPrim;
    int			 myVertex;

    int			*myDupArray;
    int			 myDupId;
};

#endif
