/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimTriBezier.h ( GEO Library, C++)
 *
 * COMMENTS:	This is the base class for all triangle mesh types.
 */

#ifndef __GEO_PrimTriBezier__
#define __GEO_PrimTriBezier__

#include "GEO_API.h"
#include "GEO_Primitive.h"

#include <GA/GA_Defines.h>
#include <GA/GA_GenericHandle.h>
#include <UT/UT_Array.h>
#include <UT/UT_Vector3.h>

class GA_AttributeRefMap;
class GEO_ConvertParms;

class GEO_API GEO_PrimTriBezier : public GEO_Primitive
{
public:
    GEO_PrimTriBezier(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Primitive(d, offset)
    {}
#if !GA_PRIMITIVE_VERTEXLIST
    virtual ~GEO_PrimTriBezier();
#endif

    virtual const GA_PrimitiveDefinition &getTypeDef() const
    {
        UT_ASSERT(theDefinition);
        return *theDefinition;
    }

    // This fills a plain point array with our positions.
    // The array is a compressed array in the same format as our own
    // native stuff.
    void		fillArray(int fakeorder, UT_Vector4 *array) const;
    void		fillArray(int fakeorder, const GA_Offset *results,
				  GA_AttributeRefMap &hlist) const;

    virtual int		getBBox(UT_BoundingBox *bbox) const;
    virtual void	addToBSphere(UT_BoundingSphere *bsphere) const;
    virtual UT_Vector3	baryCenter() const;
    virtual void	reverse();
    virtual UT_Vector3  computeNormal() const;

    virtual fpreal	calcVolume(const UT_Vector3 &refpt) const;
    virtual fpreal	calcArea() const;
    virtual fpreal	calcPerimeter() const;

    /// Load the order from a JSON value
    bool		loadOrder(const UT_JSONValue &p);

    /// @{
    /// Save/Load vertex list to a JSON stream
    bool		saveVertexArray(UT_JSONWriter &w,
				const GA_SaveMap &map) const;
    bool		loadVertexArray(UT_JSONParser &p,
				const GA_LoadMap &map);
    /// @}

    virtual bool	saveH9(std::ostream &os, bool binary,
			   const UT_Array<GA_AttribSaveDataH9> &prim_attribs,
			   const UT_Array<GA_AttribSaveDataH9> &vtx_attribs)
			    const;
    virtual bool	loadH9(UT_IStream &is,
			   const UT_Array<GA_AttribLoadDataH9> &prim_attribs,
			   const UT_Array<GA_AttribLoadDataH9> &vtx_attribs);

    virtual void	copyPrimitive(const GEO_Primitive *src);
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;

    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);

#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	addPointRefToGroup(GA_PointGroup &grp) const;
#endif

    virtual bool  	isDegenerate() const;

    // Adjust the order of the patch.
    void		 setOrder(int order);
    int			 getOrder() const { return myOrder; }

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints(GA_PointGroup &grp);

    /// Before a point is deleted, all primitives using the point will be
    /// notified.  The method should return "false" if it's impossible to
    /// delete the point.  Otherwise, the vertices should be removed.
    virtual GA_DereferenceStatus        dereferencePoint(GA_Offset point,
						bool dry_run=false);
    virtual GA_DereferenceStatus        dereferencePoints(
						const GA_RangeMemberQuery &pt_q,
						bool dry_run=false);

#if GA_PRIMITIVE_VERTEXLIST
    using GA_Primitive::getVertexOffset;
#else
    // Query the number of vertices in the array. This number may be smaller
    // than the actual size of the array.
    virtual GA_Size	getVertexCount() const;
    virtual GA_Offset	getVertexOffset(GA_Size index) const;

    virtual void	beginVertex(const_iterator &i) const;
    virtual void	nextVertex(const_iterator &i) const;
#endif

    void			 setVertexPoint(GA_Size i, GA_Offset pt)
				 {
				    if (i < myVertexList.entries())
					wireVertex(myVertexList(i), pt);
				 }

