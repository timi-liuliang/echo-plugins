 /*
  * PROPRIETARY INFORMATION.  This software is proprietary to
  * Side Effects Software Inc., and is not to be reproduced,
  * transmitted, or disclosed in any way without written permission.
  *
  * NAME:	GU_Path.h (GEO Utility library, C++)
  *
  * COMMENTS:	This class implements the utilities for the path. Path
  *             is a curve that behaves like a rope or semi-flexible string
  *             (but the behaviour of the path may be extended in the future
  *             to incorporate other behaviours). Path curve has normal
  *             attribute (and potentially a twist attribute) to define
  *             the orientation of the rope, so that it can simulate
  *             the twisting behavoiur in addition to the bending behaviour.
  *
  *		Primary use of this class is intended for the object level
  *		path normal computation (which is used when generating
  *		bones on the path and solving them in follow curve IK chop).
  *		We are primarily interested in the normals at sample points.
  *		The sample points are taken from the Bezier or NURBS curve.
  *		The setup of the object level path is such that only
  *		breakpoints are intended for maniputaling curve (via
  *		object level Path CVs). Thus the GU_Path, obtains
  *		the breakpoint data (normals, twists, etc) and uses it
  *		for interpolating the brakpoint normals among the sampled
  *		points. So, Bezier segments are preferable inputs to the
  *		GU_Path, but NRBS curves are also handled: the breakpoint
  *		data is evaluated from the CVs and then used as for Bezier
  *		curves.
  */

#ifndef __GU_Path_H__
#define __GU_Path_H__


#include "GU_API.h"
// includes
#include <UT/UT_Vector3Array.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>

// class declarations
class GA_ROAttributeRef;
class GEO_PrimPoly;
class GEO_Primitive;
class GEO_Curve;
class GU_Detail;


// Path class
class GU_API GU_Path 
{
public:

    // the calculation method of the normals
    enum gu_CalcMethod
    {
	GU_CALC_DEFAULT,    // default
	GU_CALC_NONE,	    // no normals are computed
	GU_CALC_QUAT,	    // interpolating with quaternions
	GU_CALC_TWIST_SHORT,// interpolating with twist angles in 0..180 range
	GU_CALC_TWIST_FULL  // interpolating with twist angles in any range
    };
    
    // constructor
    GU_Path();

    // destructor
    ~GU_Path();
    
    // Calculate the path object from the curve object.
    // Normals associated with the curve's breakpoints (!)
    // are retargeted for the path object, if curve has normal attribute. 
    // Otherwise path object will not
    // have normals. If curve has twist attribute, the normals for the path
    // are interpolated according to the angle around the path (like a twisted
    // rope or string), otherwise a quaternion interpolation of curve normals
    // is used.
    // INPUT:
    //   curve - the detail whose first primitive is a curve.
    //		 It may have normal attribute
    //           and twist attribute. Twist is the angle of rotation
    //           around the path (synonims: curve, rope) measured from
    //           its natural "rest" orientation (presumably defined 
    //           at the path creation time). If 
    //   mat -   the matrix used to transform the curve's sampled points,
    //           before assigning them to the path. 
    //   normal_comp_type - a method for interpolating the normals at
    //          the breakpoints. Default uses best method for which
    //          we have enough information. 
    // RETURN:
    //   true on success, false if failed
    bool	computePathFromCurve(
		    const GU_Detail * curve, 
		    const UT_DMatrix4 & mat,
		    gu_CalcMethod normal_comp_type = GU_CALC_DEFAULT,
		    int prim_num = 0);
    
    // Calculate the path from detail object containing a curve.
    // Normals for the path are calculated so that they flow naturally
    // without any twist differences between breakpoints.
    // INPUT:
    //   curve    - detail containing a curve
    //   normal   - a normal to match on cv_index-th breakpoint of the curve
    //   cv_index - index of the breakpoint whose normal should be the same
    //            as 'normal' argument
    // RETURN:
    //   true on success, false if failed
    bool	computeNaturalPathFromCurve( const GU_Detail * curve, 
					     const UT_Vector3& normal,
					     int cv_index = 0);
    
