/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeRefMap.h ( GA Library, C++)
 *
 * COMMENTS:	A mapping between destination and source attributes for
 *          	various operations manipulating multiple attributes.
 */

#ifndef __GA_AttributeRefMap__
#define __GA_AttributeRefMap__

#include "GA_API.h"
#include "GA_Types.h"
#include "GA_VertexPool.h"
#include "GA_ATIGroupBool.h"
#include "GA_Attribute.h"
#include "GA_AttributeFilter.h"

#include <UT/UT_COW.h>
#include <UT/UT_Set.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>

#include <stdarg.h>

class GA_AttributeRefMapOffsetMap;
class GA_AttributeSet;
class GA_Detail;
class GA_WeightedSum;

class ga_AttributeHandleList;
class ga_VertexPoolProxy;


/// @brief A handle to simplify manipulation of multiple attributes.
///
/// The GA_AttributeRefMap class maintains a list of attributes.
/// Operations can be done on all attributes at one time.  The handle keeps a
/// mapping between @b destination and @source (const) attributes.  All
/// destination attributes need to be in one GA_Detail.  All source attributes
/// must be from one GA_Detail.  Often, the source and destination details are
/// the same.
///
/// When performing operations on object elements, all attribute classes which
/// are uniquely identified by the owner of the object will be operated on.
/// For example, when operating on a vertex, it is possible to uniquely
/// identify a point object.  Thus, when @c addValue() is called with a vertex
/// source offset, vertex @b and point attributes on the source detail will be
/// processed.  When @c addValue() is called with a point source offset, there
/// may be multiple vertices which share the point.  So, only point attributes
/// will be processed.
///
/// This can be summarized by:
///   - @c addValue() with source point offset:  @n
///	Process GA_ATTRIB_POINT, GA_ATTRIB_GLOBAL
///   - @c addValue() with source vertex offset: @n
///	Process GA_ATTRIB_VERTEX, GA_ATTRIB_POINT, GA_ATTRIB_PRIMITIVE and
///	GA_ATTRIB_DETAIL
///   - @c addValue() with source primitive offset: @n
///	Process GA_ATTRIB_PRIMITIVE and GA_ATTRIB_DETAIL
///   - @c addValue() with source detail offset: @n
///	Process GA_ATTRIB_DETAIL
///
/// The same semantics are used for the destination offset.  For example, if a
/// primitive offset is supplied, destination primitive and global attributes
/// will be modified.
///
/// By default, operations on "P" are performed as with any other attribute.
/// However, it's possible to turn on the homogeneous flag which will ensure
/// that operations are done using homogeneous coordinates.
class GA_API GA_AttributeRefMap 
{
public:
    /// Default constructor
    GA_AttributeRefMap();
    /// Copy constructor
    GA_AttributeRefMap(const GA_AttributeRefMap &src);
    /// Construct and bind details
    GA_AttributeRefMap(GA_Detail &dest, const GA_Detail *src=0);
    /// Construct a handle from a const detail.  This will create a temporary
    /// detail.  All temporary vertices will be allocated from the temporary
    /// detail (rather than the source).
    GA_AttributeRefMap(const GA_Detail &src);
    /// Destructor
    ~GA_AttributeRefMap();

    typedef	GA_AttributeRefMapOffsetMap	OffsetMap;

