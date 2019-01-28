/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Parameterization class for fitting (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GEO_Parameterization_H__
#define __GEO_Parameterization_H__

#include "GEO_API.h"
#include <GA/GA_Parameterization.h>

class GA_Detail;
class GA_OffsetMatrix;
typedef UT_VectorT<fpreal64> UT_Vector;

class GEO_API GEO_Parameterization : public GA_Parameterization
{
public:
    // Parameterize a mesh of geo data points [0..m][0..n]
    // according to the type of parameterization specified for u and v.
    static bool	doMesh(const GA_Detail &detail,
                       const GA_OffsetMatrix &dataMesh,
		       bool uWrapped, bool vWrapped,
		       GA_ParameterizationType uType,
		       GA_ParameterizationType vType,
		       UT_Vector &uParam, UT_Vector &vParam);
    static bool	doMeshU(const GA_Detail &detail,
                       const GA_OffsetMatrix &dataMesh,
		       bool uWrapped, GA_ParameterizationType uType,
		       UT_Vector &uParam);
    static bool	doMeshV(const GA_Detail &detail,
                       const GA_OffsetMatrix &dataMesh,
		       bool vWrapped, GA_ParameterizationType vType,
		       UT_Vector &vParam);
};

#endif
