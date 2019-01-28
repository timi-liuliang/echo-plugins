/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_BVLeafIterator_h__
#define __SIM_BVLeafIterator_h__

#include "SIM_API.h"
#include <BV/BV_LeafIterator.h>
#include <UT/UT_DMatrix4.h>

class SIM_TriangulatedGeometry;

/// Iterator for leaves to be enclosed by bounding volume. This iterator
/// fills each leaf with a single primitive.
/// Iterator retrieves info about each leaf:
/// - a set of vertices forming a bounding volume for the leaf
/// - barycenter (centroid) of primitive
class SIM_API SIM_BVLeafIterator : public BV_LeafIterator
{
public:
    typedef BV_LeafIterator BaseClass;
    
    explicit SIM_BVLeafIterator(const SIM_TriangulatedGeometry *tgeo,
			        const UT_DMatrix4 &begxform,
				const UT_DMatrix4 &endxform,
				bool startPosOnly = false);
    explicit SIM_BVLeafIterator(const SIM_TriangulatedGeometry *tgeo,
				const UT_DMatrix4 &endxform,
				bool startPosOnly = false);
    virtual		~SIM_BVLeafIterator();

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
    const SIM_TriangulatedGeometry     *myTgeo;
    const UT_DMatrix4	 	      	myBegxform, myEndxform;
    int			 	      	myPrim;
    int			 		myVertex;

    int			               *myDupArray;
    int			 		myDupId;
    bool				myEndFlag, myStartPosOnly;
};

#endif
