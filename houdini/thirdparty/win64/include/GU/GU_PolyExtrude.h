/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS: Poly Extrude code.
 *
 */
#ifndef __GU_PolyExtrude_h__
#define __GU_PolyExtrude_h__

#include "GU_API.h"
#include <UT/UT_Matrix4.h>
#include <UT/UT_Array.h>
#include <UT/UT_Vector3.h>
#include <GA/GA_Types.h>

class GA_Group;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GA_ElementWranglerCache;
class GEO_Face;
class GEO_PrimPoly;
class GU_Detail;
class gu_PolyExtrudeConsolidator;
template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;

enum GU_PolyExtrudeConsolidate
{
    GU_EXTRUDE_CONSOLIDATE_FRONT       = 0x01,	// consolidate front face.
    GU_EXTRUDE_CONSOLIDATE_BACK	       = 0x02,	// consolidate back face.
    GU_EXTRUDE_CONSOLIDATE_SIDE	       = 0x04,	// consolidate sides.
    GU_EXTRUDE_CONSOLIDATE_ALL         = 0x08,	// consolidate all faces 
						// together.
    GU_EXTRUDE_CONSOLIDATE_ALLPERPOINT = 0x10,	// consolidate all originating 
						// points together.
    GU_EXTRUDE_CONSOLIDATE_PERFACE     = (GU_EXTRUDE_CONSOLIDATE_FRONT |
				          GU_EXTRUDE_CONSOLIDATE_BACK  |
				          GU_EXTRUDE_CONSOLIDATE_SIDE)
};

enum GU_PolyExtrudeSymmetryAxis
{
    GU_EXTRUDE_SYMMETRY_AXIS_X,
    GU_EXTRUDE_SYMMETRY_AXIS_Y,
    GU_EXTRUDE_SYMMETRY_AXIS_NONE,
    GU_EXTRUDE_SYMMETRY_ROTTOLEAD
};

enum GU_PolyExtrudeKeepSharedPoints
{
    GU_EXTRUDE_KSP_NO = 0,
    GU_EXTRUDE_KSP_ISECT,
    GU_EXTRUDE_KSP_AVERAGE
};

class gu_PointTargetList;
class gu_LocalSpaceAlgorithm;

typedef void  (*GU_PolyExtrudeXformCallback)(int primnum, UT_Matrix4 &xform,
					     void *user_data);
typedef float (*GU_PolyExtrudeInsetCallback)(int primnum, int vtxnum, int ptnum,
					     void *user_data);
typedef int   (*GU_PolyExtrudeDivCallback)(int primnum, void *user_data);

class GU_API GU_PolyExtrudeParms
{
public:
    GU_PolyExtrudeParms(); 

    const GA_Group	*myGroup;


    GA_PrimitiveGroup	*myFrontGroup;
    GA_PrimitiveGroup	*myBackGroup;
    GA_PrimitiveGroup	*mySideGroup;

    GA_Group	*myOutputGroup;

    bool		 myOutputFront;
    bool		 myOutputBack;
    bool		 myOutputSide;

    bool		 myPreserveGroups;

    bool		 myRemoveZeroAreaSides;
    bool		 myRemoveSharedSides;

    unsigned int	 myConsolidatePoints;  // Bit flag for fuse options


    GU_PolyExtrudeSymmetryAxis	    mySymmetryAxis;

    GU_PolyExtrudeKeepSharedPoints  myKeepSharedPointsTogether;

    // This flag is used to orient faces extruded from edges based on the
    // neighbouring faces.
    bool		 myOrientEdgeFaces;

    // Floating point tolerance for consolidating and coincident face removal.
    float		 myTolerance;

    void		 setLocalXform(const UT_Matrix4 &mat, 
				       GU_PolyExtrudeXformCallback cb);
    const UT_Matrix4	&getLocalXform(int primnum) const;

    void		 setGlobalXform(const UT_Matrix4 &mat, 
				        GU_PolyExtrudeXformCallback cb);
    const UT_Matrix4	&getGlobalXform(int primnum) const;

    void		 setInset(float inset, GU_PolyExtrudeInsetCallback cb);
    float		 getInset(int primnum, int vtxnum, int ptnum) const;

    void		 setDivisions(int div, GU_PolyExtrudeDivCallback cb);
    int			 getDivisions(int primnum) const;

    // Returns true if either the bit passed is set in our myConsolidatePoints
    // bit field or if one of the 'All' options is set.
    bool		 isFusing(unsigned int bit) const;

    // Returns true if we are using the 'All Per-Point' method of fusing
    // and myKeepFacesTogether is true.
    bool		 isConsolidatePerPoint() const;

