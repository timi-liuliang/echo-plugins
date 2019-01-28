/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RBD_SphereTree.h ( RBD Library, C++)
 *
 * COMMENTS: 
 *	This builds a tree of spheres producing bounding
 *	information for a point cloud.
 */

#ifndef __RBD_SphereTree_H__
#define __RBD_SphereTree_H__

#include "RBD_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3Array.h>

#include <SIM/SIM_DataUtils.h>
#include <SIM/SIM_OptionsUser.h>

#include "RBD_Utils.h"

/// This is the base class for nodes inside sphere trees.
class RBD_API RBD_SphereTreeNode
{
public:
			 RBD_SphereTreeNode();
    virtual		~RBD_SphereTreeNode();

    UT_Vector3		 myCenter;
    fpreal		 myRadius;

    RBD_SphereTreeNode	*myChild[2];
};

/// This is a node in the sphere tree.
class RBD_API RBD_SpherePointTreeNode : public RBD_SphereTreeNode
{
public:
			 RBD_SpherePointTreeNode(const UT_Vector3Array &pts);

    virtual		~RBD_SpherePointTreeNode();

    int64		 getMemoryUsage() const;
    
    UT_Vector3Array	 myPoints;
};

/// This class provides a generic way of describing sphere trees
/// of points, edges, or higher order things.
class RBD_API RBD_SphereTree : public SIM_Data
{
public:
    // Return the untyped root of the tree.
    virtual const RBD_SphereTreeNode	*getRoot() const = 0;

protected:
    explicit		 RBD_SphereTree(const SIM_DataFactory *factory);
    virtual		~RBD_SphereTree();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(RBD_SphereTree, SIM_Data);
};

/// This class holds a point cloud of samples.
/// It has a bounding sphere hierarchy to allow quick rejection
/// when dealing with a SDF.
class RBD_API RBD_SpherePointTree : public RBD_SphereTree, public SIM_OptionsUser
{
public:
    /// Get the Tree.
    virtual const RBD_SphereTreeNode	*getRoot() const;

protected:
    // Determines if we convert to polygons or use raw points.
    GETSET_DATA_FUNCS_B(RBD_NAME_POLYCONVERT, PolyConvert);
    GETSET_DATA_FUNCS_B(RBD_NAME_TRIANGULATE, Triangulate);
    GETSET_DATA_FUNCS_V2(RBD_NAME_POLYLOD, PolyLOD);
    GETSET_DATA_FUNCS_B(RBD_NAME_ADDBARYCENTER, AddBaryCenter);
    
    explicit		 RBD_SpherePointTree(const SIM_DataFactory *factory);
    virtual		~RBD_SpherePointTree();

    /// Overrides to properly implement this class as a SIM_Data.
    /// This object is meant to be updated from the object's geometry at
    /// each frame, so we only implement skeleton save, and load methods.
    virtual void	 initializeSubclass();
    virtual int64	 getMemorySizeSubclass() const;
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);

private:
    static const SIM_DopDescription	*getSpherePointTreeDopDescription();

    // This builds the sphere tree from the given set of points.
    void		 buildSpherePointTree(const UT_Vector3Array &pts);

    RBD_SpherePointTreeNode	*myRoot;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(RBD_SpherePointTree,
			 RBD_SphereTree,
			 "Sphere Point Tree",
			 getSpherePointTreeDopDescription());
};

#endif
