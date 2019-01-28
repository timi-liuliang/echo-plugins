/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This defines the detail of domain profiles.
 */

#ifndef __GEO_Profiles_h__
#define __GEO_Profiles_h__

#include "GEO_API.h"
#include <GD/GD_Detail.h>
#include <GD/GD_TrimLoop.h>
#include <UT/UT_BoundingBox.h>

class GA_Basis;
class GA_PrimitiveGroup;
class GD_Face;
class GD_TrimLoop;
class GEO_Face;
class GEO_TPSurf;
class GEO_Detail;
class UT_MemoryCounter;

class GEO_API GEO_Profiles : public GD_Detail
{
    friend class GEO_TPSurf;

public:
    	     GEO_Profiles(GEO_TPSurf &tpsurf);
    virtual ~GEO_Profiles();

    virtual GA_Detail	*cloneEmptyDetail(bool clone_attributes) const;

    // Take a GEO face and map it parametrically onto our surface. If the
    // bbox pointer is zero, we compute the face's bbox. Otherwise, we use
    // the given bbox instead.
    GD_Face		*map(const GEO_Face &face, UT_Axis3::axis uaxis,
			     UT_Axis3::axis vaxis, float umin, float umax,
			     float vmin, float vmax, int unif,
			     UT_BoundingBox *bbox = 0);

    // Take a GEO face and map it parametrically onto our surface without
    // constraining it to a uvmin/uvmax range.
    GD_Face		*map(const GEO_Face &face,
			     UT_Axis3::axis uaxis = UT_Axis3::XAXIS,
			     UT_Axis3::axis vaxis = UT_Axis3::YAXIS);

    // Map a whole group of primitives onto the surface. The method filters
    // out the non face types and returns the number of faces mapped (0 or
    // more). If problems occur, the value returned is -1. If gdp is 0, we
    // assume it's the gdp our surface lives in.
    int			 map(const GA_PrimitiveGroup &faces,
			     UT_Axis3::axis uaxis,
			     UT_Axis3::axis vaxis, float umin, float umax,
			     float vmin, float vmax, int unif,
			     const GEO_Detail *gdp = 0);

    // Remap an existing curve on surface so that it translates and/or 
    // scales itself in the domain of the surface. Return 0 if OK and -1
    // otherwise.
    int			remap(GD_Primitive &profile, float umin, float umax,
			      float vmin, float vmax, int unif)
			{
			    return remap(profile.getMapIndex(),umin, umax,
					 vmin, vmax, unif);
			}
    int			remap(int profileidx, float umin, float umax,
			      float vmin, float vmax, int unif);

    //
    // Reverse the faces and the trim regions:
    void	 	 reverseU(void);
    void	 	 reverseV(void);

    // Return the bounds of the valid interval of the surface we're defined
    // for.
    virtual void	 getDomainBBox(float &u0, float &v0,
				       float &u1, float &v1) const;

    GD_TrimLoop		*getTrimLoops(GD_TrimRule rule);

    int			 getMaxHeight();
    int			 getBaseDir();

    const GEO_TPSurf	&getSurf() const { return mySurf; }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += GD_Detail::getMemoryUsage(false);
        return mem;
    }

    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

private:
    // The surface we belong to:
    GEO_TPSurf		&mySurf;

    int			 getUVRange(float umin, float umax, float vmin,
				    float vmax, int unif, float &uka,
				    float &ukb, float &vka, float &vkb,
				    GA_Basis *&ubunif, GA_Basis *&vbunif,
				    GA_Basis *&uchrd, GA_Basis *&vchrd) const;

    GD_Face		*doMap(const GEO_Face &face,
				UT_Axis3::axis uaxis, UT_Axis3::axis vaxis,
				float uka, float ulen, float vka, float vlen,
				int unif, GA_Basis *ubunif,  GA_Basis *vbunif,
				GA_Basis *ubchord, GA_Basis *vbchord,
				UT_BoundingBox *bbox);

    void		 doMap(GD_Face &face,
				float uka, float ulen, float vka, float vlen,
				int unif, GA_Basis *ubunif,  GA_Basis *vbunif,
				GA_Basis *ubchord, GA_Basis *vbchord);

    /// Delete the primitive at the given offset.  Optionally, delete the points
    /// referenced by the primitive.
    virtual bool	 destroyPrimitiveOffset(GA_Offset offset,
						bool and_points = false);

    /// Destroy the given primitives.  Optionally destroy any referenced points
    /// that will no longer be in use after this operation.  Note that this
    /// option requires full topology.
    virtual GA_Size	 destroyPrimitiveOffsets(const GA_Range &it,
						bool and_points = false);

    //  Friends:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_Profiles &d)
			{
			    d.save(os, 0, 0);
			    return os;
			}
};

#endif
