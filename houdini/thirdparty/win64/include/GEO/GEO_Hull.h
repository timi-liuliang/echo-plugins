/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class is used as a base class for all patch types supported.
 *	The vertex list management is handled by this class, however, all
 *	other function should be handled by the subclass.
 *	The insertRow and insertCol are meant to be virtual since it
 *	may be possible for the patch type to insert a row or column
 *	without distorting the shape of the patch (as in NURBs or MESH)
 *
 */

#pragma once

#ifndef __GEO_Hull_H__
#define __GEO_Hull_H__

#include "GEO_API.h"
#include "GEO_Primitive.h"
#include "GEO_SurfaceType.h"
#include "GEO_Vertex.h"
#include <GA/GA_OffsetMatrix.h>
#include <UT/UT_Assert.h>
#include <UT/UT_RefMatrix.h>
#include <UT/UT_Vector3.h>
#include <iosfwd>

class UT_BitArray;
class GA_Detail;
class GA_EdgeGroup;
class UT_JSONWriter;
class UT_JSONParser;
class GA_SaveMap;
class GA_LoadMap;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;

class GEO_API GEO_HullFlags
{
public:
	GEO_HullFlags()
	{
		wrapu = 0;
		wrapv = 0;
	}
    bool	wrapu:1;
    bool	wrapv:1;
    bool	loadBinary( UT_IStream &is );
    int		saveBinary( std::ostream &os ) const;
};

class GEO_API GEO_SubHullFlags
{
public:
	GEO_SubHullFlags()
	{
		wrapu       = 0;	wrapv       = 0;
		dupTop      = 0;	dupLeft     = 0;
		dupBottom   = 0;	dupRight    = 0;
		breakThread = 0;	breakBridge = 0;
		recurse     = 1;
	}

    unsigned 	wrapu:1,
        	wrapv:1,
		dupTop:1,
        	dupLeft:1,
		dupBottom:1,
        	dupRight:1,
		breakThread:1,
        	breakBridge:1,
		recurse:1;
};

class GEO_API GEO_SubHullPart
{
public:
		GEO_SubHullPart() { h_edge = v_edge = d_edge = flags = 0; }

    int		h_edge;
    int		v_edge;
    int		d_edge;
    int		flags;
		// `flags' is used as follows:
		// Bit 0 - horizontal edge is used to break into a subhull
		// Bit 1 - vertical edge	"		"
		// Bit 2 - diagonal edge	"		"

    bool	operator==(const GEO_SubHullPart &p) const
    {	return( (h_edge==p.h_edge) && 
		(v_edge==p.v_edge) && 
		(d_edge==p.d_edge) &&
		(flags ==p.flags )   );
    }
    bool	operator!=(const GEO_SubHullPart &p) const
    { return !(*this == p); }
};

class GEO_API GEO_SubHull
{
public:
		GEO_SubHull() : mat() {}

    UT_RefMatrix<GEO_SubHullPart>	 mat;
    int					 v_rows, v_cols;
    int					 h_rows, h_cols;
    int					 d_rows, d_cols;
};