    // Returns true if we are using the 'All' or 'All Per-Point' method of 
    // fusing.
    bool		 isConsolidateAll() const;

    // Callback support for local variables
    GU_PolyExtrudeXformCallback	    myLocalXformCallback;
    GU_PolyExtrudeXformCallback	    myGlobalXformCallback;
    GU_PolyExtrudeInsetCallback	    myInsetCallback;
    GU_PolyExtrudeDivCallback	    myDivCallback;
    void			   *myCBUserData;

private:

    // These parameters can be changed by local variables and so must be
    // accessed through methods of this class.
    UT_Matrix4		 myLocalXform;
    UT_Matrix4		 myGlobalXform;
    mutable UT_Matrix4	 myTempLXform;	// For callback storage
    mutable UT_Matrix4	 myTempGXform;	// For callback storage
    int			 myDivisions;
    float		 myInset;

};


class GU_API GU_PolyExtrude
{
public:

    GU_PolyExtrude(GU_Detail *gdp);
    ~GU_PolyExtrude();

    // Performs the extrusion
    void	extrude(const GU_PolyExtrudeParms &parms);

    // Removes coincident polygons
    void	removeSharedSides(GA_PrimitiveGroup *primgrp, float tolerance);


    // Computes an edge normal
    bool	computeEdgeNormal(UT_Vector3 &nml, const GA_Edge &edge, 
				  bool cleanup);

    // Computes the local coordinate space of the polygon and returns
    // the change of basis matrix in mat.  If it fails, then false is 
    // returned, otherwise it returns true.  (Failure condition might
    // be that the polygon is degenerate.)
    static bool computeLocalSpace(const GEO_Face &face,
				  UT_Matrix4 &mat);
    bool	computeLocalSpace(const GA_Edge &edge, UT_Matrix4 &mat);

private:
    void	extrudePolys(const GU_PolyExtrudeParms &parms,
			     GA_ElementWranglerCache &wranglers);
    void	extrudeEdges(const GU_PolyExtrudeParms &parms,
			     GA_ElementWranglerCache &wranglers);
    bool	extrudeFace(const GEO_Face &face, gu_LocalSpaceAlgorithm &lsa,
			    const GU_PolyExtrudeParms &parms,
			    GA_PrimitiveGroup *sideprims,
			    const gu_PointTargetList &pt_target_list,
			    GA_ElementWranglerCache &wranglers);
    void	extrudeEdge(const GA_Edge &edge, gu_LocalSpaceAlgorithm &lsa,
			    const GU_PolyExtrudeParms &parms,
			    const gu_PointTargetList &pt_target_list,
			    GA_ElementWranglerCache &wranglers);

    void	buildRails(const GU_PolyExtrudeParms &parms,
			    UT_Array<GA_Offset> &ptlist0,
			    UT_Array<GA_Offset> &ptlist1,
			    GA_Offset pt0, GA_Offset pt1,
			    GA_Offset frontpt0, GA_Offset frontpt1,
			    const UT_Vector3 &extrudedir0,
			    const UT_Vector3 &extrudedir1,
			    float t, bool regenlist0, int divs,
			    GA_ElementWranglerCache &wranglers);

    // Returns a new point based on the myConsolidatePoints option.
    // If likept is null, then we won't copy attributes
    GA_Offset   newPoint(const GU_PolyExtrudeParms &parms,
			 const UT_Vector3 &atpos,
			 GA_Offset likept,
			 GA_ElementWranglerCache &wranglers);

    GEO_PrimPoly *buildFrontFace(const GEO_Face &face,
				gu_LocalSpaceAlgorithm &lsa,
				const GU_PolyExtrudeParms &parms,
			        const gu_PointTargetList &pt_target_list,
				GA_ElementWranglerCache &wranglers);
    void	 buildSideFaces(const GEO_Face &face,
				const GEO_Face &newface,
			        const GU_PolyExtrudeParms &parms,
			        const gu_PointTargetList &pt_target_list,
				UT_Array<GA_Offset> &new_sides,
				GA_ElementWranglerCache &wranglers);

    GA_Offset    getEdgePoint(int ptnum, int div, float t,
			      const UT_Vector3 &extrudedir,
			      const GU_PolyExtrudeParms &parms,
			      const gu_PointTargetList &pt_target_list,
			      GA_ElementWranglerCache &wranglers);


    GU_Detail	*myGdp;
    bool	 myInternalNormals;

    gu_PolyExtrudeConsolidator *myNewPointsConsolidator;
};

#endif
