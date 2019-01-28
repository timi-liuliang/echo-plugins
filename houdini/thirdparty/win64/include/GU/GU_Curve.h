/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Curve.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_Curve.h class...
 * 
 */

#ifndef __GU_Curve_h__
#define __GU_Curve_h__

#include "GU_API.h"
#include <GEO/GEO_Curve.h>

class GA_ElementWranglerCache;
class GEO_ConvertMethod;
class GEO_ConvertParms;
class GEO_Hull;
class GU_PrimPoly;
class GU_Detail;
class GU_RayInfoHit;

/////////////////////////////////////////////////////////////
//                                                         //
// Class:     GU_Curve					   //
// Descr:     This class is responsible for general type   //
//       of operations on the spline    curve classes.     //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_Curve
{
public:
    // Takes the curve and converts it to a polygon with vertices at the
    // Greville points of the curve and places the polygon in the given
    // destination detail.  If no detail is given place in the curves
    // parent detail.
    static GU_PrimPoly *makeGrevillePoly(const GEO_Curve &curve, GU_Detail *dest);

    // Returns number of hits appended, or minus one if error.
    static int           intersect(const GEO_Curve &curve, const GEO_Primitive &prim,
				    UT_Array<GU_RayInfoHit> &hitList,
				    float tol = 0.01F, int ignoretrim = 1);

    static GEO_Primitive *convert(GEO_Curve *curve, GEO_ConvertParms &parms, GA_PointGroup *usedpts);
    static GEO_Primitive *convertNew(GEO_Curve *curve, GEO_ConvertParms &parms);
    static GEO_Hull	*convertToSurfNew(GEO_Curve *curve, GEO_ConvertParms &parms);
    static GEO_Primitive *convertToPoly(GEO_Curve *curve, GEO_ConvertParms &parms);
    static GEO_Hull	*convertToTrim(GEO_Curve *curve, GEO_ConvertParms &parms);
    static GEO_Hull	*convertToPotatoChip(GEO_Curve *curve, GA_ElementWranglerCache &w);

    // Build the vextex list, and optionally append points to the detail.
    // Return 0 if OK, and -1 if error.
    static bool         create(GEO_Curve &curve, GA_Size nelems, bool closed,
					      bool appendPoints = true);

    // Return the start and and knots for a full uniform evaluation of the
    // curve.
    static void		 evaluationInterval(const GEO_Curve &curve, float lod,
				 float &ustart, float &ustop, int &nu);

    /// Fill with values within the valid interval.
    /// Returns true if OK and false otherwise.
    static bool          evaluateCurve(const GEO_Curve &curve,
                                       GEO_ConvertMethod &method,
                                       UT_Vector4Array &pos);

private:
    // We don't want anybody to instantiate a GU_Curve.
    GU_Curve() {}
};

#endif
