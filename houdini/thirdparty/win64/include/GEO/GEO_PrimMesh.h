/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class is a subclass of the GEO_Hull class.  It represents
 *	a linear patch (i.e. non-interpolating)
 *
 */

#ifndef __GEO_PrimMesh_h__
#define __GEO_PrimMesh_h__

#include "GEO_API.h"
#include "GEO_Hull.h"
#include "GEO_PrimType.h"

class GA_Detail;

class GEO_API GEO_PrimMesh : public GEO_Hull
{
protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimMesh(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Hull(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    virtual ~GEO_PrimMesh() {}

public:
    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    virtual int		evaluateBreakpoint(int uidx, int vidx,
					   UT_Vector4 &pos,
					   int du=0, int dv=0) const;

    // Raise the number of CVs to match the newcount. The shape of the face
    // (especially if parametric) should NOT change. Return 0 upon success
    // and -1 otherwise.  start and stop define which indices to examine
    // if newcount is negative it is taken as a relative value.
    virtual int         loftU(int newcount, int start=-1, int stop=-1);
    virtual int         loftV(int newcount, int start=-1, int stop=-1);

    // Warp the mesh at u,v by the given delta. Change 1 or 4 Cvs and possibly
    // insert a knot once or more as well. If a knot is inserted or we happen
    // to land exactly on a knot, we change only one CV. The bias makes sense
    // only when changing 4 CVs, and will be ignored altogether if < 0.
    // We return the CV index in warpU/V and 0 in warp() if OK; -1 otherwise.
    virtual int		 warpU(float u, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float sharpness = 0.0f, float bias = -1.0f);
    virtual int		 warpV(float v, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float sharpness = 0.0f, float bias = -1.0f);
    virtual int		 warp (float u, float v, const UT_Vector3 &delta,
			       GA_AttributeRefMap &map,
			       float usharpness = 0.f, float vsharpness = 0.f,
			       float ubias = -1.0f, float vbias = -1.0f);

    // Append another hull to us in one of two ways: blend the two endpoints
    // or connect them straight or rounded. The bias ranges from 0 to 1 and is
    // relevant only to blending. The tolerance for blending: if 0, the two
    // endpoints will merge into one point with a discontinuity; if less than
    // 1, we insert knots into the hulls to minimize the affected areas; if 1,
    // no refinement is done. For the non-blend case, the tolerance will
    // generate a span whose shape goes from round to straight; 0 tolerance
    // means straight connection. If unrefine is on, we'll try to reduce the
    // complexity of the hull if we're connecting rounded. We return 0 if OK
    // and -1 if error. Both hulls must be open and have the same order.
    virtual int		attachU(const GEO_Hull &hull, int blend = 1,
			        float bias = 0.5f, float tolerance = 1.0f,
			        int= 1,GA_PointGroup *ptgroup=0);
    virtual int		attachV(const GEO_Hull &hull, int blend = 1,
			        float bias = 0.5f, float tolerance = 1.0f,
			        int= 1,GA_PointGroup *ptgroup=0);

    virtual int		unrollU(int append_pts = 1);
    virtual int		unrollV(int append_pts = 1);

    virtual const GA_PrimitiveJSON	*getJSON() const;

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_Hull::buildFamilyMask(); }

    /// All subclasses should call this method to register the mesh intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_Hull::registerIntrinsics(defn); }

    virtual bool	savePrivateH9(std::ostream &, bool binary) const;
    virtual bool	loadPrivateH9(UT_IStream &);
    virtual bool	saveExtraH9  (std::ostream &, bool binary) const;
    virtual bool	loadExtraH9  (UT_IStream &);

    // Check the validity of the data. Meant to be called especially at loading
    // time. The method returns 1 if OK and 0 if trouble.
    virtual bool	validate(void) const;

    // Return the squared max distance between any two successive vertices
    // and the index of the "left" vertex. If the polygon is closed we
    // compare the 1st and last vertices as well and if their distance is the
    // largest, we return the index of the last vertex.
    int			findMaxDistanceU(float &maxdist2, 
					 int start, int stop) const;

    int			findMaxDistanceV(float &maxdist2, 
					 int start, int stop) const;
    
    // Evaluate the position or the derivative at unit domain point
    // (u,v), where u and v MUST be in [0,1]. Return 0 if OK and -1 otherwise.
    // The normal is not normalized.
    virtual bool	 evaluatePointRefMap( GA_Offset result,
					GA_AttributeRefMap &map,
					fpreal u, fpreal v,
					unsigned du=0, unsigned dv=0) const;
    virtual int		 evaluatePointV4( UT_Vector4 &pos, float u, float v = 0,
					unsigned du=0, unsigned dv=0) const;
 
    // Refine for all spans with specified number of divisions per span.
    virtual void	spanRefineURefMap(GA_AttributeRefMap &map,
				    int numdivs=1);
    virtual void        spanRefineUInt(int numdivs=1);
    virtual void        spanRefineVRefMap(GA_AttributeRefMap &map,
				    int numdivs=1);
    virtual void        spanRefineVInt(int numdivs=1);

private:
    friend std::ostream	&operator<<(std::ostream &os, const GEO_PrimMesh &d)
			{
			    d.saveH9(os, 0,
				     GEO_Primitive::theEmptySaveAttribs,
				     GEO_Primitive::theEmptySaveAttribs);
			    return os;
			}
};
#endif
