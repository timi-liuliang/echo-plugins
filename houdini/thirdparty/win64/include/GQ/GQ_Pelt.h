/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Pelt GU
 */


#ifndef __GQ_Pelt_h__
#define __GQ_Pelt_h__

#include "GQ_API.h"
#include <GU/GU_Detail.h>
#include <GA/GA_Edge.h>
#include <GA/GA_EdgeGroup.h>

enum
{
    GQ_PELT_XSOURCE = 0,
    GQ_PELT_YSOURCE,
    GQ_PELT_ZSOURCE,
    GQ_PELT_USOURCE,
    GQ_PELT_VSOURCE
};

class GQ_API GQ_Pelt
{
public:
    class GQ_API FramePoint
    {
    public:
        float coord[2];
	float connectedcoord[2];
    };

    static void findAllConnectedPrimitives(GU_Detail *gdp,
					   GEO_Primitive *seed,
					   GA_PointGroup *visited_points,
					   GA_PrimitiveGroup *visited_prims,
					   GA_EdgeGroup *boundary_edges);
    static void getEdges(GU_Detail *gdp, const GA_EdgeGroup *group,
			 UT_IntArray &selections);
    static void putEdges(const UT_IntArray &selections, GU_Detail *gdp, GA_EdgeGroup *group);

    static void  getPrimitives(GU_Detail *gdp, UT_IntArray &data);
    static void  putPrimitives(const UT_IntArray &data, GU_Detail *gdp);

    static void  determineCutSection(GU_Detail *gdp,
	                             const GA_EdgeGroup *cuts,
	                             GEO_Primitive *hint_prim,
				     GA_PointGroup *connected_points,
				     GA_PrimitiveGroup *connected_prims,
				     GA_PointGroup *boundary_points);

    static FramePoint *createFrame(const GEO_Face *face,
				   const GU_Detail *gdp,
				   const GA_PointGroup *boundary_points,
				   int ufrom,
				   int vfrom,
				   float orientation,
				   const char *density_attribute);
    static void destroyFrame(FramePoint *frame);

    static void remapCoord(FramePoint *frame, int num_points,
			   float minuvalue, float maxuvalue,
			   float minvvalue, float maxvvalue);

    static void updateUVs(GU_Detail *gdp, const GA_PointGroup *connected_points,
			  FramePoint *frame,
		          const GA_PrimitiveGroup *connected_prims,
			  const GA_PointGroup *boundary_points, bool do_vertex,
			  float springconstant, int maxiterations,
			  bool (*callback_func)(void *),
			  void *callback_data, const char *tension_attribute);

};

#endif
