/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeRefMapDestHandle.h ( GA Library, C++)
 *
 * COMMENTS:	A convenience class for factoring out the destination arguments
 *          	from GA_AttributeRefMap methods.
 */

#ifndef __GA_AttributeRefMapDestHandle__
#define __GA_AttributeRefMapDestHandle__

#include "GA_API.h"
#include "GA_AttributeRefMap.h"
#include "GA_AttributeRefMapOffsetMap.h"
#include "GA_Types.h"

#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>


class GA_WeightedSum;


/// @brief A handle to simplify manipulation of multiple attributes.
///
/// The GA_AttributeRefMapDestHandle class simplifies interaction with a
/// GA_AttributeRefMap by tracking the current destination element.
///
/// When performing operations on object elements, all attribute classes which
/// are uniquely identified by the owner of the object will be operated on.
/// For example, when operating on a vertex, it is possible to uniquely
/// identify a point object.  Thus, when @c add() is called, vertex @b and
/// point attributes on the source detail will be processed.  When @c add()
/// is called with the owner set to GA_ATTRIB_POINT, there may be multiple
/// vertices which share the point.  So, only point attributes will be
/// processed.
///
/// This can be summarized by:
///   - @c GA_AttributeRefMapDestHandlePoint:  @n
///	Process GA_ATTRIB_POINT, GA_ATTRIB_GLOBAL
///   - @c GA_AttributeRefMapDestHandleVertex(): @n
///	Process GA_ATTRIB_VERTEX, GA_ATTRIB_POINT, GA_ATTRIB_PRIMITIVE and
///	GA_ATTRIB_DETAIL
///   - @c GA_AttributeRefMapDestHandlePrimitive(): @n
///	Process GA_ATTRIB_PRIMITIVE and GA_ATTRIB_DETAIL
///   - @c GA_AttributeRefMapDestHandleGlobal(): @n
///	Process GA_ATTRIB_DETAIL
///
/// The same semantics are used when setting an element on the handle.  If @c
/// setPrimitive() is called, destination primitive and global attributes will
/// be modified.
///
/// By default, operations on "P" are performed as with any other attribute.
/// However, it's possible to turn on the homogeneous flag which will ensure
/// that operations are done using homogeneous coordinates.
template <GA_AttributeOwner DOwner>
class GA_API GA_AttributeRefMapDestHandle
{
private:
    /// For parameter semantics - the source and destination should always
    /// be the same owner.
    static const GA_AttributeOwner	SOwner = DOwner;

public:
    /// Minimal constructor
    GA_AttributeRefMapDestHandle(const GA_AttributeRefMap &map)
	: myMap(map)
	, myDestMap(DOwner, DOwner == GA_ATTRIB_GLOBAL ? GA_Offset(0) :
		GA_INVALID_OFFSET) {}
    /// Copy constructor
    GA_AttributeRefMapDestHandle(const GA_AttributeRefMapDestHandle &src)
	: myMap(src.myMap), myDestMap(src.myDestMap) {}
    /// Destructor
    ~GA_AttributeRefMapDestHandle() {}


    /// Return the number of attributes in the list
    int		entries() const { return myMap.entries(); }


    /// Return the number of attributes in the list that are paired with the
    /// source being P.
    int		entriesWithSourceP() const
				{ return myMap.entriesWithSourceP(); }

    /// @{
    /// Set the destination element.  Operations will write to the element
    /// specified.
    ///
    /// Only attributes which are fully qualified by the element will be
    /// operated on.
    /// - Point:  Point and detail attributes
    /// - Vertex:  Vertex, point, primitive and detail attributes
    /// - Primitive:  Primitive and detail attributes
    /// - Global:  Only global attributes
    bool	setElement(GA_Offset offset)
		    { myDestMap.reset(DOwner, offset); return true; }
    /// @}

    /// Homogenize any rational attributes of the current element
    void	homogenize() const
		    { return myMap.homogenize(myDestMap); }
    /// Dehomogenize any rational attributes of the current element
    void	dehomogenize() const
		    { return myMap.dehomogenize(myDestMap); }

    /// Copy operation:  @code dest = source @endcode
    /// This is the generic copy operation 
    void	copy(GA_Offset offset) const
		{ return myMap.copyValue(myDestMap, SOwner, offset); }

