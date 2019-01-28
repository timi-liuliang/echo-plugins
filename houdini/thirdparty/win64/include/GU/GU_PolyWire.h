/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PolyWire.h ( GU Library, C++)
 *
 * COMMENTS: This constructs a series of wires for each edge
 *	of a polygon group.
 */

#ifndef __GU_PolyWire__
#define __GU_PolyWire__

#include "GU_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_Vector3Array.h>
#include <GA/GA_Edge.h>
#include <GA/GA_EdgeMap.h>

class GU_Detail;
class GA_PrimitiveGroup;
class GA_ElementWranglerCache;
class gu_ConnectionGraph;

class GU_API GU_PolyWire
{
public:
    /// Even setting the edge data requires access to the gdp,
    /// and it doesn't make any sense to change the gdp, so
    /// it should be passed in the constructor.
    GU_PolyWire(GU_Detail *gdp);
    virtual ~GU_PolyWire();

    /// The edge data
    class EdgeData
    {
    public:
        int	nsegments;
        float	uoff;
        float	u1, u2;
        float	v1, v2;
        float	segscale1, segscale2;
    };

    /// Parameter Setting:
    /// This does NOT claim ownership of these:
    void		 setPtWidths(float *widths) { myPtWidths = widths; }
    void		 setJointScale(float *scale) { myJointScale = scale; }
    void		 setPtSmooth(bool *smooth) { myPtSmooth = smooth; }
    void		 setDivisions(int *div) { myDivs = div; }
    /// NOTE: This makes its own copy of the EdgeData to store in
    ///       myEdgeHashEntries.
    void		 setEdgeData(GA_Edge edge, const EdgeData &data);
    const EdgeData	*getEdgeData(GA_Edge edge) const;

    void		 setGrp(GA_PrimitiveGroup *grp) { myPrimGroup = grp; }
    void		 setJointCorrection(bool jc) { myJointCorrection = jc; }
    void		 setDoTextures(bool dotext) { myDoTextures = dotext; }
    void		 setUseUpVector(bool up) { myUseUpVector = up; }
    void		 setUpVectors(UT_Vector3 *up) { myUpVectors = up; }

    // Parameter access functions:
    GU_Detail		*getGdp() const { return myGdp; }
    bool		 getSmooth(GA_Index pt) const { return myPtSmooth[pt]; }
    float		 getWidth(GA_Index pt) const { return myPtWidths[pt]; }
    float		 getMaxScale(GA_Index pt) const { return myJointScale[pt]; }
    int			 getSegments(GA_Index pt1, GA_Index pt2) const;
    void		 getV(GA_Index pt1, GA_Index pt2, float &v1,
                              float &v2) const;
    void		 getU(GA_Index pt1, GA_Index pt2, float &uoff,
				float &u1, float &u2) const;
    void		 getSegScales(GA_Index pt1, GA_Index pt2,
				    float &segscale1, float &segscale2) const;
    int			 getDivisions(GA_Index pt) const { return myDivs[pt]; }
    UT_Vector3		 getUpVector(GA_Index pt) const { return myUpVectors[pt]; }
    bool		 doJointCorrection() const { return myJointCorrection; }
    bool		 doTextures() const { return myDoTextures; }
    bool		 useUpVector() const { return myUseUpVector; }

    // Deal with the texture value cache:
    float		 getPtOffTexU(GA_Offset ptoff);
    void		 setPtOffTexU(GA_Offset ptoff, float val);
    float		 getPtOffTexV(GA_Offset ptoff);
    void		 setPtOffTexV(GA_Offset ptoff, float val);
    float		 getGlbPtOffTexU(GA_Offset ptoff);
    bool		 hasGlbPtOffTexU(GA_Offset ptoff) const;
    void		 setGlbPtOffTexU(GA_Offset ptoff, float val);

    // This uses the built-in circle caches to get the position
    // on the x/y circle of the required point...
    // Valid numbers are from 0..(myDiv-1)
    UT_Vector3		 getCirclePos(int idx, int div) 
			{ buildCircleCache(div);
			  return (*myCircleCache(div))(idx); }

    // Builds all the wires:
    void		 buildWire();

    GA_Offset		 getJunctionPtOff(const UT_Vector3 &stdpos,
			    const UT_Vector3 &center, float scale);
    // Calculates realworld coordinates of the point
    void		 evaluateJunctionPoints(const UT_Vector3 &center,
			    float scale, GA_Offset cptoff);
    void		 clearJunctionPoints();

    // Go through a poly list and texture clamp all the polys...
    void		 textureClampPolys(const UT_Array<GEO_PrimPoly *>
					    &polylist, 
					    float ubase, float uwidth);

    GA_ElementWranglerCache	*getWranglers() { return myWranglers; }
	
    
protected:
    class GU_API gu_JunctionPoint
    {
    public:
        GA_Offset	ptoff;
        UT_Vector3	pos;
        float		scaletotal; // Total scale factors
        GA_Size		numpts;     // Number of points who've grabbed this.
    };

    void		 buildCircleCache(int div);

    // These are effectively attributes, but a bit cheaper.
    // Widths is the radius of the wires at each point, smooth
    // is a boolean value of whether to do smooth joining or just
    // connect them.
    // Segs and divs are how to split up tubes that enter this point.
    float		*myPtWidths;
    float		*myJointScale;
    bool		*myPtSmooth;
    int			*myDivs;
    UT_Vector3		*myUpVectors;
    bool		 myJointCorrection;
    bool		 myDoTextures;
    bool		 myUseUpVector;
    // For each point, what it's "u" value is.
    UT_FloatArray	 myTexU;
    UT_FloatArray	 myTexV;
    // These ones are fixed!
    UT_FloatArray	 myGlbTexU;

    // Cached circle poses cause I really hate sins and coses...
    // You might argue that the cost of sin & cos is buried by the
    // creation of and interpolation of points and attributes.  You
    // might even be right.
    UT_Array<UT_Vector3Array *>	myCircleCache;

    // What to act on:
    GU_Detail *const	 myGdp;
    GA_PrimitiveGroup	*myPrimGroup;

    // Our connection graph:
    gu_ConnectionGraph	*myGraph;

    // Our edge hash table:
    GA_EdgeMap<EdgeData *> 	myEdgeHash;

    // Our junction point list:
    UT_Array<gu_JunctionPoint>	myJunctionPoints;

    // Our hash table values:
    UT_Array<EdgeData *>	myEdgeHashEntries;

    GA_ElementWranglerCache	*myWranglers;
};

#endif

