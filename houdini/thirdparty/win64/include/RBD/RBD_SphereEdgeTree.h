/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RBD_SphereEdgeTree.h ( RBD Library, C++)
 *
 * COMMENTS: 
 *	This builds a tree of spheres producing bounding
 *	information for an edge cloud.
 */

#ifndef __RBD_SphereEdgeTree_H__
#define __RBD_SphereEdgeTree_H__

#include "RBD_API.h"
#include "RBD_SphereTree.h"
#include "RBD_Utils.h"

#include <SIM/SIM_DataUtils.h>
#include <SIM/SIM_OptionsUser.h>
#include <GA/GA_Edge.h>
#include <UT/UT_Array.h>
#include <UT/UT_Set.h>
#include <UT/UT_Vector3Array.h>

class SIM_Geometry;
class RBD_SphereEdgeTree;

/// This describes an edge.  The indices point to the sphere trees
/// point list.
class RBD_API rbd_sphereedge
{
public:
    GA_Index		a, b;
};

/// This is a node in the sphere tree.
class RBD_API RBD_SphereEdgeTreeNode : public RBD_SphereTreeNode
{
public:
			 RBD_SphereEdgeTreeNode(RBD_SphereEdgeTree *tree,
			    const UT_Array<rbd_sphereedge> &edges);

    virtual		~RBD_SphereEdgeTreeNode();

    int64		 getMemoryUsage() const;
    
    UT_Array<rbd_sphereedge>	 myEdges;
};

/// This class holds an edge cloud of samples.
/// It has a bounding sphere hierarchy to allow quick rejection
/// when dealing with a SDF.
class RBD_API RBD_SphereEdgeTree : public RBD_SphereTree, public SIM_OptionsUser
{
public:
    /// Get the Tree.
    virtual const RBD_SphereTreeNode	*getRoot() const;

    /// Get a single point.
    const UT_Vector3			&getPoint(int idx) const;

protected:
    // Determines if we convert to polygons or use raw points.
    GETSET_DATA_FUNCS_B(RBD_NAME_POLYCONVERT, PolyConvert);
    GETSET_DATA_FUNCS_B(RBD_NAME_TRIANGULATE, Triangulate);
    GETSET_DATA_FUNCS_V2(RBD_NAME_POLYLOD, PolyLOD);
    GETSET_DATA_FUNCS_B(RBD_NAME_ADDBARYCENTER, AddBaryCenter);

    explicit		 RBD_SphereEdgeTree(const SIM_DataFactory *factory);
    virtual		~RBD_SphereEdgeTree();

    /// Overrides to properly implement this class as a SIM_Data.
    /// This object is meant to be updated from the object's geometry at
    /// each frame, so we only implement skeleton save, and load methods.
    virtual void	 initializeSubclass();
    virtual int64	 getMemorySizeSubclass() const;
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);

private:
    static const SIM_DopDescription	*getSphereEdgeTreeDopDescription();

    /// This builds the sphere tree from the given GU_Detail.
    void		 buildSphereEdgeTree(const SIM_Geometry *geo);

    /// Adds an edge to our list of edges.  Tests to see if it
    /// is already present by using the given hash table.
    /// Return true if added.
    bool		 addEdge(UT_Array<rbd_sphereedge> &edges,
                                 UT_Set<GA_EdgeT<GA_Index,false> > &hash,
				 GA_Index a, GA_Index b);
    bool		 isDisabled(GA_Offset pt,
			       const GA_ROHandleF &attDisable) const;
    RBD_SphereEdgeTreeNode	*myRoot;
    UT_Vector3Array	 	 myPoints;

    friend class		 RBD_SphereEdgeTreeNode;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(RBD_SphereEdgeTree,
			 RBD_SphereTree,
			 "Sphere Edge Tree",
			 getSphereEdgeTreeDopDescription());
};

#endif
