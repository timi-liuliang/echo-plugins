/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RAY_ProcIsoBase.h ( RAY Library, C++)
 *
 * COMMENTS:	Iso-surface procedural baseclass
 */

#ifndef __RAY_ProcIsoBase__
#define __RAY_ProcIsoBase__

#include "RAY_API.h"
#include <GA/GA_Handle.h>
#include <UT/UT_BoundingBox.h>
#include "RAY_Procedural.h"

class RAY_API RAY_ProcIsoBase : public RAY_Procedural
{
public:
	     RAY_ProcIsoBase();
    virtual ~RAY_ProcIsoBase();

    virtual const char	*className() const = 0;

    //
    // The sub-class is responsible for initializing parameters.  The
    // initialization follows the rules for RAY_Procedural.
    virtual int		 initialize(const UT_BoundingBox *box) = 0;
    //
    // The evalDensity method is used to evaluate the density of the iso
    // surface.  The surface will be generated where the density is 0.
    virtual fpreal	 evaluateDensity(const float pos[3]) const = 0;

    //
    // The render method may require the full surface to be split into smaller
    // procedurals.  The box passed in to the createChild() method is the
    // bounding box without motion vectors applied.  This method is responsible
    // for:
    //		a)  Creating a new sub-class instance
    //		b)  Setting the myBox member data.  The myBox must be
    //		    totally enclosed in the bounding box passed in.
    //		c)  Setting the myVelBox member data (possibly to the same
    //		    value as myBox if there is no motion blur)
    //
    virtual RAY_ProcIsoBase	*createChild(const UT_BoundingBox &box) = 0;

    //
    // Attributes must be created for geometry before it's added to mantra, but
    // after the points and polygons have been created.  The attributes can be
    // arbitrary.  However, since velocity is used in generation of motion blur
    // data, the index of the velocity attribute in the gdp needs to be
    // returned by this method.  Thus the return codes are:
    //
    //	false	- An un-recoverable error
    //	true	- The velocity_attribute parameter is either invalid (no
    //		  velocity attribute) or valid and refers to the velocity
    //		  attribute.
    //
    // By default there is no velocity attribute added
    //
    // If velocity attributes are added to the detail, an arbitrary scale can
    // also be applied to the velocity (i.e. shutter).
    virtual bool	addAttributes(GU_Detail *gdp, float &velocityscale,
				    GA_ROHandleV3 &velocity_attribute);

    //
    // By default, this will return the bounding box with velocity motion
    // vectors applied.  This is required since mantra needs to know the entire
    // possible range of geometry.
    virtual void	 getBoundingBox(UT_BoundingBox &box);

    //
    // The render method does not need to be overridden by the sub-class.
    virtual void	 render();

protected:
    // Data which is unique per iso-procedural
    // Member data is:
    //	myBox		- The bounding box without velocity motion blur
    //			  information applied.
    //	myVelBox	- The bounding box including motion blur vectors
    //	myLodScale	- An level of detail scale independent of the shading
    //			  quality of the instance.  This allows larger polygons
    //			  to be generated without sacrificing shading.
    UT_BoundingBox	 myBox;
    UT_BoundingBox	 myVelBox;
    float		 myLodScale;
};

#endif

