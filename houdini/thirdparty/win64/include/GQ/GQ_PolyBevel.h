/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GQ_PolyBevel.h ( GQ Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __GQ_PolyBevel_h__
#define __GQ_PolyBevel_h__

#include "GQ_API.h"
#include "GQ_Detail.h"
#include <GEO/GEO_WorkVertexBuffer.h>
#include <GA/GA_AttributeRefMap.h>
#include <UT/UT_Map.h>

class GQ_API gq_BevelEdgeNode
{
public:
    GEO_PrimPoly	*myPoly;
    GA_Offset		 myPt1;
    GA_Offset		 myPt2;
    UT_Vector3		 myDir1;
    UT_Vector3		 myDir2;
    UT_Vector3		 myCorner1;
    UT_Vector3		 myCorner2;
};

SYS_DEPRECATED_PUSH_DISABLE()

class GQ_API GQ_PolyBevel
{
public:
    GQ_PolyBevel(GU_Detail *gdp, GA_VertexPool &pool, int relative,
		 float inset, int type, int repetitions);
    ~GQ_PolyBevel();

    GA_PrimitiveGroup	*pointBevel(GA_PointGroup *pointgroup);
    GA_PrimitiveGroup	*edgeBevel(GA_EdgeGroup *edgegroup);

protected:
    void	smoothBevel();
    
    // in GQ_BevelPoints.C:
    void	bevelPoints();
    void	cornerBevelPoints();

    // in GQ_BevelEdges.C:
    void	bevelEdges();

    // in GQ_SmoothBevel.C:
    void	smoothBevelEdges();
    void	smoothBevelPoints();
    

private:
    class GQ_API gq_BevelPointNode
    {
    public:
	GEO_PrimPoly	*myPoly;
	GEO_Vertex	*myVertex;
    };
    
    // in GQ_BevelPoints.C:
    void		 findBevelPoint(GA_Offset, float, GQ_Edge *,
			    GA_Offset &, bool &);
    void		 addCornerPoints(GA_Offset, GA_Offset,
			    UT_Array<GA_Offset> &, UT_Array
			    <GA_Offset> &, GEO_PrimPoly *);

    // in GQ_BevelEdges.C:
    void		 selectBevelPoints(GA_PointGroup *&);
    void		 findEdgePoint(GA_Offset, GQ_Edge *,
			    UT_Array<GA_Offset>&,
			    GA_Offset&, GA_Offset, bool &);
    void	 	 findBeforePoint(GA_Offset, GQ_Edge *,
			    UT_Array<GA_Offset> &,
			    GA_Offset &);
    void		 findAfterPoint(GA_Offset, GQ_Edge *,
			    UT_Array<GA_Offset> &,
			    GA_Offset &, GA_Offset);
    void		 startBevelFace(GQ_Edge *, UT_Array<GQ_Face *>&,
			    GA_Offset, GA_Offset);
    void		 finishBevelFace(GQ_Edge *, UT_Array<GQ_Face *>&,
			    GA_Offset, GA_Offset);
    GEO_PrimPoly	*createMidPoly(GA_Index, const UT_Array<GA_Offset> &);

    // in GQ_Smoothbevel.C:
    void		 addPerimeter(GQ_Face *,
			    UT_Array<GA_Offset> &, const UT_Vector3 &,
			    const GEO_Vertex &);
    void		 addMajorDivisions(GQ_Face *,
			    const UT_Array<GA_Offset> &,
			    UT_Array<GA_Offset> &, const UT_Vector3 &,
			    const GEO_Vertex &);
    void		 findMajorPoints(const UT_Vector3 &, float borderw,
			    UT_Array<GA_Offset> &, const UT_Vector3 &,
			    const UT_Plane &, const UT_Vector3 &,
			    const GEO_Vertex &, GQ_Edge *);
    void		 addMinorDivisions(GQ_Face *,
			    const UT_Array<GA_Offset> &,
			    const UT_Array<GA_Offset> &,
			    const UT_Vector3 &,	const GEO_Vertex &);
    void		 addFillPolys(GQ_Face *face, const
			    UT_Array<GA_Offset> &, const
			    UT_Array<GA_Offset> &);
    void 		 addPatchPolys(GQ_Face *face,
			    const UT_Array<GA_Offset> &);
    void		 addCornerPolys(const UT_Array<GQ_Face *> &,
			    GA_PrimitiveGroup &);
    void		 addEdgePolys(const UT_Array<GQ_Face *> &,
			    GA_PrimitiveGroup &,
			    const UT_Array<gq_BevelEdgeNode> &);
    UT_Array<GA_Offset>	 addEdgePoints(GQ_Edge *,
				    const UT_Vector3 &, const UT_Vector3 &,
				    const UT_Vector3 &);
    GEO_PrimPoly	*addTriangle(GA_Offset, GA_Offset, GA_Offset,
			    GEO_PrimPoly *);
    GEO_PrimPoly	*addRectangle(GA_Offset, GA_Offset, GA_Offset,
			    GA_Offset, GEO_PrimPoly *);

    gq_BevelPointNode	*getOldPoint(exint &index);
    
    using PolyMap = UT_Map<GEO_PrimPoly *, exint>;
    
    GU_Detail				*myGdp;
    GQ_Detail				*myGqd;
    int					 myRepetitions;
    float				 myInset;
    int					 myType;
    bool				 myRelative;
    GA_PrimitiveGroup			*myOutput;
    GA_PointGroup			*myPointGroup;
    GA_EdgeGroup			*myEdgeGroup;
    UT_Array<gq_BevelEdgeNode>		 myBevelList;
    UT_Array<gq_BevelPointNode *>	 myOldPoints;
    GEO_WorkVertexBuffer		 myVertexBuffer;
    PolyMap				*myBevelTable;
    PolyMap				*myPointTable;
    GA_PrimitiveGroup			*myNewOutput;
    UT_Array<GA_Offset>			 myAddedPoints;
    UT_Array<GA_Offset>			 myAddedPointsonFace;
};

SYS_DEPRECATED_POP_DISABLE()

#endif
