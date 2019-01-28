/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ElementWrangler.h ( GA Library, C++)
 *
 * COMMENTS:	A utility class to abstract common preamble from GA_GBElement
 * 		methods to a higher level.
 */

#ifndef __GA_ElementWrangler__
#define __GA_ElementWrangler__

#include "GA_API.h"
#include "GA_AttributeFilter.h"
#include "GA_AttributeRefMap.h"
#include "GA_Types.h"

#include <UT/UT_NonCopyable.h>
#include <SYS/SYS_Types.h>


class GA_Attribute;
class GA_Detail;
class GA_Range;


/// @brief A utility class to amortize overhead from GA_GBElement attribute
/// handling methods.
///
/// The GA_ElementWrangler class provides a simplified interface for common
/// attribute operations as elements are added or removed.
///
/// This class is not thread-safe, so each thread should have its own copy,
/// and any attributes added after its instantiation will not be processed.

class GA_API GA_ElementWrangler
{
public:
    void	 copyAttributeValues(GA_Offset dest, GA_Offset src)
		 { 
		     myMap.copyValue(myOwner, dest, myOwner, src);
		 }
    void	 copyAttributeValues(const GA_Range &dest, GA_Offset src);
    void	 scaleAttributeValues(GA_Offset dest, fpreal scale)
		 {
		     myMap.multiply(myOwner, dest, scale);
		 }
    void	 addAttributeValues(GA_Offset dest, GA_Offset src,
				    fpreal scale=1)
		 {
		     myMap.addValue(myOwner, dest, myOwner, src, scale);
		 }
    void	 lerpAttributeValues(GA_Offset dest, GA_Offset src0,
				     GA_Offset src1, fpreal bias)
		 {
		     myMap.lerpValue(myOwner, dest, myOwner, src0, src1, bias);
		 }

    bool	 isValid(GA_Detail &dest) const
		 {
		     return myMap.getDestDetail() == &dest &&
			    myMap.getSourceDetail() == &dest;
		 }
    bool	 isValid(GA_Detail &dest, const GA_Detail &src) const
		 {
		     return myMap.getDestDetail() == &dest &&
			    myMap.getSourceDetail() == &src;
		 }
    int		 getNumAttributes() const
		 { return myMap.entries(); }
    void         bumpAllDestDataIds()
    {
        myMap.bumpAllDestDataIds();
    }
protected:
    GA_ElementWrangler(GA_AttributeOwner owner, GA_Detail &dest,
		       const GA_Attribute *exclude = 0);
    GA_ElementWrangler(GA_AttributeOwner owner, GA_Detail &dest,
		       const GA_Detail &src, const GA_Attribute *exclude = 0);

    GA_ElementWrangler(GA_AttributeOwner owner, GA_Detail &dest,
		       const GA_AttributeFilter &filter);
    GA_ElementWrangler(GA_AttributeOwner owner, GA_Detail &dest,
		       const GA_Detail &src, const GA_AttributeFilter &filter);

    /// Protected destructor as we don't support polymorphic destruction.
    ~GA_ElementWrangler() {}

    GA_AttributeRefMap			&getMap() { return myMap; }
private:
    GA_AttributeOwner		 	 myOwner;
    GA_AttributeRefMap		 	 myMap;
};

/// By default, operations on "P" are performed as with any other attribute.
/// However, it's possible to turn on the homogeneous flag which will ensure
/// that operations are done using homogeneous coordinates.
class GA_API GA_PointWrangler : public GA_ElementWrangler
{
public:
    enum IncludeP { EXCLUDE_P, INCLUDE_P };

     /// The GA_PointWrangler::IncludeP argument passed to the constructor
     /// determines whether the base class operations include "P".
     GA_PointWrangler(GA_Detail &dest, IncludeP p);
     GA_PointWrangler(GA_Detail &dest, const GA_Detail &src, IncludeP p);

     GA_PointWrangler(GA_Detail &dest, const GA_AttributeFilter &filter);
     GA_PointWrangler(GA_Detail &dest, const GA_Detail &src,
		      const GA_AttributeFilter &filter);

    ~GA_PointWrangler()
     {
	 delete myAlternateMap;
     }

    bool	 includesP() const { return myBaseIncludeP == INCLUDE_P; }

    using GA_ElementWrangler::copyAttributeValues;
    using GA_ElementWrangler::scaleAttributeValues;
    using GA_ElementWrangler::addAttributeValues;
    using GA_ElementWrangler::lerpAttributeValues;

