/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Grid.h	
 *
 * COMMENTS:
 *
 */

#ifndef __GU_GRID__
#define __GU_GRID__

#include "GU_API.h"
#include "GU_Detail.h"

class GU_API GU_GridParms
{
public:
    GU_GridParms();

    int				 rows, cols;
    float			 xsize, ysize;
    float			 xcenter, ycenter, zcenter;
    GU_OrientationType		 plane;
    GEO_SurfaceType		 type;
    int				 interpEndsU, interpEndsV;
    int				 orderU, orderV;
};

#endif
