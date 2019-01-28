/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Adjacent Polygon Iterator
 *
 * COMMENTS:	This iterator has multiple functionalities:
 *		    1. iterate through polygons adjacent to a single point
 *		    2. iterate through polygons adjacent to a single polygon
 *		    3. iterate through polygons adjacent to a list of polygons
 *	    
 */

#ifndef __GEO_AdjPolyIterator_h__
#define __GEO_AdjPolyIterator_h__

#include "GEO_Detail.h"
#include "GEO_PrimPoly.h"
#include <UT/UT_Interrupt.h>
#include <queue>

class GEO_API GEO_AdjPolyIterator
{
protected:
    GEO_PrimPoly *myAdjacentPoly;
    GEO_Detail *myGdp;
    const GA_PrimitiveGroup *myProcess;
    GA_Size myNumAdjacentPolys;
    GA_Size myIterationCount;

public:
    // iteration routines
    virtual bool advance() = 0;
    void rewind() { myIterationCount = 0; }
    GEO_AdjPolyIterator &operator++()   { advance(); return *this; }

    // state accessors
    GEO_PrimPoly *getAdjPoly() { return myAdjacentPoly; }

    // constructor
    explicit GEO_AdjPolyIterator( GEO_Detail *gdp,
				  const GA_PrimitiveGroup *process_grp = NULL )
	: myGdp(gdp), myProcess(process_grp), myIterationCount(-1)
	, myAdjacentPoly(NULL), myNumAdjacentPolys(-1)
	{ }

    virtual ~GEO_AdjPolyIterator() { }
};


// This is an iterator of a UT_ValArray<GA_Offset> returned from
// getPrimitivesReferencingPoint() from GEO_Detail. However it skipps all
// primitives that aren't of the type GEO_PRIMPOLY
class GEO_API GEO_PointAdjPolyIterator : public GEO_AdjPolyIterator
{
private:
    GA_OffsetArray myAdjacentPolys;

public:
    virtual bool advance()
    {
	while( ++myIterationCount < myNumAdjacentPolys )
	{
	    GA_Offset adj_poly_off = myAdjacentPolys(myIterationCount);

	    if( myProcess && !myProcess->containsOffset(adj_poly_off) )
		continue;
	    
	    GA_Primitive *adj_prim =
		myGdp->getPrimitiveList().get(adj_poly_off);
	    myAdjacentPoly = static_cast<GEO_PrimPoly*>(adj_prim);

	    return true;
	}
	
	rewind();
	return false;
    }

    explicit GEO_PointAdjPolyIterator( GEO_Detail	 *gdp,
				       GA_Offset	  pt_offset,
				 const GA_PrimitiveGroup *process_grp = NULL )
	: GEO_AdjPolyIterator(gdp, process_grp)
	{
	    myNumAdjacentPolys =
		gdp->getPrimitivesReferencingPoint(myAdjacentPolys, pt_offset);
	}

    virtual ~GEO_PointAdjPolyIterator() { }
};

// This is an iterator of a an array of adjacency data that includes the
// adjacent polygon offset, and the point shared between the original polygon
// and the adjacency. This data is returned by getPointAdjacentPolygons()
// from GEO_Detail
class GEO_API GEO_PolyAdjPolyIterator : public GEO_AdjPolyIterator
{
private:
    GEO_Detail::GEO_PointAdjArray myAdjacencyData;
    GA_Offset mySharedPoint;

public:
    virtual bool advance()
    {
	while( ++myIterationCount < myNumAdjacentPolys )
	{
	    GEO_Detail::PointAdjacencyData data =
		myAdjacencyData(myIterationCount);
	    GA_Offset adj_poly_off = data.myAdjacentPolygon;

	    if( myProcess && !myProcess->containsOffset(adj_poly_off) )
		continue;
	    
	    GA_Primitive *adj_prim =
		myGdp->getPrimitiveList().get(adj_poly_off);
	    myAdjacentPoly = static_cast<GEO_PrimPoly*>(adj_prim);
	    mySharedPoint = data.mySharedPoint;

	    return true;
	}
	
	rewind();
	return false;
    }

