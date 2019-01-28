/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	The className method is used for saving/loading so there should
 *	  be no spaces in the name.
 *
 */

#pragma once

#ifndef __GEO_Face_H__
#define __GEO_Face_H__

#include "GEO_TriMesh.h"
#include "GEO_API.h"
#include <UT/UT_Array.h>
#include <iosfwd>

class GA_Basis;
class GD_Face;
class GD_Detail;
class GEO_Detail;
class GEO_Vertex;
class GEO_PolyCounts;
class GU_RayInfoHit;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;

class GEO_API GEO_Face : public GEO_TriMesh
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    SYS_FORCE_INLINE
    GEO_Face(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_TriMesh(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_Face() {}

public:
    // Calculate the normal of this face
    UT_Vector3	computeNormal() const override;

    // Accumulate the normal of this face onto its points
    void	normal(NormalComp &output) const override;

    bool	saveH9(std::ostream &os, bool binary,
		       const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
		       const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
			const override;
    bool	loadH9(UT_IStream &is,
		       const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
		       const UT_Array<GA_AttribLoadDataH9> &vtx_attribs) override;

    void	copyPrimitive(const GEO_Primitive *src) override;
    GEO_Primitive	*copy(int preserve_shared_pts = 0) const override;

    void	copyUnwiredForMerge(const GA_Primitive *src,
				    const GA_MergeMap &map) override;

    bool  	isDegenerate() const override;

    virtual unsigned	getOrder(void) const = 0;

    // Convert the real domain values of the primitive to unit values to be
    // used in the evaluation methods below:
    void	 realToUnitPair(float  ureal, float  vreal,
				float &uunit, float &vunit) const override;
    // and vice versa
    void	 unitToRealPair(float  uunit, float  vunit,
				float &ureal, float &vreal) const override;
  
    // Map the normalized length (distance value [0,1]) parameter to the unit 
    // parameterization of the primitve
    void unitLengthToUnitPair(float  ulength, float  vlength,
			      float &uparm,  float &vparm) const override;
    void unitLengthToUnitPair(float  ulength, float  vlength,
			      float &uparm,  float &vparm, float tolerance) const override;
    void unitToUnitLengthPair(float  uparm,   float  vparm, 
			      float &ulength, float &vlength) const override;

    // Evaluate one point (when du=0), or the du-th derivative.
    // Return 0 if successful, and -1 otherwise.
    SYS_DEPRECATED_HDK(13.0)
    bool		evaluate(fpreal u, GEO_Vertex result,
				 GEO_AttributeHandleList &gah,
				 int du=0, int uOffset=-1) const;
    bool		evaluate(fpreal u, GA_Offset result_vtx,
				 GA_AttributeRefMap &gah,
				 int du=0, int uOffset=-1) const
				{ return evaluateRefMap(u, result_vtx, 
							gah, du, uOffset); }
    int			evaluate(float u, UT_Vector4 &pos,
                                 unsigned du=0, int uOffset=-1) const
				{ return evaluateV4(u, pos, du, uOffset); }

    // Evaluate the position or the derivative at domain point (u,v), where
    // u and v MUST be in [0,1]. "v" and "dv" will be ignored here. Return 0 if
    // OK and -1 otherwise.
    // NB: evaluatePoint will return the normal to the face if dv is set.
    int		 evaluateNormalVector(UT_Vector3 &nml, float u,
				      float v = 0, float w = 0) const override;

    // Evaluaes the frenet frame at the u parameter given.  If accurate
    // is zero, the normal of the face is used for the binormal as opposed
    // to the correct value.
    virtual int		evaluateFrenetFrame(UT_Vector3 &x, 
				UT_Vector3 &t, UT_Vector3 &m, UT_Vector3 &b, 
				float u, int accurate = 1, int uoffset = -1) 
				    const;

    // Fill in parameter values that interpolate breakpoints.  The start
    // and stop indices are breakpoint indices.  lod specifies the number
    // of isoparms to insert between breakpoints in each dimension.
    void		fillBreakParameters(UT_FloatArray &uvals,
					    int ustartidx, int ustopidx,
					    int ulod) const;

    // Evaluate the curve for the given parameter array.  The provided
    // result array should provide storage for uvals.entries() values.
    virtual bool	evaluateMesh(const UT_FloatArray &uvals,
			    GA_Offset *results,
			    GA_AttributeRefMap &hlist, unsigned du=0) const;
    virtual bool	evaluateMesh(const UT_FloatArray &uvals,
			    UT_Vector4 *pos, unsigned du=0) const;

    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    virtual int		evaluateBreakpoint(int uidx, UT_Vector4 &pos,
					   int du=0) const = 0;

    // This method takes a start and a stop index in the valid domain, a lod
    // representing number of points between every two breakpoints, an integer
    // index, and return the value of the DOMAIN of point index after the
    // domain has lod points inserted between breakpoints.
    virtual float	breakSegmIndexToDomain(int ustartidx, int ustopidx,
			    int lod, int index) const;

    // Remove all repeated vertices - (only gets consecutive vertices)
    // checkorder refers to inspecting getOrder() and not removing repeated
    // vertices until they exceed order in count.  It has nothing to do
    // with whether the vertices are checked in order.  count_only means
    // that we return the number of repeated vertices without removing them.
    virtual GA_Size	 removeRepeatedVertices(bool check_order = false,
						bool count_only = false,
						bool deleteOrphanedPoints = false);
    virtual GA_Size	 removeRepeatedPoints(float tol = 0.001F,
					      bool check_order = false,
					      bool count_only = false,
					      bool deleteOrphanedPoints = false);

    // Assuming the face is closed, "unroll" it so that the CVs that are
    // shared to form a wrapped curve are made unique. Also, the face becomes
    // open. The base class method only flips the open flag. If the face is
    // not closed, the method returns -1. Otherwise it returns 0.
    virtual int		unroll(int append_pts = 1);


    // Raise the number of CVs to match the newcount. The shape of the face
    // (especially if parametric) should NOT change. Return 0 upon success
    // and -1 otherwise.  start and stop define which indices to examine
    // if newcount is negative it is taken as a relative value.
    virtual int		loft(int newcount, int start=-1, int stop=-1) = 0;


    // Given a parameter in the face domain, insert as many CVs as necessary 
    // to create a discontinuity at the corresponding point on the face. The 
    // shape of the face should NOT change. Return u's index upon success 
    // and -1  otherwise.
    int		subdivide(float u, GA_AttributeRefMap &map)
		    { return subdivideRefMap(u, map); }
    int		subdivide(float u)
		    { return subdivideFloat(u); }

    // Warp the curve at u by the given delta. Change 1 or 2 Cvs and possibly
    // insert a knot once or more as well. If a knot is inserted or we happen
    // to land exactly on a knot, we change only one CV. The bias makes sense
    // only when changing 2 CVs, and will be ignored altogether if < 0.
    // We return the index of the affected knot in the sequence, or -1 if
    // there's an error.
    virtual int		 warp(float u, const UT_Vector3 &delta,
			      GA_AttributeRefMap &map,
			      float sharpness = 0.0f, float bias = -1.0f) = 0;

    // Warp the face along its normal at u. Return the index of the afected
    // knot in the sequence, or -1 if there's an error.
    int			warpAlongNormal(float u, float distance,
				GA_AttributeRefMap &map,
				float sharpness, float bias);

    // Append another face to us in one of two ways: blend the two endpoints
    // or connect them straight or rounded. The bias ranges from 0 to 1 and is
    // relevant only to blending. The tolerance for blending: if 0, the two
    // endpoints will merge into one point with a discontinuity; if less than
    // 1, we insert knots into the curves to minimize the affected areas; if 1,
    // no refinement is done. For the non-blend case, the tolerance will
    // generate a span whose shape goes from round to straight; 0 tolerance
    // means straight connection. If unrefine is on, we'll try to reduce the
    // complexity of the face if we're connecting rounded. We return 0 if OK
    // and -1 if error. Both curves must be open and have the same order.
    virtual int		attach(const GEO_Face &face, int blend = 1,
			       float bias = 0.5f, float tolerance = 1.0f,
			       int unrefine=1, GA_PointGroup *ptgroup=0) = 0;

    // Build a planar (domain) face of the same type as us and with the same
    // number of vertices. Copying the (x,y) values of our points to the planar
    // face is optional.
    virtual GD_Face	*planar(GD_Detail &dgdp, int copyxy = 0) const = 0;

    // Go from a normalized domain ([0,1]) to the real domain or vice versa.
    virtual float	unitToRealDomain(float u_unit) const;
    virtual float	realToUnitDomain(float u_real) const;
    
    // Map the normalized length (distance value [0,1]) parameter to the unit 
    // parameterization of the primitve. The tolerance specifies max
    // error of the returned value (cannot be 0.0f).
    virtual float	 unitLengthToUnitDomain(float  ulength,
						float tolerance = 1e-04F) const;

    virtual float	 unitToUnitLengthDomain(float  uparm ) const;


    // Calculate the real domains for n consecutive operations on the domain
    // given n normalized domains and the operation
    // Here the real domain ranges from 0 to vertexcount - 1 + isClosed().
    virtual void	unitToRealSequence(float *uunit, float *ureal, 
				int ulen) const;
    static void		unitToRealAdjust(float *uunit, float *ureal, 
				int ulen, int num);

    // Return the bounds of the valid evaluation interval in domain space:
    // For polygons this refers to cv indices.
    virtual void	validInterval(int &a, int &b) const;
    virtual void	validRange(float &ua, float &ub) const;

    SYS_FORCE_INLINE bool isClosed() const
    {
        return myVertexList.getExtraFlag();
    }
    /// NOTE: This won't update the basis for NURBS/Bezier curves!
    ///       It just sets the closed flag, so make sure the bases
    ///       get set afterward!
    SYS_FORCE_INLINE void setClosed(bool isclosed)
    {
        myVertexList.setExtraFlag(isclosed);
    }
    virtual void	 close(int rounded = 1, int preserveShape = 0);
    virtual void	 open(int preserveShape = 0, int safe = 0);

    bool	 hasEdge(const GA_Edge &edge) const override;

    // Methods to handle vertex attributes for the attribute dictionary
    void iterateEdges(GA_IterateEdgesFunc apply_func) const override;
    void iterateEdgesByVertex(GA_IterateEdgesByVertexFunc apply_func) const override;
    
    // Returns the number of edges on this face.
    int			 getEdgeCount() const;

    // Reverses the vertices of a given face. It's virtual because some faces,
    // such as rational splines, need to be aware of it.
    void reverse() override;

    // Append n points equally spaced between the 1st and last vertex.
    void		 sampleEnds(int n);

    // Shift the array of vertices by an offset and wrap around. The offset
    // can be either negative or positive.
    virtual int	 	 cycle(int amount, int = 1);

    // Find the center and radius of the face
    float		 radius() const;

    // Finds which vertex#'s the given points have.  The points should make
    // an edge.
    // Returns false if the points do not define an edge on this face,
    // true if they do (in which case pidx0 and pidx1 are the vertex#'s)
    bool		 findEdgePoints(GA_Offset pta, GA_Offset ptb,
				        GA_Size &pidx0, GA_Size &pidx1) const;
    bool		 findEdgePoints(const GA_Edge &edge,
				        GA_Size &pidx0, GA_Size &pidx1) const;


    // True if the directed edge a->b is in this face.
    bool		 hasDEdge(GA_Offset a, GA_Offset b) const;

    /// True if both the directed edges a->b and b->a are inside this
    /// face
    bool		 isBridge(GA_Offset a, GA_Offset b) const;

    // Change the multiplicity of the domain value by inserting it after
    // checking its current multiplicity.  Return -1 if invalid, otherwise
    // return the index of the inserted knot

    int			refine(float k, GA_AttributeRefMap &gah, int r=1)
			    { return refineRefMap(k, gah, r); }
    int			refine (float k, int r=1)
			    { return refineInt(k, r); }

    // Find enclosing integral values(c1, c2) which validly enclose c
    // and return the distance to c1
    static fpreal	getIndices(fpreal c, int &c1, int &c2, 
				   int maxIndex, int wrap);

    // Halper function to compute real parameter values that interpolate
    // breakpoints for curves.
    static void		fillBreakVals(const GA_Basis &ub,
				      UT_FloatArray &uvals,
				      int ustartidx,
				      int ustopidx,
				      int ulod, bool wrap);

    // Determine whether the values in `vals` are uniformly spaced in
    // [idx,nextidx) and if so return true and store the spacing in `step`
    static bool		getUniformStep(const UT_FloatArray &vals,
				       int idx, int nextidx,
				       fpreal &step);

    /// This method determines whether the point is inside or outside the
    /// polygon.  The point is assumed to be on the plane of the polygon.
    /// Note that normal is a pointer to this primitive's normal which cannot
    /// be NULL.
    /// If the point is on the boundary of the face, it is reported as being
    /// inside the face with -1 return.
    virtual int		pointInFace(const UT_Vector3 &pos,
				    const UT_Vector3 *normal) const;

    // Find distance in parameter space, aware of wrapping.
    float	 uvDist(float u1, float v1, float u2, float v2) const override;

    // Do nothing for poly's
    virtual void	weights(unsigned short onOff);
    virtual int		breakCount() const = 0;

    fpreal	calcVolume(const UT_Vector3 &refpt) const override;
    fpreal	calcArea() const override;
    fpreal	calcPerimeter() const override;

    // Intersect this with the other primitive, as a series of edges, not
    // a filled face.
    virtual int		intersect(const GEO_Primitive &prim,
				  UT_Array<GU_RayInfoHit> &hitList,
				  float tol = 0.01F, int ignoretrim=1) const =0;

    virtual void computeInteriorPointWeights(
		    UT_Array<GA_Offset> &vtxlist, UT_Array<float> &weightlist, 
		    fpreal u, fpreal v, fpreal w) const override;

protected:
    /// Builds faces using the specified range of point offsets,
    /// as dictated by facesizelist and facepointnumbers, in parallel.
    /// facepointnumbers lists the *offsets* of the points used by
    /// each face *MINUS* startpt, i.e. they are offsets relative to startpt,
    /// *not* indices relative to startpt. The offset of the first face is
    /// returned, and the rest are at consecutive offsets. All
    /// facepointnumbers must be between 0 (inclusive) and
    /// npoints (exclusive).
    ///
    /// NOTE: Existing primitives *are* allowed to be using the points in
    /// the specified range already, and the faces being created do not
    /// need to use all of the points in the range.  However,
    /// these cases may impact performance.
    ///
    /// NOTE: If you can guarantee that no other primitives are using the
    /// points and that the vertices of these new faces will be using the
    /// points starting from startpt in order and continuing contiguously,
    /// e.g. with independent curves whose points were allocated with
    /// GA_Detail::appendPointBlock, pass in NULL for facepointnumbers.
    /// This option should be used with caution.
    /// @{
    static GA_Offset buildBlock(
        GA_PrimitiveTypeId type,
        GA_Detail *detail,
        const GA_Offset startpt,
        const GA_Size npoints,
        const GEO_PolyCounts &facesizelist,
        const int *facepointnumbers,
        const bool closed = true);
    static GA_Offset buildBlock(
        GA_PrimitiveTypeId type,
        GA_Detail *detail,
        const GA_Offset startpt,
        const GA_Size npoints,
        const GA_PolyCounts &facesizelist,
        const int *facepointnumbers,
        const bool closed = true);
    /// @}

    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    {
				return static_cast<GA_PrimitiveFamilyMask>(
					    GEO_TriMesh::buildFamilyMask() |
					    GEO_FAMILY_FACE
					);
			    }

    // Declare intrinsic attribute functions
    GA_DECLARE_INTRINSICS(override);

    bool	savePrivateH9(std::ostream &os, bool binary) const override= 0;
    bool	loadPrivateH9(UT_IStream &is) override = 0;

    // Check the validity of the data. Meant to be called especially at loading
    // time. The method returns 1 if OK and 0 if trouble.
    bool	 validate() const override;

    GA_Size	 getMinVertexCount() const override { return isClosed() ? 3:2; }

    bool	evaluatePointRefMap(GA_Offset result_vtx,
				    GA_AttributeRefMap &vtxdata,
				    fpreal u, fpreal v=0,
				    uint du=0, uint dv=0) const override;
    int		evaluatePointV4(UT_Vector4 &pos, float u_unit, float=0,
			        unsigned du=0, unsigned dv = 0) const override;

    virtual bool	evaluateRefMap(fpreal u, GA_Offset result_vtx,
				 GA_AttributeRefMap &gah,
				 int du=0, int uOffset=-1) const;
    virtual int		evaluateV4(float u, UT_Vector4 &pos,
                                 unsigned du=0, int uOffset=-1) const;

    virtual int		subdivideRefMap(float u, GA_AttributeRefMap &map);
    virtual int		subdivideFloat(float u);

    virtual int		refineRefMap(float k, GA_AttributeRefMap &gah, int r=1);
    virtual int		refineInt(float k, int r=1);

private:

    friend std::ostream	&operator<<(std::ostream &os, const GEO_Face &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()
#endif