    /// @brief Automatically expand attribute data pages for threading
    ///
    /// Normally, threading is allowed only if each thread is guaranteed to
    /// write to an individual page of data.  Not all algorithms are amenable
    /// to this constraint.  Using the ThreadHarden class will force the
    /// attribute map to harden all write attributes so their data pages can be
    /// written to by multiple threads across page boundaries.  Multiple
    /// threads are still prohibited from writing to the same offset.  In its
    /// destructor, the class will automatically call the compress method to
    /// regain memory efficiency.
    /// For example: @code
    /// void process(GA_RWAttributeRef &write, GA_ROAttributeRef &read)
    /// {
    ///	  GA_AttributeRefMap::ThreadHarden	harden(map);
    ///	  // There's no need to harden the read-only attribute, but now that
    ///	  // we've hardened the write attribute, our threaded algorithm is able
    ///	  // to write data across page boundaries.
    ///	  UTparallelFor(range, functor(map));
    /// }
    ///
    /// WARNING: It is *NOT* *SAFE* to do random-access writes to a group
    ///          in parallel, even if it is hardened, unordered, and has its
    ///          cached entries count invalidated, because the access to the
    ///          individual bits isn't atomic.
    class GA_API ThreadHarden
    {
    public:
        ThreadHarden(const GA_AttributeRefMap &map,
                GA_Offset start_offset = GA_Offset(0),
                GA_Offset end_offset = GA_INVALID_OFFSET)
            : myMap(map)
            , myStart(start_offset)
            , myEnd(end_offset)
        {
            for (int i = 0; i < myMap.entries(); ++i)
            {
                GA_Attribute *attrib = myMap.getDestAttribute(i);
                attrib->hardenAllPages(myStart, myEnd);
                if (attrib->getScope() == GA_SCOPE_GROUP)
                {
                    GA_ATIGroupBool *group = static_cast<GA_ATIGroupBool *>(attrib);
                    // We can't write to ordered groups in parallel, and
                    // it's not clear what the desired result would be,
                    // so make all ordered groups unordered.
                    group->clearOrdered();
                    // Invalidate the cached group entries before writing,
                    // because there's no need to update the value at all
                    // while writing.
                    group->invalidateGroupEntries();
                }
            }
        }
        ~ThreadHarden()
        {
            for (int i = 0, n = myMap.entries(); i < n; ++i)
            {
                GA_Attribute *attrib = myMap.getDestAttribute(i);
                attrib->tryCompressAllPages(myStart, myEnd);
                attrib->bumpDataId();
                if (attrib->getScope() == GA_SCOPE_GROUP)
                {
                    GA_ATIGroupBool *group = static_cast<GA_ATIGroupBool *>(attrib);
                    // Invalidate the cached group entries afterward, just
                    // in case someone called entries() on the group after
                    // constructing this ThreadHarden and before writing
                    // multi-threaded.
                    // NOTE: Maybe we should just disallow that and avoid this.
                    group->invalidateGroupEntries();
                }
            }
        }
    private:
        const GA_AttributeRefMap	&myMap;
        GA_Offset			 myStart;
        GA_Offset			 myEnd;
    };

    void        bumpAllDestDataIds()
    {
        for (int i = 0, n = entries(); i < n; ++i)
        {
            GA_Attribute *attrib = getDestAttribute(i);
            attrib->bumpDataId();
        }
    }

    /// Bind details.  If the @c src detail is @c NULL, the destination will be
    /// used as the source.
    void	bind(GA_Detail &dest, const GA_Detail &src);

    /// Unbind details.
    void	unbind();

    /// Clears the list of attributes.
    /// NOTE: Just the list is cleared.  No attributes are modified, and the
    ///       detail(s) stay bound.
    void clear();

    /// Call on a bound instance to replace the source detail as an efficient
    /// alternative to binding both a new destination and source.
    ///
    /// Set @c as_custom_map to true to use the current source attributes as
    /// the template to look up attributes in the new source detail instead
    /// of the destination attributes (default).
    void	replaceSource(const GA_Detail &src, bool as_custom_map = false);

    /// Prepare the handle for use in a separate thread.  This is necessary
    /// to unshare any objects that cannot be safely shared with a handle
    /// used by another thread.
    ///
    /// Typically code will populate a template handle and then allocate a
    /// handle for each thread using the copy constructor, resulting in all
    /// handles sharing some objects, like the vertex pool.  This method is
    /// then called to disassociate each thread's handle from that sharing.
    void	initThreadInstance();

