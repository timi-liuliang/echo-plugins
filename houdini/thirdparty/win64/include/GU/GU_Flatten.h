/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS:	Flatten GU
 */

#ifndef __GU_Flatten_h__
#define __GU_Flatten_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include <GA/GA_Edge.h>
#include <GA/GA_EdgeGroup.h>
#include <UT/UT_StringStream.h>
#include <UT/UT_Map.h>
#include <GEO/GEO_HedgeInterface.h>

class UT_Classifier;

#define PROJECTED_INITIAL_VALUES 0

class UT_AutoInterrupt;

class GU_API GU_Flatten
{
public:
			 GU_Flatten(GU_Detail *gdp,
				 const GA_PrimitiveGroup *prims,
				 const GA_EdgeGroup *seams,
				 const GA_EdgeGroup *extra_seams,
				 bool use_input_uv = false,
				 bool axis_align_islands = false,
				 bool repack_wasted = false);

			~GU_Flatten();

    int 		 getNumIslands()
			    { return myNumIslands; }

    /// Updates UVs in myTGdp and packs all islands into unit square.
    /// Once finished, myTGdp will hold UVs as created after flattening
    /// with the initially assigned pins.
    /// Returns true if interrupted and false otherwise
    bool 		 flattenAndPack();

    /// Re-flattens islands affected by changes in current pins.
    /// Returns 1 if was interrupted and 0 otherwise
    int			 reflattenWithPins();

    /// Creates a vertex attribute for angular distortion and computes the
    /// distortion for all islands. It also creates the input vertex
    /// equivalence classes (myInputVertexClasses).
    void 		 addDistortionAttribute();

    // Adds an vertex attribute called "uvpointid" mapping vertices of myGdp to
    // the corresponding point indices of myRGdp; this is used in MPI and MSS
    void		 addPointIdAttribute();

    /// Creates a primitive attribute for island number
    void		 addIslandAttribute(bool add);

    /// Change the extra cuts edge group
    void		 updateExtraSeamsGroup(const GA_EdgeGroup *exgp);

    /// Change the primitive group
    void 		 updatePrimitiveGroup(const GA_PrimitiveGroup *prims);

    /// Add one pinned point
    void		 addPin(int prim, int vert, fpreal u, fpreal v);

    /// Create a vertex group of all original pins.
    void		 addOriginalPinGroup(const char *name);

    /// Create a vertex group of all pins used during the flattening, both
    /// initial and user-specified.
    void		 addActivePinGroup(const char *name);

    void		 addSeamsGroup(const char *name);

    // The class encapsulating a pin
    class Pin
    {
    public:
	Pin() :
	    myInputVertex(GA_INVALID_OFFSET), myPoint(GA_INVALID_OFFSET),
	    myIslandIndex(-1), myU(0.0), myV(0.0), myToRemove(false) {}

	Pin(GA_Offset voff, GA_Offset ptof, int is, fpreal u, fpreal v) :
	    myInputVertex(voff), myPoint(ptof), myIslandIndex(is),
	    myU(u), myV(v), myToRemove(false) {}

	Pin(const Pin &other) :
	    myInputVertex(other.myInputVertex), myPoint(other.myPoint),
	    myIslandIndex(other.myIslandIndex), myU(other.myU), myV(other.myV),
	    myToRemove(other.myToRemove) {}

	GA_Offset 	 getInputVertex() const	{ return myInputVertex; }
	GA_Offset	 getPoint() const 	{ return myPoint; }
	inline void	 setU(fpreal u) 	{ myU = u; }
	inline void	 setV(fpreal v) 	{ myV = v; }
	inline fpreal	 u() const 		{ return myU; }
	inline fpreal 	 v() const 		{ return myV; }
	inline int	 getIslandIndex() const { return myIslandIndex; }
	inline void	 setToRemove(bool b) 	{ myToRemove = b; }
	inline bool	 isToRemove() const 	{ return myToRemove; }
    private:
	GA_Offset 	 myInputVertex; // vertex offset in myGdp
	GA_Offset 	 myPoint; 	// point offset in myRGdp for regular
					// pins or in myTGdp for "original" pins
	fpreal 		 myU, myV;
	bool 		 myToRemove;
	int 		 myIslandIndex;
    };

