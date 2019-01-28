/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO Library (C++)
 *
 * COMMENTS:  This class builds a useful structure for determining the 
 *	      connectivity of primitives.
 *             
 */

#ifndef __GEO_PrimConnector_h__
#define __GEO_PrimConnector_h__

#include "GEO_API.h"
#include "GEO_Detail.h"
#include <UT/UT_Array.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_IntArray.h>

class GEO_Primitive;

typedef UT_Array<GEO_Primitive *> geo_ConnectedPrimArray;

class GEO_PrimClassifier;

class GEO_API GEO_PrimConnector
{
public:
    GEO_PrimConnector(GEO_Detail &gdp,
		      const GEO_PrimClassifier &classifier);
    virtual ~GEO_PrimConnector();

    int  		 	 getTreeCount() const 
				 { return (int)myForest.entries(); }
    int				 getMaxTreeCount() const;

    // Query the prims list of tree i.
    const geo_ConnectedPrimArray	&connectedPrims(int i) const
		 { return myForest((unsigned)i)->myConnectedPrims; }

    // Clears the connections built
    virtual void		 clear();

    // This method returns the tree index for primitives connected to
    // the supplied primitive.  Returns -1 if we have not yet built the
    // tree for this connected group.
    int				 getPrimTree(const GEO_Primitive &prim);

    int				 buildTree(const GEO_Primitive &prim);

    // This method fills the trees array with all the indices of any
    // primitive trees which reference the given point. The return
    // value is the number of trees found, whether those trees are added
    // to the trees list or not (we don't add dupliactes). This method
    // will construct the trees if they do not already exist.
    virtual int			 getPrimTrees(GA_Offset ptoff,
					      UT_IntArray &trees) = 0;

    int64			 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myForest.getMemoryUsage(false);
        for (exint i = 0; i < myForest.entries(); i++)
        {
            if (myForest(i))
            {
                mem += sizeof(*myForest(i));
                mem += myForest(i)->myConnectedPrims.getMemoryUsage(false);
            }
        }
        mem += myVisitedPoints.getMemoryUsage(false);
        return mem;
    }

protected:
    int				 buildClassTree(int class_num);

    class GEO_API geo_ConnectedTree
    {
	public:
#ifdef INTEL_COMPILER
		geo_ConnectedTree() { }
#endif		
	    geo_ConnectedPrimArray	myConnectedPrims;
	private:
#ifdef INTEL_COMPILER		
		geo_ConnectedTree(const geo_ConnectedTree& ) { }
#endif
		
    };

    // Members:
    GEO_Detail				&myGdp;
    const GEO_PrimClassifier		*myClassifier;

    // The forest contains the lists of connected points (trees)
    UT_Array<geo_ConnectedTree *>	 myForest;
    UT_BitArray				 myVisitedPoints;
};

class GEO_PointClassifier;

class GEO_API GEO_PrimConnector3D : public GEO_PrimConnector
{
public:
    GEO_PrimConnector3D(GEO_Detail &gdp,
		        const GEO_PrimClassifier &classifier,
			const GEO_PointClassifier &ptclassifier);
    virtual ~GEO_PrimConnector3D();

    // This method fills the trees array with all the indices of any
    // primitive trees which reference the given point.  The return
    // value is the number of trees found.  This method will construct
    // the trees if they do not already exist.
    virtual int			 getPrimTrees(GA_Offset ptoff,
					      UT_IntArray &trees);

private:
    const GEO_PointClassifier		*myPointClassifier;
};

class GEO_API GEO_PrimConnectorUV : public GEO_PrimConnector
{
public:
    GEO_PrimConnectorUV(GEO_Detail &gdp,
		        const GEO_PrimClassifier &classifier);
    virtual ~GEO_PrimConnectorUV();

    // This method fills the trees array with all the indices of any
    // primitive trees which reference the given point.  The return
    // value is the number of trees found.  This method will construct
    // the trees if they do not already exist.
    virtual int			 getPrimTrees(GA_Offset ptoff,
					      UT_IntArray &trees);
};

#endif
