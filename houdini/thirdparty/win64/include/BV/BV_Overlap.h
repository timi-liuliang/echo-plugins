/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __BV_Overlap_h__
#define __BV_Overlap_h__

#include "BV_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>

class BV_API bvTriangle {
public:
    void		 computeNormal();
    void		 transform(const UT_Matrix3 &R, const UT_Vector3 &T);
    
    UT_Vector3		 myP[3], myNormal;
};

class BV_API bvQuad {
public:
    /// Returns two non-overlapping triangles from the vertices of this
    /// (coplanar) quadrilateral.  Returns false if this is a skew quadrilateral
    /// The degenerate case will have ta as the correct triangle
    bool		dissect(bvTriangle &ta, 
				bvTriangle &tb, 
				bool &degen) const;
    /// Returns true if the quadrilateral is coplanar
    bool		isCoplanar() const;
    UT_Vector3		myP[4];
};


/// A class for determining if geometry objects overlap
class BV_API BV_Overlap
{
public:
    // Code for doTriBoxOverlap and doTriTriOverlap is modified from 
    // OPCODE 1.3 - Optimized Collision Detection Engine.
    // doTriTriOverlap is based on Tomas Moller's "A fast triangle-triangle
    // intersection test", Journal of graphic tools 2(2):25-30, 1997
    // OPCODE license (actually there is no license) permit the 
    // usage of its code in any commercial program.
    // See http://www.codercorner.com/Opcode.htm
       
    /// Returns true if two coplanar triangles overlap.
    static bool doCoplanarTriTriOverlap(const bvTriangle &tri1,
					const bvTriangle &tri2,
					fpreal tol = 0.0f);
 
    /// Returns true if two triangles overlap
    static bool doTriTriOverlap(const bvTriangle &tri1,
				const bvTriangle &tri2,
				fpreal tol=0.0f);

    /// Returns the intersection points when two triangles overlap
    static bool doTriTriOverlap(const bvTriangle &tri1,
				const bvTriangle &tri2,
				bool &coplanar,
				UT_Vector3 &isectpt1,
				UT_Vector3 &isectpt2,
				fpreal tol=0.0f);
 
    /// Returns true if two coplanar quadrilaterals overlap
    static bool doCoplanarQuadQuadOverlap(const bvQuad &q1, 
					  const bvQuad &q2,
					  fpreal tol=0.0f);
    
    /// Returns true if a plane and a box overlap
    static bool	doPlaneBoxOverlap(const UT_Vector3& normal, 
				const fpreal d, 
				const UT_Vector3& maxbox,
				fpreal tol = 0.0f);
 
    /// Uses the Seperating Axis Theorem (SAT) to test for triangle/box
    /// intersection.  The lite version avoids edge tests, but may generate
    /// false positives.
    static bool	doTriBoxOverlap(const bvTriangle &tri, 
				const UT_Vector3 &ra, 
				const UT_Matrix3 &R,
				const UT_Vector3 &T,
				// fpreal tol=0.f, 
				bool lite=false);

    /// This checks for overlaps for two static OBBs.  This method is based on
    /// the Seperating Axis Theorem (SAT).  The lite version of this, 
    /// algorithm, dubbed SAT-lite, satistically has only about 15% chance 
    /// of reporting disjoint bounding boxes as intersecting.  
    /// However, this may improve runtime in some situations since this 
    /// is 2.5 times faster than the original SAT, by only checking
    /// the face-face axis (and ignoring the edges).  This may generate
    /// false positives however.
    static bool	doBoxBoxOverlap(const UT_Vector3 &ra,
				const UT_Vector3 &rb,
				const UT_Matrix3 &R,
				const UT_Vector3 &T,
				fpreal tol=0.f, 
				bool lite=false);
    /// Given an AABB (with radii ra -- half of dimension) and center at origin
    /// calculate an AABB (parallel to the original AABB) that contains the 
    /// original AABB and an OBB by applying xform to the original AABB.
    static void calcAABB(const UT_Vector3 &ra,  
				const UT_DMatrix4 &xform,
				UT_Vector3 &rnew, 
				UT_Vector3 &Tnew);
    static void calcAABB(const UT_Vector3 &ra,  
				const UT_DMatrix4 &xform,
				const UT_Matrix3 &R,
				const UT_Vector3 &T,
				UT_Vector3 &rnew, 
				UT_Vector3 &Tnew); 
   
    /// Given two OBBs (second determined by a relative transform R & T), 
    /// checks for overlaps between the OBBs with each
    /// containing one original OBB and the OBB after applying xform.
    static bool doBoxBoxOverlap(const UT_Vector3 &ra, 
				const UT_Vector3 &rb,
				const UT_DMatrix4 &xform1, 
				const UT_DMatrix4 &xform2,
				const UT_Matrix3 &R, 
				const UT_Vector3 &T, 
				fpreal tol, 
				bool lite = false,
				bool useOBB = true);
   
private:
    static bool doCoplanarTriTriOverlap(const UT_Vector3& n, 
				const UT_Vector3& v0, 
				const UT_Vector3& v1, 
				const UT_Vector3& v2, 
				const UT_Vector3& u0, 
				const UT_Vector3& u1,
				const UT_Vector3& u2);
 
};

#endif


