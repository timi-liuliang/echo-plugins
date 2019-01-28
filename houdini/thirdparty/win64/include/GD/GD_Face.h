/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GD_Face_H__
#define __GD_Face_H__

#include "GD_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Array.h>
#include "GD_Primitive.h"
#include <iosfwd>

class GD_Detail;
class GD_TrimLoop;
class GD_TrimHitInfo;
class GA_AttributeRefMap;
class GA_Basis;
class GA_LoadMap;
class GA_SaveMap;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;

class GD_API GD_Face : public GD_Primitive
{
public:
    GD_Face(GD_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GD_Primitive(d, offset)
    {}
#if !GA_PRIMITIVE_VERTEXLIST
    virtual ~GD_Face();
#endif

    virtual int		getBBox(UT_BoundingBox  *bbox) const;
    virtual void	addToBSphere(UT_BoundingSphere  *bsphere) const;
    virtual void	getBRect(UT_BoundingRect *brect) const;
    virtual UT_Vector3	computeNormal() const;
    virtual UT_Vector3	baryCenter() const;

    /// @{
    /// Return the basis for the face (or @c NULL if there is no basis)
    virtual GA_Basis		*getFaceBasis();
    virtual const GA_Basis	*getFaceBasis() const;
    /// @}

    /// @{
    /// Load/Save basis from a JSON value.  By default, these methods fail
    virtual bool		 jsonLoadBasis(UT_JSONParser &p);
    virtual bool		 jsonSaveBasis(UT_JSONWriter &w) const;
    virtual bool		 jsonLoadBasis(UT_JSONParser &p,
						const UT_JSONValue &v);
    virtual bool		 jsonSaveBasis(UT_JSONValue &v) const;
    /// @}

    virtual bool	saveH9(std::ostream &os, bool binary,
			   const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
			   const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
			    const;
    virtual bool	loadH9(UT_IStream &is,
			   const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
			   const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);

    /// @{
    /// Save/Load the vertex lists to/from a JSON stream
    bool	saveVertexArray(UT_JSONWriter &p, const GA_SaveMap &map) const;
    bool	loadVertexArray(UT_JSONParser &p, const GA_LoadMap &map);
    /// @}

#if !GA_PRIMITIVE_VERTEXLIST
    virtual void clearForDeletion();
#endif

    virtual GD_Primitive *copy(int preserve_shared_pts = 0) const;
    virtual int		  copy(const GD_Primitive &src, int ptindexdiff);

#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	  copyUnwiredForMerge(const GA_Primitive *src,
					      const GA_MergeMap &map);
#endif

    // Evaluate one point (when du=0), or the du-th derivative.
    // Return 0 if successful, and -1 otherwise.
    virtual int		evaluate(float u, UT_Vector3 &pos,
				 unsigned du=0, int uOffset=-1) const = 0;
    /// Evaluate the face at the given @c u position, storing the result in the
    /// vertex given by @c vtx.
    /// @param vtx - Vertex to store evaluated result
    /// @param h - Handle of attributes to evaluate
    /// @param u - Parametric evaluation position
    /// @param du - Derivative
    /// @param uoffset - Basis offset
    virtual bool	evaluate(GA_Offset vtx, GA_AttributeRefMap &h,
				fpreal u, uint du=0, int uoffset=-1) const=0;

#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	addPointRefToGroup(GA_PointGroup &grp) const;
#endif

    virtual bool  	isDegenerate() const;

    virtual unsigned	getOrder(void) const = 0;

    // Adjust the size of the vertex array.
#if GA_PRIMITIVE_VERTEXLIST
    SYS_FORCE_INLINE
    void setSize(GA_Size nvertices)
    { setNumVertices(nvertices); }
#else
    void		 setSize(GA_Size sz);
#endif

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints (GA_PointGroup &grp);

//  Remove all repeated vertices - (only gets consecutive vertices)
    virtual int		 removeRepeatedVertices(int check_order = 0);

    // Insert or delete vertices. The insertion methods return the index if
    // successful and -1 otherwise. The deletion methods return 0 if ok and
    // -1 otherwise.
    virtual GA_Size	insertVertex(GA_Offset ptoff=GA_INVALID_OFFSET, GA_Size where=0);
    virtual GA_Size	appendVertex(GA_Offset ptoff=GA_INVALID_OFFSET);
    virtual int   	deleteVertex(GA_Size i);

#if !GA_PRIMITIVE_VERTEXLIST
    // Query the number of vertices in the array. This number may be smaller
    // than the actual size of the array.
    virtual GA_Size	getVertexCount() const;

    virtual GA_Offset	getVertexOffset(GA_Size index) const;
#endif

    UT_Vector2 getPos2(GA_Size index) const
    {
        UT_Vector3 pos = getPos3(index);
        return UT_Vector2(pos.x(), pos.y());
    }

    // Both methods find the index of the given vertex (vtx or the vertex
    // that contains ppt). Return -1 if not found. 
    GA_Size	findPoint(GA_Offset ptoff) const;

    void		setVertexPoint(GA_Size i, GA_Offset ptoff)
    {
        if (i >= 0 && i < myVertexList.entries())
            wireVertex(myVertexList(i), ptoff);
    }
    /// @warning vertexPoint() doesn't check the bounds.  Use with caution.
    GA_Offset		vertexPoint(unsigned int i) const
			    { return getDetail().vertexPoint(myVertexList(i)); }

    SYS_FORCE_INLINE
    bool		 isClosed() const { return myVertexList.getExtraFlag(); }
    virtual void	 close(int rounded = 1, int preserveShape = 0);
    virtual void	 open(int preserveShape = 0, int safe = 0);

    int			 isClockwise(void) const;

    // Find out if we're visible at all (ie. in the valid interval of the
    // parent surface). Partial visibility returns 1. Full visibility
    // returns 2. 0 is returned if the face is completely invisible.
    virtual int		 isVisible() const;

    // Return the barycenter of the part that visible (inside the surf domain).
    // 1 if found, 0 otherwise.
    virtual int		 visibleBaryCenter(float &uctr, float &vctr) const;

    // Append n points equally spaced between the 1st and last vertex.
    void		 sampleEnds(int n);

#if !GA_PRIMITIVE_VERTEXLIST
    // Methods to handle vertex properties:
    virtual bool	 vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void *data = 0) const;
#endif

