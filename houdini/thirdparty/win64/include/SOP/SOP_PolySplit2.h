/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP Library (C++)
 *
 * COMMENTS: PolySplit tool.  Slices and dices polygons.
 *
 */

#ifndef __SOP_PolySplit2_h__
#define __SOP_PolySplit2_h__

#include "SOP_API.h"
#include <GEO/GEO_PrimClassifier.h>
#include <UT/UT_ValArray.h>
#include "SOP_Node.h"

class MSS_PolySplitState2;
class GU_RayIntersect;

class SOP_API SOP_PolySplit2 : public SOP_Node
{
public:
	     SOP_PolySplit2(OP_Network *net, const char *name,
			     OP_Operator *entry);
    virtual ~SOP_PolySplit2();

    virtual bool		 updateParmsFlags();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template	 	 myTemplateList[];

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);
    virtual const char 	*inputLabel(unsigned idx) const;

private:
    // projects the points in curve onto the input geometry, and converts their
    // locations into GU_SplitLoc's. If they are close to points or edges they
    // are snapped to them to preserve topology
    // curve must contain the points to be projected
    // splitlocs will contain the projected SplitLoc's if successful upon return
    // tol is the tolerance used for snapping points to existing points/edges
    // returns true if successful, false otherwise
    bool 	projectCurvePoints(const GU_Detail *curve,
				   GU_SplitLocArray &splitlocs,
                                   float tol);

    // returns true if there is a splitloc without a polygon pointer, which 
    // occurs when the split string is of the form "p2-3" and edge 2-3 does not
    // exist
    bool        pointsWithoutPoly(const GU_SplitLocArray &splitlocs);

    // returns true if all of the splitlocs are on the same connected component
    // of the geometry
    bool	pointsOnSameComponent(const GU_SplitLocArray &splitlocs);

    // returns true if all of the splitlocs are on edges. Used to check edgeloop
    // data
    bool	allPointsOnEdges(const GU_SplitLocArray &splitlocs);

    // returns true if all of the splitlocs are not on faces.
    bool 	allPointsOffFaces(const GU_SplitLocArray &splitlocs);

    // Properties
    void	getSplitLoc(UT_String &str, fpreal t)
					{ evalString(str, "splitloc", 0, t); }

    void	setSplitLoc(UT_String &str, fpreal t, CH_StringMeaning meaning)
					{ setString(str, meaning, "splitloc", 0, t); }

    int		PATHTYPE()		{ return evalInt("pathtype", 0, 0); }
    // MSS needs this so it can create the node in quad strip mode
    void	SET_PATHTYPE(int i)	{ setInt("pathtype", 0, 0, i); }

    int		CLOSED()		{ return evalInt("close", 0, 0); }
    void	SET_CLOSED(int i)	{ setInt("close", 0, 0, i); }

    int 	ALLOWFACES()		{ return evalInt("allowfaces", 0, 0); }
    void 	SET_ALLOWFACES(int i)	{ setInt("allowfaces", 0, 0, i); }

    int		NUMLOOPS()		{ return evalInt("numloops", 0, 0); }
    int         PARALLELLOOP()          { return evalInt("parallellooptoggle", 0, 0); }
    void        SET_PARALLELLOOP(int i) { setInt("parallellooptoggle", 0, 0, i); }
    int         PARALLELFLIP()          { return evalInt("parallelfliptoggle", 0, 0); }
    void        SET_PARALLELFLIP(int i) { setInt("parallelfliptoggle", 0, 0, i); }

    int		QUADCOMPLETE()		{ return evalInt("quadcomplete", 0, 0); }
    void	SET_QUADCOMPLETE(int i) { setInt("quadcomplete", 0, 0, i); }

    int		EDGEPERCENTTOGGLE()	{ return evalInt("edgepercenttoggle", 0, 0); }
    void	SET_EDGEPERCENTTOGGLE(int i) { setInt("edgepercenttoggle", 0, 0, i); }
    float	EDGEPERCENTAGE()	{ return evalFloat("edgepercent", 0, 0); }

    int		UPDATENORMS(fpreal t)	{ return evalInt("updatenorms", 0, t); }

    int 	CREATEGROUP()		{ return evalInt("grouptoggle", 0, 0); }
    void	GROUPNAME(UT_String &str, fpreal t) { evalString(str, "groupname", 0, t); }
    int         GROUPAPPEND()           { return evalInt("groupappend", 0, 0); }

    float	TOLERANCE(fpreal t)	{ return evalFloat("tolerance", 0, t); }

    // returns true if there is something in the second input
    bool	curveInput()		{ return getInput(1) != NULL; }

    // cached data about input 1 (the geomtry we are splitting) so that we can
    // keep the GU_RayIntersect between cooks. This is especially important when
    // editing the input curve in the viewport, because at that point we will be
    // cooking every frame, and constructing a GU_RayIntersect is very expensive
    GU_RayIntersect    *myRayIntersect;
    exint 		myCachedInputCookCount;
    exint		myCachedInputUniqueId;
    GEO_PrimClassifier  myPrimClassifier;

    friend class sop_SplitParser2;
    friend class MSS_PolySplitState2;
};

#endif
