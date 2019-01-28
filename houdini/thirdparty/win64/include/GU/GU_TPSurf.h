/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_TPSurf.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_TPSurf.h class...
 * 
 */

#ifndef __GU_TPSurf_h__
#define __GU_TPSurf_h__

#include "GU_API.h"
#include <GEO/GEO_SurfaceType.h>
#include <UT/UT_Axis.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector3Array.h>
#include <UT/UT_Vector4Array.h>

class GA_PointGroup;
class GA_PrimitiveGroup;
class GD_Primitive;
class GEO_ConvertMethod;
class GEO_ConvertParms;
class GEO_Curve;
class GEO_Face;
class GEO_Primitive;
class GEO_TPSurf;
class GU_PrimPoly;
class GU_RayInfo;
class GU_Detail;
class GU_IsectCurveSet;
class GU_RayIntersect;

class GU_API GU_ProjectParms
{
public:
    GU_ProjectParms();

    int			type;	// by vector (0) or by parametric (1) mapping

    UT_Vector3		axis;	// projection axis if projecting along vector
    int			fnorm;	// use face normal for projection axis
    int			side;	// closest (0), farthest (1) , or all sides (2)
    int			sdivs;	// number of divisions per 3D curve span
    float		rtol;	// ray intersection tolerance
    float		ftol;	// 2D tolerance for fitting
    float		uvgap;	// max % uv distance for joining profiles
    int			sharp;	// maintain sharpness when fitting
    int			order;	// 2D spline order for the fit
    int			ignoretrim;	// If true, ignores trimming info
    int			algebraic;	// If true, uses Algebraic Pruning.

    UT_Axis3::axis	uaxis;	// u axis for parametric mapping
    UT_Axis3::axis	vaxis;	// v axis for parametric mapping
    int			range;	// use the range below to map the curve into
    float		umin;	// 0 <= umin <= 1
    float		umax;	// 0 <= umax <= 1
    float		vmin;	// 0 <= vmin <= 1
    float		vmax;	// 0 <= vmax <= 1
    int			unif;	// uniform (1) or arc length (0) mapping
};