    /// Access to the vertex pool.  In most situations you'll want to bind it
    /// to a GA_WorkVertexBuffer instead of manipulating it directly.
    GA_VertexPool	&getVertexPool();
    /// @{
    /// Map from a GA_VertexPoolIndex to a GA_Offset.
    ///
    /// It's recommended to use a GA_WorkVertexBuffer instead of calling these
    /// methods directly.
    GA_Offset		getTempVertex(GA_VertexPoolIndex i) const;
    GA_Offset		getTempPoint(GA_VertexPoolIndex i) const;
    /// @}
    /// @{
    /// Allocate a temporary vertex/point.
    ///
    /// It's recommended to use a GA_WorkVertexBuffer instead of calling these
    /// methods directly.
    GA_VertexPoolIndex	 appendTempVertex(GA_Offset pt=GA_INVALID_OFFSET)
			    { return getVertexPool().appendVertex(pt); }
    GA_VertexPoolIndex	 appendTempPoint()
			    { return getVertexPool().appendPoint(); }
    /// @}
    /// @{
    /// Free a temporary vertex/point.
    ///
    /// It's recommended to use a GA_WorkVertexBuffer instead of calling these
    /// methods directly.
    void		 freeTempVertex(GA_VertexPoolIndex v)
			    { getVertexPool().freeVertex(v); }
    void		 freeTempPoint(GA_VertexPoolIndex p)
			    { getVertexPool().freePoint(p); }
    /// @}

    /// @{
    /// Perform attribute lookup.  Since it's possible that there are two
    /// details involved in the handle, this method provides a convenient
    /// method to look up one attribute given the other.
    const GA_Attribute	*findSourceAttribute(GA_Attribute *dest_atr) const;
    GA_Attribute	*findDestAttribute(const GA_Attribute *src_atr) const;
    /// @}

    /// Add an attribute to the handle.  The function fails if the attributes
    /// were not defined on the correct detail.
    ///
    /// @param dest The destination attribute must be defined on the dest detail
    /// @param src The source attribute must be defined on the source detail
    bool	append(GA_Attribute *dest, const GA_Attribute *src);

    /// Append an attribute from the destination detail
    bool	appendDest(GA_Attribute *dest)
		{
		    const GA_Attribute	*src = findSourceAttribute(dest);
		    return src ? append(dest, src) : false;
		}
    /// Append an attribute from the source detail
    bool	appendSource(const GA_Attribute *src)
		{
		    GA_Attribute	*dest = findDestAttribute(src);
		    return dest ? append(dest, src) : false;
		}

    /// @{
    /// Add attributes based on an attribute filter.  Returns the number of
    /// attributes added.
    int		append(const GA_AttributeFilter &filter,
			const GA_AttributeOwner search_order[],
			int search_order_size);
    int		append(const GA_AttributeFilter &filter,
			GA_AttributeOwner owner)
		    { return append(filter, &owner, 1); }
    /// @}

    /// @{
    /// One common scenario we encounter is the use of a temporary detail as
    /// the destination when evaluating attributes from a constant detail.
    /// In this scenario the original constant detail acts as the source and
    /// we populate our reference map by cloning the attributes we want to
    /// evaluate in the destination detail.

    /// Clone and append an attribute from the source detail in the destination
    /// detail.
    bool	cloneAndAppendSource(const GA_Attribute *src);

    /// Clone and append an attribute from the source detail, based on name, in
    /// the destination detail.
    bool	cloneAndAppendSource(GA_AttributeOwner owner,
				     GA_AttributeScope scope,
				     const char *name);
    
    /// Clone and append attributes based on an attribute filter.  Returns the
    /// number of attributes added.
    int		cloneAndAppendFromSource(const GA_AttributeFilter &filter,
					 const GA_AttributeOwner search_order[],
					 int search_order_size);
    /// Clone and append attributes based on an attribute filter.  Returns the
    /// number of attributes added.
    int		cloneAndAppendFromSource(const GA_AttributeFilter &filter,
					 GA_AttributeOwner owner)
		    { return cloneAndAppendFromSource(filter, &owner, 1); }

    /// @}

    /// Return the number of attributes in the list
    int		entries() const;

    /// Return the nth destination attribute
    GA_Attribute	*getDestAttribute(int i) const;
    /// Return the nth source attribute
    const GA_Attribute	*getSourceAttribute(int i) const;

