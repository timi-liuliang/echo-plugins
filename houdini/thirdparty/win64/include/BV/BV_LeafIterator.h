/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BV_LeafIterator.h
 *
 * COMMENTS:
 */

#ifndef __BV_LeafIterator_h__
#define __BV_LeafIterator_h__

#include "BV_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>

/// Iterator for leaves to be enclosed by bounding volume.
/// Iterator retrieves info about each leaf:
/// - a set of vertices forming a bounding volume for the leaf
/// - barycenter (centroid) of leaf
///
/// Optionally, the iterator may be able to skip duplicate vertices - i.e.,
/// if leaves in the set share vertices.
class BV_API BV_LeafIterator
{
public:
			 BV_LeafIterator();
    virtual		~BV_LeafIterator();

    /// Advance iterator to next leaf
    void		 advance()
			 { advanceSubclass(); }
    /// Random access: jump to given leaf id
    void		 jump(int leafId)
			 { jumpSubclass(leafId); }
    /// Test to see if we've passed the last leaf.
    /// Note: returns false on the last leaf, and true if we've advanced
    /// *past* the last leaf.
    bool		 atEnd() const
			 { return atEndSubclass(); }
    /// Reset to first leaf
    void		 rewind()
			 { rewindSubclass(); }

    /// Get id of current leaf
    int			 getLeafId() const
			 { return getLeafIdSubclass(); }
    /// Get barycenter of current leaf
    UT_Vector3		 calcBarycenter() const
			 { return calcBarycenterSubclass(); }
    /// Test if the current leaf is disabled.
    bool		 isDisabled() const
			 { return isDisabledSubclass(); }


    // Sub-iterator: iterate over vertices within leaf

    /// Get number of vertices bounding current leaf
    void		 advanceVertex()
			 { advanceVertexSubclass(); }
    bool		 atEndVertex() const
			 { return atEndVertexSubclass(); }
    void		 rewindVertex()
			 { rewindVertexSubclass(); }
    /// Get given bounding vertex of current leaf
    UT_Vector4		 getVertex() const
			 { return getVertexSubclass(); }
    /// Gets radius to expand each vertex for calculation of the bounding
    /// volume.
    virtual fpreal	 getVertexRadius() const
			 { return 0.0f; }
    /// Flag: hint to skip duplicate points when iterating over vertices of
    /// leaves.
    /// TODO: this is obsolete and possibly broken; it's also dangerous
    /// when combined with a non-zero vertex radius. Remove it.
    void		 setSkipDupVerts(bool flag)
			 { mySkipDups = flag; setSkipDupVertsSubclass(flag); }
    bool		 getSkipDupVerts() const
			 { return mySkipDups; }


protected:
    virtual void	 advanceSubclass() = 0;
    virtual void	 jumpSubclass(int leafId) = 0;
    virtual bool	 atEndSubclass() const = 0;
    virtual void	 rewindSubclass() = 0;
    virtual int		 getLeafIdSubclass() const = 0;

    virtual UT_Vector3	 calcBarycenterSubclass() const = 0;
    virtual bool	 isDisabledSubclass() const;

    virtual void	 advanceVertexSubclass() = 0;
    virtual bool	 atEndVertexSubclass() const = 0;
    virtual void	 rewindVertexSubclass() = 0;
    virtual UT_Vector4	 getVertexSubclass() const = 0;

    virtual void	 setSkipDupVertsSubclass(bool flag) = 0;

private:
    /// Disallowed.
    /// @{
			 BV_LeafIterator(const BV_LeafIterator &);
    BV_LeafIterator	&operator=(const BV_LeafIterator &);
    /// @}

    bool		 mySkipDups;
};

#endif