    GA_Offset getSharedPoint() { return mySharedPoint; }

    explicit GEO_PolyAdjPolyIterator( GEO_Detail	 *gdp,
				      GA_Offset		  poly_offset,
				const GA_PrimitiveGroup  *process_grp = NULL )
	: GEO_AdjPolyIterator(gdp, process_grp)
	, mySharedPoint(GA_INVALID_OFFSET)
	{
	    myNumAdjacentPolys =
		gdp->getPointAdjacentPolygons(myAdjacencyData, poly_offset);
	}

    virtual ~GEO_PolyAdjPolyIterator() { }
};

// This is an iterator of an array of adjacency data that includes the
// adjacent polygon offset, the shared edge and a boolean indicating whether
// the winding order between poly and it's adjacent poly is reversed or not.
// Information is provided by the getEdgeAdjacentPolygons() call from
// GEO_Detail.
class GEO_API GEO_EdgeAdjPolyIterator : public GEO_AdjPolyIterator
{
private:
    GEO_Detail::GEO_EdgeAdjArray myAdjacencyData;
    GA_Offset myStartEdgePt;
    GA_Offset myEndEdgePt;
    bool myWindingReversed;

public:
    virtual bool advance()
    {
	while( ++myIterationCount < myNumAdjacentPolys )
	{
	    GEO_Detail::EdgeAdjacencyData data =
		myAdjacencyData(myIterationCount);
	    GA_Offset adj_poly_off = data.myAdjacentPolygon;

	    if( myProcess && !myProcess->containsOffset(adj_poly_off) )
		continue;
	    
	    GA_Primitive *adj_prim =
		myGdp->getPrimitiveList().get(adj_poly_off);
	    myAdjacentPoly = static_cast<GEO_PrimPoly*>(adj_prim);
	    myStartEdgePt = data.myStartEdgePoint;
	    myEndEdgePt = data.myEndEdgePoint;
	    myWindingReversed = data.myWindingReversed;

	    return true;
	}
	
	rewind();
	return false;
    }

    bool isWindingReversed()	{ return myWindingReversed; }
    GA_Offset getStartEdgePt()	{ return myStartEdgePt; }
    GA_Offset getEndEdgePt()	{ return myEndEdgePt; }

    explicit GEO_EdgeAdjPolyIterator( GEO_Detail	 *gdp,
				      GA_Offset		  poly_offset,
				const GA_PrimitiveGroup *process_grp = NULL )
	: GEO_AdjPolyIterator(gdp, process_grp), myStartEdgePt(GA_INVALID_OFFSET)
	, myEndEdgePt(GA_INVALID_OFFSET), myWindingReversed(false)
	{
	    myNumAdjacentPolys =
		gdp->getEdgeAdjacentPolygons(myAdjacencyData, poly_offset);
	}

    virtual ~GEO_EdgeAdjPolyIterator() { }
};

//
// Iterator Visitors
//

// TODO: rename the callbacks to something more meaningful
class GEO_API GEO_BFSTraverserVisitor
{
public:
    // these are pure virtual to insure that you can't instantiate this class
    virtual void onVisitedPoly(GEO_PrimPoly *visited) = 0;
    virtual void onEndOfConnectedGeometry() = 0;
    virtual void onStartOfConnectedGeometry(GEO_PrimPoly *source) = 0;
    virtual ~GEO_BFSTraverserVisitor() {}
};

class GEO_API GEO_BFSEdgeAdjPolyVisitor : public GEO_BFSTraverserVisitor
{
public:
    virtual void onVisitedPoly(GEO_PrimPoly *visited) { }
    virtual void onEndOfConnectedGeometry() { }
    virtual void onStartOfConnectedGeometry(GEO_PrimPoly *source) { }

    // Override these callbacks for EDGE adjacent BFS traverser
    virtual void onNewlyDiscoveredPoly(GEO_PrimPoly *visited,
				     GEO_PrimPoly *discovered,
				     GA_Offset edge_start_pt,
				     GA_Offset edge_end_pt,
				     bool winding_reversed) { }

