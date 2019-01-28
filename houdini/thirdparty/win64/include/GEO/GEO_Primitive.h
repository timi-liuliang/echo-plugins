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

#ifndef __GEO_Primitive_H__
#define __GEO_Primitive_H__

#include "GEO_API.h"
#include <iosfwd>
#include <UT/UT_Matrix4.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_BoundingSphere.h>
#include <GA/GA_Primitive.h>
#include "GEO_PrimType.h"

class GA_AttributeRefMap;
class GA_Detail;
class GA_PrimitiveWrangler;
class GEO_AttributeHandleList;
class GEO_ConvertParms;
class GEO_Detail;
class GEO_MetaPrim;
class GEO_Vertex;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;

SYS_DEPRECATED_PUSH_DISABLE()

class GEO_API GEO_Primitive : public GA_Primitive
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    SYS_FORCE_INLINE
    GEO_Primitive(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GA_Primitive(*d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors; only GEO_Detail::destroyStashed() should be
    ///       calling delete on GEO_Primitive pointers, and GEO_Detail
    ///       is a friend.
    virtual ~GEO_Primitive() {}

public:
    virtual GA_PrimCompat::TypeMask	 getPrimitiveId() const;

    /// Copy attribute data from the source primitive.  Also, copy over all the
    /// group membership information from the source primitive.
    void	copyAttributesAndGroups(const GEO_Primitive &src,
				    bool copy_groups=true)
		{
		    copyAttributeData(src);
		    if (copy_groups)
			copyGroupMembership(src);
		}
    void	copyAttributesAndGroups(const GEO_Primitive &src,
				    GA_AttributeRefMap &gah,
				    bool copy_groups=true)
		{
		    copyAttributeData(src, gah);
		    if (copy_groups)
			copyGroupMembership(src);
		}
    /// NOTE: The copying of groups only works if src is in the same detail
    void	copyAttributesAndGroups(const GEO_Primitive &src,
				    GA_PrimitiveWrangler &wrangler,
				    bool copy_groups=true)
		{
		    copyAttributeData(src, wrangler);
		    if (copy_groups)
			copyGroupMembership(src);
		}

    /// Copy the attribute data over from the source primitive.  Often you will
    /// want to call copyAttributesAndGroups() instead.
    void	copyAttributeData(const GEO_Primitive &src);
    void	copyAttributeData(const GEO_Primitive &src,
				  GA_AttributeRefMap &gah);
    void	copyAttributeData(const GEO_Primitive &src,
				  GA_PrimitiveWrangler &wrangler);

    void	copyAttributeValues(const GEO_Primitive &src,
				    GA_PrimitiveWrangler &wrangler);

    /// Copy the group membership from the source primitive to the current
    /// primitive.  Both primitives must be in the same detail.
    void	copyGroupMembership(const GEO_Primitive &src);
	    
    // NOTE: The point *indices* will be the same as in src.
    //       If this and src are in the same detail, this means that
    //       the point offsets will also be the same.  There must be
    //       sufficient points in this' detail.
    // NOTE: To remap the points, you can iterate through the vertices
    //       of the primitive after copying, and call getPointOffset
    //       and setPointOffset.
    //
    // copyPrimitive(), unlike copy(), will not copy the primitive attributes.
    // (see copyAttributeData() or copyAttributesAndGroups()).
#if GA_PRIMITIVE_VERTEXLIST
    virtual void copyPrimitive(const GEO_Primitive *src);
#else
    virtual void	 copyPrimitive(const GEO_Primitive *src)=0;
#endif
    virtual GEO_Primitive *copy(int preserve_shared_pts = 0) const;

    // Transforms the matrix associated with this primitive.  The
    // translate component is ignored: Translate the vertices of
    // the primitive to translate the primitive.
    // This only works with quadrics (sphere, tube, metaballs) and volumes.
    virtual void	 transform(const UT_Matrix4 &);

    // Convert the real domain values of the primitive to unit values to be
    // used in the evaluation methods below:
    virtual void	 realToUnitPair(float  ureal, float  vreal,
					float &uunit, float &vunit) const;

    // Convert the unit values of the primitive to the real domain values
    virtual void	 unitToRealPair(float  uunit, float  vunit,
					float &ureal, float &vreal) const;

    // Map the normalized length (distance value [0,1]) parameter to the unit 
    // parameterization of the primitve
    virtual void	 unitLengthToUnitPair(float  ulength, float  vlength,
					      float &uparm,  float &vparm)const;
    virtual void	 unitLengthToUnitPair(float  ulength, float  vlength,
					      float &uparm,  float &vparm, float tolerance)const;

    virtual void	 unitToUnitLengthPair(float  uparm,   float  vparm,
					      float &ulength, float &vlength)
									const;

    // Evaluate a set of attributes at the bary center of the primitive,
    // returning true on success, false on failure.
    bool	 evaluateBaryCenter(GA_Offset result_vtx, 
				    GA_AttributeRefMap &map) const
		 { return evaluateBaryCenterRefMap(result_vtx, map); }
 
    // Evaluate a set of attributes at a u,v position.  Optionally, evaluate
    // the derivative in the u or v direction.
    // The method returns true if the point could be evaluated
    bool	 evaluatePoint(GA_Offset result_vtx,
				GA_AttributeRefMap &map,
				fpreal u, fpreal v=0,
				uint du=0, uint dv=0) const
		 { return evaluatePointRefMap(result_vtx, map, u, v, du, dv); }
   
    // Evaluate the position, the derivative or the normal at domain point
    // (u,v), where u and v MUST be in [0,1]. "v" and "dv" will be ignored
    // when dealing with one-dimensional types such as circles and polygons.
    // Return 0 if OK and -1 otherwise. The normal is not normalized.
    int		 evaluatePoint( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const
					{ return evaluatePointV4(pos, u, v, du, dv); }

    virtual int		 evaluateNormalVector(UT_Vector3 &nml, float u,
					float v = 0, float w = 0) const;
    
    // Evaluate the position at domain point (u,v) in the interior of the
    // geometry.  This calls evaluatePoint by default, but has a different
    // implementation for triangles and quadrilaterals, where barycentric
    // coords or bilinear interpolants are used to get an interior point,
    // rather than a point on the perimeter of the poly.  This was added
    // so particles can stick to triangles and quads.
    bool	    evaluateInteriorPoint(GA_Offset result_vtx,
					GA_AttributeRefMap &map,
					fpreal u, fpreal v, fpreal w = 0) const
			    { return evaluateInteriorPointRefMap(
					result_vtx, map, u, v, w); }
    int		    evaluateInteriorPoint(UT_Vector4 &pos, 
				       fpreal u, fpreal v, fpreal w = 0) const
			    { return evaluateInteriorPointV4(pos, u, v, w); }


    /// Finds the weightings of the vertices that will compute an interior
    /// point given the u,v,w coordinates.  The offsets are into the gdp's 
    /// vertex list, not indices into this primitive's vertex list.
    /// Weights are normalized.
    /// Note this cannot be used to recover the Position as it often has
    /// special case logic (as in spheres, or rational splines), in those
    /// cases use evaluateInteriorPoint(UT_Vector4 &pos, ...)
    /// It has the advantage over the other evaluateInteriorPoints in that
    /// it allows you to avoid creating any temporary vertices.
    /// Behaviour for non-sublcassed types is to return the 0th vertex.
    virtual void computeInteriorPointWeights(
		    UT_Array<GA_Offset> &vtxlist,
		    UT_Array<float> &weightlist,
		    fpreal u, fpreal v, fpreal w) const;

    SYS_FORCE_INLINE GEO_Detail		*getParent() const
    { 
	// We cannot do a static_cast because we have only forward
	// declared GEO_Detail and GA_Detail
	return reinterpret_cast<GEO_Detail *>(&getDetail()); 
    }
    static GA_PrimCompat::TypeMask	 getPrimitiveMaskH9(const char *maskstr);

    /// Compute the bounding box of the primitive.  Return 0 if unable to
    /// compute bounds (1 on successful computation)
    virtual int		 getBBox(UT_BoundingBox *bbox) const = 0;

    /// @{
    /// If the attribute is "P" the base-class will call getBBox()
    virtual bool	enlargeBoundingBox(UT_BoundingRect &b,
					const GA_Attribute *P) const;
    virtual bool	enlargeBoundingBox(UT_BoundingBox &b,
					const GA_Attribute *P) const;
    /// @}

    /// Return a normal vector for the primitive
    virtual UT_Vector3	 computeNormal() const = 0;

    /// Reverse the order of vertices
    virtual void	 reverse() = 0;

    /// Return the center of the primitive.  This defaults to the center of the
    /// bounding box.  A better approximation might be the average value of the
    /// vertex positions (which could be different).
    virtual UT_Vector3	 baryCenter() const;

    /// Enlarge the bounding sphere with the primitive's bounds.
    /// By default, this will enlarge the bounding sphere by the primitive's
    /// bounding box (i.e. getBBox()).  It may be possible to have a tighter
    /// bound (i.e. the vertex hull for many primitives is a much better
    /// metric).
    virtual void	 addToBSphere(UT_BoundingSphere *bsphere) const;

    /// Method to isolate a pasted surface
    virtual void	 isolate() { /* No implementation here. */ }

    // Take the whole set of points into consideration when applying the
    // point removal operation to this primitive. The method returns 0 if
    // successful, -1 if it failed because it would have become degenerate,
    // and -2 if it failed because it would have had to remove the primitive
    // altogether.
    virtual int		 detachPoints (GA_PointGroup &grp) = 0;

    virtual bool	 hasEdge(const GA_Edge &edge) const;

    /// These functions are only implemented to get guide points/edges
    /// for spheres, tubes, and circles.  The ID meaning is type-specific.
    /// @{
    virtual bool         hasGuideEdge(int edgeid, UT_Vector3 &a,
                                                  UT_Vector3 &b) const;
    virtual bool         hasXsectPoint(int pointid, UT_Vector3 &p) const; 
    /// @}

    /// Apply the function to each vertex of the primitive.  Break
    /// when the function returns true.  Return the value
    /// of the last function applied (false if never called).
#if !GA_PRIMITIVE_VERTEXLIST
    virtual bool	 vertexApply(bool (*apply)(GA_Offset vtx, void *),
				     void  *data = 0) const;
#endif

    SYS_DEPRECATED_HDK(13.0)
    GEO_Vertex		 getVertexElement(GA_Size i) const;

//
//  Method to get the tessera data for meta-primitives
    virtual GEO_MetaPrim	*castToMetaPrim(void);
    virtual const GEO_MetaPrim	*castToMetaPrim(void) const;

    // Return the surrounding values of the real-space u,v parameters.
    // Returns 1 if succesful, 0 if out-of-range.

    virtual int		 parametricBBox(float u, float v, 
					float *u0, float *u1,
					float *v0, float *v1);

    // Returns distance between two points in parameter space, aware
    // of any possible wrapping.
    virtual float	 uvDist(float u1, float v1, float u2, float v2) const;

    // Intersects a ray with the bounding box, iteratively growing it until
    // an intersection is found or maxtries is reached.
    int			 bboxIntersectRay(const UT_Vector3 &rayorig,
	                                  const UT_Vector3 &raydir,
					  int   maxtries = 10,
					  float tmax = 1E17F,
					  float *distance = 0,
					  UT_Vector3 *nml=0) const;

    /// @{
    /// If the method isn't known, or doesn't make sense, we return 0.
    virtual fpreal	 calcVolume(const UT_Vector3 &) const { return 0; }
    virtual fpreal	 calcArea() const { return 0; }
    virtual fpreal	 calcPerimeter() const { return 0; }
    /// @}

    // Is this primitive a GEO_Quadric?
    virtual bool	 isQuadric() const { return false; }

    // Compute normals on points using a subclass of NormalComp.  This
    // class receives the point offset and vertex normal at that point, and
    // will either add or subtract the normal from the existing data.
    class GEO_API NormalComp {
    public:
	virtual ~NormalComp() {}
	virtual void	add(GA_Offset offset, const UT_Vector3 &nml) {}
	virtual void	sub(GA_Offset offset, const UT_Vector3 &nml) {}
    };

    // Compute point normals into an array, indexed by the point order
    // number
    class GEO_API NormalCompArray : public NormalComp {
    public:
	NormalCompArray(const GA_Detail &gdp, UT_Vector3Array &output)
	    : myGdp(gdp)
	    , myOutput(output) {}
	virtual void	add(GA_Offset offset, const UT_Vector3 &nml)
	{ myOutput(myGdp.pointIndex(offset)) += nml; }
	virtual void	sub(GA_Offset offset, const UT_Vector3 &nml)
	{ myOutput(myGdp.pointIndex(offset)) -= nml; }

    private:
	const GA_Detail	&myGdp;
	UT_Vector3Array	&myOutput;
    };

    // Compute normals into a normal attribute
    class GEO_API NormalCompAttr : public NormalComp {
    public:
	NormalCompAttr(const GA_RWAttributeRef &attr)
	    : myHandle(attr.getAttribute()) {}
	NormalCompAttr(const GA_RWHandleV3 &attr)
	    : myHandle(attr) {}
	virtual void	add(GA_Offset offset, const UT_Vector3 &nml)
	{ myHandle.add(offset, nml); }
	virtual void	sub(GA_Offset offset, const UT_Vector3 &nml)
	{ myHandle.add(offset, -nml); }

	GA_RWHandleV3	&getHandle()	{ return myHandle; }

    protected:
	GA_RWHandleV3 myHandle;
    };

    // Compute normals into a normal attribute
    class GEO_API NormalCompBuffered : public NormalComp
    {
    public:
        NormalCompBuffered(NormalComp &parent, UT_Lock &lock)
	    : myParent(parent)
            , myLock(lock)
            , myBufferEntries(0)
        {}
        virtual ~NormalCompBuffered()
        {
            flush();
        }
	virtual void	add(GA_Offset offset, const UT_Vector3 &nml)
	{
            myOffsetBuffer[myBufferEntries] = offset;
            myDataBuffer[myBufferEntries] = nml;
            ++myBufferEntries;
            if (myBufferEntries == theBufferSize)
                flush();
        }

	virtual void	sub(GA_Offset offset, const UT_Vector3 &nml)
	{
            myOffsetBuffer[myBufferEntries] = offset;
            myDataBuffer[myBufferEntries] = -nml;
            ++myBufferEntries;
            if (myBufferEntries == theBufferSize)
                flush();
        }

        void            flush()
        {
            UT_AutoLock lock(myLock);
            for (GA_Size i = 0; i < myBufferEntries; ++i)
            {
                myParent.add(myOffsetBuffer[i], myDataBuffer[i]);
            }
            myBufferEntries = 0;
        }

    private:
        static const GA_Size     theBufferSize = 1024;

        NormalComp              &myParent;
        UT_Lock                 &myLock;
        GA_Size                  myBufferEntries;
        GA_Offset                myOffsetBuffer[theBufferSize];
        UT_Vector3               myDataBuffer[theBufferSize];
    };

    virtual void	normal(NormalComp &output) const = 0;

    // Conversion Methods

    // This method converts the primitive to the specified toType and deletes
    // the old primitive. 

    virtual GEO_Primitive	*convert(GEO_ConvertParms &parms,
					 GA_PointGroup *usedpts = 0) = 0;

    // This method creates a new primitive by converting the old primitive to 
    // the specified toType.

    virtual GEO_Primitive	*convertNew(GEO_ConvertParms &parms) = 0;

    // The default implementation just checks the bounding box and
    // deletes the primitive if the bounding box is beyond the plane.
    // NOTE: normal should be normalized
    virtual void	clip(UT_Vector3 normal, float distance = 0,
				GA_PrimitiveGroup *clipgrp = NULL);

    // The default implementation just intersects against the bounding box
    virtual int		intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				float tmax = 1E17F, float tol = 1E-12F,
				float *distance = 0, UT_Vector3 *pos = 0,
				UT_Vector3 *nml = 0, int accurate = 0,
				float *u = 0, float *v = 0,
				int ignoretrim = 1) const;


    /// Convience objects to pass as arguments to saveH9()/loadH9().
    static const UT_Array<GA_AttribSaveDataH9> &theEmptySaveAttribs;

protected:
    /// All subclasses should call this method to register the primitive
    /// intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveDefinition &defn)
			{ return GA_Primitive::registerIntrinsics(defn); }

    virtual bool	 evaluatePointRefMap(GA_Offset result_vtx,
					GA_AttributeRefMap &map,
					fpreal u, fpreal v=0,
					uint du=0, uint dv=0) const = 0;
   
    /// Evaluate the position for the given parametric coordinates (with the
    /// given derivatives).  Return 0 if successful, or -1 if failure.
    /// The default implementation returns {0,0,0,0};
    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const;

    /// By default, this will call evaluateInteriorPointRefMap with u=.5 and
    /// v=.5.  This is likely not the perfect solution.  At the current time,
    /// this is only used in POPs to birth points at primitive centers.
    virtual bool	 evaluateBaryCenterRefMap(GA_Offset result_vtx, 
				    GA_AttributeRefMap &map) const;

    virtual bool	 evaluateInteriorPointRefMap(GA_Offset result_vtx,
					GA_AttributeRefMap &map,
					fpreal u, fpreal v, fpreal w = 0) const;
    virtual int		 evaluateInteriorPointV4(UT_Vector4 &pos, 
				       fpreal u, fpreal v, fpreal w = 0) const;

private:
    friend class	 GEO_Detail;	// Allow detail to get at private stuff

    // TODO: Eliminate, deprecate, or switch to saving json format.
    friend std::ostream	&operator<<(std::ostream &os, const GEO_Primitive &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
};

SYS_DEPRECATED_POP_DISABLE()

#endif