    // calculates the positions of the joints along the path. 
    // The joints mark uniform arc length segments along the path.
    // INPUT:
    //   num_of_segments - number of segments (n+1 sample points) for resampling
    // OUTPUT:
    //   joints  - array of vertices that resample the path
    //   normals - array of normals associated with joints. This array
    //             is set to empty if path has no normals.
    void	resamplePath( int num_of_segments,
			      UT_Vector3Array & joints, 
			      UT_Vector3Array & normals ) const;

    // obtains the twist angles between path's breakpoint normals
    // and the vectors passed in
    // INPUT:
    //   vectors - vectors against which to compute twist angles. Should
    //        be the same size as the array returned by getBreakpointNormals()
    // OUTPUT:
    //   angles - angles in degrees by which i-th breakpoint normal needs to be
    //        rotated while traversing the path to match the i+1-th vector.
    //        The angles are in the interval [-180, 180].
    // RETURN:
    //   true on success, false on failure
    bool	getBreakpointTwists( const UT_Vector3Array & vectors,
				     UT_FloatArray & angles );

    // do a spherical linear interpolation (slerp) between two vectors
    // using t = [0, 1] as a blending factor
    static UT_Vector3	slerp( const UT_Vector3 & v0, const UT_Vector3 & v1, 
			double t );

    // obtains path's vertices
    const UT_Vector3Array & getVertices() const
    {
	return myVertices;
    }

    // obtains path's normals
    const UT_Vector3Array & getNormals() const
    {
	return myNormals;
    }
    
    // obtains path breakpoints' normals
    const UT_Vector3Array & getBreakpointNormals() const
    {
	return myBreakpointNormals;
    }

    // true if path is closed
    bool		    isClosed() const
    {
	return myIsClosed;
    }

    // obtains a tangent at the path start point
    UT_Vector3		    getHeadTangent() const
    {
	return myHeadTangent;
    }

    // obtains a tangent at the path end point
    UT_Vector3		    getTailTangent() const
    {
	return myTailTangent;
    }
 
private: // methods
    
    // suck data from the poly and transfer it to ourselves. Curve
    // is used for finding and interpolating the attributes like
    // the normals. If it is NULL, then no normals will be computed 
    // for the path object. Otherwise the normal_comp_type determines
    // the method for interpolating the normals (provided enough
    // information is given in curve)
    void	getDataFromPoly( const GU_Detail * detail,
				 const GEO_PrimPoly * poly,
				 const GEO_Curve * curve,
				 const UT_DMatrix4 & mat,
				 gu_CalcMethod normal_comp_type =
				    GU_CALC_DEFAULT );

    // construct the path from poly, and assign natually flowing normals
    // to the vertices. If curve is not NULL, also the breakpoint normals
    // for that curve are computed and stored in member data field.
    void	computeNaturalPathFromPoly(const GEO_PrimPoly * poly,
				const GEO_Curve * curve,
				const UT_Vector3& normal, int cv_index );
    
    // interpolate normals between the breakpoints of the curve and
    // set them for our path object 
    // (i.e., for each verex in myVertices compute appropriate normal
    // in myNormals)
    // NB: curve must have the normal attribute
    void	setInterpolatedNormals( const GU_Detail * detail,
					const GEO_Curve * curve );
    
    // for each vertex in myVertices interpolate twist (rotation angle
    // around the curve) between the breakpoints of the curve and 
    // use this twist to find the normal (and set it in myNormals).
    // If clamp_range is true, the twist between the breakpoints
    // is going to be within [-180, 180]. Otherwise the twist is unlimited
    // in its range
    // NB: curve must have the normal attribute
    // NB: all the breakpoints of the curve, must have a corresponding point
    //     in vertices array myVertices that have the same position.
    void	setTwistedNormals( const GU_Detail * detail,
				   const GEO_Curve * curve,
				   bool clamp_range = false);

    // for each vertex in myVertices assign a normal (stored in myNormals)
    // using the natural flow. If curve is not null then its index-th control
    // will have the 'normal' orientation. If curve is null the index-th
    // vertex will have the orientation alligned with normal.
    void	setNaturalNormals( const GEO_Curve *curve, 
				   const UT_Vector3& normal, int index );
	    