    /// @{
    /// Copy operations:  @code dest = position @endcode
    /// Sets any attributes that read the position attribute to an explicit
    /// value.
    void	copyExplicitPosition(const UT_Vector2 &p) const
		    { myMap.copyExplicitPosition(myDestMap, p); }
    void	copyExplicitPosition(const UT_Vector2D &p) const
		    { myMap.copyExplicitPosition(myDestMap, p); }
    void	copyExplicitPosition(const UT_Vector3 &p) const
		    { myMap.copyExplicitPosition(myDestMap, p); }
    void	copyExplicitPosition(const UT_Vector3D &p) const
		    { myMap.copyExplicitPosition(myDestMap, p); }
    void	copyExplicitPosition(const UT_Vector4 &p) const
		    { myMap.copyExplicitPosition(myDestMap, p); }
    void	copyExplicitPosition(const UT_Vector4D &p) const
		    { myMap.copyExplicitPosition(myDestMap, p); }
    /// @}

    /// Add operation:  @code dest += source*scale @endcode
    /// This is the generic add operation
    void	add(GA_Offset source_index, fpreal scale=1) const
		{
		    myMap.addValue(myDestMap, SOwner, source_index,
				   scale);
		}

    /// Sub operation:  @code dest -= source @endcode
    /// This is the generic sub operation
    void	sub(GA_Offset source_index) const
		{
		    myMap.subValue(myDestMap, SOwner, source_index);
		}

    /// Mul operation:  @code dest *= source @endcode
    /// This is the generic mul operation
    void	mul(GA_Offset source_index) const
		{
		    myMap.mulValue(myDestMap, SOwner, source_index);
		}

    /// Linear interpolation operation:  @code dest = s0 + (s1 - s0)*t @endcode
    /// Generic linear interpolation between two of the same elements
    void	lerp(GA_Offset s0, GA_Offset s1, fpreal t) const
		{ myMap.lerpValue(myDestMap, SOwner, s0, s1, t); }
    void	lerpH(GA_Offset s0, GA_Offset s1, fpreal t) const
		{ myMap.lerpHValue(myDestMap, SOwner, s0, s1, t); }

    /// @{
    /// Weighted sum operation:  @code dest = sum(w[i]*s[i]) @endcode
    /// @code
    ///	    // Compute the average value of the P attribute and store in a
    ///	    // global attribute "averageP".
    ///	    GA_WeightedSum		sum;
    ///	    GA_AttributeRefMap		map(gdp);
    ///	    GA_AttributeRefMapDestHandleGlobal	gah(map);
    ///	    int				npts;
    ///	    map.append( detail.findGlobalAttribute("averageP"),
    ///			detail.findPointAttribute("P") );
    ///	    gah.startSum(sum);
    ///	    for (GA_Iterator it = gdp.getPointRange(); !it.atEnd(); ++it)
    ///		gah.sumPoint(sum, it.getOffset(), 1);
    ///	    npts = gdp.getNumPoints();
    ///	    gah.finishSum(sum, npts ? 1./npts : 0);
    /// @endcode
    void	startSum(const GA_WeightedSum &sum) const
		{ myMap.startSum(sum, myDestMap); }
    void	startHSum(const GA_WeightedSum &sum) const
		{ myMap.startHSum(sum, myDestMap); }
    void	finishSum(const GA_WeightedSum &sum,
			fpreal normalization=1) const
		{ myMap.finishSum(sum, myDestMap, normalization); }
    void	finishHSum(const GA_WeightedSum &sum,
			fpreal normalization=1) const
		{ myMap.finishHSum(sum, myDestMap, normalization); }
    /// @}

    /// Add a value into the weighted sum.  This advances the weighted sum with
    /// the weight given.
    void	add(GA_WeightedSum &sum,
			GA_Offset source_index,
			fpreal w) const
		{
		    myMap.addSumValue(sum, myDestMap, SOwner,
				      source_index, w); 
		}
    void	addH(GA_WeightedSum &sum,
			GA_Offset source_index,
			fpreal w) const
		{
		    myMap.addHSumValue(sum, myDestMap, SOwner,
				       source_index, w); 
		}

    /// Compute barycentric coordinates for 3 values
    /// @code result = (1 - u - v)*p0 + u*p1 + v*p2 @endcode
    void	barycentric(GA_Offset p0, GA_Offset p1, GA_Offset p2,
			    fpreal u, fpreal v) const
		{
		    myMap.barycentricValue(myDestMap, SOwner,
					   p0, p1, p2, u, v);
		}

