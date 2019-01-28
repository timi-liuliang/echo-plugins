/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS:	UVPack GU
 */

#ifndef __GU_UVPack_h__
#define __GU_UVPack_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include <GEO/GEO_HedgeInterface.h>

class GU_API GU_UVPack
{
public:
			 GU_UVPack(GU_Detail *gdp, 
				 const GA_PrimitiveGroup *grp = nullptr,
				 int resolution = 1000,
				 int padding = 10, bool pad_boundary = false,
				 bool correct_area_proprtions = false,
				 bool axis_align_islands = false,
				 bool repack_wasted = false);

			~GU_UVPack();

    void 	 	 setPadding(int padding)
			    { myPadding = padding; }

    // Pack all islands into a single tile (unit square) at maximum
    // possible scale.
    int 	 	 tilePack(UT_IntArray *islands = NULL,
				  int tile_u = 0, int tile_v = 0);

    // Pack islands into as few squares as possible a the given fixed scale.
    void	 	 scalePack(fpreal scale);

    // Packs everything according to the udim attribute
    void 	 	 udimPack(bool guess_udim_from_uvs = false);


    // a structure presenting a rasterized copy of a UV island
    struct GU_API Raster
    {
			 Raster(int w, int h, int res, fpreal orig_scale);
	void		 setScale(fpreal scale);
	inline void 	 updateLow(int x, int y)
			    { if (y < myOrigLow(x)) myOrigLow(x) = y; }

	inline void 	 updateHigh(int x, int y)
			    { if (y > myOrigHigh(x)) myOrigHigh(x) = y; }

	inline int 	 getLow(int x)
			    { return myTrivialRaster ? 0 : myLow(x); }

	inline int 	 getInvertedLow(int x)
			    { return myTrivialRaster ? 0 :
				myHeight - 1 - myHigh(myWidth - 1 - x); }

	inline int 	 getHigh(int x)
			    { return myTrivialRaster ?
				myHeight - 1: myHigh(x); }

	inline int 	 getInvertedHigh(int x)
			    { return myTrivialRaster ? myHeight - 1:
				myHeight - 1 - myLow(myWidth - 1 - x); }

	inline int 	 getWidth() { return myWidth; }
	inline int 	 getHeight() { return myHeight; }
	inline fpreal	 getOrigScale() { return myOrigScale; }

	inline void	 setMaxU(fpreal u) { myMaxU = u; }
	inline void 	 setMaxV(fpreal v) { myMaxV = v; }
	inline fpreal	 getMaxU() { return myMaxU; }
	inline fpreal 	 getMaxV() { return myMaxV; }

	void		 dumpOrig(int xstep, int ystep);
	void		 dump(int xstep, int ystep);
	inline bool	 isTrivial() { return myTrivialRaster; }

    private:

	int 		 myOrigWidth;
	int 		 myOrigHeight;
	fpreal 		 myOrigScale;
	UT_IntArray 	 myOrigLow; 	// lower horizon
	UT_IntArray 	 myOrigHigh; 	// upper horizon

	int 		 myWidth;	// width at current scale
	int		 myHeight;	// height at current scale
	fpreal		 myScale;	// current scale
	UT_IntArray 	 myLow; 	// current scale lower horizon
	UT_IntArray 	 myHigh; 	// current sacle upper horizon

	int 		 myRes;
	bool 		 myTrivialRaster;
	fpreal		 myMaxU, myMaxV;
    };

    struct Horizon
    {
    			 Horizon(int width);
    	void		 clear();
    	int		 getMinHeight();
    	int 		 getMaxHeight() { return myMaxHeight; }
    	inline int	 getHeight(int i) { return myHeight(i); }
    	inline void 	 updateHeight(int i, int h)
    			    { if (h > myHeight(i)) myHeight(i) = h; }
    	inline void 	 updateMaxHeight(int h)
    			    { if (h > myMaxHeight) myMaxHeight = h; }
    	int		 getWidth()  { return myHeight.entries(); }
        const UT_IntArray& getHeightData() const { return myHeight; }
        private:
    	UT_IntArray	 myHeight;
    	int		 myMaxHeight;
    };

    Raster		*getRaster(int island, int inst)
    {
	return myRasters(myNumInstances * island + inst);
    }

private:
    inline int		 vertexIsland(GA_Offset v)
			    { return myPrimIsland(myGdp->vertexPrimitive(v)); }

    // Shifts UVs of all island vertices so that the minimum U and V
    // values are both zero.
    void	 	 translateIslandsToOrigin();

    // computes and writes area of all UV islands into array myArea
    void	 	 computeIslandAreas(bool correct_area_proportions);

    // transforms an island to a set of texels
    Raster	*rasterizeIsland(int island, int inst, int resolution,
				fpreal scale);

    void		 prepareIslandRasters(UT_IntArray &islands,
				int resolution, fpreal scale);

    void		 resetRasters();
    bool		 isIslandTooLarge(int island, int resolutino);

    void axisAlignIslands();

    enum IslandPackStatus
    {
	PACKED,
	NO_ROOM,	// island fits in empty tile but not enough room now
	TOO_LARGE	// island wider or taller than the tile
    };