    /// Add attributes from two different details, mapping attributes by name.
    /// The attributes may come from different element classes.
    int		append(GA_AttributeSet *dest, GA_AttributeOwner downer,
		       const GA_AttributeSet *src, GA_AttributeOwner sowner);
    int		append(GA_AttributeSet *dest, GA_AttributeOwner downer,
		       const GA_AttributeFilter &dfilter,
		       const GA_AttributeSet *src, GA_AttributeOwner sowner,
		       const GA_AttributeFilter &sfilter);

    /// Return the number of attributes in the list that are paired with the
    /// source being P.
    int		entriesWithSourceP() const;

    /// Homogenize any rational attributes of the target element
    void	homogenize(GA_AttributeOwner dest_owner, GA_Offset dest) const;
    void	homogenize(OffsetMap &dest) const;
    /// Dehomogenize any rational attributes of the target element
    void	dehomogenize(GA_AttributeOwner dest_owner,GA_Offset dest) const;
    void	dehomogenize(OffsetMap &dest) const;

    /// Copy operation:  @code dest = source @endcode
    /// This is the generic copy operation 
    void	copyValue(GA_AttributeOwner downer, GA_Offset doffset,
			  GA_AttributeOwner sowner, GA_Offset soffset) const;
    void	copyValue(OffsetMap &dest,
			  GA_AttributeOwner sowner, GA_Offset soffset) const;

    /// @{
    /// Compare value with another elements
    bool	isEqual(GA_AttributeOwner aowner, GA_Offset aoffset,
			GA_AttributeOwner bowner, GA_Offset boffset) const;
    bool	isEqual(OffsetMap &a,
			GA_AttributeOwner bowner, GA_Offset boffset) const;
    bool	isAlmostEqual(GA_AttributeOwner aowner, GA_Offset aoffset,
			GA_AttributeOwner bowner, GA_Offset boffset) const;
    bool	isAlmostEqual(OffsetMap &a, 
			GA_AttributeOwner bowner, GA_Offset boffset) const;
    /// @}
    /// @{
    /// Copy operations:  @code dest = position @endcode
    /// Sets any attributes that read the position attribute to an explicit
    /// value.
    void	copyExplicitPosition(GA_AttributeOwner downer, GA_Offset dest,
				     const UT_Vector2 &p) const;
    void	copyExplicitPosition(OffsetMap &dest, 
				     const UT_Vector2 &p) const;
    void	copyExplicitPosition(GA_AttributeOwner downer, GA_Offset dest,
				     const UT_Vector2D &p) const;
    void	copyExplicitPosition(OffsetMap &dest, 
				     const UT_Vector2D &p) const;
    void	copyExplicitPosition(GA_AttributeOwner downer, GA_Offset dest,
				     const UT_Vector3 &p) const;
    void	copyExplicitPosition(OffsetMap &dest,
				     const UT_Vector3 &p) const;
    void	copyExplicitPosition(GA_AttributeOwner downer, GA_Offset dest,
				     const UT_Vector3D &p) const;
    void	copyExplicitPosition(OffsetMap &dest,
				     const UT_Vector3D &p) const;
    void	copyExplicitPosition(GA_AttributeOwner downer, GA_Offset dest,
				     const UT_Vector4 &p) const;
    void	copyExplicitPosition(OffsetMap &dest,
				     const UT_Vector4 &p) const;
    void	copyExplicitPosition(GA_AttributeOwner downer, GA_Offset dest,
				     const UT_Vector4D &p) const;
    void	copyExplicitPosition(OffsetMap &dest,
				     const UT_Vector4D &p) const;
    /// @}

    /// Add operation:  @code dest += source*scale @endcode
    /// This is the generic add operation
    void	addValue(GA_AttributeOwner downer, GA_Offset dest,
			 GA_AttributeOwner owner, GA_Offset source_index,
			 fpreal scale=1) const;
    void	addValue(OffsetMap &dest,
			 GA_AttributeOwner owner, GA_Offset source_index,
			 fpreal scale=1) const;

    /// Sub operation:  @code dest -= source @endcode
    /// This is the generic sub operation
    void	subValue(GA_AttributeOwner downer, GA_Offset dest,
			 GA_AttributeOwner owner, GA_Offset source_index) const;
    void	subValue(OffsetMap &dest,
			 GA_AttributeOwner owner, GA_Offset source_index) const;

