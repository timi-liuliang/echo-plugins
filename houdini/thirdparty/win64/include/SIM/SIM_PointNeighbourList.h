/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SIM_PointNeighbourList.h ( SIM Library, C++)
 *
 * COMMENTS: 
 *	This class builds a GU_NeighbourList of the given Geometry
 *	as an alternative representation, allowing one to cache
 *	this useful structure.
 */

#ifndef __SIM_PointNeighbourList_H__
#define __SIM_PointNeighbourList_H__

#include "SIM_API.h"

#include "SIM_Data.h"
#include "SIM_DataUtils.h"

#include "SIM_Utils.h"

class GU_NeighbourList;

/// This class holds a GU_NeighbourList which people can query.
/// The pointtree refers to the point numbers of the SIM_Geometry
/// this is an alternative represenation of.
class SIM_API SIM_PointNeighbourList : public SIM_Data
{
public:
    /// Get the list.
    GU_NeighbourList	*getList() const;

    /// Forces a rebuild of the neighbour list, used when the
    /// point positions have changed.
    void		 forceRebuild(const GU_Detail *gdp);

protected:
    explicit		 SIM_PointNeighbourList(const SIM_DataFactory *factory);
    virtual		~SIM_PointNeighbourList();

    /// Overrides to properly implement this class as a SIM_Data.
    /// This object is meant to be updated from the object's geometry at
    /// each frame, so we only implement skeleton save, and load methods.
    virtual void	 initializeSubclass();
    virtual int64	 getMemorySizeSubclass() const;
    virtual bool	 getIsAlternateRepresentationSubclass() const;
    virtual void	 initAlternateRepresentationSubclass(const SIM_Data &);

private:
    GU_NeighbourList	*myList;
    
    DECLARE_STANDARD_GETCASTTOTYPE();

    DECLARE_DATAFACTORY(SIM_PointNeighbourList,
			 SIM_Data,
			 "Point Neighbour List",
			 getEmptyDopDescription());
};

#endif
