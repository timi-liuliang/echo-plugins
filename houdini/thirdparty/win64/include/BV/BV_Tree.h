/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BV_Tree.h
 *
 */

#ifndef __BV_Tree_h__
#define __BV_Tree_h__

#include "BV_API.h"
#include <UT/UT_Matrix4.h>
#include <iosfwd>

class UT_IStream;
class BV_Callback;
class BV_LeafIterator;

/// Base class for bounding volume hierarchies. It's not really intended to
/// be an all-encompassing base class; it's more intended as a rough
/// template for other bounding volume classes to follow.
class BV_API BV_Tree
{
public:
    enum BV_Status
    {
	BV_PASS,
	/// Rotations are not allowed.
	BV_FAIL_ROTATIONS_UNSUPPORTED,
	/// Tree B was an unsupported type.
	BV_FAIL_BAD_TYPE,
	/// Failed for some other reason.
	BV_FAIL
    };
			 BV_Tree();
    virtual		~BV_Tree();
    BV_Tree             *clone() const
                        { return cloneSubclass(); }

    virtual const char	*getType() const = 0;
    virtual const BV_Tree
			*castTo(const char *type) const;
    virtual BV_Tree	*castTo(const char *type);

    int64                getMemoryUsage() const
                         { return getMemoryUsageSubclass(); }
    int			 getNumLeaves() const
			 { return getNumLeavesSubclass(); }

    /// If the "onlyStructure" flag is set, then the dimensions of the
    /// boxes are not saved/loaded, only the structure of the tree.
    // @{
    void		 save(std::ostream &os, bool onlyStructure) const
                         { saveSubclass(os, onlyStructure); }
    bool		 load(UT_IStream &is, bool onlyStructure)
			 { return loadSubclass(is, onlyStructure); }
    // @}


    /// Construct a volume hierarchy from the given primitives.
    void		 build(BV_LeafIterator &leafIt)
			 { buildSubclass(leafIt); }
    /// Update an existing volume hierarchy by updating the bounding box
    /// sizes without changing the hierarchy structure.  
    void		 updateExtents(BV_LeafIterator &leafIt)
			 { updateExtentsSubclass(leafIt); }
    /// Find intersections of bounding volumes of this tree ("A") with tree B,
    /// rotated by ra and rb and translated by ta and tb respectively. For
    /// each intersection, call the callback with the leaves whose bounding
    /// volumes overlap.
    ///
    /// The callback can return a "stop" status to stop testing for further
    /// intersections.
    ///
    /// Note:
    /// - Not all trees can deal with rotations.
    ///   e.g., an AABB tree can't, unless it expands all nodes to handle
    ///         the contents at any orientation.
    /// - method may not succeed if treeb is a different type.
    /// @{
    /// Static boxes only!
    BV_Status		intersect(BV_Callback &callback,
				   const BV_Tree &treeb,
				   const UT_DMatrix4 &xforma,
				   const UT_DMatrix4 &xformb,
				   fpreal tol = 0.001F) const
			 {
			     return intersectSubclass(callback, treeb,
						      xforma, xforma,
						      xformb, xformb,
						      tol);
			 }
    /// Static boxes only!
    BV_Status		intersect(BV_Callback &callback,
				   const BV_Tree &treeb,
				   fpreal tol = 0.001F) const
			 {
			     return intersectSubclass(callback, treeb,
						     UT_DMatrix4(1.f),
						     UT_DMatrix4(1.f),
						     UT_DMatrix4(1.f),
						     UT_DMatrix4(1.f),
						     tol);
			 }
    /// Moving boxes only!
    BV_Status		intersect(BV_Callback &callback,
				   const BV_Tree &treeb,
				   const UT_DMatrix4 &startxforma,
				   const UT_DMatrix4 &startxformb,
				   const UT_DMatrix4 &endxforma,
				   const UT_DMatrix4 &endxformb,
				   fpreal tol = 0.001F) const
			{
			    return intersectSubclass(callback, treeb,
						    startxforma,
						    startxformb,
						    endxforma,
						    endxformb,
						    tol);
			}
    /// Moving boxes only!
    BV_Status		intersect(BV_Callback &callback,
				   const BV_Tree &treeb,
				   const UT_DMatrix4 &xforma,
				   const UT_DMatrix4 &startxforma,
				   const UT_DMatrix4 &startxformb,
				   const UT_DMatrix4 &xformb,
				   const UT_DMatrix4 &endxforma,
				   const UT_DMatrix4 &endxformb,
				   fpreal tol = 0.001F) const
			{
			    // TODO: test
			    return intersectSubclass(callback, treeb,
						    startxforma * xforma,
						    startxformb * xformb,
						    endxforma * xforma,
						    endxformb * xformb,
						    tol);
			}

    /// @}
    
protected:
			 BV_Tree(const BV_Tree &tree);

    virtual BV_Tree     *cloneSubclass() const = 0;
    virtual void	 buildSubclass(BV_LeafIterator &leafIt) = 0;
    virtual int64        getMemoryUsageSubclass() const = 0;
    virtual int		 getNumLeavesSubclass() const = 0;
    virtual void	 saveSubclass(std::ostream &os, bool onlyStructure)const = 0;
    virtual bool	 loadSubclass(UT_IStream &is, bool onlyStructure) = 0;
    virtual void	 updateExtentsSubclass(BV_LeafIterator &leafIt) = 0;
    virtual BV_Status	 intersectSubclass(BV_Callback &callback,
				   const BV_Tree &treeb,
				   const UT_DMatrix4 &startxforma, 
				   const UT_DMatrix4 &startxformb,
				   const UT_DMatrix4 &endxforma,
				   const UT_DMatrix4 &endxformb,
				   fpreal tol) const = 0;

private:
    /// Disallowed.
    BV_Tree		&operator=(const BV_Tree &);
};

#endif