    void	 copyAttributeValues(GA_Offset dest, GA_Offset src,
				     IncludeP copy_p)
		 { 
		     if (copy_p == myBaseIncludeP)
			 copyAttributeValues(dest, src);
		     else
		     {
			 initAlternateHandle();
			 myAlternateMap->copyValue(
				 GA_ATTRIB_POINT, dest,
				 GA_ATTRIB_POINT, src);
		     }
		 }
    void	 scaleAttributeValues(GA_Offset dest, fpreal scale,
				      IncludeP scale_p)
		 {
		     if (scale_p == myBaseIncludeP)
			 scaleAttributeValues(dest, scale);
		     else
		     {
			 initAlternateHandle();
			 myAlternateMap->multiply(
				 GA_ATTRIB_POINT, dest, scale);
		     }
		 }
    void	 addAttributeValues(GA_Offset dest, GA_Offset src,
				    fpreal scale, IncludeP add_p)
		 {
		     if (add_p == myBaseIncludeP)
			 addAttributeValues(dest, src, scale);
		     else
		     {
			 initAlternateHandle();
			 myAlternateMap->addValue(
				 GA_ATTRIB_POINT, dest,
				 GA_ATTRIB_POINT, src, scale);
		     }
		 }
    void	 lerpAttributeValues(GA_Offset dest, GA_Offset src0,
				     GA_Offset src1, fpreal bias,
				     IncludeP lerp_p)
		 {
		     if (lerp_p == myBaseIncludeP)
			 lerpAttributeValues(dest, src0, src1, bias);
		     else
		     {
			 initAlternateHandle();
			 myAlternateMap->lerpValue(
				 GA_ATTRIB_POINT, dest,
				 GA_ATTRIB_POINT, src0, src1, bias);
		     }
		 }
private:
    void	 initAlternateHandle()
		 {
		     if (!myAlternateMap)
			 allocAlternateHandle();
		 }
    void	 allocAlternateHandle();

    // We keep track of whether the base class handle includes "P" since we
    // may need to allocate an alternate handle when an operation requests
    // the opposite behavior.
    IncludeP				myBaseIncludeP;
    GA_AttributeFilter			myFilter;
    GA_AttributeRefMap			*myAlternateMap;
};

class GA_API GA_VertexWrangler : public GA_ElementWrangler
{
public:
    GA_VertexWrangler(GA_Detail &dest)
	: GA_ElementWrangler(GA_ATTRIB_VERTEX, dest) {}
    GA_VertexWrangler(GA_Detail &dest, const GA_Detail &src)
	: GA_ElementWrangler(GA_ATTRIB_VERTEX, dest, src) {}

    GA_VertexWrangler(GA_Detail &dest, const GA_AttributeFilter &filter)
	: GA_ElementWrangler(GA_ATTRIB_VERTEX, dest, filter) {}
    GA_VertexWrangler(GA_Detail &dest, const GA_Detail &src,
		      const GA_AttributeFilter &filter)
	: GA_ElementWrangler(GA_ATTRIB_VERTEX, dest, src, filter) {}
private:
};

class GA_API GA_PrimitiveWrangler : public GA_ElementWrangler
{
public:
    GA_PrimitiveWrangler(GA_Detail &dest)
	: GA_ElementWrangler(GA_ATTRIB_PRIMITIVE, dest) {}
    GA_PrimitiveWrangler(GA_Detail &dest, const GA_Detail &src)
	: GA_ElementWrangler(GA_ATTRIB_PRIMITIVE, dest, src) {}

    GA_PrimitiveWrangler(GA_Detail &dest, const GA_AttributeFilter &filter)
	: GA_ElementWrangler(GA_ATTRIB_PRIMITIVE, dest, filter) {}
    GA_PrimitiveWrangler(GA_Detail &dest, const GA_Detail &src,
			 const GA_AttributeFilter &filter)
	: GA_ElementWrangler(GA_ATTRIB_PRIMITIVE, dest, src, filter) {}
private:
};

class GA_API GA_DetailWrangler : public GA_ElementWrangler
{
public:
    GA_DetailWrangler(GA_Detail &dest)
	: GA_ElementWrangler(GA_ATTRIB_GLOBAL, dest) {}
    GA_DetailWrangler(GA_Detail &dest, const GA_Detail &src)
	: GA_ElementWrangler(GA_ATTRIB_GLOBAL, dest, src) {}

    GA_DetailWrangler(GA_Detail &dest, const GA_AttributeFilter &filter)
	: GA_ElementWrangler(GA_ATTRIB_GLOBAL, dest, filter) {}
    GA_DetailWrangler(GA_Detail &dest, const GA_Detail &src,
			 const GA_AttributeFilter &filter)
	: GA_ElementWrangler(GA_ATTRIB_GLOBAL, dest, src, filter) {}
private:
};

