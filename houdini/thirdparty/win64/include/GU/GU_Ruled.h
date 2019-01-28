#ifndef __GU_Ruled_h__
#define __GU_Ruled_h__

#include "GU_API.h"
#include <GEO/GEO_Primitive.h>
#include <GEO/GEO_Hull.h>
#include "GU_Cap.h"

class GA_PrimitiveGroup;

enum GU_RuledClosureType
{
    GU_RULED_CLOSE_NONE	= 0,
    GU_RULED_CLOSE	= 1,
    GU_RULED_CLOSE_AUTO	= 2
};

enum GU_RuledInterpType
{
    GU_RULED_INTERP_NONE	= 0,
    GU_RULED_INTERP	= 1,
    GU_RULED_INTERP_AUTO	= 2
};

class GU_API GU_RuledParms
{
public:
    GU_RuledParms(); 
   ~GU_RuledParms() {}

    GEO_SurfaceType	 type;
    GU_RuledClosureType	 closeU, closeV;
    GU_RuledInterpType	 interpU, interpV;
    int 		 orderV;
    int			 keepPrimitives;
    int			 outputPolys;
    GA_PrimitiveGroup	*primGroup;
};

#endif
