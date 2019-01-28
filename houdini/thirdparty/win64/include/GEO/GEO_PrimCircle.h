/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:	Circle (ellipse) primitive.
 *
 *
 */

#ifndef __GEO_PrimCircle_H__
#define __GEO_PrimCircle_H__

#include "GEO_API.h"
#include "GEO_PrimType.h"
#include "GEO_Quadric.h"
#include "GEO_Vertex.h"

#define GEO_CIRCLE_XSECT_XP	0
#define GEO_CIRCLE_XSECT_XN	1
#define GEO_CIRCLE_XSECT_YP	2
#define GEO_CIRCLE_XSECT_YN	3

#define GEO_CIRCLE_GUIDEEDGE_X	0
#define GEO_CIRCLE_GUIDEEDGE_Y	1

class GEO_Detail;

class GEO_API GEO_PrimCircle : public GEO_Quadric
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimCircle(GEO_Detail *d, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_Quadric(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimCircle() {}

public:
    // Methods inherited from the parent class:
    virtual int		 getBBox(UT_BoundingBox *bbox) const;
    virtual void	 addToBSphere(UT_BoundingSphere *bsphere) const;
    virtual UT_Vector3	 computeNormal() const;
    virtual UT_Vector3	 baryCenter() const;

    virtual bool        hasGuideEdge(int edgeid, UT_Vector3 &a,
						 UT_Vector3 &b) const;
    virtual bool        hasXsectPoint(int pointid, UT_Vector3 &p) const;

    virtual fpreal	calcVolume(const UT_Vector3 &refpt) const;
    virtual fpreal	calcArea() const;
    virtual fpreal	calcPerimeter() const;

    virtual const GA_PrimitiveJSON	*getJSON() const;

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_Quadric::buildFamilyMask(); }

    /// All subclasses should call this method to register the curve intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_Quadric::registerIntrinsics(defn); }

    // Evaluate the position or the derivative at domain point (u,v), where
    // u and v MUST be in [0,1]. "v" and "dv" will be ignored here. Return 0 if
    // OK and -1 otherwise.
    virtual bool	 evaluatePointRefMap(GA_Offset result_vtx,
					GA_AttributeRefMap &hlist,
					fpreal u_unit, fpreal,
					uint du, uint) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u_unit, 
					    float=0, unsigned du=0, 
					    unsigned = 0) const;

private:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimCircle &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
};
#endif
