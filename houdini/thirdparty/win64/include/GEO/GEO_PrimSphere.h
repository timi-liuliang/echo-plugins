/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_PrimSphere_H__
#define __GEO_PrimSphere_H__

#include "GEO_API.h"
#include <UT/UT_Vector3.h>
#include "GEO_Quadric.h"
#include "GEO_PrimType.h"
#include "GEO_Vertex.h"

#define GEO_SPHERE_XSECT_XP	0
#define GEO_SPHERE_XSECT_XN	1
#define GEO_SPHERE_XSECT_YP	2
#define GEO_SPHERE_XSECT_YN	3
#define GEO_SPHERE_XSECT_ZP	4
#define GEO_SPHERE_XSECT_ZN	5

#define GEO_SPHERE_GUIDEEDGE_X	0
#define GEO_SPHERE_GUIDEEDGE_Y	1
#define GEO_SPHERE_GUIDEEDGE_Z	2

class GEO_API GEO_PrimSphere : public GEO_Quadric
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimSphere(GEO_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Quadric(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimSphere() {}

public:
    virtual int		 evaluateNormalVector(UT_Vector3 &nml, float u,
					float v = 0, float w = 0) const;

    virtual int		getBBox(UT_BoundingBox *bbox) const;
    virtual void	addToBSphere(UT_BoundingSphere *bsphere) const;

    virtual bool        hasGuideEdge(int edgeid, UT_Vector3 &a,
                                                 UT_Vector3 &b) const;
    virtual bool        hasXsectPoint(int pointid, UT_Vector3 &p) const;

    virtual fpreal	 calcVolume(const UT_Vector3 &refpt) const;
    virtual fpreal	 calcArea() const;
    virtual fpreal	 calcPerimeter() const;

    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// Builds a sphere primitive of the specified type, with either the
    /// specified point offset, or a new point if the provided offset
    /// is invalid.
    static GEO_PrimSphere *build(
        GA_Detail &detail, GA_Offset existing_ptoff = GA_INVALID_OFFSET)
    {
        return UTverify_cast<GEO_PrimSphere *>(
            GEO_Quadric::build(detail, GA_PRIMSPHERE, existing_ptoff));
    }

    static GA_Offset buildBlock(
        GA_Detail &detail, GA_Size nspheres, bool append_points = true)
    {
        return GEO_Quadric::buildBlock(
            detail, GA_PRIMSPHERE, nspheres, append_points);
    }

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_Quadric::buildFamilyMask(); }

    /// All subclasses should call this method to register the sphere
    /// intrinsics.
    /// @see GA_AttributeIntrinsic
    static GA_IntrinsicManager::Registrar
		registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_Quadric::registerIntrinsics(defn); }

    // Evaluate the position or the derivative at domain point (u,v), where
    // u and v MUST be in [0,1]. "v" and "dv" will be ignored when dealing
    // with one-dimensional types such as circles and polygons. Return 0 if
    // OK and -1 otherwise.
    virtual bool	evaluatePointRefMap( GA_Offset result_vtx,
					GA_AttributeRefMap &hlist,
					fpreal u, fpreal v,
					unsigned du, unsigned dv) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const;

private:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimSphere &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
};
#endif