class GA_API GA_ElementWranglerCache : private UT_NonCopyable
{
public:
     GA_ElementWranglerCache(GA_Detail &dest,
			     GA_PointWrangler::IncludeP include_p)
	: myDestGdp(&dest), mySrcGdp(&dest), myPrimitiveWrangler(0),
	  myVertexWrangler(0), myPointWrangler(0), myIncludeP(include_p) {}
     GA_ElementWranglerCache(GA_Detail &dest, const GA_Detail &src,
			     GA_PointWrangler::IncludeP include_p)
	: myDestGdp(&dest), mySrcGdp(&src), myPrimitiveWrangler(0),
	  myVertexWrangler(0), myPointWrangler(0), myIncludeP(include_p) {}

    GA_ElementWranglerCache(GA_Detail &dest,
			    const GA_AttributeFilter &filter)
	: myDestGdp(&dest), mySrcGdp(&dest), myPrimitiveWrangler(0),
	  myVertexWrangler(0), myPointWrangler(0), myFilter(filter) {}
     GA_ElementWranglerCache(GA_Detail &dest, const GA_Detail &src,
			     const GA_AttributeFilter &filter)
	: myDestGdp(&dest), mySrcGdp(&src), myPrimitiveWrangler(0),
	  myVertexWrangler(0), myPointWrangler(0), myFilter(filter) {}

    ~GA_ElementWranglerCache()
    {
	if (myPrimitiveWrangler)
	    delete myPrimitiveWrangler;
	if (myVertexWrangler)
	    delete myVertexWrangler;
	if (myPointWrangler)
	    delete myPointWrangler;
    }

    void			 clear()
    {
	if (myPrimitiveWrangler)
	{
	    delete myPrimitiveWrangler;
	    myPrimitiveWrangler = 0;
	}
	if (myVertexWrangler)
	{
	    delete myVertexWrangler;
	    myVertexWrangler = 0;
	}
	if (myPointWrangler)
	{
	    delete myPointWrangler;
	    myPointWrangler = 0;
	}
    }

    void			 reset(GA_Detail &dest)
				 {
				     myDestGdp = &dest;
				     mySrcGdp = &dest;
				     clear();
				 }
    void			 reset(GA_Detail &dest, const GA_Detail &src)
				 {
				     myDestGdp = &dest;
				     mySrcGdp = &src;
				     clear();
				 }
    bool			 isValid(const GA_Detail &dest) const
				 {
				     return myDestGdp == &dest &&
					    mySrcGdp == &dest;
				 }
    bool			 isValid(const GA_Detail &dest,
					 const GA_Detail &src) const
				 {
				     return myDestGdp == &dest &&
					    mySrcGdp == &src;
				 }

    GA_PrimitiveWrangler	&getPrimitive()
    {
	if (!myPrimitiveWrangler)
	{
	    if (myFilter.isValid())
		myPrimitiveWrangler = new GA_PrimitiveWrangler(*myDestGdp,
							       *mySrcGdp,
							       myFilter);
	    else
		myPrimitiveWrangler = new GA_PrimitiveWrangler(*myDestGdp,
							       *mySrcGdp);
	}
	return *myPrimitiveWrangler;
    }
    GA_VertexWrangler		&getVertex()
    {
	if (!myVertexWrangler)
	{
	    if (myFilter.isValid())
		myVertexWrangler = new GA_VertexWrangler(*myDestGdp,
							 *mySrcGdp,
							 myFilter);
	    else
		myVertexWrangler = new GA_VertexWrangler(*myDestGdp,
							 *mySrcGdp);
	}
	return *myVertexWrangler;
    }
    GA_PointWrangler		&getPoint()
    {
	if (!myPointWrangler)
	{
	    if (myFilter.isValid())
		myPointWrangler = new GA_PointWrangler(*myDestGdp, *mySrcGdp,
							myFilter);
	    else
		myPointWrangler = new GA_PointWrangler(*myDestGdp, *mySrcGdp,
						       myIncludeP);
	}
	return *myPointWrangler;
    }

private:
    GA_Detail			*myDestGdp;
    const GA_Detail		*mySrcGdp;
    GA_PrimitiveWrangler	*myPrimitiveWrangler;
    GA_VertexWrangler		*myVertexWrangler;
    GA_PointWrangler		*myPointWrangler;
    GA_PointWrangler::IncludeP	myIncludeP;
    GA_AttributeFilter		myFilter;
};

#endif
