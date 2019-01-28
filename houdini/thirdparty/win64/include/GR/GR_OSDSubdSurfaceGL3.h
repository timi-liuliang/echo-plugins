/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_OSDSubdSurfaceGL3.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Class for rendering OSD subdivision surface using GL3.
 */
#ifndef GR_OSDSubdSurfaceGL3_h
#define GR_OSDSubdSurfaceGL3_h

#include "GR_Defines.h"
#include "GR_PolySurfaceGL3.h"

#include <UT/UT_UniquePtr.h>

class GT_UtilOpenSubdiv;

class GR_API GR_OSDSubdSurfaceGL3 : public GR_PolySurfaceGL3
{
public:
	     GR_OSDSubdSurfaceGL3(GR_Primitive *prim);
    virtual ~GR_OSDSubdSurfaceGL3();

    virtual const char *className() const { return "GR_OSDSubdSurfaceGL3"; }

    virtual void	 update(RE_Render *r,
				const GT_PrimitiveHandle &primh,
				const GR_UpdateParms &p,
				const char *cache_name);

    bool	isReducedLOD() const { return myReducedLOD; }
    int		getSubdivisionLevel() const { return mySubdLevel; }

    void	resetSubdivision();
    
private:
    UT_UniquePtr<GT_UtilOpenSubdiv>	     myOsd;
    GT_PrimitiveHandle			     mySurface;
    int64	myVertexAttributeDataIdHash;
    bool	myReducedLOD;
    int		mySubdLevel;       
};

#endif

