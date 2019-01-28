/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: 	GOP_Guide.h	
 *
 * COMMENTS:	The guide geometry and how to draw it.
 *
 */

#ifndef __GOP_Guide_h_
#define __GOP_Guide_h_

#include "GU_API.h"
#include "GU_Detail.h"

enum GOP_GuideType
{
    OP_GUIDE_ONE,
    OP_GUIDE_TWO
};

enum GOP_GuideListType
{
    OP_GUIDELIST_GOPGUIDE,
    OP_GUIDELIST_GUDETAIL
};

class GA_PointGroup;

//  This is a class which handles the drawing of guide geometry

class GU_API GOP_Guide : public GU_Detail
{
public:
    GOP_Guide(GOP_GuideType intype = OP_GUIDE_ONE);
    GOP_Guide(const GOP_Guide &source);
    virtual ~GOP_Guide();

    void		displayGeometry  (short f)  { drawGeometry = f;   }
    void		displayFont      (short f)  { drawAsFont   = f;   }
    void		displayHulls     (short f)  { drawHulls	   = f;   }
    void		displayText   	 (short f)  { drawText	   = f;   }
    void		displayPrimNums  (short f)  { drawPrimNums = f;   }
    void		displayAsPoints  (short f)  { drawAsPoints = f;   }
    
    /// Display this guide as shaded when in viewport shaded modes.
    void		displayShaded    (bool  b)  { myDrawShaded = b;   }
    /// Always display this guide as shaded, regardless of viewport shading mode
    void		forceShaded	 (bool  b)  { myForceShaded = b;  }
    /// Draw the guide so that it can be seen through solid objects as wireframe
    void		drawXRay	 (bool  b)  { myDrawXRay = b;	  }

    int			displayGeometry	 () const { return drawGeometry;  }
    int			displayFont	 () const { return drawAsFont;    }
    int			displayHulls     () const { return drawHulls;     }
    int			displayText   	 () const { return drawText;	  }
    int			displayPrimNums	 () const { return drawPrimNums;  }
    int			displayAsPoints  () const { return drawAsPoints;  }
    bool		displayShaded    () const { return myDrawShaded;  }
    bool		forceShaded	 () const { return myForceShaded; }
    bool		drawXRay	 () const { return myDrawXRay;    }

    GOP_GuideType	getType		 () const { return myType; 	  }

    void		buildVector(const UT_Vector3 &origin,
				    const UT_Vector3 &direction,
				    const GU_Detail *boxgdp);

    void		buildCurve(const GEO_Primitive *prim,
				   GA_PointGroup *guidepts,
				   int doU, float depth,
				   float width1, float width2);

    void		buildDomain(const GU_Detail *srcgdp,
				    const GA_PrimitiveGroup *srcgroup,
				    const GEO_Primitive *auxprim,
				    int first_group,
				    GA_PointGroup *guidepts,
				    int doU, int loop, float ldepth,
				    float lwidth1, float lwidth2,
				    float rdepth, float rwidth1,
				    float rwidth2);

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += GU_Detail::getMemoryUsage(false);
        return mem;
    }

private:
    GOP_GuideType		 myType;

    unsigned int		 drawGeometry	: 1,
				 drawAsFont	: 1,
				 drawHulls	: 1,
				 drawText	: 1,
				 drawPrimNums	: 1,
				 drawAsPoints	: 1,
				 myDrawShaded   : 1,
				 myForceShaded  : 1,
				 myDrawXRay	: 1;
};

#endif
