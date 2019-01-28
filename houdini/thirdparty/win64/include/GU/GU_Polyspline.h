#ifndef __GU_Polyspline_h__
#define __GU_Polyspline_h__

#include "GU_API.h"
enum GU_PolysplineBasisType
{
    GU_SPLINE_BASIS_BEZIER	= 0,
    GU_SPLINE_BASIS_SBEZIER	= 1,
    GU_SPLINE_BASIS_C1BEZIER	= 2,
    GU_SPLINE_BASIS_DEGREE2	= 3,
    GU_SPLINE_BASIS_BSPLINE	= 4,
    GU_SPLINE_BASIS_CARDINAL	= 5,
    GU_SPLINE_BASIS_LINEAR	= 6
};

enum GU_PolysplineDivideType
{
    GU_SPLINE_DIVIDE_STANDARD	= 0,
    GU_SPLINE_DIVIDE_EVENLEN	= 1,
    GU_SPLINE_DIVIDE_EVENX	= 2,
    GU_SPLINE_DIVIDE_EVENY	= 3,
    GU_SPLINE_DIVIDE_EVENZ	= 4
};

enum GU_PolysplineClosureType
{
    GU_SPLINE_CLOSURE_NONE	= 0,
    GU_SPLINE_CLOSURE_ALWAYS	= 1,
    GU_SPLINE_CLOSURE_IFPOLY	= 2
};


class GU_Detail;

class GU_API GU_PolysplineParms
{
public:

    GU_PolysplineBasisType	 basis;
    GU_PolysplineDivideType	 divide;
    GU_PolysplineClosureType	 closure;

    int				 edgeDivs;
    int				 polyDivs;
    float			 segSize;

    int				 first;
    int				 last;
    float			 tension;

};

#endif