    /// Compute bilinear interpolation over 4 values of a quadrilateral.  The
    /// math is the same as SYSbilerp(): @code
    ///  (u=0,v=1) u0v1     u1v1 (u=1,v=1)
    ///              +--------+
    ///              |        |
    ///              |        |
    ///              +--------+
    ///  (u=0,v=0) u0v0     u1v0 (u=1,v=0)
    /// @endcode
    void	bilinear(GA_Offset u0v0, GA_Offset u1v0,
			GA_Offset u0v1, GA_Offset u1v1,
			fpreal u, fpreal v) const
		{
		    myMap.bilinearValue(myDestMap, SOwner, u0v0, u1v0,
					u0v1, u1v1, u, v); 
		}

    /// Zero an attribute.  This is equivalent to a weighted sum of 0 elements.
    void	zero() const { myMap.zeroElement(myDestMap); }

    /// Multiply operation:  @code dest *= scale @endcode
    void	multiply(fpreal scale) const
		    { myMap.multiply(myDestMap, scale); }

    /// @{
    /// Transform attributes.  This uses tags on the attribute to determine how
    /// to perform the transform.
    /// @param m The transform matrix
    /// @param im The inverse of @m (i.e. @c m.invert(im) )
    /// Transforms are independent of the source geometry.
    void	transform(const UT_Matrix4 &m, const UT_Matrix4 &im) const
		    { myMap.transform(m, im, myDestMap); } 
    void	transform(const UT_DMatrix4 &m, const UT_DMatrix4 &im) const
		    { myMap.transform(m, im, myDestMap); } 
    /// @}

    /// @{
    /// Compare value with another elements
    bool	isEqual(GA_Offset cmp) const
		    { return myMap.isEqual(myDestMap, SOwner, cmp); }
    bool	isAlmostEqual(GA_Offset cmp) const
		    { return myMap.isAlmostEqual(myDestMap, SOwner, cmp); }
    /// @}

    /// @{
    /// Standard operations read the source and write to the destination.
    /// However.  These operations operate by reading and writing the
    /// destination.
    /// @see copyValue(), addValue(), subValue(), lerpValue()
    void	copyDest(GA_Offset offset) const
		    { myMap.copyDestValue(myDestMap, SOwner, offset); }

    void	addDest(GA_Offset offset) const
		    { myMap.addDestValue(myDestMap, SOwner, offset); }

    void	subDest(GA_Offset offset) const
		    { myMap.subDestValue(myDestMap, SOwner, offset); }

    void	lerpDest(GA_Offset s0, GA_Offset s1, fpreal t) const
		    {
			myMap.lerpDestValue(myDestMap,
					    SOwner, s0, s1, t);
		    }

    void	addDest(GA_WeightedSum &sum,
			GA_Offset offset, fpreal w) const
		    {
			myMap.addSumDestValue(sum, myDestMap, 
					      SOwner, offset, w);
		    }
    void	addHDest(GA_WeightedSum &sum,
			GA_Offset offset,
			fpreal w) const
		    {
			myMap.addHSumDestValue(sum, myDestMap,
					       SOwner, offset, w);
		    }
    /// @}

    /// Debug statement to dump all the attributes to stdout
    void	dump(const char *msg, ...) const;

private:

    const GA_AttributeRefMap	&myMap;
    /// myDestMap caches topological lookups and so must be mutable.  Note
    /// that we need not concern ourselves with thread-safety as more than
    /// one thread should not be writing to the bound element anyway.
    mutable GA_AttributeRefMapOffsetMap	 myDestMap;
};

typedef GA_AttributeRefMapDestHandle<GA_ATTRIB_VERTEX>	GA_AttributeRefMapDestHandleVertex;
typedef GA_AttributeRefMapDestHandle<GA_ATTRIB_POINT>	GA_AttributeRefMapDestHandlePoint;
typedef GA_AttributeRefMapDestHandle<GA_ATTRIB_PRIMITIVE>	GA_AttributeRefMapDestHandlePrimitive;
typedef GA_AttributeRefMapDestHandle<GA_ATTRIB_GLOBAL>	GA_AttributeRefMapDestHandleGlobal;

#endif