    const std::string	 getWarningMessage()
    			    { return myWarningMessage.str().toStdString(); }
private:

    void		 buildIslands(GU_Detail *gdp, int num_islands);

    class Island
    {
    public:
	Island(GU_Detail *gdp, int island_id, bool pos_from_uv = false);

    	GU_Detail	*getDetail() { return myGdp; }
    	int		 getIndex() const { return myIndex; }
    	GA_RWHandleV3	&getUVHandle() { return myUV; }

    	const
    	GA_OffsetArray	&getPoints() { return myPoints; }
    	const
    	GA_OffsetArray	&getPrimitives() { return myPrimitives; }
    	const
    	GA_OffsetArray	&getInputPrimitives() { return myInputPrimitives; }

    	inline
	UT_Vector3	 getPos3(GA_Offset pt) const;

    	inline void	 appendPrimitive(GA_Offset primoff)
    				    { myPrimitives.append(primoff); }
    	inline void	 appendPoint(GA_Offset ptoff)
    				    { myPoints.append(ptoff); }
    	inline void	 appendInputPrimitive(GA_Offset primoff)
    				    { myInputPrimitives.append(primoff); }

#if PROJECTED_INITIAL_VALUES
    	void		 setCenter(UT_Vector3 ctr) { myCenter = ctr; }
    	UT_Vector3	 getCenter() const { return myCenter; }

    	void		 setXAxis(UT_Vector3 ctr) { myXAxis = ctr; }
    	UT_Vector3	 getXAxis() const { return myXAxis; }

    	void		 setYAxis(UT_Vector3 ctr) { myYAxis = ctr; }
    	UT_Vector3	 getYAxis() const { return myYAxis; }

    	void		 setWidth(fpreal w) { myWidth = w; }
    	fpreal		 getWidth() const { return myWidth; }
#endif

    private:
    	GA_OffsetArray	 myPrimitives;
    	GA_OffsetArray	 myPoints;
    	GA_OffsetArray	 myInputPrimitives;

#if PROJECTED_INITIAL_VALUES
    	UT_Vector3	 myXAxis, myYAxis, myCenter;
    	fpreal		 myWidth;
#endif

    	GU_Detail 	*myGdp;
    	int		 myIndex;
    	bool		 myPosFromUV;
    	GA_RWHandleV3	 myUV;
    	GA_ROHandleV3	 myOrigUV;
     };

    typedef UT_Array<Island *>	 	IslandArray;
    typedef UT_Array<Pin>	 	PinArray;
    typedef UT_Map<int, PinArray> 	PinMap;

    void 		 calculateDistortion(GA_RWHandleF &disth,
				Island &island);

    /// Re-flatten an island using current pins
    void 		 flattenIsland(Island &island,
        		        UT_AutoInterrupt &boss);

    /// Flatten an island using the auto-assigned initial pins
    void 		 flattenIslandWithInitialPins(Island &island,
				UT_AutoInterrupt &boss);

    /// The main flattening method; flattens the given island with the given
    /// pins. Returns true if interrupted and false otherwise.
    void		 flattenLSCM(Island &island, const PinArray &pins,
				bool write_to_input_detail,
				UT_AutoInterrupt &boss);

    /// Translate UVs of an island by vector t
    void 		 translateIsland(UT_Vector3 t, Island &island,
				bool write_to_input_detail = true);

    /// Attach symmetric pins to each original island. Fill myOriginalPins
    void		 assignIslandInitialPins(Island &island);

    /// Construct myRGdp by cutting myTGdp along extra edges
    void 		 recutWithExtraSeams(bool force_rebuild_islands = false);

    void		 writeUVsToInputDetail(GA_ROHandleV3 &uvh);