    /// Multiply operation:  @code dest *= source @endcode
    /// This is the generic mul operation
    void	mulValue(GA_AttributeOwner downer, GA_Offset dest,
			 GA_AttributeOwner owner, GA_Offset source_index) const;
    void	mulValue(OffsetMap &dest,
			 GA_AttributeOwner owner, GA_Offset source_index) const;

    /// Linear interpolation operation:  @code dest = s0 + (s1 - s0)*t @endcode
    /// Generic linear interpolation between two of the same elements
    void	lerpValue(GA_AttributeOwner downer, GA_Offset dest,
			  GA_AttributeOwner owner, GA_Offset s0, GA_Offset s1,
			  fpreal t) const;
    void	lerpValue(OffsetMap &dest,
			  GA_AttributeOwner owner, GA_Offset s0, GA_Offset s1,
			  fpreal t) const;
    void	lerpHValue(GA_AttributeOwner downer, GA_Offset dest,
			   GA_AttributeOwner owner, GA_Offset s0, GA_Offset s1,
			   fpreal t) const;
    void	lerpHValue(OffsetMap &dest,
			   GA_AttributeOwner owner, GA_Offset s0, GA_Offset s1,
			   fpreal t) const;

    /// @{
    /// Weighted sum operation:  @code dest = sum(w[i]*s[i]) @endcode
    /// @code
    ///	    // Compute the average value of the P attribute and store in a
    ///	    // global attribute "averageP".
    ///	    GA_WeightedSum		sum;
    ///	    GA_AttributeRefMap		map(gdp);
    ///	    GA_AttributeRefMap::Handle	gah(map);
    ///	    int				npts;
    ///	    map.append( detail.findGlobalAttribute("averageP"),
    ///			detail.findPointAttribute("P") );
    ///	    gah.setGlobal();	// Write to global attributes
    ///	    gah.startSum(sum);
    ///	    for (GA_Iterator it = gdp.getPointRange(); !it.atEnd(); ++it)
    ///		gah.sumPoint(sum, it.getOffset(), 1);
    ///	    npts = gdp.getNumPoints();
    ///	    gah.finishSum(sum, npts ? 1./npts : 0);
    /// @endcode
    void	startSum(const GA_WeightedSum &sum, GA_AttributeOwner downer,
			 GA_Offset dest) const;
    void	startSum(const GA_WeightedSum &sum, OffsetMap &dest) const;
    void	startHSum(const GA_WeightedSum &sum, GA_AttributeOwner downer,
			  GA_Offset dest) const { startSum(sum, downer, dest); }
    void	startHSum(const GA_WeightedSum &sum, OffsetMap &dest) const
						{ startSum(sum, dest); }
    void	finishSum(const GA_WeightedSum &sum, GA_AttributeOwner downer,
			  GA_Offset dest, fpreal normalization=1) const;
    void	finishSum(const GA_WeightedSum &sum, OffsetMap &dest,
			 fpreal normalization=1) const;
    void	finishHSum(const GA_WeightedSum &sum, GA_AttributeOwner downer,
			   GA_Offset dest, fpreal normalization=1) const;
    void	finishHSum(const GA_WeightedSum &sum, OffsetMap &dest,
			   fpreal normalization=1) const;
    /// @}

    /// Add a value into the weighted sum.  This advances the weighted sum with
    /// the weight given.
    void	addSumValue(GA_WeightedSum &sum,
			    GA_AttributeOwner downer, GA_Offset dest,
			    GA_AttributeOwner owner, GA_Offset source_index,
			    fpreal w) const;
    void	addSumValue(GA_WeightedSum &sum,
			    OffsetMap &dest,
			    GA_AttributeOwner owner, GA_Offset source_index,
			    fpreal w) const;
    void	addHSumValue(GA_WeightedSum &sum,
			     GA_AttributeOwner downer, GA_Offset dest,
			     GA_AttributeOwner owner, GA_Offset source_index,
			     fpreal w) const;
    void	addHSumValue(GA_WeightedSum &sum,
			     OffsetMap &dest,
			     GA_AttributeOwner owner, GA_Offset source_index,
			     fpreal w) const;

