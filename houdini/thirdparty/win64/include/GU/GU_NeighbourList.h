/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry library (C++)
 *
 * COMMENTS:	For a set of geometric points with a fixed interatction
 * 		radius, build a list of neighbours for each point.
 * 		This is useful for things like particle-based fluid
 * 		simulations.
 *		
 */

#ifndef __GU_NEIGHBOUR_LIST_H__
#define __GU_NEIGHBOUR_LIST_H__

#include "GU_API.h"

#include "GU_Detail.h"

#include <GA/GA_Types.h>

#include <UT/UT_HashGrid.h>
#include <UT/UT_Array.h>
#include <UT/UT_String.h>
#include <UT/UT_ThreadedAlgorithm.h>

class UT_SmallAlloc;
class GEO_PointTree;

///
/// GU_NeighbourListParms
/// Parameters necessary for building the neighbour list data structure
///
class GU_API GU_NeighbourListParms
{
public:
	     GU_NeighbourListParms();
    virtual ~GU_NeighbourListParms();

    // We can support either fixed radii, or varying radii amongst
    // the provided point set, though varying radii is more expensive
    enum InteractionMode
    {
	UNIFORM,
	NONUNIFORM
    };

    void			 setMode( InteractionMode mode );
    InteractionMode		 getMode() const;

    void			 setOverrideRadius( bool overrideRadius );
    bool			 getOverrideRadius() const;

    void			 setRadius( fpreal radius );
    fpreal			 getRadius() const;

    void			 setRadiusScale( fpreal radiusScale );
    fpreal			 getRadiusScale() const;

    void			 setRadiusAttribute( const char *name );
    const char			*getRadiusAttribute() const;

private:
    InteractionMode		 myMode;

    /// Whether or not to override provided radii with a
    /// fixed radius, and the desired fixed radius.
    bool			 myOverrideRadius;
    fpreal			 myRadius;

    /// Scaling factor for the desired radius (either from the
    /// given override radius, or obtained from point attributes).
    fpreal			 myRadiusScale;

    /// String storing the attribute name used to store
    /// per-point interaction radii.
    UT_String			 myRadiusAttributeName;

};

///
/// GU_NeighbourList
/// This class associates with each point in the provided geometry
/// a fixed interaction radius.  Based on this interaction radius, a
/// list of neighbour points can be efficiently determined for each
/// point in the geometry.
///
class GU_API GU_NeighbourList
{
public:
    typedef GA_Index		    IdxType;
    typedef UT_ValArray<IdxType>    IdxArrayType;

	     GU_NeighbourList();
    virtual ~GU_NeighbourList();

    /// Build the neighbour list structure from the given gdp
    void			    build(const GU_Detail *gdp,
    				          const GU_NeighbourListParms &parms);

    /// Erases our storage.
    void			    clearLists();

    /// Fills the giving point array with the list of neighbours of the
    /// given point index.
    /// Out of range point indices will be return empty lists.
    /// If the gdp has too few points, the out of range neighbours are
    /// ignored.
    void			    getNeighbours(int pointidx, const GU_Detail *gdp,
					UT_Array<GA_Offset> &ptlist) const;

    int64			    getMemoryUsage() const;

private:
    /// Determine what fixed radius to do when using fixed neighbour
    /// interaction radii.  This works by finding the largest radius
    /// amongst all particles in the input.
    bool			    setNeighbourRadius(
    					  const GU_Detail *gdp,
					  const GU_NeighbourListParms &parms);

    /// Helper function which creates a hash grid data structure
    /// for the particle field.
    void			    buildPointGrid(
    					   const GU_Detail *gdp,
					   const GU_NeighbourListParms &parms);
    void			    destroyPointGrid();

    /// Threaded method to look up in the point grid the neighbour
    /// list for each element.
    THREADED_METHOD1(GU_NeighbourList, gdp->getNumPoints() > 1000,
		    buildListFromPointGrid,
		    const GU_Detail *, gdp)
    void			buildListFromPointGridPartial(const GU_Detail *gdp,
					    const UT_JobInfo &info);

    /// Threaded method to look up in the point tree the neighbour
    /// list for each element.
    THREADED_METHOD3(GU_NeighbourList, gdp->getNumPoints() > 1000,
		    buildListFromPointTree,
		    const GU_Detail *, gdp,
		    fpreal, radscale,
		    const char *, radname)
    void			buildListFromPointTreePartial(const GU_Detail *gdp,
					    fpreal radscale,
					    const char *radname,
					    const UT_JobInfo &info);

    /// Helper function which constructs a KD-tree when using
    /// non-uniform radii.
    void			    buildPointTree(
    					   const GU_Detail *gdp,
					   const GU_NeighbourListParms &parms);
    void			    destroyPointTree();

private:
    GU_NeighbourListParms::InteractionMode
    				    myMode;

    /// Per-point neighbour information.
    UT_Array<IdxType *>		    myNeighbourIdx;
    UT_IntArray			    myNeighbourLen;

    /// Our storage tables where we keep our actual data
    /// We have one per thread.
    UT_Array<UT_SmallAlloc *>	    myAllocTables;

    /// Fixed neighbour radius used for the uniform data structure
    fpreal			    myFixedRadius;

    /// Data structure used to efficiently initialize the neighbour
    /// lists.
    UT_HashGrid<GA_Offset> *myPointGrid;

    /// Data structure used to initialize the neighbour lists
    /// when using non-uniform radii.
    GEO_PointTree		   *myPointTree;

};

#endif