class GEO_API GEO_Hull : public GEO_Primitive
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_Hull(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Primitive(d, offset)
#if GA_PRIMITIVE_VERTEXLIST
        , myRows(0)
        , myCols(0)
#endif
    {}

#if !GA_PRIMITIVE_VERTEXLIST
    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_Hull();
#endif

public:
    bool		saveVertexArray(UT_JSONWriter &w,
				const GA_SaveMap &save) const;
    bool		loadVertexArray(UT_JSONParser &p,
				const GA_LoadMap &load);

    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    virtual int		evaluateBreakpoint(int uidx, int vidx,
					   UT_Vector4 &pos,
					   int du=0, int dv=0) const = 0;

    // Evaluate one point (when du=dv=0), or the du-th dv-th derivative.
    // Returns true if successful, and false otherwise.
    bool	    evaluateIndex(fpreal u, fpreal v, GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				unsigned du, unsigned dv) const
    { return evaluateIndexRefMap(u, v, result_vtx, hlist, du, dv); }
    int		    evaluateIndex(float iu, float iv, UT_Vector4 &pos,
				unsigned du=0, unsigned dv=0) const
			{ return evaluateIndexV4(iu, iv, pos, du, dv); }

    virtual void computeInteriorPointWeights(
		    UT_Array<GA_Offset> &vtxlist, UT_Array<float> &weightlist, 
		    fpreal u, fpreal v, fpreal w) const override;

    void normal(NormalComp &output) const override;

    // Evaluate the normal using unit u and v coordinates (i.e. in [0,1]).
    // The normal is not normalized. The method does NOT fail if the point
    // attribute is missing. We return 0 if OK and -1 if error.
    int evaluateNormalVector(UT_Vector3 &nml, float u, 
			     float v = 0, float w = 0) const override;

    // Evaluate the normal at real (u,v). The method does not fail if there
    // is no point attribute, it instead just returns the face normal.
    // Return 0 if successful, and -1 otherwise.
    virtual int		evaluateNormal(float u,float v, UT_Vector3 &nml) const;
    virtual int		normalIndex(float iu, float iv, UT_Vector3 &nml) const;

    virtual int		uMinValidIndex() const;
    virtual int		uMaxValidIndex() const;
    virtual int		vMinValidIndex() const;
    virtual int		vMaxValidIndex() const;

    bool saveH9(std::ostream &os, bool binary,
		const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
		const UT_Array<GA_AttribSaveDataH9> &vtx_attribs) const override;
    bool loadH9(UT_IStream &is,
		const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
		const UT_Array<GA_AttribLoadDataH9> &vtx_attribs) override;
    int	getBBox(UT_BoundingBox *bbox) const override;
    void addToBSphere(UT_BoundingSphere *bsphere) const override;
    UT_Vector3	baryCenter() const override;
    UT_Vector3	computeNormal() const override;
    void copyPrimitive(const GEO_Primitive *src) override;
    GEO_Primitive *copy(int preserve_shared_pts = 0) const override;

    void copyUnwiredForMerge(const GA_Primitive *src,
			     const GA_MergeMap &map) override;

#if !GA_PRIMITIVE_VERTEXLIST
    void addPointRefToGroup(GA_PointGroup &grp) const override;
#endif

    // Reverse rows (V) and/or columns (U):
    void reverse () override;
    virtual void reverseU();
    virtual void reverseV();

    // Shift the array of vertices by an offset and wrap around. The offset
    // can be either negative or positive.  Cycles in complete rows/cols
    virtual int 	cycleU(int amount, int = 1);
    virtual int 	cycleV(int amount, int = 1);

    // All current rows and columns will be reinitialized
    int			setRowCol(int r, int c);
    virtual int		insertRow(unsigned int beforeWhich, bool appendPts=true);
    int			appendRow(bool appendPts=true)
    {
#if GA_PRIMITIVE_VERTEXLIST
        return insertRow(myRows, appendPts);
#else
        return insertRow(myVertexMatx.getRows(), appendPts);
#endif
    }
    virtual int		insertCol(unsigned int beforeWhich, bool appendPts=true);
    int			appendCol(bool appendPts=true)
    {
#if GA_PRIMITIVE_VERTEXLIST
        return insertCol(myCols, appendPts);
#else
        return insertCol(myVertexMatx.getCols(), appendPts);
#endif
    }

    virtual int 	deleteRow(unsigned int which);
    virtual int 	deleteCol(unsigned int which);

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    int detachPoints (GA_PointGroup &grp) override;

    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    GA_DereferenceStatus dereferencePoint(
	GA_Offset point, bool dry_run=false) override;
    GA_DereferenceStatus dereferencePoints(
	const GA_RangeMemberQuery &pt_q, bool dry_run=false) override;

    // Given a parameter in the domain, insert as many CVs as necessary to
    // create a discontinuity at the corresponding point on the curve.The shape
    // of the curve should NOT change. Return u's index upon success and -1
    // otherwise.
    int		subdivideU(float u, GA_AttributeRefMap &map)
		    { return subdivideURefMap(u, map); }
    int		subdivideU(float u)
		    { return subdivideUFloat(u); }
    int		subdivideV(float u, GA_AttributeRefMap &map)
		    { return subdivideVRefMap(u, map); }
    int		subdivideV(float u)
		    { return subdivideVFloat(u); }

    virtual void	subdivide(int numdivs, GA_PointGroup *ptgroup=0);

    // Warp the hull at u,v by the given delta. Change 1 or 4 Cvs and possibly
    // insert a knot once or more as well. If a knot is inserted or we happen
    // to land exactly on a knot, we change only one CV. The bias makes sense
    // only when changing 4 CVs, and will be ignored altogether if < 0.
    // We return the CV index in warpU/V and 0 in warp() if OK; -1 otherwise.
    virtual int	warpU(float u, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float sharpness = 0.0f, float bias = -1.0f) = 0;
    virtual int	warpV(float v, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float sharpness = 0.0f, float bias = -1.0f) = 0;
    virtual int	warp (float u, float v, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float usharpness = 0.0f, float vsharpness = 0.f,
			       float ubias = -1.0f, float vbias = -1.0f) = 0;


    // Warp the hull along its normal at u,v. We warp u if the flag is 0,
    // warp v if flag is 1, and warp both if flag is 2.
    // We return 0 if OK, or -1 if there's an error.
    int			 warpAlongNormal (float u, float v, float distance,
					  GA_AttributeRefMap &map,
					  float usharpness, float vsharpness,
					  float ubias, float vbias,
					  int u_v_both);

    // Append another hull to us in one of two ways: blend the two endpoints
    // or connect them straight or rounded. The bias ranges from 0 to 1 and is
    // relevant only to blending. The tolerance for blending: if 0, the two
    // endpoints will merge into one point with a discontinuity; if less than
    // 1, we insert knots into the hulls to minimize the affected areas; if 1,
    // no refinement is done. For the non-blend case, the tolerance will
    // generate a span whose shape goes from round to straight; 0 tolerance
    // means straight connection. If unrefine is on, we'll try to reduce the
    // complexity of the hull if we're connecting rounded. We return 0 if OK
    // and -1 if error. Both hulls must be open and have the same order.
    virtual int		attachU(const GEO_Hull &hull, int blend = 1,
			        float bias = 0.5f, float tolerance = 1.0f,
			        int unrefine=1, GA_PointGroup *ptgroup=0) = 0;
    virtual int		attachV(const GEO_Hull &hull, int blend = 1,
			        float bias = 0.5f, float tolerance = 1.0f,
			        int unrefine=1, GA_PointGroup *ptgroup=0) = 0;

    // Change the multiplicity of the domain value by inserting it after
    // checking its current multiplicity.  Return -1 if invalid, otherwise
    // return the index of the inserted knot

    virtual int			refineU(float k,
					GA_AttributeRefMap &hlist,
					int i=1);

    virtual int                 refineU		( float k, int i=1);
    virtual int			refineV(float k,
					GA_AttributeRefMap &hlist,
					int i=1);
    virtual int                 refineV		( float k, int i=1);

    // Refine numdivs times in each span.
    void			spanRefineU(GA_AttributeRefMap &map,
					    int numdivs=1)
				    { spanRefineURefMap(map, numdivs); }
    void			spanRefineU( int numdivs=1)
				    { spanRefineUInt(numdivs); }
    void			spanRefineV(GA_AttributeRefMap &map,
					    int numdivs=1)
				    { spanRefineVRefMap(map, numdivs); }
    void			spanRefineV( int numdivs=1)
				    { spanRefineVInt(numdivs); }

    // Remove rows and or columns based on a curvature tolerance. Return 1 of
    // something was removed, else 0.
    int				unrefineU(int kidx, GA_AttributeRefMap &h,
					int mult=0, float tol=1e-4f,
					GA_PointGroup *delgroup=0)
				    { return unrefineURefMap(kidx, h, mult,
							    tol, delgroup); }
    int				unrefineU(int kidx, int mult = 0,
						 float tol = 1e-4F,
						 GA_PointGroup *delgroup = 0)
				    { return unrefineUFloat(kidx, mult, tol,
								delgroup); }
    int				unrefineV(int kidx, GA_AttributeRefMap &h,
					int mult=0, float tol=1e-4f,
					GA_PointGroup *delgroup=0)
				    { return unrefineVRefMap(kidx, h, mult,
							     tol, delgroup); }
    int				unrefineV(int kidx, int mult = 0,
						 float tol = 1e-4F,
						 GA_PointGroup *delgroup = 0)
				    { return unrefineVFloat(kidx, mult, tol,
								delgroup); }

    // If the hull is wrapped in U and/or V, explicitly add the wrapped vertex
    // and open the hull in the direction(s) it's wrapped in.
    virtual void	fixSeamsU(void);
    virtual void	fixSeamsV(void);

    /// Apply row-column texture. If ptattrib is true, we deal with
    /// points, else with vertices. Returns false iff problems.
    bool		rowColTexture(const GA_RWHandleV3 &txth, bool ptattrib);

    SYS_FORCE_INLINE
    int getNumRows() const
#if GA_PRIMITIVE_VERTEXLIST
    { return myRows; }
#else
    { return (int)myVertexMatx.getRows(); }
#endif
    SYS_FORCE_INLINE
    int getNumCols() const
#if GA_PRIMITIVE_VERTEXLIST
    { return myCols; }
#else
    { return (int)myVertexMatx.getCols(); }
#endif

    // Subscript operators. The const version does not check the boundaries.
    // For safer but slower use, call getVertex() and setVertex().
    SYS_DEPRECATED_PUSH_DISABLE()
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Vertex	operator()(unsigned int r, unsigned int c) const
					{ return getVertexElement(r, c); }
    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex		operator()(unsigned int r, unsigned int c)
					{ return getVertexElement(r, c); }

    SYS_DEPRECATED_POP_DISABLE()
    SYS_DEPRECATED_HDK(13.0)
    const GEO_Vertex	getVertexElement(unsigned int r, unsigned int c) const;

#if GA_PRIMITIVE_VERTEXLIST
    using GA_Primitive::getVertexOffset;
#endif

    SYS_FORCE_INLINE GA_Offset getVertexOffset(unsigned int r, unsigned int c) const
    {
        UT_ASSERT_P(r < getNumRows() && c < getNumCols());
        if (r >= getNumRows() || c >= getNumCols())
            return GA_INVALID_OFFSET;
#if GA_PRIMITIVE_VERTEXLIST
        return getVertexOffset(r*exint(myCols) + c);
#else
        return myVertexMatx(r, c);
#endif
    }
    SYS_FORCE_INLINE GA_Index getVertexIndex(unsigned int r, unsigned int c) const
    {
        GA_Offset vtxoff = getVertexOffset(r, c);
        return GAisValid(vtxoff) ? getDetail().vertexIndex(vtxoff)
                                 : GA_INVALID_INDEX;
    }
    SYS_FORCE_INLINE GA_Offset getPointOffset(unsigned int r, unsigned int c) const
    {
        if (r >= getNumRows() || c >= getNumCols())
        {
            UT_ASSERT(!"Invalid offset");
            return GA_INVALID_OFFSET;
        }

        return vertexPoint(r, c);
    }
    SYS_FORCE_INLINE GA_Index getPointIndex(unsigned int r, unsigned int c) const
    {
        GA_Offset ptoff = getPointOffset(r, c);
        return GAisValid(ptoff) ? getDetail().pointIndex(ptoff)
                                : GA_INVALID_INDEX;
    }
    SYS_FORCE_INLINE UT_Vector3 getPos3(unsigned int r, unsigned int c) const
    { return getDetail().getPos3(getPointOffset(r, c)); }
    SYS_FORCE_INLINE void setPos3(unsigned int r, unsigned int c, const UT_Vector3 &pos) const
    { getDetail().setPos3(getPointOffset(r, c), pos); }
    SYS_FORCE_INLINE UT_Vector4 getPos4(unsigned int r, unsigned int c) const
    { return getDetail().getPos4(getPointOffset(r, c)); }
    SYS_FORCE_INLINE void setPos4(unsigned int r, unsigned int c, const UT_Vector4 &pos) const
    { getDetail().setPos4(getPointOffset(r, c), pos); }
    SYS_FORCE_INLINE void setPointOffset(unsigned int r, unsigned int c, GA_Offset ptoff) const
    { getDetail().getTopology().wireVertexPoint(getVertexOffset(r,c), ptoff); }

    SYS_FORCE_INLINE GA_Size getFastVertexCount() const
    {
        return exint(getNumRows()) * exint(getNumCols());
    }
    void setVertexPoint(unsigned int r, unsigned int c, GA_Offset pt)
    {
        if (r >= getNumRows() || c >= getNumCols())
            return;

        setPointOffset(r, c, pt);
    }

#if !GA_PRIMITIVE_VERTEXLIST
    void	beginVertex(const_iterator &i) const override;
    void	nextVertex(const_iterator &i) const override;
#endif

    GEO_SurfaceType	getSurfaceType() const		 { return surfaceType; }
    void		setSurfaceType(GEO_SurfaceType t) { surfaceType = t; }

    /// @{
    /// Get or set the surface type by name token
    const char		*getSurfaceTypeName() const;
    bool		 setSurfaceTypeName(const char *name);
    /// @}

    bool		isWrappedU() const { return flags.wrapu; }
    bool		isWrappedV() const { return flags.wrapv; }

    /// @{
    /// Convenience method to set wrapping based on intrinsic properties
    bool		setWrapU(bool dowrap);
    bool		setWrapV(bool dowrap);
    /// @}

    virtual void	wrapU(int rounded = 1, int preserveShape = 0);
    virtual void	openU(int preserveShape = 0, int safe = 0);
    virtual void	wrapV(int rounded = 1, int preserveShape = 0);
    virtual void	openV(int preserveShape = 0, int safe = 0);
    
    virtual bool	isClampedU() const;
    virtual bool	isClampedV() const;

    // Assuming the hull is closed in u/v, "unroll" it so that the CVs that are
    // shared to form a wrapped surface are made unique. Also, the hull becomes
    // open. The base class method only flips the open flag. If the face is
    // not closed, the method returns -1. Otherwise it returns 0.
    virtual int         unrollU(int append_pts = 1);
    virtual int         unrollV(int append_pts = 1);

    bool	hasEdge(const GA_Edge &edge) const override;

    // Check degenerate condition by testing face area in u and v
    bool	isDegenerate() const override;

#if !GA_PRIMITIVE_VERTEXLIST
    bool vertexApply(bool (*apply)(GA_Offset vtx, void *), 
		     void  *data = nullptr) const override;
#endif

    void iterateEdges(GA_IterateEdgesFunc apply_func) const override;
    void iterateEdgesByVertex(GA_IterateEdgesByVertexFunc apply_func) const override;

#if !GA_PRIMITIVE_VERTEXLIST
    GA_Size	getVertexCount() const override;
    GA_Offset	getVertexOffset(GA_Size index) const override;
#endif

    /// return the index of a vertex within our vertex list
#if GA_PRIMITIVE_VERTEXLIST
    GA_Size findVertex(GA_Offset vtx) const { return myVertexList.find(vtx); }
#else
    GA_Size findVertex(GA_Offset vtx) const { return myVertexMatx.find(vtx); }
#endif

    // Have we been deactivated and stashed?
    virtual void stashed(bool beingstashed, 
			GA_Offset offset = GA_INVALID_OFFSET) override;

    // Raise the number of CVs to match the newcount. The shape of the face
    // (especially if parametric) should NOT change. Return 0 upon success
    // and -1 otherwise.  start and stop define which indices to examine
    // if newcount is negative it is taken as a relative value.
    virtual int         loftU(int newcount, int start=-1, int stop=-1) = 0;
    virtual int         loftV(int newcount, int start=-1, int stop=-1) = 0;


    // Convert the real domain values of the primitive to unit values to be
    // used in the common evaluation methods:
    void realToUnitPair(float  ureal, float  vreal,
			float &uunit, float &vunit) const override;
    // and vice versa
    void unitToRealPair(float  uunit, float  vunit,
			float &ureal, float &vreal) const override;

    // Go from a normalized domain ([0,1]) to the real domain or vice versa.
    // In the base class (i.e. here) the real domain ranges from 0 to
    // vertexcount - 1 + isWrapped() for u and v respectively.
    virtual void	unitToRealDomain(float  u_unit, float  v_unit,
					 float &u_real, float &v_real) const;
    virtual void	realToUnitDomain(float  u_real, float  v_real,
					 float &u_unit, float &v_unit) const;

    // Calculate the real domains for n consecutive operations on the domain
    // given n normalized domains and the operation

    virtual void	unitToRealSequenceU(float *uunit, float *ureal, 
					int ulen) const;
    virtual void	unitToRealSequenceV(float *vunit, float *vreal, 
					int vlen) const;

    // Finds the vertex#'s of the end points of an edge.
    // returns 0 if it fails (the edge is not on the hull) , 1 if it succeeds
    virtual int		findEdgePoints(const GA_Edge &edge,
				       int *pr0, int *pc0, 
				       int *pr1, int *pc1) const;

    virtual int		findEdgePoints(GA_Offset a, GA_Offset b,
				       int *pr0, int *pc0, 
				       int *pr1, int *pc1) const;

    // This method fills the horizontal and vertical edge matrices
    // based on the input linked list.  An element of the matrix is
    // non-zero if one of the selected edges exists on that spot on the
    // hull, and zero otherwise.  It returns 0 if no edges are on the hull,
    // and 1 if at least one edge matches.
    int			 makeEdgeMatrix(const GA_EdgeGroup &edges,
					GEO_SubHull &subhull);

    // Append n points equally spaced between the 1st and last vertex.
    void                sampleEndsU(int n);
    void                sampleEndsV(int n);

    // Return the bounds of the valid evaluation interval in domain space:
    // For meshes this refers to cv indices.
    virtual void	validURange(float &ua, float &ub) const;
    virtual void	validVRange(float &va, float &vb) const;
    virtual void	validUInterval(int &a, int &b) const;
    virtual void	validVInterval(int &a, int &b) const;

    // Find enclosing integral values(c1, c2) which validly enclose c
    // and return the distance to c1
    float               getIndices(float c, int &c1, int &c2,
				   int maxIndex, int wrap) const;

    // Reverse the roles of rows and columns
    virtual void	transpose();

    // Return the surrounding values of the real-space u,v parameters.
    // Returns 1 if succesful, 0 if out-of-range.
    int		parametricBBox(float u, float v,
			       float *u0, float *u1,
			       float *v0, float *v1) override;

    // Find distance in parameter space, aware of wrapping.
    float	 uvDist(float u1, float v1, float u2, float v2) const override;

    virtual void	 weights(unsigned short onOff);

    fpreal	calcVolume(const UT_Vector3 &refpt) const override;
    fpreal	calcArea() const override;
    fpreal	calcPerimeter() const override;

    static const inline UT_Vector3 quadNormal(
        const UT_Vector3 &p1, const UT_Vector3 &p2,
        const UT_Vector3 &p3, const UT_Vector3 &p4)
    {
        UT_Vector3 nml(0, 0, 0);
        nml.normal(p1, p2);
        nml.normal(p2, p3);
        nml.normal(p3, p4);
        nml.normal(p4, p1);

        nml.normalize();
        return nml;
    }

    /// This is a wrapper for a single quad of a GEO_Hull, so that
    /// it can be treated like a GEO_PrimPoly in a template.
    class Poly {
    public:
        Poly(const GEO_Hull &hull, const exint row, const exint col)
            : myHull(hull)
            , myRow(row)
            , myCol(col)
            , myRowP1((row+1 == hull.getNumRows()) ? 0 : (row+1))
            , myColP1((col+1 == hull.getNumCols()) ? 0 : (col+1))
        {}
        SYS_FORCE_INLINE GA_Size getFastVertexCount() const
        { return 4; }
        SYS_FORCE_INLINE GA_Offset getFastVertexOffset(GA_Size i) const
        { return myHull.getVertexOffset((i >= 2) ? myRowP1 : myRow, (i==1 || i==2) ? myColP1 : myCol); }
        SYS_FORCE_INLINE GA_Offset getPointOffset(GA_Size i) const
        { return myHull.getPointOffset((i >= 2) ? myRowP1 : myRow, (i==1 || i==2) ? myColP1 : myCol); }
        SYS_FORCE_INLINE UT_Vector3 getPos3(GA_Size i) const
        { return myHull.getPos3((i >= 2) ? myRowP1 : myRow, (i==1 || i==2) ? myColP1 : myCol); }
        SYS_FORCE_INLINE bool isClosed() const
        { return true; }
        float calcArea() const
        {
            UT_Vector3 p[4];
            for (GA_Size i = 0; i < getFastVertexCount(); ++i)
                p[i] = getPos3(i);
            float area = cross(p[0]-p[1],p[2]-p[1]).length()
                       + cross(p[0]-p[3],p[2]-p[3]).length();
            area *= 0.5f;
            return area;
        }
    private:
        const GEO_Hull &myHull;
        const exint myRow;
        const exint myRowP1;
        const exint myCol;
        const exint myColP1;
    };

protected:
#if !GA_PRIMITIVE_VERTEXLIST
    void	clearForDeletion() override;
#endif

    void cycleOffsetListRows(int shift);
    void cycleOffsetListCols(int shift);

    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GEO_FAMILY_HULL; }

    // Declare intrinsic attribute functions
    GA_DECLARE_INTRINSICS(override);

    // The following three methods are used by removeEdges to
    // remove all possible selected edges from a hull.

    // This computes where the hull needs to be split, recursing downwards
    // if necessary.  It calls getSubHull to split the hulls.  Returns 1 if
    // the hull needs to be deleted as a result of breaking.
    int			 breakHull(GEO_SubHull &subhull, 
				   int top, int left, int bottom, int right,
				   GEO_SubHullFlags break_flags);

    // This figures out which edges were actually used in the edge deletion.
    // It returns 1 if all the selected edges were used, 0 if some were not
    int			 checkUsedEdges(UT_BitArray *remove_edges,
					const GEO_SubHull &subhull);

    // Implemented in GU.  Gets a part of this hull, creating a new one
    // if break_flags.recurse is set.
    virtual GEO_Hull	*getSubHull(int top, int left, int bottom, int right,
				    GEO_SubHullFlags break_flags) = 0;

    // Speacial save and load methods for the sub-classes:
    virtual bool	savePrivateH9(std::ostream &os, bool binary) const = 0;
    virtual bool	loadPrivateH9(UT_IStream &is) = 0;
    virtual bool	saveExtraH9  (std::ostream &os, bool binary) const = 0;
    virtual bool	loadExtraH9  (UT_IStream &is) = 0;

    // Check the validity of the data. Meant to be called especially at loading
    // time. The method returns 1 if OK and 0 if trouble.
    virtual bool	validate(void) const = 0;

    // Clip or wrap c to find enclosing integral values(c1, c2)
    // and return the distance to c1 
    float 		getLRIndex(float c, int &c1, int &c2, 
				   int minIndex, int maxIndex, int wrap) const;

    /// @warning vertexPoint() doesn't check the bounds.  Use with caution.
    SYS_FORCE_INLINE GA_Offset vertexPoint(GA_Size r, GA_Size c) const
    {
        UT_ASSERT_P(r < getNumRows() && c < getNumCols());
#if GA_PRIMITIVE_VERTEXLIST
        return getDetail().vertexPoint(getVertexOffset(r*exint(myCols) + c));
#else
        return getDetail().vertexPoint(myVertexMatx(r,c));
#endif
    }