    // Shift the array of vertices by an offset and wrap around. The offset
    // can be either negative or positive.
    virtual int	 	 cycle(int amount, int = 1);

    // Reverses the vertices of a given face. It's virtual because some faces,
    // such as rational splines, need to be aware of it.
    virtual void	 reverse();

    // Return the bounds of the valid evaluation interval in domain space:
    virtual void	validInterval(int   &a,  int   &b ) const;
    void		validRange   (fpreal64 &ua, fpreal64 &ub) const
			    {
				validRangeImpl(ua, ub);
			    }
    void		validRange   (fpreal32 &ua, fpreal32 &ub) const
			    {
				fpreal64	ua64, ub64;
				validRangeImpl(ua64, ub64);
				ua = ua64; ub = ub64;
			    }
private:
    virtual void	validRangeImpl(fpreal64 &ua, fpreal64 &ub) const;
public:

    // Find the first visible range, i.e. one that is at most as big as the
    // valid interval and is visible on the surface. Partial visibility returns
    // 1. Full visibility returns 2. 0 is returned if completely invisible.
    int			visibleRange(fpreal64 &ua, fpreal64 &ub,
				     fpreal64 tol=1E-4) const;
    int			visibleRange(fpreal32 &ua, fpreal32 &ub,
				     fpreal32 tol=1E-4F) const;

    /// Evaluate the face between breakpoints by taking a start/stop index in
    /// the valid domain and a level of detail representing number of points to
    /// be interpolated between every two breakpoints. The method @b always
    /// interpolates the encountered breakpoints (aka "edit points").
    /// This returns true if successful, else false.
    virtual bool	evaluateBreakSegm(int ustartidx, int ustopidx,
			    int lod, UT_Vector3Array &pos, unsigned du=0) const;

    /// Evaluate the face between breakpoints by taking a start/stop index in
    /// the valid domand and a level of detail representing the number of
    /// points to be interpolated between breakpoints.  The method @b always
    /// interpolates the encountered breakpoints (aka "edit points").  The
    /// method returns the number of vertices in thelist (or -1 if
    /// unsuccessful).
    ///
    /// This code assumes that the destination and source geometry is the same.
    virtual int		evaluateBreakSegment(int ustart, int uend,
			    int lod, GA_Offset *vertices,
			    GA_AttributeRefMap &h, uint du=0) const;

    // Build a trim loop (which will be open unless the face is closed). The
    // loop is build every time you call this method, so it's expensive. You
    // must free the loop yourself when you are done with it. The trim pieces
    // are generated in the [ustart,ustop] or [ustop,ustart] interval, where
    // ustart and ustop are parametric values.
    virtual GD_TrimLoop	*trimLoop(float ustart, float ustop) const = 0;

    // Intersects us with the domain boundaries. The list is sorted in t:
    int			 intersectDomain(UT_Array<GD_TrimHitInfo> &hitlist,
					 float tol = 1E-4F) const;

    virtual int		breakCount() const = 0;
    // Get approximately "numdivs" number of points.  if the flag "usebreak"
    // is set, then, use the "evaluateBreakSeg" to get breakpoints along with
    // other points.  Otherwise, just get numdiv+1 points.
    int			getPoints(int numdivs, UT_Vector3Array &domain,
				  int usebreak);

    // Determine rationality of face.
    virtual void	weights(unsigned short) {}

protected:
    /// All subclasses should call this method to register the face intrinsics.
    /// @see GA_AttributeIntrinsic
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveDefinition &definition)
			{ return GD_Primitive::registerIntrinsics(definition); }

    virtual bool savePrivateH9(std::ostream &os, bool binary) const = 0;
    virtual bool loadPrivateH9(UT_IStream &is) = 0;
    // Methods for finding out about point references
    virtual GA_DereferenceStatus        dereferencePoint(GA_Offset point,
						bool dry_run=false);
    virtual GA_DereferenceStatus        dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false);

    // Check the validity of the data. Meant to be called especially at loading
    // time. The method returns 1 if OK and 0 if trouble.
    virtual bool	 validate(void) const;

    // Insert a vertex multiple times in the list. Return the index.
    int			 multipleInsert(unsigned int where, 
				        unsigned int count,
				        unsigned int appendPoints = 1);

#if !GA_PRIMITIVE_VERTEXLIST
    /// Defragmentation
    virtual void	swapVertexOffsets(const GA_Defragment &defrag);

    /// Report approximate memory usage for myVertexList (exclusive)
    virtual int64	getBaseMemoryUsage() const;

    // This is called by the subclasses to count the
    // memory used by myVertexList (exclusive)
    virtual void	countBaseMemory(UT_MemoryCounter &counter) const;
#endif

private:
#if !GA_PRIMITIVE_VERTEXLIST
    GA_OffsetList	myVertexList;
#endif

    friend std::ostream	&operator<<(std::ostream &os, const GD_Face &d)
			{
			    d.saveH9(os, 0,
				     GD_Primitive::theEmptySaveAttribs,
				     GD_Primitive::theEmptySaveAttribs);
			    return os;
			}
};

#endif
