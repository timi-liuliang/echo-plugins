/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_PointTree.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	This class builds a GEO_PointTree of the given Geometry
 *	as an alternative representation, allowing one to cache
 *	this useful structure.
 */

#ifndef __SIM_PointTree_H__
#define __SIM_PointTree_H__

#include "SIM_API.h"

#include "SIM_Data.h"
#include "SIM_DataUtils.h"

#include "SIM_Utils.h"

class GEO_PointTree;

/// This class holds a GEO_PointTree which people can query.
/// The pointtree refers to the point numbers of the SIM_Geometry
/// this is an alternative represenation of.
/// This caches the point positions in the SIM_Geometry's space,
/// so are independent of SIM_Geometry's transform.
class SIM_API SIM_PointTree : public SIM_Data
{
public:
    /// Get the tree
    GEO_PointTree	*getTree() const;

protected:
    explicit		 SIM_PointTree(const SIM_DataFactory *factory);
    virtual		~SIM_PointTree();

    /// Overrides to properly implement this class as a SIM_Data.
    /// This object is meant to be updated from the object's geometry at
    /// each frame, so we only implement skeleton save, and load methods.
    virtual void	 initializeSubclass();
    virtual int64	 getMemorySizeSubclass() const;
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);

private:
    GEO_PointTree	*myTree;
    
    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_PointTree,
			 SIM_Data,
			 "Point Tree",
			 getEmptyDopDescription());
};

#endif