    virtual void onPreviouslyDiscoveredPoly(GEO_PrimPoly *visited,
					    GEO_PrimPoly *discovered,
					    GA_Offset edge_start_pt,
					    GA_Offset edge_end_pt,
					    bool winding_reversed) { }

};

class GEO_API GEO_BFSPointAdjPolyVisitor : public GEO_BFSTraverserVisitor
{
public:
    virtual void onVisitedPoly(GEO_PrimPoly *visited) { }
    virtual void onEndOfConnectedGeometry() { }
    virtual void onStartOfConnectedGeometry(GEO_PrimPoly *source) { }

    // Override these callbacks for POINT adjacent BFS traverser
    virtual void onNewlyDiscoveredPoly(GEO_PrimPoly *visited,
				     GEO_PrimPoly *discovered,
				     GA_Offset shared_pt) { }

    virtual void onPreviouslyDiscoveredPoly(GEO_PrimPoly *visited,
					    GEO_PrimPoly *discovered,
					    GA_Offset shared_pt) { }
};

// Breadth First Polygon Iterator
// iterator over all adjacent polygons via BFS
class GEO_API GEO_BFSAdjPolyTraverser
{
protected:
    GEO_BFSTraverserVisitor &myVisitor;
    GEO_PrimPoly *myVisitedPoly, *myDiscoveredPoly;
    GA_Offset myVisitedOffset, myDiscoveredOffset;
    GEO_Detail *myGdp;
    GA_PrimitiveGroup *myProcess;

    // visited polygons. Used to avoid cycles in the BFS algorithm
    GA_PrimitiveGroup *myDiscoveredPolyGrp;

    // Queue implementation used for breadth-first traversal
    class OffsetQueue : public std::queue<GA_Offset>
    {
    public:
	void enqueue(GA_Offset thing) { std::queue<GA_Offset>::push(thing); }

	GA_Offset dequeue()
	{
	    GA_Offset thing = std::queue<GA_Offset>::front();
	    std::queue<GA_Offset>::pop();
	    return thing;
	}
    } myQueue;

private:
    GA_Iterator	myProcessIt;

    GEO_PrimPoly * getPolyFromOffset(GA_Offset poly_off);

    UT_Interrupt *myBoss;
    GA_Size myProgressCounter;
protected:
    virtual void iterateThroughAdjacentPolygons() = 0;

public:
    // Trigger
    void run();

private:
    void init( bool insure_poly_only );

public:
    // constructors to iterate over all polygons and their adjacencies
    // contained in the process group, bitarray or the whole gdp via
    // breadth first search
    // TODO: initialize EVERYTHING
    explicit GEO_BFSAdjPolyTraverser( GEO_Detail *gdp,
				      const GA_PrimitiveGroup *process_grp,
				      GEO_BFSTraverserVisitor &visitor,
				      UT_Interrupt *boss,
				      bool insure_poly_only )
	: myVisitedPoly(NULL), myGdp(gdp), myVisitor(visitor), myBoss(boss), myProgressCounter(0)
    { 
	myProcess = myGdp->newInternalPrimitiveGroup();
	myProcess->copyMembership(*process_grp); // make a copy
	init( insure_poly_only ); 
    }

    explicit GEO_BFSAdjPolyTraverser( GEO_Detail *gdp,
				      GEO_BFSTraverserVisitor &visitor,
				      UT_Interrupt *boss)
	: myVisitedPoly(NULL), myGdp(gdp), myVisitor(visitor), myBoss(boss), myProgressCounter(0)
    {
	myProcess = myGdp->newInternalPrimitiveGroup();
	myProcess->toggleEntries();
	init( true );
    }

    virtual ~GEO_BFSAdjPolyTraverser()
    {
	myGdp->destroyPrimitiveGroup(myProcess);
	myGdp->destroyPrimitiveGroup(myDiscoveredPolyGrp);
    }
};