    struct Placement
    {
    public:
	Placement() :
	    myBaseU(0.0), myBaseV(0.0), myInstance(0), myInverted(false) {}

	inline void 	 setBaseU(fpreal u) { myBaseU = u; }
	inline void	 setBaseV(fpreal v) { myBaseV = v; }
	inline void	 setInverted(bool b) { myInverted = b; }
	inline void 	 setInstance(int i) { myInstance = i; }

	inline fpreal	 getBaseU() { return myBaseU; }
	inline fpreal	 getBaseV() { return myBaseV; }
	inline bool	 isInverted() { return myInverted; }
	inline int	 getInstance() { return myInstance; }

    private:
	fpreal		 myBaseU, myBaseV;
	int		 myInstance;
	bool		 myInverted;
    };

    // Structure which tracks lost lower space after horizon adjustment.
    struct WastedSpace
    {
        public:
            WastedSpace(int w, int x, int y) :
                myMinHeight(std::numeric_limits<int>::max()),
                myMaxHeight(std::numeric_limits<int>::min()),
                myWidth(w), myPosX(x), myPosY(y)
            {
                mySpaceLower.entries(w);
                mySpaceUpper.entries(w);
            }

            inline void updateBounds(int x, int lower, int upper)
            {
                mySpaceLower(x) = lower;
                mySpaceUpper(x) = upper;

                int height = upper - lower;

                if (height < myMinHeight)
                    myMinHeight = height;

                if (height > myMaxHeight)
                {
                    myMaxHeight = height;
                }
            }

            inline int getMinHeight() const
            {
                return myMinHeight;
            }

            inline int getMaxHeight() const
            {
                return myMaxHeight;
            }

            inline int getWidth() const
            {
                return myWidth;
            }

            inline int getPosX() const
            {
                return myPosX;
            }

            inline int getPosY() const
            {
                return myPosY;
            }

            inline int getLower(int i) const
            {
                return mySpaceLower(i);
            }

            inline int getUpper(int i) const
            {
                return mySpaceUpper(i);
            }

        private:
            UT_IntArray mySpaceLower;
            UT_IntArray mySpaceUpper;
            int myMinHeight, myMaxHeight;
            int myWidth;
            int myPosX, myPosY;
    };

    IslandPackStatus packIsland(int i, Horizon &horizon,
        UT_Array<WastedSpace> &wasted_spaces,
        int padding, Placement &placement);

    void		 updateUVs(bool all_tiles, int tile_u, int tile_v,
				   int resolution, fpreal scale);

    // Calculate "island" attribute based on UV connectivity.
    void		 findIslands(const GA_PrimitiveGroup *grp);

    // Finds the best location to drop the island raster into one of the
    // recorded lower wasted spaces.
    IslandPackStatus findBestWastedPacking(int island,
        UT_Array<WastedSpace> &wasted_spaces, int &inst,
        int &x, int &y, bool &inverted, int padding);

    // Finds the best location to drop the island raster into a horizon.
    // Returns true if the best packing requires turning the island upside-down.
    bool		 findBestPacking(Raster &raster,
        		       Horizon &horizon, int &x, int &y);

    IslandPackStatus	 findBestPacking(int island, Horizon &hor, int &inst,
    			       int &x, int &y, bool &inverted);

    bool verifyWastedSpaceHorizonPacking(const WastedSpace &wasted_space,
        int horizon_value, int shift, int padding, int pos) const;

    void		 setRaster(int island, int inst, Raster *raster)
    {
	int idx = myNumInstances * island + inst;
	if (myRasters(idx))
	    delete myRasters(idx);
	myRasters(idx) = raster;
    }


    int			 myResolution; 		// packing horizon width
    int			 myPadding;
    bool		 myPadBoundary;		// apply padding to square frame
    bool		 myUse3DArea;
    int			 myNumInstances;
    int 		 myNumIslands;
    bool         myAxisAlignIslands;
    bool         myRepackWasted;

    // Maximum UV values for each island, after being translated to origin
    UT_FprealArray	 myMaxU;
    UT_FprealArray	 myMaxV;
    UT_FprealArray	 myIslandArea;		// UV Area of islands
    UT_IntArray 	 myIslandsOrder;	// Order to pack islands
    UT_IntArray		 myIslandTile;		// tile assigned to each island
    UT_IntArray		 myOrigIslandTileU;	// tile assigned to each island
    UT_IntArray		 myOrigIslandTileV;	// tile assigned to each island
    UT_IntArray		 myIslandTileU;		// tile assigned to each island
    UT_IntArray		 myIslandTileV;		// tile assigned to each island
    UT_IntArray		 myIslandDefaultUdim;	// lowest udim number tiles
						// containing vertices of island

    // Final placement of UV islands in their respective tiles
    UT_Array<Placement>	 myPlacement;

    // Array of rasters (one raster per instance per island)
    // Raster j of island i will be at index (i * myNumInstances + j)
    UT_Array<Raster *>	 myRasters;

    UT_Array<GEO_Hedge>	 myBoundaryHedges;


    UT_IntArray		 myPrimIsland;		// maps prim offset to island

    GA_RWHandleV3	 myUV;
    GU_Detail		*myGdp;

    GEO_DetachedHedgeInterface   myHI;
};


#endif
