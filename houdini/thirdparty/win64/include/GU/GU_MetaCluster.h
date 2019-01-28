/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_MetaCluster.h ( GU Library, C++)
 *
 * COMMENTS:	Generates clusters of metaballs for better conversion
 *		to polygons
 */

#ifndef __GU_MetaCluster__
#define __GU_MetaCluster__

#include "GU_API.h"
class gu_MetaCluster;

class GU_API GU_MetaClusterParms
{
public:
     GU_MetaClusterParms(const char *group_prefix = "mcluster",
			 int internal_groups = 1);
    ~GU_MetaClusterParms();

    void	setGroupPrefix(const char *pfix) { myGroupPrefix = pfix; }
    void	createInternalGroups(int onoff)	 { myInternalFlag = onoff; }

    // Generate clusters of metaballs (i.e. mutually exclusive sets)
    void	makeClusters(GU_Detail *gdp, GA_PrimitiveGroup *group = 0);

    // After the clusters have been made, the following methods can be called.
    int				 getNClusters()	{ return (int)myClusters.entries(); }
    GA_PrimitiveGroup		*getCluster(int i);
    const UT_BoundingBox	&getClusterBox(int i);

private:
    void				 clearClusters();

    gu_MetaCluster			*myRoot;
    const char				*myGroupPrefix;
    int					 myInternalFlag;
    UT_ValArray<gu_MetaCluster *>	 myClusters;
};

#endif