    // And now the operators that let you treat it like a portion of
    // a tensor product patch:
    GA_Size		 getRawIndex(GA_Size row, GA_Size col) const
			    { return getRawIndex(row, col, myOrder); }
    GA_Size		 getRawIndex(GA_Size row, GA_Size col, int order)const
			    { return (order - row)*(order-row-1)/2 + col; }
    GA_Offset		getVertexOffset(GA_Size i, GA_Size j) const
				 { return getVertexOffset(getRawIndex(i, j)); }
    GA_Offset		getPointOffset(GA_Size i, GA_Size j) const
				 { return GEO_Primitive::getPointOffset(getRawIndex(i, j)); }
    UT_Vector3		getPos3(GA_Size i, GA_Size j) const
				 { return GEO_Primitive::getPos3(getRawIndex(i, j)); }
    void		setPos3(GA_Size i, GA_Size j, const UT_Vector3 &pos) const
				 { GEO_Primitive::setPos3(getRawIndex(i, j), pos); }
    void		setVertexPoint(GA_Size i, GA_Size j, GA_Offset pt)
				 {
				    i = getRawIndex(i, j);
				    if (i < myVertexList.entries())
					wireVertex(myVertexList(i), pt);
				 }

#if !GA_PRIMITIVE_VERTEXLIST
    // Have we been deactivated and stashed?
    virtual void	 stashed(bool beingstashed,
					GA_Offset offset = GA_INVALID_OFFSET);

    // Methods to handle vertex attributes for the attribute dictionary
    virtual bool	 vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void  *data = 0) const;
#endif

    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// Fill a tesselation of the mesh for a given attribute.  @c ndivs must be
    /// greater than or equal to 2.  The @c pos array should be able to hold @c
    /// (ndivs)*(ndivs+1)/2 entries.
    template <typename T>
    bool		evaluateMesh(int ndivs,
				    const GA_ROGenericHandleVertex<T> &h,
				    T *pos) const;

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    static GEO_PrimTriBezier *build(GA_Detail *gdp, int order,
        bool appendPts = true);

    // Conversion Methods
    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
        GA_PointGroup *usedpts = nullptr);
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);

    virtual void normal(NormalComp &output) const;

protected:
#if !GA_PRIMITIVE_VERTEXLIST
    virtual void	clearForDeletion();
#endif

    static GA_PrimitiveFamilyMask 	buildFamilyMask()
					    { return GA_FAMILY_NONE; }

    // Declare intrinsic methods
    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE)

    // Methods for finding out about point references
    virtual bool	 savePrivateH9(std::ostream & /*os*/,int /*binary*/) const
						{return true;}
    virtual bool loadPrivateH9(UT_IStream &)	{ return true; }

    // Check the validity of the data. Meant to be called especially at loading
    // time. The method returns 1 if OK and 0 if trouble.
    virtual bool	 validate(void) const;

    void		 setSize(GA_Size sz);

    /// @warning vertexPoint() doesn't check the bounds.  Use with caution.
    GA_Offset		vertexPoint(GA_Size i) const
			    { return getDetail().vertexPoint(myVertexList(i)); }
    GA_Offset		vertexPoint(GA_Size i, GA_Size j) const
			    { return vertexPoint(getRawIndex(i, j)); }

    /// @warning swapVertices() doesn't check the bounds, nor that
    /// (i1,j1) != (i2,j2).  Use with caution.
    void		swapVertices(GA_Size i1, GA_Size j1,
				     GA_Size i2, GA_Size j2)
			{
			    GA_Size i = getRawIndex(i1, j1);
			    GA_Size j = getRawIndex(i2, j2);
			    GA_Offset	tmp = myVertexList(i);
			    myVertexList.set(i, myVertexList(j));
			    myVertexList.set(j, tmp);
			}

#if !GA_PRIMITIVE_VERTEXLIST
    /// Defragmentation
    virtual void	swapVertexOffsets(const GA_Defragment &defrag);
#endif

#if !GA_PRIMITIVE_VERTEXLIST
    GA_OffsetList		myVertexList;
#endif
    int				myOrder;

    virtual bool	evaluatePointRefMap(GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				fpreal u, fpreal v, uint du, uint dv) const;
    virtual int		evaluatePointV4(UT_Vector4 &pos, float u, float v = 0,
				    unsigned du=0, unsigned dv = 0) const;
    virtual bool	evaluateBaryCenterRefMap(GA_Offset result_vertex,
				GA_AttributeRefMap &hlist) const;
    virtual void	computeInteriorPointWeights(
				UT_Array<GA_Offset> &vtxlist,
				UT_Array<float> &weightlist,
				fpreal u, fpreal v, fpreal w) const;

private:
    static GA_PrimitiveDefinition *theDefinition;
    friend class GU_PrimitiveFactory;

    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimTriBezier &d)
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