#if !GA_PRIMITIVE_VERTEXLIST
    /// Defragmentation
    void swapVertexOffsets(const GA_Defragment &defrag) override;
#endif

#if GA_PRIMITIVE_VERTEXLIST
    SYS_FORCE_INLINE
    GA_Size matrixIndex(int r, int c) const
    {
        UT_ASSERT_P(r < getNumRows() && c < getNumCols());
        return r*exint(myCols) + c;
    }

    int                         myRows;
    int                         myCols;
#else
    GA_OffsetMatrix		myVertexMatx;
#endif
    GEO_HullFlags		flags;
    GEO_SurfaceType		surfaceType;

    bool evaluateBaryCenterRefMap(GA_Offset result_vertex,
				  GA_AttributeRefMap &map) const override;

    // Evaluate one point (when du=dv=0), or the du-th dv-th derivative.
    // Return 0 if successful, and -1 otherwise.
    virtual bool	evaluateIndexRefMap(fpreal u, fpreal v, GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				unsigned du, unsigned dv) const;
    virtual int 	evaluateIndexV4(float iu, float iv, UT_Vector4 &pos, 
				unsigned du=0, unsigned dv=0) const;
 
    virtual int	    subdivideURefMap(float u, GA_AttributeRefMap &map);
    virtual int	    subdivideUFloat(float u);
    virtual int	    subdivideVRefMap(float u, GA_AttributeRefMap &map);
    virtual int	    subdivideVFloat(float u);

    // Refine numdivs times in each span.
    virtual void		spanRefineURefMap(GA_AttributeRefMap &map,
					    int numdivs=1) = 0;
    virtual void		spanRefineUInt( int numdivs=1) = 0;
    virtual void		spanRefineVRefMap(GA_AttributeRefMap &map,
					    int numdivs=1) = 0;
    virtual void		spanRefineVInt( int numdivs=1) = 0;

    virtual int			unrefineURefMap(int kidx, GA_AttributeRefMap &h,
					int mult=0, float tol=1e-4f,
					GA_PointGroup *delgroup=0);
    virtual int			unrefineUFloat(int kidx, int mult = 0,
						 float tol = 1e-4F,
						 GA_PointGroup *delgroup = 0);
    virtual int			unrefineVRefMap(int kidx, GA_AttributeRefMap &h,
					int mult=0, float tol=1e-4f,
					GA_PointGroup *delgroup=0);
    virtual int			unrefineVFloat(int kidx, int mult = 0,
						 float tol = 1e-4F,
						 GA_PointGroup *delgroup = 0);

#if !GA_PRIMITIVE_VERTEXLIST
    /// Report approximate memory usage for myVertexMatx (exclusive)
    int64 getBaseMemoryUsage() const;

    // This is called by the subclasses to count the
    // memory used by myVertexMatx (exclusive)
    void countBaseMemory(UT_MemoryCounter &counter) const;
#endif

private:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_Hull &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
    void		increaseDensityInRows(GA_AttributeRefMap &m,
					int numdivs, GA_PointGroup *pgp);
    void		increaseDensityInCols(GA_AttributeRefMap &m,
					int numdivs, GA_PointGroup *pgp);
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()
#endif
