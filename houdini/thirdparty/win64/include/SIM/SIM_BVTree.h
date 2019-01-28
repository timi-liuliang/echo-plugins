/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_BVTree_h__
#define __SIM_BVTree_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"

#include <UT/UT_DMatrix4.h>

#define SIM_NAME_BVTREETYPE    "bvtreetype"

#define SIM_NAME_GUIDE_DEPTH   "guidedepth"
#define SIM_NAME_GUIDE_PRIM    "guideprim"
#define SIM_NAME_GUIDE_COLOR   "guidecolor"

class UT_IStream;
class BV_LeafIterator;
class BV_Tree;

/// A base class for bounding volume trees.
class SIM_API SIM_BVTreeBase : public SIM_Data
{
public:
    enum simBVTreeType
    {
        SIM_NOTREE = -1,
        SIM_OBBTREE,
        SIM_AABBTREE,
        SIM_14DOPTREE,
        SIM_18DOPTREE,
        SIM_26DOPTREE
    };
    simBVTreeType        getBVTreeType() const
                         { return myBVTreeType; }

protected:
    explicit		 SIM_BVTreeBase(const SIM_DataFactory *factory);
    virtual		~SIM_BVTreeBase();

    virtual void	 makeEqualSubclass(const SIM_Data *source);
    virtual int64	 getMemorySizeSubclass() const;

    const BV_Tree       *getTree() const
                         { return myBVTree; }
    BV_Tree             *getTree()
                         { return myBVTree; }

protected:
    /// Sets myBVTree to a tree of the given type. Any existing tree of the
    /// wrong type will be destroyed.
    void                 createMyTree(simBVTreeType);

private:
    BV_Tree             *myBVTree;
    simBVTreeType        myBVTreeType;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_BVTreeBase, SIM_Data);
};

/// This class is intended to store the *structure* of a BV tree. For
/// algorithms that rely on building a BV tree once and then just
/// updating its extents over time, the structure of that BV tree is
/// important, and needs to be saved to disk. This class does exactly that:
/// it stores structure (but not extents) to disk.
///
/// See the SIM_ColliderBFA class for an example of usage.
class SIM_API SIM_BVTreeStructure : public SIM_BVTreeBase
{
public:
    /// Build the OBB tree from some geometry. Takes ownership of leafIt.
    void		 createTree(simBVTreeType, BV_LeafIterator &leafIt);

protected:
    explicit		 SIM_BVTreeStructure(const SIM_DataFactory *factory);
    virtual		~SIM_BVTreeStructure();

    virtual void	 saveSubclass(std::ostream &os) const;
    virtual bool	 loadSubclass(UT_IStream &is);

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_BVTreeStructure,
			SIM_BVTreeBase,
			"Bounding Volume Tree Structure",
			getEmptyDopDescription());
};

/// This class is a regular BV tree. It is implemented as an "alternate
/// representation". When being initialized, it does not grab data directly
/// from the parent data, but looks for a child SIM_BVTreeStructure of
/// the parent, and builds itself from that.
class SIM_API SIM_BVTree : public SIM_BVTreeBase
{
public:
    /// Creation is unnecessary if using the auxiliary BVStructure data.
    /// If BVStructure is not being used, however, this method can be used
    /// to set up a vanilla bv tree.
    void		 createTree(simBVTreeType, BV_LeafIterator &leafIt);
    /// Initialize usingany structure data on the parent, then update the
    /// extents.
    void		 updateExtents(BV_LeafIterator &leafIt);

    BV_Tree             *getTree();
    const BV_Tree       *getTree() const;

    static PRM_Template	 theGuideTemplates[];

protected:
    explicit		 SIM_BVTree(const SIM_DataFactory *factory);
    virtual		~SIM_BVTree();

    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);

    virtual SIM_Guide	*createGuideObjectSubclass() const;
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

private:
    static SIM_DopDescription	*getBVTreeDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_BVTree,
			SIM_BVTreeBase,
			"Bounding Volume Tree",
			getBVTreeDopDescription());
};
#endif