/////////////////////////////////////////////////////////////
//                                                         //
// Class:     GU_TPSurf					   //
// Descr:     This class is responsible for general type   //
//       of operations on nurbs and the surf  classes.     //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_TPSurf
{
public:
    static int   intersectRay(const GEO_TPSurf &surf,
                              const UT_Vector3 &o, const UT_Vector3 &d,
                              float tmax = 1E17F, float tol = 1E-12F,
                              float *distance = 0, UT_Vector3 *pos = 0,
                              UT_Vector3 *nml = 0, int accurate = 0,
                              float *u = 0, float *v = 0,
                              int ignoretrim = 1);
    static int   intersectSurf(GEO_TPSurf &surf1, GEO_TPSurf &surf2,
                               GU_IsectCurveSet &curveset,
                               float worldtol = 1e-4F,
                               float domaintol = 1e-2F,
                               int steps = 100,
                               bool docompact = true);
    static int   doesIntersect(const GEO_TPSurf &surf,const GEO_Primitive &prim,
                               float worldtol = 1e-4F, int nontrivial = 0);

    // Send a ray to see if it intersects the surface. The hitinfo should
    // contain all the needed info, such as tolerance and find farthest.
    // The hit info is updated if a hit is found, and 1 is returned. The
    // (u,v) intersection point is also set in hitinfo.
    static int   rayHit(const GEO_TPSurf &surf,
                        const UT_Vector3 &o, const UT_Vector3 &dir,
                        GU_RayInfo &hitinfo, GU_RayIntersect *intersect);

    /// Extract one or more (if not fully inside the domain) 3D curve(s) out of
    /// a profile's image onto the surface. Return the first 3D curve if
    /// successful and 0 otherwise. Only one (the first visible piece of the
    /// curve) will be extracted if only_first_visible_part is set.
    /// The second extract doeas a parametric extract, so the whole profile will
    /// be extracted regardless of its visibility. The resulting 3D curve will
    /// be a planar XY curve at the given z elevation.
    static GEO_Curve	*extract(const GEO_TPSurf &surf,
                                 GU_Detail *gdp,
                                 const GD_Primitive &profile, int smooth = 1,
				 int sdivs = 20, float tol = 0.001F,
				 int sharp = 1, int order = 4,
				 int only_first_visible_part = 0);
    static GEO_Face	*extract(const GEO_TPSurf &surf,
                                 GU_Detail *gdp,
                                 const GD_Primitive &profile, float z=0);

    // Project the face or the group of faces onto the surface to create a
    // curve on surface. Return 0 if OK and -1 otherwise.
    // If the projection type is set to parametric mapping, the group method
    // maps the faces in sequence rather than one on top of each other. Also,
    // if the gdp pointer is zero, we assume the faces live in our gdp.
    static int		 project(GEO_TPSurf &surf,
                                 const GEO_Face &f, const GU_ProjectParms &p,
                                 GU_RayIntersect *intersect = NULL);
    static int		 project(GEO_TPSurf &surf,
                                 const GA_PrimitiveGroup &faces,
				 const GU_ProjectParms &p, const GU_Detail *gdp = 0);

    // Finds the points on the surface that is minimums to the points on the 
    // face.  We will find the points so that the face is divided into 
    // "numdivs" segments.  Returns via the UT_Array.  Returns the actual
    // number of points in the domain.
    static int		 findMins(const GEO_TPSurf &surf,
                                  const GEO_Face &face, int numdivs,
				  UT_Vector3Array &domain,
				  const GEO_Face *dface = 0,
				  UT_Vector4Array *displaces = 0);
    // Projects points from the face to the surface in the direction of
    // projdir.  We will find the points so that the face is divided into 
    // "numdivs" segments.  Returns the (u,v) pairs of the domain via the 
    // UT_Array.  Returns the actual number of points added.
    static int		 findProjects(const GEO_TPSurf &surf,
                                      GEO_Face& face, int numdivs,
				      const UT_Vector3 &projdir,
				      UT_Vector3Array &domain,
				      const GEO_Face *dface = 0,
				      UT_Vector4Array *displaces = 0);

    // Fill the hull with row and column data. Return 0 if OK, and -1 if error.
    static int		 create(GEO_TPSurf *surf, int rows, int cols,
			       GEO_SurfaceType type, bool wrapu, bool wrapv,
			       bool appendPoints = true);

    // Return the start and and knots for a full uniform evaluation of the
    // mesh.
    static void		 evaluationInterval(const GEO_TPSurf &surf,
                                 float lodu, float lodv,
				 float &ustart, float &ustop, int &nu,
				 float &vstart, float &vstop, int &nv);

    // Fill with values within the valid uv interval. Return 0 if OK and -1
    // otherwise.
    static int		 evaluateSurface(const GEO_TPSurf &surf,
                                         GEO_ConvertMethod &method,
					 UT_Vector4Array &pos,
					 int &nu, int &nv,
					 UT_FloatArray *uval = 0,
					 UT_FloatArray *vval = 0);

    static GEO_Primitive *convert(GEO_TPSurf *surf, GEO_ConvertParms &parms, GA_PointGroup *usedpts);
    static GEO_Primitive *convertNew(GEO_TPSurf *surf, GEO_ConvertParms &parms);
    static GEO_Primitive *convertToPoly(GEO_TPSurf *surf, GEO_ConvertParms &parms);

private:
    // We don't want anybody to instantiate a GU_TPSurf.
    GU_TPSurf() {}

    // Helpers for curve-on-surface projections:
    static GU_PrimPoly	*projectOnePoint(const GEO_TPSurf &surf,
                               const UT_Vector3 &orig,
			       const UT_Vector3 &dir, float uvgap,
			       GU_RayInfo &hitinfo, GU_Detail &polygdp,
			       float &pdot, int &pclose, int &projcount,
			       GU_PrimPoly *&poly,
			       GU_PrimPoly *&pfirst, GU_PrimPoly *&nfirst,
			       const GEO_Face &face, int cridx, int lod,
			       int numdiv, GU_RayIntersect *intersect);
    static int		 projectOnSide(const GEO_TPSurf &surf,
                               const UT_Vector4Array &pos,
			       const UT_Vector3 &dir, const UT_Vector3 &ndir,
			       int myfar, float rtol, float uvgap, int cls,
			       GU_Detail &pgdp, const GEO_Face &face, int lod,
			       int algebraic, int ignoretrim,
                               GU_RayIntersect *intersect);
    static int		 projectOnAllSides(const GEO_TPSurf &surf,
                               const UT_Vector4Array &pos,
			       const UT_Vector3 &dir, const UT_Vector3 &ndir,
			       float rtol, float uvgap, int cls,
			       GU_Detail &pgdp, const GEO_Face &face, int lod,
			       int algebraic, int ignoretrim,
                               GU_RayIntersect *intersect);
    static int		 projectAlongMinimum(const GEO_TPSurf &surf,
                               const UT_Vector4Array &pos,
			       int closed, float uvgap,
			       GU_Detail &polygdp);
    static void		 processSideHit(const GEO_TPSurf &surf,
                               GU_PrimPoly *pinpoly,
			       GU_PrimPoly *&poutpoly, GU_PrimPoly *&pfirst,
			       GU_PrimPoly *ninpoly, GU_PrimPoly *&noutpoly,
			       GU_PrimPoly *&nfirst, float uvgap,
			       GU_Detail &polygdp, int &pstartidx,
			       int &nstartidx, int cridx);
    static void		 processEndHit(const GEO_TPSurf &surf,
                               GU_Detail &polygdp, GU_PrimPoly &poly,
			       GU_PrimPoly *first, float uvgap, int close);
    static void		 joinHits(const GEO_TPSurf &surf,
                               GU_PrimPoly &apoly, GU_PrimPoly &bpoly,
			       GU_PrimPoly *&pfirst, GU_PrimPoly *&nfirst);
    static int		 connectToEdge(const GEO_TPSurf &surf,
                               GU_PrimPoly *poly, GU_Detail &polygdp,
			       float uvgap, int isfirst);
    static int		 projectAddBisectPoints(const GEO_TPSurf &surf,
                               GU_PrimPoly *poly,
			       GU_Detail &polygdp, const GEO_Face &face,
			       int lod, int index, int isfirst,
			       const UT_Vector3 &dir, GU_RayInfo &hitinfo,
			       float uvgap, float pdot, int numdiv,
                               GU_RayIntersect *intersect);

    // Project Function Pointers
    typedef int (*GU_PFP)(const GEO_TPSurf &,
                          UT_Vector3 &, float&, float&, const UT_Vector3 &);

    // Functions as input for findMinOrProj
    static int		 minimumFunction(const GEO_TPSurf &surf,
                                         UT_Vector3 &from, float &u, float &v,
					 const UT_Vector3 &);
    static int		 projectFunction(const GEO_TPSurf &surf,
                                         UT_Vector3 &from, float &u, float &v,
					 const UT_Vector3 &projdir);
    // Finds the points on the surface that is minimums to the points on the
    // face.  We will find the points so that the face is divided into
    // "numdivs" segments.  Returns via the UT_Array.  Returns the actual
    // number of points in the domain.
    static int		 findMinsOrProj(const GEO_TPSurf &surf,
                                        GU_PFP fp, const GEO_Face &face,
					int numdivs, const UT_Vector3 &projdir,
					UT_Vector3Array &domain,
					const GEO_Face *dface = 0,
					UT_Vector4Array *displaces = 0);
};

#endif