class GEO_API GEO_BFSEdgeAdjPolyTraverser : public GEO_BFSAdjPolyTraverser
{
protected:
    virtual void iterateThroughAdjacentPolygons();

public:
    explicit GEO_BFSEdgeAdjPolyTraverser( GEO_Detail *gdp,
				      const GA_PrimitiveGroup *process_grp,
				      GEO_BFSTraverserVisitor &visitor,
				      UT_Interrupt *boss = NULL,
				      bool insure_poly_only = false )
	: GEO_BFSAdjPolyTraverser( gdp, process_grp, visitor, boss, insure_poly_only )
	{ }

    explicit GEO_BFSEdgeAdjPolyTraverser( GEO_Detail *gdp,
				      GEO_BFSTraverserVisitor &visitor,
				      UT_Interrupt *boss = NULL)
	: GEO_BFSAdjPolyTraverser( gdp, visitor, boss )
	{ }

};

class GEO_API GEO_BFSPointAdjPolyTraverser : public GEO_BFSAdjPolyTraverser
{
protected:
    virtual void iterateThroughAdjacentPolygons();

public:
    explicit GEO_BFSPointAdjPolyTraverser( GEO_Detail *gdp,
				      const GA_PrimitiveGroup *process_grp,
				      GEO_BFSTraverserVisitor &visitor,
				      UT_Interrupt *boss = NULL,
				      bool insure_poly_only = false )
	: GEO_BFSAdjPolyTraverser( gdp, process_grp, visitor, boss, insure_poly_only )
	{ }

    explicit GEO_BFSPointAdjPolyTraverser( GEO_Detail *gdp,
				      GEO_BFSTraverserVisitor &visitor,
				      UT_Interrupt *boss)
	: GEO_BFSAdjPolyTraverser( gdp, visitor, boss )
	{ }

};

//
// Macros
// 
// WARNING: each of these iterators allocates space for an array of
// primitives so use with care

// iterate through polygons adjacent to a single point
#define GEO_FOR_ALL_POLY_ADJ_TO_PT( gdp, pt_offset, adj_poly ) \
    GEO_PointAdjPolyIterator poly_it( (gdp), (pt_offset) ); \
    while( poly_it.advance() && (adj_poly = poly_it.getAdjPoly()) ) 

#define GEO_FOR_ALL_GROUP_POLY_ADJ_TO_PT( gdp, grp, pt_offset, adj_poly ) \
    GEO_PointAdjPolyIterator poly_it( (gdp), (pt_offset), (grp) ); \
    while( poly_it.advance() && (adj_poly = poly_it.getAdjPoly()) ) 

// iterate through polygons adjacent to a single polygon
#define GEO_FOR_ALL_POLY_EDGE_ADJ_TO_POLY( gdp, poly, adj_poly ) \
    GEO_EdgeAdjPolyIterator poly_it( (gdp), (poly)->getMapOffset() ); \
    while( poly_it.advance() && (adj_poly = poly_it.getAdjPoly()) ) 

#define GEO_FOR_ALL_GROUP_POLY_EDGE_ADJ_TO_POLY( gdp, grp, poly, adj_poly ) \
    GEO_EdgeAdjPolyIterator poly_it( (gdp), (poly)->getMapOffset(), (grp) ); \
    while( poly_it.advance() && (adj_poly = poly_it.getAdjPoly()) ) 

#define GEO_FOR_ALL_POLY_ADJ_TO_POLY( gdp, poly, adj_poly ) \
    GEO_PolyAdjPolyIterator poly_it( (gdp), (poly)->getMapOffset() ); \
    while( poly_it.advance() && (adj_poly = poly_it.getAdjPoly()) ) 

#define GEO_FOR_ALL_GROUP_POLY_ADJ_TO_POLY( gdp, grp, poly, adj_poly ) \
    GEO_PolyAdjPolyIterator poly_it( (gdp), (poly)->getMapOffset(), (grp) ); \
    while( poly_it.advance() && (adj_poly = poly_it.getAdjPoly()) ) 

// for BFS iteration please use the visitors

#endif