    /// Compute barycentric coordinates for 3 values
    /// @code result = (1 - u - v)*p0 + u*p1 + v*p2 @endcode
    void	barycentricValue(GA_AttributeOwner downer, GA_Offset dest,
				 GA_AttributeOwner owner,
				 GA_Offset p0, GA_Offset p1, GA_Offset p2,
				 fpreal u, fpreal v) const;
    void	barycentricValue(OffsetMap &dest,
				 GA_AttributeOwner owner,
				 GA_Offset p0, GA_Offset p1, GA_Offset p2,
				 fpreal u, fpreal v) const;

    void	barycentricValue(GA_AttributeOwner downer, GA_Offset dest,
				 GA_AttributeOwner owner,
				 GA_Offset p0, GA_Offset p1,
				 GA_Offset p2, GA_Offset p3,
				 fpreal u, fpreal v, fpreal w) const;
    void	barycentricValue(OffsetMap &dest,
				 GA_AttributeOwner owner,
				 GA_Offset p0, GA_Offset p1,
				 GA_Offset p2, GA_Offset p3,
				 fpreal u, fpreal v, fpreal w) const;

    /// Compute bilinear interpolation over 4 values of a quadrilateral.  The
    /// math is the same as SYSbilerp(): @code
    ///  (u=0,v=1) u0v1     u1v1 (u=1,v=1)
    ///              +--------+
    ///              |        |
    ///              |        |
    ///              +--------+
    ///  (u=0,v=0) u0v0     u1v0 (u=1,v=0)
    /// @endcode
    void	bilinearValue(GA_AttributeOwner downer, GA_Offset dest,
			      GA_AttributeOwner owner,
			      GA_Offset u0v0, GA_Offset u1v0,
			      GA_Offset u0v1, GA_Offset u1v1,
			      fpreal u, fpreal v) const;
    void	bilinearValue(OffsetMap &dest,
			      GA_AttributeOwner owner,
			      GA_Offset u0v0, GA_Offset u1v0,
			      GA_Offset u0v1, GA_Offset u1v1,
			      fpreal u, fpreal v) const;

    /// Zero an attribute.  This is equivalent to a weighted sum of 0 elements.
    void	zeroElement(GA_AttributeOwner downer, GA_Offset dest) const;
    void	zeroElement(OffsetMap &dest) const;

    /// Multiply operation:  @code dest *= scale @endcode
    void	multiply(GA_AttributeOwner downer, GA_Offset dest,
			 fpreal scale) const;
    void	multiply(OffsetMap &dest, fpreal scale) const;

    /// @{
    /// Transform attributes.  This uses tags on the attribute to determine how
    /// to perform the transform.
    /// @param m The transform matrix
    /// @param im The inverse of @m (i.e. @c m.invert(im) )
    /// Transforms are independent of the source geometry.
    void	transform(const UT_Matrix4 &m, const UT_Matrix4 &im,
			  GA_AttributeOwner downer, GA_Offset dest) const;
    void	transform(const UT_Matrix4 &m, const UT_Matrix4 &im,
			  OffsetMap &dest) const;
    void	transform(const UT_DMatrix4 &m, const UT_DMatrix4 &im,
			  GA_AttributeOwner downer, GA_Offset dest) const;
    void	transform(const UT_DMatrix4 &m, const UT_DMatrix4 &im,
			  OffsetMap &dest) const;
    /// @}

    /// @{
    /// Standard operations read the source and write to the destination.
    /// However.  These operations operate by reading and writing the
    /// destination.
    /// @see copyValue(), addValue(), subValue(), lerpValue()
    void	copyDestValue(GA_AttributeOwner downer, GA_Offset dest,
			      GA_AttributeOwner owner, GA_Offset offset) const;
    void	copyDestValue(OffsetMap &dest,
			      GA_AttributeOwner owner, GA_Offset offset) const;

    void	addDestValue(GA_AttributeOwner downer, GA_Offset dest,
			     GA_AttributeOwner owner, GA_Offset offset) const;
    void	addDestValue(OffsetMap &dest,
			     GA_AttributeOwner owner, GA_Offset offset) const;