    /// Returns a representative input vertex for a given point in myTGdp
    inline GA_Offset	 workPointToInputVertex(GA_Offset work_pt)
			    { return myWorkPointToInputVertex(work_pt); }

    /// Returns a representative vertex in the myTGdp (equivalently myRGdp)
    // for the given input vertex
    inline GA_Offset	 getWorkVertex(GA_Offset vtx)
			    { return myWorkVertex(vtx); }

    inline GA_Offset	 getInputVertex(GA_Offset vtx)
    			    { return myInputVertex(vtx); }

    void		 getIslandPins(PinMap &pin_map, int island,
				PinArray &pins);

    int			 cutDetailAlongSeams(GU_Detail *gdp,
				const GEO_DetachedHedgeInterface *hip,
				const GA_EdgeGroup *seams,
				GU_Detail *seams_gdp,
				GA_RWHandleI &islandh);

    enum PinStatus
    {
	NEW_PIN,
	DUPLICATE_PIN,
	MODIFIED_PIN
    };

    PinStatus		 addPinToPinMap(GU_Flatten::PinMap &pin_map, Pin pin);
    void		 processPins();

    /// map an input vertex to a work point
    inline GA_Offset 	 getWorkPoint(GA_Offset in_vtx)
    {
	GA_Offset vtx = getWorkVertex(in_vtx);
	if (!GAisValid(vtx))
	    return GA_INVALID_OFFSET;
	return myRGdp->vertexPoint(vtx);
    }

    GA_Attribute 	*getTextureAttribute(GU_Detail *gdp,
				bool add_if_missing = true);
    GA_Attribute	*getIslandAttribute(GU_Detail *gdp,
				bool add_if_missing = true);

    void		 purgeIslands(int new_num_islands = 0);
    Island		*getIsland(int i, GU_Detail *gdp);
    void		 buildInputVertexClasses();

    GEO_DetachedHedgeInterface
			*getInputHedgeInterface();

    // We compute a partitioning of input vertices into equivalence classes
    // for computation of distortion.
    // Two vertices are placed in the same equivalence class if and only if
    // they have the same 3D position and the same assigned UV coordinates.
    // The root of each class is used for the purpose of accumulation of
    // distortion amounts.

    UT_Classifier	 *myInputVertexClasses;

    PinMap		 myInitialPins;
    PinMap		 myCurrentPins;

    GA_OffsetArray	 myWorkPointToInputVertex;

    GA_OffsetArray	 myWorkVertex;
    GA_OffsetArray	 myInputVertex;

    GU_Detail 		 *myGdp;	// input detail
    GU_Detail		 *myTGdp;	// triangulated and cut detail
    GU_Detail		 *myRGdp;	// re-cut (additional seams) detail

    GA_RWHandleV3	 myUV;
    GA_RWHandleV3	 myTUV;
    GA_RWHandleV3 	 myRUV;			// attribute handle on myRGdp
    GA_RWHandleI	 myPrimitiveIsland;	// attribute handle on myRGdp

    UT_Array<bool>	 myReflattenIslandFlags;
    UT_BitArray		 myIslandsToReflatten;

    GA_EdgeGroup	*myInitialSeams;
    GA_EdgeGroup	*myExtraSeams;

    int			 myInitialNumIslands;
    int 		 myNumIslands;
    IslandArray		 myIslands;

    fpreal 		 myTotalArea;		// used for scaling

    bool		 myUseInputUV;
    bool		 myAxisAlignIslands;
    bool		 myRepackWasted;
    UT_StringStream	 myWarningMessage;

    GEO_DetachedHedgeInterface  *myInputHI;
    GEO_DetachedHedgeInterface  *myHip;		// on myTTGdp
    GA_PrimitiveGroup	*myGroup;	// input primitive group
};


UT_Vector3
GU_Flatten::Island::getPos3(GA_Offset pt) const
{
    if (myPosFromUV)
	return myOrigUV.get(myGdp->pointVertex(pt));
    else
	return myGdp->getPos3(pt);
}

#endif
