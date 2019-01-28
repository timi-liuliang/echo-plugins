/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GU_Polypatch.h (C++)
 *
 * COMMENTS:
 * 
 */

#ifndef __GU_Polypatch_h__
#define __GU_Polypatch_h__

#include "GU_API.h"
#include <GEO/GEO_SurfaceType.h>

enum GU_PolypatchBasisType
{
    GU_POLYPATCH_BASIS_CARDINAL	= 0,
    GU_POLYPATCH_BASIS_BSPLINE	= 1
};

enum GU_PolypatchWrapType
{
    GU_POLYPATCH_WRAP_OFF	= 0,
    GU_POLYPATCH_WRAP_ON	= 1,
    GU_POLYPATCH_WRAP_IFPRIM	= 2
};

enum GU_PolypatchClampType
{
    GU_POLYPATCH_CLAMP_OFF	= 0,
    GU_POLYPATCH_CLAMP_ON	= 1,
    GU_POLYPATCH_CLAMP_IFPRIM	= 2
};

class GU_API GU_PolypatchParms
{
public:
    GU_PolypatchParms();
   ~GU_PolypatchParms();

    // Basis parameters
    GU_PolypatchBasisType	basis;			// Spline basis
    GEO_SurfaceType		connectivity;		// Patch connectivity

    // Wrapping parameters
    GU_PolypatchWrapType	isWrappedU;		// Wrapped in U?
    GU_PolypatchWrapType	isWrappedV;		// Wrapped in V?

    // Clamping parameters
    GU_PolypatchClampType	isClampedFirstU;	// First U clamped?
    GU_PolypatchClampType	isClampedLastU;		// Last U clamped?
    GU_PolypatchClampType	isClampedFirstV;	// First V clamped?
    GU_PolypatchClampType	isClampedLastV;		// Last V clamped?

    // Division parameters
    int				uDivisions;		// Div/spline seg in U
    int				vDivisions;		// Div/spline seg in V

    // Output parameters
    int				outputPolygons;		// Output polygons?
};

#endif
