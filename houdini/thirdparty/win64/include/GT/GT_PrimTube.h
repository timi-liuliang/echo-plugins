/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimTube.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimTube__
#define __GT_PrimTube__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_Transform.h"
#include <SYS/SYS_Math.h>

/// @brief A tapered tube primitive (mapping to GEO_PrimTube)
///
/// The tube primitive represents a tapered tube along the @b Z axis.  Unlike
/// the GEO counterpart, this primitive's axis is aligned along the Z axis (not
/// the Y axis).
class GT_API GT_PrimTube : public GT_Primitive
{
public:
    /// Default constructor
    GT_PrimTube()
    {}
    /// Useful constructor
    GT_PrimTube(const GT_AttributeListHandle &attribs,
		const GT_TransformHandle &transform,
		fpreal taper,
		bool caps)
	: myTaper(SYSmax(0.0, taper))
	, myCaps(caps)
	, myAttributes(attribs)
	, GT_Primitive()
    { setPrimitiveTransform(transform); }
    /// Copy constructor
    GT_PrimTube(const GT_PrimTube &src)
	: GT_Primitive(src)
        , myTaper(src.myTaper)
	, myCaps(src.myCaps)
	, myAttributes(src.myAttributes)
    {
    }
    /// Destructor
    virtual ~GT_PrimTube();

    virtual const char *className() const { return "GT_PrimTube"; }
    virtual bool	save(UT_JSONWriter &w) const;
    
    /// @{
    /// Methods defined on GT_Primitive
    virtual int			getPrimitiveType() const;
    /// @}

    /// Initialize a tube with
    /// - @c attribs @n
    ///	  Attributes defined on the tube
    /// - @c transform @n
    ///	  The transform attribute list should contain a single transform
    ///	  representing the 4x4 transform of the tube.
    bool	init(const GT_AttributeListHandle &attribs,
			const GT_TransformHandle &transform,
			fpreal taper,
			bool caps)
		{
		    myTaper = SYSmax(0.0, taper);
		    myCaps = caps;
		    myAttributes = attribs;
                    setPrimitiveTransform(transform);
		    return true;
		}

    /// Get a tesselation of a tube with a given set of refinement
    /// parameters.
    static GT_PrimitiveHandle	makePolygonMesh(const GT_RefineParms *parms, 
						fpreal taper = 1.0f);

    /// @{
    /// Accessors
    const GT_AttributeListHandle	&getAttributes() const
					    { return myAttributes; }
    const fpreal			&getTaper() const
					    { return myTaper; }
    const bool				&getCaps() const
					    { return myCaps; }
    /// @}

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myAttributes; }
    /// @}

    /// @{
    /// Methods defined on GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				    int nsegments) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimTube(*this); }
    /// @}

    /// Convenience method to refine to a polygon mesh
    GT_PrimitiveHandle	refineToPolygonMesh(
				    const GT_RefineParms *parms) const;

protected:
    GT_AttributeListHandle	myAttributes;
    fpreal			myTaper;
    bool			myCaps;
};

#endif
