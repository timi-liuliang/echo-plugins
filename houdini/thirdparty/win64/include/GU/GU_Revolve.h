#ifndef __GU_Revolve_h__
#define __GU_Revolve_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include "GU_Cap.h"

enum GU_RevolveType
{
    GU_REVOLVE_CLOSED		= 0,
    GU_REVOLVE_OPEN_ARC		= 1,
    GU_REVOLVE_CLOSED_ARC	= 2
};

class GU_API GU_RevolveParms 
{
public:
    GU_RevolveParms();

    const GU_Detail 		*crossSection;
    GEO_SurfaceType  		 type;
    GU_CircleType 		 revType;
    float			 cx, cy, cz;
    float			 nx, ny, nz;
    float			 beginAngle;
    float			 endAngle;
    int				 divs;
    int				 order;
    int				 imperfect;
    int				 outputPolys;
    const GA_PrimitiveGroup	*crossGroup;		// cross section group
};

#endif