    // sets our normals from the poly. It is one to one correspondence
    // with the poly (the normals), thus the poly must have the same
    // number of vertices as we do. 
    void	setPolyNormals( const GU_Detail *detail, 
				const GEO_PrimPoly * poly );
    
    // calculates length as a sum of distances between 'points'. Points
    // should have at least 2 entries. Additionally, if curve and 
    // break_arc_lengts are not null, break_arc_lengths will be filled
    // with arc length parameter for each of curve's breakpoint measured
    // along the points. All curve's breakpoint locations must coincide
    // with some of the points (up to tollerance level). If 
    // vertex_indices is not null it is filled with vertex indices that
    // coincide with the curve's breakpoints.
    float	calculateLength( const UT_Vector3Array & points,
				 const GEO_Curve * curve = NULL,
				 UT_FloatArray * break_arc_lengths = NULL,
				 UT_IntArray * break_vertex_indices =NULL)const;
    
    // returns the angular twist (radians) difference around the curve. That is,
    // the angle by which the normal n0 needs to be rotated, as we traverse
    // down the curve, to allign it with n1. index0 and index1 are point indices
    // corresponing to the normals n0 and n1. 
    // RETURNS:
    //   the twist angle between points index0 and index1 that alligns 
    //   the normals
    float	calculateTwistDelta( int index0, int index1,
			const UT_Vector3 & n0, const UT_Vector3 & n1, 
			const UT_Vector3Array & points ) const;
   
    // rotates to_transform according to the quaternion rotation of the from
    // vector to the to vector
    void	rotateVector( UT_Vector3 from, UT_Vector3 to,
			      UT_Vector3 & to_transform ) const;
    
    // rotates to_transform vector around about vector by the angle
    void	rotateVector( UT_Vector3 about, float angle,
		  	      UT_Vector3 & to_transform ) const;
    
    // pushes the normal forward along the points preserwing zero twist
    // INPUTS:
    //   from_index - the vertex index from which to move normal
    //   normal     - normal to move ( associated with from_index-th vertex )
    //   reuse_direction - the normalized direction from vertex from_index 
    //                to vertex from_index+1. If vector is zero, it is
    //                computed internaly. This is intended for reuse during
    //                forward traversals.
    // OUTPUTS:
    //   normal - moved and transformed normal
    //   reuse_direction - the normalized direction from vertex from_index+1
    //                to vertex from_index+2. If no vertices of such indecis
    //                it is set to zero.
    void    moveNormalForward( const UT_Vector3Array & vertices,
				int from_index, UT_Vector3 & reuse_direction,
				UT_Vector3 & normal ) const;

    // pushes the normal backward along the points preserving zero twist
    // INPUTS:
    //   from_index - the vertex index from which to move normal
    //   normal     - normal to move ( associated with from_index-th vertex )
    //   reuse_direction - the normalized direction from vertex from_index 
    //                to vertex from_index-1. If vector is zero, it is
    //                computed internaly. This is intended for reuse during
    //                backward traversals.
    // OUTPUTS:
    //   normal - moved and transformed normal
    //   reuse_direction - the normalized direction from vertex from_index-1
    //                to vertex from_index-2. If no vertices of such indecis
    //                it is set to zero.
    void    moveNormalBackward( const UT_Vector3Array & vertices,
				int from_index, UT_Vector3 & reuse_direction,
				UT_Vector3 & normal ) const;

    // calculates the start and end tangents of a prim and set them
    // as the path front and back tangents
    void    setTangents( const GEO_Primitive * prim, const UT_DMatrix4 & mat );

    
private: // data
    
    bool		myIsClosed; // true if path is a closed loop
    UT_Vector3Array 	myVertices; // vertices of the path (sampled rope)
    UT_Vector3Array 	myNormals;  // normals of the vertices
    UT_Vector3Array	myBreakpointNormals; // normals of breakpoint 
					     // contol vertices
    UT_IntArray		myBreakpointIndices; // indices to myVertices for each
					     // breakpoint
    UT_Vector3		myHeadTangent;	// tangent at the path start point
    UT_Vector3		myTailTangent;	// tangent at the path end point
};

#endif // __GU_Path_H__
