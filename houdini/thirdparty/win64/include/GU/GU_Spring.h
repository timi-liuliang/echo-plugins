/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GU_Spring_h__
#define __GU_Spring_h__

#include "GU_API.h"
#include <UT/UT_ValArray.h>
#include <GEO/GEO_Detail.h>
#include "GU_Topology.h"

class GU_SpringEdge;

class GU_API GU_Spring 
{
public:

	     GU_Spring (void);
    virtual ~GU_Spring (void);

    void	setupEdges	(GU_Detail *gdp, GA_PrimitiveGroup *group = 0,
				 int force = 0);
    void	computeEdges	(GU_Detail *gdp, int behavior, 
				 float tension,
				 const GA_ROAttributeRef &tensionoffset,
				 const GA_ROAttributeRef &veloffset,
				 float timeinc);
    void	getForce   	(UT_Vector3& force, GA_Offset ptoff, GA_Index ptind,
				 float springk,
				 const GA_ROHandleF &springkattrib);

private:

    UT_ValArray<GU_SpringEdge*>	 	edges;
    GU_Topology				topology;

    void	cleanEdges	(void);
    void	buildEdges	(GEO_Primitive *prim);

};

#endif
