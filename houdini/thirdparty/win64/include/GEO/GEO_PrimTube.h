/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *   This is a cylinder.  There are two ways of storing a cylinder/tube:
 *	a) By setting two point references for the base and top
 *		of the cylinder.  In this method, the height of the
 *		cylinder is locked.  As well, the axes are fixed.
 *		This is the least preferred method since the X and Y
 *		axes are undefined (too many degrees of freedom).
 *	b) The preferred way is to specify a single point.  The tube
 *		will extend in the Z axis according to the height
 *		specified.  The tube may then be rotated or whatever
 *		and the X and Y axes will always be well defined.
 *
 */

#ifndef __GEO_PrimTube_H__
#define __GEO_PrimTube_H__

#include "GEO_API.h"
#include "GEO_Quadric.h"
#include "GEO_PrimType.h"
#include "GEO_Vertex.h"

#define GEO_TUBE_XSECT_BOT_XP	 0
#define GEO_TUBE_XSECT_BOT_XN	 1
#define GEO_TUBE_XSECT_BOT_ZP	 2
#define GEO_TUBE_XSECT_BOT_ZN	 3
#define GEO_TUBE_XSECT_MID_XP	 4
#define GEO_TUBE_XSECT_MID_XN	 5
#define GEO_TUBE_XSECT_MID_ZP	 6
#define GEO_TUBE_XSECT_MID_ZN	 7
#define GEO_TUBE_XSECT_TOP_XP	 8
#define GEO_TUBE_XSECT_TOP_XN	 9
#define GEO_TUBE_XSECT_TOP_ZP	 10
#define GEO_TUBE_XSECT_TOP_ZN	 11
#define GEO_TUBE_XSECT_BOT_XN2	 12
#define GEO_TUBE_XSECT_TOP_XN2	 13
#define GEO_TUBE_XSECT_BOT_XP2	 14
#define GEO_TUBE_XSECT_TOP_XP2	 15
#define GEO_TUBE_XSECT_BOT_ZN2	 16
#define GEO_TUBE_XSECT_TOP_ZN2	 17
#define GEO_TUBE_XSECT_BOT_ZP2	 18
#define GEO_TUBE_XSECT_TOP_ZP2	 19
#define GEO_TUBE_XSECT_BOT_CTR	 20
#define GEO_TUBE_XSECT_TOP_CTR	 21

#define GEO_TUBE_GUIDEEDGE_BX	 0
#define GEO_TUBE_GUIDEEDGE_BZ	 1
#define GEO_TUBE_GUIDEEDGE_MX	 2
#define GEO_TUBE_GUIDEEDGE_MZ	 3
#define GEO_TUBE_GUIDEEDGE_TX	 4
#define GEO_TUBE_GUIDEEDGE_TZ	 5
#define GEO_TUBE_GUIDEEDGE_XN	 6
#define GEO_TUBE_GUIDEEDGE_XP	 7
#define GEO_TUBE_GUIDEEDGE_ZN	 8
#define GEO_TUBE_GUIDEEDGE_ZP	 9
#define GEO_TUBE_GUIDEEDGE_Z	 10

class GEO_API GEO_PrimTubeFlags
{
public:
	GEO_PrimTubeFlags()
	{
	    endcaps	= 0;
	}
	GEO_PrimTubeFlags(const GEO_PrimTubeFlags &s)
	    : endcaps(s.endcaps)
	{
	}
	unsigned	endcaps:1;	// Tube has end caps
};

class GEO_API GEO_PrimTube : public GEO_Quadric
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimTube(GEO_Detail *d, GA_Offset offset = GA_INVALID_OFFSET);

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimTube() {}

public:
    virtual int		 evaluateNormalVector(UT_Vector3 &nml, float u,
					float v = 0, float w = 0) const;
    virtual int			 getBBox(UT_BoundingBox *bbox) const;
    virtual void		 addToBSphere(UT_BoundingSphere *bsphere) const;
    // virtual UT_Vector3		 baryCenter() const;
    virtual GEO_Primitive	*copy(int preserve_shared_pts = 0) const;
    virtual void		 copyPrimitive(const GEO_Primitive *src);

    virtual void	copyUnwiredForMerge(const GA_Primitive *src,
					    const GA_MergeMap &map);

    virtual bool         hasGuideEdge(int edgeid, UT_Vector3 &a,
                                                  UT_Vector3 &b) const;
    virtual bool         hasXsectPoint(int pointid, UT_Vector3 &p) const;

//
//  End cap handling for tube primitive
    unsigned		 endcaps(void) const { return flags.endcaps; }
    void		 setEndCaps(int onOff) { flags.endcaps = onOff; }
    float		 getTaper(void) const { return myTaper; }
    void		 setTaper(float t) { myTaper = t; }

//
//  Tube Capture Region handling
//

//  computes weight of point in this capture region tube.  If the point
//  is outside the region, the weight is zero.  Otherwise the weight is
//  .0 at the edge of the region, 1.0 along the center bone line of the
//  region.   
//  pos must be in tube's SOP space, i.e. you've multiplied by the 
//  inverse of getTransform4()
    float		 getCaptureRegionWeight( const UT_Vector3 &pos,
			     const GA_ROAttributeRef &tubeCaptAtrOffset) const;
    static float	 computeCaptureRegionWeight( const UT_Vector3 &pos,
					 float bcap, float tcap, float taperx,
					 float taperz, float min_weight,
					 float max_weight );

    virtual fpreal	 calcVolume(const UT_Vector3 &refpt) const;
    virtual fpreal	 calcArea() const;
    virtual fpreal	 calcPerimeter() const;

    virtual const GA_PrimitiveJSON	*getJSON() const;

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_Quadric::buildFamilyMask(); }

    GA_DECLARE_INTRINSICS(GA_NO_OVERRIDE)

    // Save and load the privates of this class.
    virtual bool 	 savePrivateH9(std::ostream &os, bool binary) const;
    virtual bool 	 loadPrivateH9(UT_IStream &is);

    // Evaluate the position or the derivative at domain point (u,v), where
    // u and v MUST be in [0,1]. "v" and "dv" will be ignored when dealing
    // with one-dimensional types such as circles and polygons. Return 0 if
    // OK and -1 otherwise.
    virtual bool	 evaluatePointRefMap( GA_Offset result_vtx,
				GA_AttributeRefMap &hlist,
				fpreal u, fpreal v, uint du, uint dv) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const;

private:
    GEO_PrimTubeFlags	 flags;
    // The taper is the radius of the bottom half of the default tube.
    // The top half's radius is one.  These radii are then modified by
    // myXform.
    float		 myTaper;

    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimTube &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
};
#endif
