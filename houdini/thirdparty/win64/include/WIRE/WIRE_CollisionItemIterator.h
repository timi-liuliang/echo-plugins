/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        WIRE_CollisionItemIterator.h
 *
 * COMMENTS:
 */

#ifndef __WIRE_CollisionItemIterator_h__
#define __WIRE_CollisionItemIterator_h__
                                                                                
#include "WIRE_API.h"
#include "WIRE_SimpleDetailReader.h"
#include <SIM/SIM_Object.h>
#include <SIM/SIM_Time.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Vector3.h>

// class to explore the collision elements for a wire object
class WIRE_API WIRE_CollisionItemIterator
{
public:
    explicit	 WIRE_CollisionItemIterator(const SIM_Object &object,
				     const SIM_Time &timestep);

    bool	 isValid() const { return myReader.isValid(); }

    // methods to iterate over the collision elements
    bool	 atEnd() const;
    fpreal	 getRadius() const;
    void	 advance();
    void	 rewind();

    // methods to iterate over the vertices making up the current collision
    // element being explored
    bool	 atEndVertex() const;
    UT_Vector3	 getVertexPosition() const;
    UT_Vector3	 getVertexVelocity() const;
    void	 advanceVertex();
    void	 rewindVertex();

    // returns true if the geometry from the i0th and i1th elements reference
    // a common point
    bool	 hasCommonPoint(int i0, int i1) const;

    // retrieves the primitive and first vertex for a given collision element
    void	 getEdge(int i, int &prim, int &vtx) const;

private:
    const WIRE_SimpleDetailReader	myReader;
    SIM_Time		 myTimestep;
    UT_IntArray		 myIdMapping;
    int			 myVertexId;
    int			 myPrim;
    int			 myEdge;
};

#endif