    void	subDestValue(GA_AttributeOwner downer, GA_Offset dest,
			     GA_AttributeOwner owner, GA_Offset offset) const;
    void	subDestValue(OffsetMap &dest,
			     GA_AttributeOwner owner, GA_Offset offset) const;

    void	lerpDestValue(GA_AttributeOwner downer, GA_Offset dest,
			      GA_AttributeOwner owner,
			      GA_Offset s0, GA_Offset s1, fpreal t) const;
    void	lerpDestValue(OffsetMap &dest,
			      GA_AttributeOwner owner,
			      GA_Offset s0, GA_Offset s1, fpreal t) const;

    void	addSumDestValue(GA_WeightedSum &sum,
				GA_AttributeOwner downer, GA_Offset dest,
				GA_AttributeOwner owner, GA_Offset offset,
				fpreal w) const;
    void	addSumDestValue(GA_WeightedSum &sum,
				OffsetMap &dest,
				GA_AttributeOwner owner, GA_Offset offset,
				fpreal w) const;
    void	addHSumDestValue(GA_WeightedSum &sum,
				 GA_AttributeOwner downer, GA_Offset dest,
				 GA_AttributeOwner owner, GA_Offset offset,
				 fpreal w) const;
    void	addHSumDestValue(GA_WeightedSum &sum,
				 OffsetMap &dest,
				 GA_AttributeOwner owner, GA_Offset offset,
				 fpreal w) const;
    /// @}

    /// Debug statement to dump all the attributes to stdout
    void	dump(GA_AttributeOwner downer, GA_Offset dest,
		     const char *msg, ...) const;
    void	dump(OffsetMap &dest,
		     const char *msg, ...) const;
    void	vdump(GA_AttributeOwner downer, GA_Offset dest,
		      const char *msg, va_list args) const;
    void	vdump(OffsetMap &dest,
		      const char *msg, va_list args) const;

    GA_Detail		*getDestDetail() const		{ return myD; }
    const GA_Detail	*getSourceDetail() const	{ return myS; }

    /// A specialized filter that can match multiple attributes
    /// in a space-separated string, and will accept "Pw" in the
    /// pattern to refer to the "P" attribute, if there's no "Pw"
    /// attribute.
    class FilterPattern : public GA_AttributeFilterImpl
    {
    public:
        // Filter based on a pattern string
        FilterPattern(const char *pattern,
                      bool matchPw=false)
            : myPattern(pattern)
            , myMatchPw(matchPw)
        {}
        virtual ~FilterPattern() { }
        virtual bool	match(const GA_Attribute *a) const
        {
            UT_String	name(a->getName());
            if (name.multiMatch(myPattern) != 0)
                return true;
            if (myMatchPw &&
                a->getOwner() == GA_ATTRIB_POINT &&
                strcmp(a->getName(), "P") == 0)
            {
                name = "Pw";
                return name.multiMatch(myPattern) != 0;
            }
            return false;
        }
        const char *const myPattern;
        const bool myMatchPw;
    };

    /// Appends all sourceowner source attributes matching matchpattern
    /// to this attribute map.  If the attribute doesn't exist on the
    /// destination, it will be created based on the source attribute.
    /// If alreadymappeddest is non-NULL and the destination attribute
    /// is in alreadymappeddest, it won't be appended, and all appended
    /// attributes will be added to alreadymappeddest, to help avoid
    /// collisions.  Ordered groups on the destination will be changed
    /// to unordered groups before being added.  If matchpattern is NULL,
    /// no attributes will be matched.
    void appendAndCreateAllSource(
        GA_AttributeOwner destowner,
        GA_AttributeOwner sourceowner,
        const char *matchpattern,
        UT_Set<const GA_Attribute *> *alreadymappeddest = NULL,
        bool includegroups = true);

private:

	  GA_Detail		*myD;
    const GA_Detail		*myS;

    UT_COWHandle<ga_AttributeHandleList>	myList;
    UT_SharedPtr<ga_VertexPoolProxy>		myVertexPoolProxy;
};

#endif
